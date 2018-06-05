#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include <unordered_map>
using std::unordered_map;
using std::max;

namespace hex {

	struct HexPattern {
		static int get_id(const HexagonInt64 &hex) { return ((int(hex.q()) + 1) << 2) | (int(hex.r()) + 1); }

		static unordered_map<int8_t, int8_t> get(const HexagonInt64 & hex) {return get(hex.q(), hex.r(), hex.s());}

		static unordered_map<int8_t, int8_t> get(int64_t q, int64_t r, int64_t s)
		{
			if (q == 0 && r == 0) 	// 中央	
				return { { 6,1 },{ 2,1 },{ 9,1 },{ 5,0 },{ 1,1 } ,{ 8,1 },{ 4,1 } };

			if (q == 0)	{
				if (r > 0)			//北　角
					return { { 6,1 },{ 2,1 },{ 9,1 },{ 5,0 },{ 1,0 } ,{ 8,0 },{ 4,-1 } };
				else				//南　角
					return { { 6,-1 },{ 2,0 },{ 9,0 },{ 5,0 },{ 1,1 } ,{ 8,1 },{ 4,1 } };
			}else if (r == 0){
				if (q > 0)			//北東　角
					return { { 6,1 },{ 2,0 },{ 9,1 },{ 5,0 },{ 1,-1 } ,{ 8,1 },{ 4,0 } };
				else				//南西　角
					return { { 6,0 },{ 2,1 },{ 9,-1 },{ 5,0 },{ 1,1 },{ 8,0 },{ 4,1 } };
			}else if (s == 0){
				if (r > 0)			//北西　角
					return { { 6,1 },{ 2,1 },{ 9,0 },{ 5,0 },{ 1,1 },{ 8,-1 },{ 4,0 } };
				else				//南東　角
					return { { 6,0 },{ 2,-1 },{ 9,1 },{ 5,0 },{ 1,0 },{ 8,1 },{ 4,1 } };
			}else{
				const int64_t aq{ abs(q) };
				const int64_t ar{ abs(r) };
				const int64_t as{ abs(s) };

				if (aq > ar && aq > as)
					if (q > 0)		// 東　辺
						return { { 6,0 },{ 2,-1 },{ 9,1 },{ 5,0 },{ 1,-1 } ,{ 8,1 },{ 4,0 } };
					else			// 西　辺
						return { { 6,0 },{ 2,1 },{ 9,-1 },{ 5,0 },{ 1,1 } ,{ 8,-1 },{ 4,0 } };
				else
					if (ar > as)
						if (r > 0)	//北西　辺
							return { { 6,1 },{ 2,1 },{ 9,0 },{ 5,0 },{ 1,0 } ,{ 8,-1 },{ 4,-1 } };
						else		//南東　辺
							return { { 6,-1 },{ 2,-1 },{ 9,0 },{ 5,0 },{ 1,0 } ,{ 8,1 },{ 4,1 } };
					else
						if (s > 0)	//南西　辺
							return { { 6,-1 },{ 2,0 },{ 9,-1 },{ 5,0 },{ 1,1 } ,{ 8,0 },{ 4,1 } };
						else		//北東　辺
							return { { 6,1 },{ 2,0 },{ 9,1 },{ 5,0 },{ 1,-1 } ,{ 8,0 },{ 4,-1 } };
			}
		}
	};
}