#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
namespace hex {

	/**
	 * Hexagon class
	 */
	template<typename DataT>
	class Hexagon	{
	public:
		// constructors
		explicit Hexagon() : q_(0), r_(0), s_(0) { }
		explicit Hexagon(DataT q, DataT r) : q_(q), r_(r), s_(-q - r) {}
		explicit Hexagon(DataT q, DataT r, DataT s) : q_(q), r_(r), s_(-q - r) {}

		// copy constructor
		Hexagon(const Hexagon& hex) : q_(hex.q()), r_(hex.r()), s_(hex.s()) {}
	
		//--- operators ---
		// = operator
		Hexagon<DataT>& operator = (const Hexagon& hex)
		{
			if (&hex != this){
				q_ = hex.q();
				r_ = hex.r();
				s_ = hex.s();

			}
			return *this;
		}
		// equal operator
		bool operator==(const Hexagon& hex) const
			{ return q() == hex.q() && r() == hex.r() && s() == hex.s();}
		// not equal operator
		bool operator!=(const Hexagon& hex) const	{return !(*this == hex);}

		// greater operator
		bool operator<(const Hexagon& hex) const
			{ return (abs(q()) + abs(r()) + abs(s())) < (abs(hex.q()) + abs(hex.r()) + abs(hex.s()));}

		// less operator
		bool operator>(const Hexagon& hex) const
			{ return (abs(q()) + abs(r()) + abs(s())) > (abs(hex.q()) + abs(hex.r()) + abs(hex.s()));}

		// add operator
		friend Hexagon operator+(const Hexagon& hex0, const Hexagon& hex1)
			{ return Hexagon<DataT>(hex0.q() + hex1.q(), hex0.r() + hex1.r(), hex0.s() + hex1.s());}

		// subtract operator
		friend Hexagon operator-(const Hexagon& hex0, const Hexagon& hex1)
			{ return Hexagon<DataT>(hex0.q() - hex1.q(), hex0.r() - hex1.r(), hex0.s() - hex1.s());}

		// multiplication( hex * n )
		friend Hexagon operator*(const Hexagon& hex, const DataT& n)
			{ return Hexagon<DataT>((hex.q() * n), (hex.r() * n), (hex.s() * n));}

		// multiplication( n * hex ) 
		friend Hexagon operator*(const DataT& n, const Hexagon& hex)
			{ return Hexagon<DataT>((hex.q() * n), (hex.r() * n), (hex.s() * n));}

		
		friend std::ostream& operator << (std::ostream& os, const Hexagon& hex)
		{
			 os<< "(" << hex.q() << "," << hex.r() << "," << hex.s() << ")";
			return os;
		};



		// --- members  ---
		DataT& q() { return q_; }
		DataT& r() { return r_; }
		DataT& s() { return s_; }

		DataT q() const { return q_; }
		DataT r() const { return r_; }
		DataT s() const { return s_; }

	private:
		DataT q_;
		DataT r_;
		DataT s_;
	};
	/* –9,223,372,036,854,775,808 to 9,223,372,036,854,775,807 */
	using HexagonInt64 = Hexagon<int64_t>;
	/* -128 to 127 */
	using HexagonInt8 = Hexagon<int8_t>;

	struct hash_int64 {
		typedef size_t result_type;
		size_t operator()(const HexagonInt64& h) const 
		{
			std::hash<int64_t> fn_hash;
			size_t hq = fn_hash(h.q());
			size_t hr = fn_hash(h.r());
			return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
		}
	};
	struct hash_int8 {
		typedef size_t result_type;
		size_t operator()(const HexagonInt8& h) const 
		{
			std::hash<int8_t> fn_hash;
			size_t hq = fn_hash(h.q());
			size_t hr = fn_hash(h.r());
			return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
		}
	};


}