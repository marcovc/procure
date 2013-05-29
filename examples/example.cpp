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

#include <procure/util/util.hpp>
#include <procure/quadrature/taylor.hpp>

using namespace Procure;

int main(void)
{
	Var x1("x1");
	Var x2("x2");
	Expr e = x1*x2*x2+x1*x1;
	std::cout << "e=" << e << std::endl;
	std::cout << "de/dx2=" << e.d(x2).simplify() << std::endl;

	Detail::Func f({x1,x2},e);
	std::cout << "f=" << f << std::endl;
	std::cout << "f(1,2)=" << f({1,2}) << std::endl;
	std::cout << "f([0..1],[-1..0])=" << f({{0,1},{-1,0}}) << std::endl;

	std::cout << "df/dx2=" << f.d(x2) << std::endl;

	auto m = Detail::multichoose(2,2);
	for (auto s: m)
	{
		for (auto e: s)
			std::cout << e << ",";
		std::cout << "|";
	}
	std::cout << std::endl;

	auto fa = Detail::factAlpha(2,2);
	for (auto e: fa)
		std::cout << e << ",";
	std::cout << std::endl;

	auto fd = Detail::deriv(f,2);
	for (auto e: fd)
		std::cout << e << ",";
	std::cout << std::endl;

	Detail::TaylorModel tm(f,1);
	std::cout << tm({{0,1},{0,1}}) << std::endl;

	std::set<Expr,Detail::ExprLessId> nodes;
	e.flatten(nodes);
	std::cout << nodes.size() << std::endl;
}
