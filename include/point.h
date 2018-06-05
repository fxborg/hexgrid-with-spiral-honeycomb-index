#pragma once
struct Point
{
	
	explicit Point(const double x_, const double y_) : x(x_), y(y_) {}
	Point(const Point& pt) : x(pt.x), y(pt.y) {}

	double x;
	double y;

};
