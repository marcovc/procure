/*
 * func.hpp
 *
 *  Created on: May 26, 2013
 *      Author: marco
 */

#ifndef PROCURE_KERNEL_FUNC_HPP_
#define PROCURE_KERNEL_FUNC_HPP_

#include <procure/kernel/interval.hpp>
#include <procure/kernel/var.hpp>
#include <procure/kernel/expr.hpp>

#include <vector>

namespace Procure {
namespace Detail {

struct Func
{
	Func(const std::vector<Var>& avars,
	     const Expr& aexpr) :
	    	 vars(avars),expr(aexpr) {}

	Interval operator()(const std::vector<Interval>& vals);
	unsigned int getArity() const {	return vars.size();	}
	Var& getVar(unsigned int i) {	return vars[i];	}
	const Var& getVar(unsigned int i) const {	return vars[i];	}
	Expr& getExpr() {	return expr; }
	const Expr& getExpr() const {	return expr; }
	Func d(const Var& v) const {	return Func(vars,expr.d(v));}
	private:
	std::vector<Var> vars;
	Expr expr;
};

std::ostream& operator<<(std::ostream&, const Func&);

} // Detail
}	// Procure



#endif /* PROCURE_KERNEL_FUNC_HPP_ */
