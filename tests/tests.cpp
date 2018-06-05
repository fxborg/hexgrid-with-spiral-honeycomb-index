#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "hexgrid.h"
#include <iostream>
#include "pyplot.h"
using std::cout;
using std::endl;
using std::string;
using std::ostringstream;
using namespace hex;

int main(int argc, char* argv[])
{
	Catch::Session().run(argc, argv);
	return 0;
}

TEST_CASE("Spiral Honeycomb Index", "[Hixagon Grid]") {
	HexGrid hexgrid(1, Layout::HORIZONTAL);
	int n = 50;
	ostringstream buf;

	vector<vector<int>> data = {};
	for (int q = -n; q <= n; q++) {
		for (int r = max(-n, -q - n); r <= min(n, -q + n); r++) {
			data.push_back({ q,r });
		}
	}
	HexagonInt64 origin(0, 0);
	Point o = hexgrid.hex_to_pixel(origin);

	buf << o.x << " " << o.y << endl;
	buf << "marker o" << endl;
	buf << "color r" << endl;
	buf << '$' << endl;


	for (auto & v : data) {
		HexagonInt64 inp(v[0], v[1]);
		HexagonInt64 hex = origin - inp;
		Point pt = hexgrid.hex_to_pixel(hex);
		hexgrid.add_point(hex, pt);
	}
	vector<string> color_list = { "r", "g", "b", "c", "m", "y", "k" };
	
	auto result_map=hexgrid.neighbors(origin, 15);
	for (auto & map : result_map) {
		for (auto & v : map.second) {
			auto pts = hexgrid.get_points(v);
			for (auto & pt : pts) {
				buf << pt.x << " " << pt.y << endl;
			}
		}
		string clr = color_list[(map.first % 7)];
		buf << "marker +" << endl;
		buf << "color " << clr << endl;
		buf << '$' << endl;
	}
	cout << buf.str();
	PyPlot::plot(buf);
}

TEST_CASE("Hixagon Grid", "Hixagon Grid") {
	HexGrid hexgrid(1, Layout::HORIZONTAL);
	SECTION("Hexagon Ring") {
	vector<HexagonInt64> hex_positions = {
		HexagonInt64(+0,+0,+0),	// 中央　位置
		HexagonInt64(+0,-1,+1),	// 南　　位置
		HexagonInt64(-1,+0,+1),	// 南西　位置
		HexagonInt64(-1,+1,+0),	// 北西　位置
		HexagonInt64(+0,+1,-1),	// 北　　位置
		HexagonInt64(+1,+0,-1),	// 北東　位置
		HexagonInt64(+1,-1,+0)	// 南東　位置
	};

	int n = 50;
	ostringstream buf;

	vector<vector<int>> data = {};
	for (int q = -n; q <= n; q++) {
		for (int r = max(-n, -q - n); r <= min(n, -q + n); r++) {
			data.push_back({ q,r });
		}
	}
	for (auto & v : data) {
		HexagonInt64 hex(v[0], v[1]);
		Point pt = hexgrid.hex_to_pixel(hex);
		hexgrid.add_point(hex, pt);
		//			cout << hex << "<--" << pt.x << "," << pt.y << endl;
	}

	HexagonInt64 origin(0, 0);

	// 起点のインデックス位置を確認
	int8_t seven_num = HexIndex::hex_to_seven_num(origin);//

	HexagonInt64 relative_pos = hex_positions[seven_num]; //インデックス位置からの座標
	HexagonInt64 origin_index_pos = origin - relative_pos;//インデックス位置
	int8_t filter = HexIndex::get_filter(seven_num);//インデックス用フィルタ

	unordered_map<int, vector<HexagonInt64>> merge_results{};


	vector<string> color_list = { "r", "g", "b", "c", "m", "y", "k" };
	vector<string> mark_list = { "^", "v", ">", "<", "d", "p", "h" };
	int mark = 0;


	for (int n = 0; n < 12; n++) {
		auto hexagons = hexgrid.walk_on_ring(origin_index_pos, n);
		for (auto const& hex : hexagons) {
			int dist = hexgrid.hex_distance(origin_index_pos - hex);
			Point pt = hexgrid.hex_to_pixel(hex);
			buf << pt.x << " " << pt.y << endl;
			string clr = color_list[((dist) % 7)];
			buf << "marker o" << endl;
			buf << "color " << clr << endl;
			buf << '$' << endl;

			unordered_map<int, vector<HexagonInt64>>  results = hexgrid.get_hexagons(origin_index_pos, relative_pos, filter, hex, dist);
			for (auto & res : results) {
				std::copy(res.second.begin(), res.second.end(), std::back_inserter(merge_results[res.first]));
			}
			string mk = "+";
			for (auto & res : results) {
				for (auto & v : res.second) {
					Point pt = hexgrid.hex_to_pixel(v);
					buf << pt.x << " " << pt.y << endl;

				}
				string clr = color_list[(res.first % 7)];
				buf << "marker " << mk << endl;
				buf << "color " << clr << endl;
				buf << '$' << endl;
			}
			mark++;

		}
	}
	PyPlot::plot(buf);

	}
}
