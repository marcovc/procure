/*
 * expr.cpp
 *
 *  Created on: May 22, 2013
 *      Author: marco
 */

#include <procure/model/expr.hpp>

namespace Procure {

namespace Detail {

struct VarExpr : IExprImpl
{
	VarExpr(const Var& avar) : var(avar) {}
	void print(std::ostream& os) const
	{	os << var;	}
	Interval operator()() const
	{	return var.getDomain();	}
	Expr d(const Var& v) const
	{	return (v == var)?1.0:0.0;	}
	Var var;
};

struct ConstExpr : IExprImpl
{
	ConstExpr(const Interval& ai) : i(ai) {}
	void print(std::ostream& os) const
	{	os << i;	}
	Interval operator()() const
	{	return i;	}
	Expr d(const Var& v) const
	{	return 0.0;	}
	Interval i;
};

struct IBinaryExprImpl : IExprImpl
{
	IBinaryExprImpl(const Expr& ae1, const Expr& ae2) :
		e1(ae1),e2(ae2) {}
	Expr e1;
	Expr e2;
};

struct SumExprImpl : IBinaryExprImpl
{
	SumExprImpl(const Expr& ae1, const Expr& ae2) :
		IBinaryExprImpl(ae1,ae2) {}
	void print(std::ostream& os) const
	{	e1.print(os);	os << "+";	 e2.print(os);	}
	Interval operator()() const
	{	return e1() + e2();	}
	Expr d(const Var& v) const
	{	return e1.d(v)+e2.d(v);	}
};

struct SubExprImpl : IBinaryExprImpl
{
	SubExprImpl(const Expr& ae1, const Expr& ae2) :
		IBinaryExprImpl(ae1,ae2) {}
	void print(std::ostream& os) const
	{	e1.print(os);	os << "-";	 e2.print(os);	}
	Interval operator()() const
	{	return e1() - e2();	}
	Expr d(const Var& v) const
	{	return e1.d(v)-e2.d(v);	}
};

struct MulExprImpl : IBinaryExprImpl
{
	MulExprImpl(const Expr& ae1, const Expr& ae2) :
		IBinaryExprImpl(ae1,ae2) {}
	void print(std::ostream& os) const
	{	e1.print(os);	os << "*";	 e2.print(os);	}
	Interval operator()() const
	{	return e1() * e2();	}
	Expr d(const Var& v) const
	{	return e1.d(v)*e2  + e1*e2.d(v);	}
};

struct DivExprImpl : IBinaryExprImpl
{
	DivExprImpl(const Expr& ae1, const Expr& ae2) :
		IBinaryExprImpl(ae1,ae2) {}
	void print(std::ostream& os) const
	{	e1.print(os);	os << "/";	 e2.print(os);	}
	Interval operator()() const
	{	return e1() / e2();	}
	Expr d(const Var& v) const
	{	return (e1.d(v)*e2  - e1*e2.d(v))/(e2*e2);	}	// FIXME: e2*e2 -> sqr(e2)
};

}


#define FUN_DEF_2(Fun,Obj) \
		Expr Fun(const Expr& e1, const Expr& e2) \
		{	return new Obj(e1,e2);	}

FUN_DEF_2(operator+,Detail::SumExprImpl)
FUN_DEF_2(operator-,Detail::SubExprImpl)
FUN_DEF_2(operator*,Detail::MulExprImpl)
FUN_DEF_2(operator/,Detail::DivExprImpl)

#undef FUN_DEF_2


Expr::Expr(const Var& var) :
		Super(new Detail::VarExpr(var))
{}

Expr::Expr(const Interval& i) :
		Super(new Detail::ConstExpr(i))
{}

Expr::Expr(const Real& r) :
		Super(new Detail::ConstExpr(r))
{}

void Expr::print(std::ostream& os) const
{
	getImpl().print(os);
}

Interval Expr::operator()() const
{
	return getImpl()();
}

Expr Expr::d(const Var& v) const
{
	return getImpl().d(v);
}

std::ostream& operator<<(std::ostream& os, const Expr& expr)
{
	expr.print(os);
	return os;
}

} // Procure
