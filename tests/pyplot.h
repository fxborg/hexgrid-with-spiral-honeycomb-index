#pragma once
#include <iostream>
#include <cstdio> 
#include <vector>
#include <stdexcept>
#include "point.h"

struct PyPlot {



	//	color_list = ["r", "g", "b", "c", "m", "y", "k", "w"]
	//	marker_list = ["o", "+", "*", ".", "x", "s", "d", "^", "v", ">", "<", "p", "h"]


	static void plot(const std::ostringstream & buf)
	{
#if defined(_WIN32)
		std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen("plot.py", "w"), _pclose);
#else
		std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("./plot.py", "w"), pclose);
#endif
		if (!pipe)throw std::runtime_error(std::string("Executable file not found."));
		fputs(buf.str().c_str(), pipe.get());
	}
};

