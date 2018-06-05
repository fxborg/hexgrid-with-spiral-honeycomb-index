#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <map>
#include <unordered_map>
#include "hexagon.h"
#include "hexpattern.h"

using std::abs;
using std::ceil;
using std::pair;
using std::vector;
using std::map;
using std::unordered_map;
using hex::Hexagon;
using hex::HexPattern;

namespace hex {

	class HexIndex{
	public:
		explicit HexIndex() {}
		unordered_map<int, vector<HexagonInt64>> get_hexagons(
			const HexagonInt64 & origin_index_pos,
			const HexagonInt64 & relative_pos,
			const uint8_t filter,
			const HexagonInt64 & current_index_pos,
			const int distance)
		{
		

			const HexagonInt64 direction = current_index_pos - origin_index_pos;
			const auto pattern = HexPattern::get(direction);

			const auto hexagons = get_list(current_index_pos, filter);
			const HexagonInt64 center_pos = current_index_pos + relative_pos;
			return measure_distance(hexagons, center_pos, pattern, distance);

		}

		unordered_map<int, vector<HexagonInt64>> measure_distance(
			const vector<HexagonInt64> & hexagons,
			const HexagonInt64 & center_pos,
			const unordered_map<int8_t, int8_t> & pattern,
			const int distance) const
		{
			unordered_map<int, vector<HexagonInt64>> result;

			for (auto const& hex : hexagons) {
				int8_t pattern_id=HexPattern::get_id(hex - center_pos);
				auto ptn = pattern.find(pattern_id);
				if(ptn!=pattern.end()){
					const int dist{ int(ptn->second + distance) };
					result[dist].push_back(hex);
				}
			}
			return result;
		}
		vector<HexagonInt64> get_list(const HexagonInt64 & index, const int8_t filter) const
		{
			auto it = m_indexes.find(index);
			if (it == m_indexes.end()) return {};
			vector<HexagonInt64> result;
			for (auto const& v : it->second) {
				if (v.second & filter)
					result.push_back(v.first);
			}
			return result;

		}
		
		vector<HexagonInt64> get_index() const
		{
			vector<HexagonInt64> result;
			for (auto const & idx : m_indexes){
				result.push_back(idx.first);
			}
			return result;
		}
		void set_indexes(const HexagonInt64 & hex) 
		{
			vector<pair<HexagonInt64, uint8_t>> belongs = belongs_to_center(hex);
			for (auto const & idx : belongs){
				


				set_index(hex+idx.first, hex, idx.second);
			}
		}

		void set_index(const HexagonInt64 & idx, const HexagonInt64 & data, uint8_t filter)
		{
			if (m_indexes.count(idx) == 0) {
				m_indexes[idx] = {{data,filter} };
			}else if (m_indexes[idx].count(data) == 0) {
				m_indexes[idx][data] = filter;
			}
		}

		vector<pair<HexagonInt64, uint8_t>> belongs_to_center(const HexagonInt64 &h) const
		{
			size_t n = static_cast<size_t>(hex_to_seven_num(h));
			return m_index_directions[n];
		}
		static HexagonInt64 get_centor(const HexagonInt64 &h)
		{
			int8_t n  = hex_to_seven_num(h);
			switch (n) {
				case 0:	return h + HexagonInt64(+0, +0, +0);/*中央*/
				case 1: return h + HexagonInt64(+0, +1, -1);/* 北 */
				case 2: return h + HexagonInt64(+1, +0, -1);/*北東*/
				case 3: return h + HexagonInt64(+1, -1, +0);/*南東*/
				case 4: return h + HexagonInt64(+0, -1, +1);/* 南 */
				case 5: return h + HexagonInt64(-1, +0, +1);/*南西*/
				case 6: return h + HexagonInt64(-1, +1, +0);/*北西*/
		
				
				default:return h;
			}
		}


		static int8_t hex_to_seven_num(const HexagonInt64 &h)
		{
			return to_seven_num(mod7(int64_t(h.q() - h.r() * 2)));

		}

		static int8_t to_seven_num(int8_t n)  
		{
			switch (n) {
			case 0:	return 0;
			case 1:	return 5;
			case 2:	return 1;
			case 3:	return 6;
			case 4:	return 3;
			case 5:	return 4;
			case 6:	return 2;
			default:return 0;
			}
		}
		
		static uint8_t get_filter(int8_t n) 
		{
			switch (n) {
			case 0: return	0b10000000;
			case 1: return	0b00000001;
			case 2: return	0b00000010;
			case 3: return	0b00000100;
			case 4: return	0b00001000;
			case 5: return	0b00010000;
			case 6: return	0b00100000;
			default: return	0b10000000;
			}

		}
		static int8_t mod7(int64_t a) {
			if (a >= 0) {
				return static_cast<int8_t>(a % 7);
			}else{
				return static_cast<int8_t>(7-((-a) % 7));
			}		
		}

	private:


		float fmod(float a, float b)
		{
			return a - static_cast<int>(a / b) * b;
		}

		const std::vector<vector<pair<HexagonInt64, uint8_t>>> m_index_directions = {
			{ { HexagonInt64(+0,+0,+0),0b10111111 }/*中央*/ },
			/*------------------------------------------------------------------------------------------------*/
			{ { HexagonInt64(+0,+1,-1),0b10100011 }/* 南 */,{ HexagonInt64(+1,-2,+1),0b00001100 }/*北北西*/,{ HexagonInt64(-2,+0,+2),0b00010000 }/*北西２*/ },
			{ { HexagonInt64(+1,+0,-1),0b10000111 }/*南西*/,{ HexagonInt64(-1,-1,+2),0b00011000 }/*北北東*/,{ HexagonInt64(-2,+2,+0),0b00100000 }/*南東２*/ },
			{ { HexagonInt64(+1,-1,+0),0b10001110 }/*北西*/,{ HexagonInt64(-2,+1,+1),0b00110000 }/* 東２ */,{ HexagonInt64(+0,+2,-2),0b00000001 }/* 南２ */ },
			{ { HexagonInt64(+0,-1,+1),0b10011100 }/* 北 */,{ HexagonInt64(-1,+2,-1),0b00100001 }/*南南東*/,{ HexagonInt64(+2,+0,-2),0b00000010 }/*南西２*/ },
			{ { HexagonInt64(-1,+0,+1),0b10111000 }/*北東*/,{ HexagonInt64(+1,+1,-2),0b00000011 }/*南南西*/,{ HexagonInt64(+2,-2,+0),0b00000100 }/*北西２*/ },
			{ { HexagonInt64(-1,+1,+0),0b10110001 }/*南東*/,{ HexagonInt64(+2,-1,-1),0b00000110 }/* 西２ */,{ HexagonInt64(+0,-2,+2),0b00001000 }/* 北２ */ }
			/*------------------------------------------------------------------------------------------------*/
		};


		//
		unordered_map< HexagonInt64, unordered_map<HexagonInt64, uint8_t, hash_int64>, hash_int64 > m_indexes;

	};
}
