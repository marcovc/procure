/*
 * taylor.hpp
 *
 *  Created on: May 27, 2013
 *      Author: marco
 */

#ifndef PROCURE_QUADRATURE_TAYLOR_HPP_
#define PROCURE_QUADRATURE_TAYLOR_HPP_

#include <procure/kernel/expr.hpp>
#include <procure/kernel/func.hpp>

#include <list>

namespace Procure {
namespace Detail {

std::list<uint> factAlpha(uint n,uint k);
std::list<Expr> deriv(const Func& f, uint k);

struct TaylorModel
{
	typedef std::vector<uint> MultiIndex;

	TaylorModel(const Func& f, uint order);

	Interval operator()(const std::vector<Interval>& x);

	private:
	Expr buildContribExpr(uint ha);

	Func f;
	std::vector<Var>	r;
	Expr expr1;
	Expr expr2;
};

} // Detail
} // Procure

#endif /* PROCURE_QUADRATURE_TAYLOR_HPP_ */
