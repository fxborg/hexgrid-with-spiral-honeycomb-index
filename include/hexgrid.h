#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <map>
#include <unordered_map>
#include "hexagon.h"
#include "point.h"
#include "hexindex.h"
#include "hexpattern.h"
using std::merge;
using std::abs;
using std::ceil;
using std::pair;
using std::vector;
using std::map;
using std::min;
using std::unordered_map;
using hex::Hexagon;
using hex::HexPattern;


namespace hex {
	const int64_t HEXAGON_MIN = -92233720368547758; // INT_64_MIN*0.01
	const int64_t HEXAGON_MAX =  92233720368547758; // INT_64_MAX*0.01

	enum Layout{
		HORIZONTAL, // Flat
		VERTICAL // Pointy
	};


	struct Orientation{
		const double f0;
		const double f1;
		const double f2;
		const double f3;
		const double b0;
		const double b1;
		const double b2;
		const double b3;
		Orientation(double f0_, double f1_, double f2_, double f3_, double b0_, double b1_, double b2_, double b3_) : f0(f0_), f1(f1_), f2(f2_), f3(f3_), b0(b0_), b1(b1_), b2(b2_), b3(b3_) {}
	};

	class HexGrid{
	public:

		explicit HexGrid(double radius_, Layout layout_) : m_radius(radius_), m_orientation(set_orientation(layout_)) {}

		HexagonInt64 opposite(const HexagonInt64 & hex)
		{
			return	HexagonInt64(-1 * hex.q(), -1 * hex.r(), -1 * hex.s());
		}

		HexagonInt64 round_hex(double frac_q, double frac_r, double frac_s)
		{
			double q = (round(frac_q));
			double r = (round(frac_r));
			double s = (round(frac_s));

			double q_diff = abs(q - frac_q);
			double r_diff = abs(r - frac_r);
			double s_diff = abs(s - frac_s);

			if (q_diff > r_diff && q_diff > s_diff)	{
				q = -r - s;
			}else if (r_diff > s_diff)	{
				r = -q - s;
			}else{
				s = -q - r;
			}
			if (max({ q,r,s }) > HEXAGON_MAX) {
				return HexagonInt64(0, 0, 0);
			}
			if (min({ q,r,s }) < HEXAGON_MIN) {
				return HexagonInt64(0, 0, 0);
			}

			return HexagonInt64(int64_t(q), int64_t(r), int64_t(s));
		}

		Point hex_to_pixel(const HexagonInt64 &h)
		{
			const Orientation M{ m_orientation };
			double x = (M.f0 * h.q() + M.f1 * h.r())*m_radius;
			double y = (M.f2 * h.q() + M.f3 * h.r())*m_radius;
			return Point(x, y);
		}


		HexagonInt64 pixel_to_hex(const Point &p)
		{
			const Point pt{ p.x / m_radius , p.y / m_radius };
			const Orientation M{ m_orientation };
			double q = double(M.b0 * pt.x + M.b1 * pt.y);
			double r = double(M.b2 * pt.x + M.b3 * pt.y);
			return round_hex(q, r, -q - r);
		}


		void add_point(const HexagonInt64 & hex, const Point & pt)
		{
			auto it = m_hexagon_map.find(hex);
			if (it != m_hexagon_map.end()) {
				m_hexagon_map[hex].push_back(pt);
			}else{
				m_hexagon_map[hex] = { pt };
				m_index.set_indexes(hex);

			}
		}

		vector<Point> get_points(const HexagonInt64 & hex)
		{
			auto it = m_hexagon_map.find(hex);
			if (it != m_hexagon_map.end()) {
				return it->second;
			}
			return {};
		}
		/**
		* 円環状にセルを移動しながら点群が存在する座標を調べます。
		*/
		vector<HexagonInt64> walk_on_ring(const HexagonInt64 & center, int step) 
		{
			// グリッド座標をセットする
			vector<HexagonInt64> results;
			if (step < 0) return {};
			if (step == 0) return { center };

			// 5の方角に移動する。
			HexagonInt64 hex = center + (m_hex_directions[5] * step);
			for (int i = 1; i <= 6; i++) {// ６回方向を変える
				for (int j = 0; j < step; j++) { // 辺の長さ分直進する
					hex = m_hex_directions[i] + hex;
					results.push_back(hex);
				}
			}
			return results;
		}

