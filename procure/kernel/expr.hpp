/*
 * expr.hpp
 *
 *  Created on: May 15, 2013
 *      Author: marco
 */

#ifndef PROCURE_KERNEL_EXPR_HPP_
#define PROCURE_KERNEL_EXPR_HPP_

#include <procure/kernel/common.hpp>
#include <procure/kernel/var.hpp>

#include <set>

namespace Procure {

struct Expr;

namespace Detail {

struct ExprLessId;

struct IExprImpl
{
	virtual ~IExprImpl() {}
	virtual void print(std::ostream& os) const = 0;
	virtual Interval operator()() const = 0;
	virtual Expr d(const Var&) const = 0;
	virtual bool isConst() const = 0;
	virtual Expr simplify() const = 0;
	virtual uint getArity() const = 0;
	virtual void flatten(const Expr&,std::set<Expr,ExprLessId>&) const = 0;

	virtual std::function<void()> getEvalFunc(Interval& r) const
	{	PROCURE_UNREACHABLE();	}
	virtual std::function<void()> getEvalFunc(Interval& r, Interval& a1) const
	{	PROCURE_UNREACHABLE();	}
	virtual std::function<void()> getEvalFunc(Interval& r, Interval& a1,
	                                  Interval& a2) const
	{	PROCURE_UNREACHABLE();	}
};

} // Detail


struct Expr: Util::SPImplIdiom<Detail::IExprImpl>
{
	typedef Util::SPImplIdiom<Detail::IExprImpl> Super;

	Expr() : Super(NULL) {}

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
	bool isConst() const;
	Expr simplify() const;
	uint getArity() const;

	std::function<void()> getEvalFunc(Interval& r) const;
	std::function<void()> getEvalFunc(Interval& r, Interval& a1) const;
	std::function<void()> getEvalFunc(Interval& r, Interval& a1,
	                                  Interval& a2) const;

	void flatten(std::set<Expr,Detail::ExprLessId>& s) const;
};

Expr operator-(const Expr&);
Expr inverse(const Expr&);
Expr operator+(const Expr&, const Expr&);
Expr operator-(const Expr&, const Expr&);
Expr operator*(const Expr&, const Expr&);
Expr operator/(const Expr&, const Expr&);
Expr pow(const Expr&, const Expr&);

std::ostream& operator<<(std::ostream&, const Expr&);

namespace Detail {

struct ExprLessId
{
	bool operator()(const Expr& e1, const Expr& e2) const
	{	return e1.getPImpl() < e2.getPImpl();	}
};

} // Detail

} // Procure

#endif /* PROCURE_KERNEL_EXPR_HPP_ */
