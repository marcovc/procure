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

#include "procure.hpp"

#include <realpaver/Realpaver.h>
#include <iostream>

int main(void)
{
	Realpaver::Variable* pv1 = new Realpaver::RealVariable("v1");
	Realpaver::Expression e(pv1);
	std::cout << e+2 << std::endl;
}
