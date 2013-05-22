/*
 * expr.hpp
 *
 *  Created on: May 15, 2013
 *      Author: marco
 */

#ifndef PROCURE_EXPR_HPP_
#define PROCURE_EXPR_HPP_

#include <procure/model/var.hpp>

namespace Procure {

struct Expr;

namespace Detail {

struct IExprImpl
{
	virtual ~IExprImpl() {}
	virtual void print(std::ostream& os) const = 0;
	virtual Interval operator()() const = 0;
	virtual Expr d(const Var&) const = 0;
};

} // Detail


struct Expr: Util::SPImplIdiom<Detail::IExprImpl>
{
	typedef Util::SPImplIdiom<Detail::IExprImpl> Super;

	Expr(const Expr& expr) : Super(expr)
	{}

	Expr(Detail::IExprImpl* t) : Super(t)
	{}

	Expr(const Var& var);

	Expr(const Interval& i);

	Expr(const Real& i);

	void print(std::ostream& os) const;
	Interval operator()() const;
	Expr d(const Var&) const;
};

Expr operator+(const Expr&, const Expr&);
Expr operator-(const Expr&, const Expr&);
Expr operator*(const Expr&, const Expr&);
Expr operator/(const Expr&, const Expr&);

std::ostream& operator<<(std::ostream&, const Expr&);

} // Procure

#endif /* PROCURE_EXPR_HPP_ */