		/**
		* 近傍点を探索します。
		*/
		unordered_map<int, vector<HexagonInt64>> neighbors(const HexagonInt64 & origin, double distance) {
			// 点群が存在するグリッド座標を格納する
			unordered_map<int, vector<HexagonInt64>> merge_results{};

			// グリッド間の距離の計算
			double grid_size = (2 * m_radius* sqrt(3));
			// 探索対象範囲の上限
			int limit = int(ceil(distance / grid_size)) + 1;


			// 起点のインデックス位置を確認
			int8_t seven_num = HexIndex::hex_to_seven_num(origin);//
			HexagonInt64 relative_pos = m_hex_positions[seven_num]; //インデックス位置からの座標			
			HexagonInt64 origin_index_pos = origin - relative_pos;//インデックス位置
			int8_t filter = HexIndex::get_filter(seven_num);//インデックス用フィルタ

			int n = 0;
			while (3*n-1  <= limit){
				auto hexagons = walk_on_ring(origin_index_pos, n);
				for (auto const& hex : hexagons) {
					int dist = hex_distance(origin_index_pos - hex);
					unordered_map<int, vector<HexagonInt64>> results = m_index.get_hexagons(origin_index_pos, relative_pos, filter, hex, dist);
					for (auto & res : results) {
						std::copy(res.second.begin(), res.second.end(), std::back_inserter(merge_results[res.first]));
					}
				}
				n++;
			}
			return merge_results;
		}


		const Orientation set_orientation(const Layout & layout)
		{
			if (layout == Layout::HORIZONTAL){
				return{ 3.0 / 2.0, 0.0, sqrt(3.0) / 2.0, sqrt(3.0), 2.0 / 3.0, 0.0, -1.0 / 3.0, sqrt(3.0) / 3.0 };
			}else{
				return{ sqrt(3.0), sqrt(3.0) / 2.0, 0.0, 3.0 / 2.0, sqrt(3.0) / 3.0, -1.0 / 3.0, 0.0, 2.0 / 3.0 };
			}
		}
		
		vector<HexagonInt64> get_index() const
		{
			return  m_index.get_index();
		}

		vector<HexagonInt64> get_list(const HexagonInt64 & index, const int8_t filter) const
		{
			return  m_index.get_list(index, filter);
		}

		unordered_map<int, vector<HexagonInt64>> get_hexagons(
			const HexagonInt64 & origin_index_pos,
			const HexagonInt64 & relative_pos,
			const uint8_t filter,
			const HexagonInt64 & current_index_pos,
			const int distance)
		{
			return m_index.get_hexagons(origin_index_pos, relative_pos, filter, current_index_pos, distance);
		}

		static int hex_distance(HexagonInt64 hex)
		{
			return int(max({ abs(hex.q()), abs(hex.r()), abs(hex.s()) }));
		}
	private:

		const vector<HexagonInt64> m_hex_positions = {
			HexagonInt64(+0,+0,+0),	// 中央　位置
			HexagonInt64(+0,-1,+1),	// 南　　位置
			HexagonInt64(-1,+0,+1),	// 南西　位置
			HexagonInt64(-1,+1,+0),	// 北西　位置
			HexagonInt64(+0,+1,-1),	// 北　　位置
			HexagonInt64(+1,+0,-1),	// 北東　位置
			HexagonInt64(+1,-1,+0)	// 南東　位置
		};

		// 7方向へ移動する為の座標
		const vector<HexagonInt64> m_hex_directions = {
			HexagonInt64(+0,+0,+0),	// 中央
			HexagonInt64(+1,-3,+2),	// 南
			HexagonInt64(-2,-1,+3),	// 南西
			HexagonInt64(-3,+2,+1),	// 北西
			HexagonInt64(-1,+3,-2),	// 北
			HexagonInt64(+2,+1,-3),	// 北東
			HexagonInt64(+3,-2,-1)	// 南東
		};


		const double m_radius;
		const Orientation m_orientation;
		unordered_map< HexagonInt64, vector<Point>, hash_int64 > m_hexagon_map;

		HexIndex m_index;

	};
}
