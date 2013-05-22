/*
 ============================================================================
 Name        : example.cpp
 Author      : Marco Correia
 Version     :
 Copyright   : 2013 Copyright © Marco Correia (marco.v.correia@gmail.com)
 Description : Uses shared library to print greeting
 To run the resulting executable the LD_LIBRARY_PATH must be
 set to ${project_loc}/procure/.libs
 Alternatively, libtool creates a wrapper shell script in the
 build directory of this program which can be used to run it.
 Here the script will be called example.
 ============================================================================
 */

#include <procure/procure.hpp>

#include <iostream>

using namespace Procure;

int main(void)
{
	Var x1(2.0,5.6,"x1");
	Var x2(-4.3,1,"x2");
	std::cout << x1+x2 << std::endl;
	std::cout << (x1+x2)() << std::endl;
	std::cout << (x1*x2).d(x2) << std::endl;
	std::cout << (x1*x2).d(x2)() << std::endl;
}
