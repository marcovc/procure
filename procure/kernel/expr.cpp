/*
 * expr.cpp
 *
 *  Created on: May 22, 2013
 *      Author: marco
 */

#include <procure/kernel/expr.hpp>
#include <vector>

namespace Procure {

namespace Detail {

struct VarExpr : IExprImpl
{
	VarExpr(const Var& avar) : var(avar) {}
	void print(std::ostream& os) const
	{	os << var;	}
	Interval operator()() const
	{	return var.getValue();	}
	Expr d(const Var& v) const
	{	return (v == var)?1.0:0.0;	}
	Expr simplify() const
	{	return var;	}
	bool isConst() const
	{	return false;	}
	std::function<void()> getEvalFunc(Interval& r) const
	{
		Var avar(var);
		return [&r,avar](){	 r = avar.getValue();};
	}
	uint getArity() const
	{	return 0;	}
	void flatten(const Expr& self, std::set<Expr,Detail::ExprLessId>& s) const
	{	s.insert(self);	}

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
	Expr simplify() const
	{	return i;	}
	bool isConst() const
	{	return true;	}
	std::function<void()> getEvalFunc(Interval& r) const
	{
		Interval ai(i);
		return [&r,ai](){	 r = ai;};
	}
	uint getArity() const
	{	return 0;	}
	void flatten(const Expr& self, std::set<Expr,Detail::ExprLessId>& s) const
	{	s.insert(self);	}
	Interval i;
};

struct IUnaryExprImpl : IExprImpl
{
	IUnaryExprImpl(const Expr& ae1) :
		e1(ae1) {}
	bool isConst() const
	{	return e1.isConst();	}
	uint getArity() const
	{	return 1;	}
	void flatten(const Expr& self, std::set<Expr,Detail::ExprLessId>& s) const
	{	s.insert(self); e1.flatten(s);	}
	Expr e1;
};

struct IBinaryExprImpl : IExprImpl
{
	IBinaryExprImpl(const Expr& ae1, const Expr& ae2) :
		e1(ae1),e2(ae2) {}
	bool isConst() const
	{	return e1.isConst() and e2.isConst();	}
	uint getArity() const
	{	return 2;	}
	void flatten(const Expr& self, std::set<Expr,Detail::ExprLessId>& s) const
	{	s.insert(self); e1.flatten(s);	e2.flatten(s);}
	Expr e1;
	Expr e2;
};


struct SymExprImpl : IUnaryExprImpl
{
	SymExprImpl(const Expr& ae1) :
		IUnaryExprImpl(ae1) {}
	void print(std::ostream& os) const
	{	os << "(-1)*"; e1.print(os);	}
	Interval operator()() const
	{	return -e1();	}
	Expr d(const Var& v) const
	{	return -e1.d(v);	}
	Expr simplify() const
	{
		auto s1 = e1.simplify();
		if (s1.isConst() and s1()<=0)
			return -s1();
		return -s1;
	}
	std::function<void()> getEvalFunc(Interval& r, Interval& a1) const
	{	return [&](){	 r = -a1;};	}
};

struct InvExprImpl : IUnaryExprImpl
{
	InvExprImpl(const Expr& ae1) :
		IUnaryExprImpl(ae1) {}
	void print(std::ostream& os) const
	{	os << "("; e1.print(os); os << ")^(-1)";	}
	Interval operator()() const
	{	return e1().inverse();	}
	Expr d(const Var& v) const
	{	return -e1.d(v)/e1;	}
	Expr simplify() const
	{
		auto s1 = e1.simplify();
		if (s1.isConst() and s1()==1)
			return 1;
		return 1/s1;
	}
	std::function<void()> getEvalFunc(Interval& r, Interval& a1) const
	{	return [&](){	 r = a1.inverse();};	}
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
	Expr simplify() const
	{
		auto s1 = e1.simplify();
		auto s2 = e2.simplify();
		if (s1.isConst() and s1()==0)
			return s2;
		if (s2.isConst() and s2()==0)
			return s1;
		return s1+s2;
	}
	std::function<void()> getEvalFunc(Interval& r, Interval& a1,
	                                  Interval& a2) const
	{	return [&](){	 r = a1+a2;};	}
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
	Expr simplify() const
	{
		auto s1 = e1.simplify();
		auto s2 = e2.simplify();
		if (s1.isConst() and s1()==0)
			return (-s2).simplify();
		if (s2.isConst() and s2()==0)
			return s1;
		return s1-s2;
	}
	std::function<void()> getEvalFunc(Interval& r, Interval& a1,
	                                  Interval& a2) const
	{	return [&](){	 r = a1-a2;};	}
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
	Expr simplify() const
	{
		auto s1 = e1.simplify();
		auto s2 = e2.simplify();
		if (s1.isConst())
		{
			if (s1()==0)
				return 0.0;
			if (s1()==1)
				return s2;
		}
		if (s2.isConst())
		{
			if (s2()==0)
				return 0.0;
			if (s2()==1)
				return s1;
		}
		return s1*s2;
	}
	std::function<void()> getEvalFunc(Interval& r, Interval& a1,
	                                  Interval& a2) const
	{	return [&](){	 r = a1*a2;};	}
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
	Expr simplify() const
	{
		auto s1 = e1.simplify();
		auto s2 = e2.simplify();
		if (s1.isConst())
		{
			if (s1()==0)
				return 0.0;
			if (s1()==1)
				return inverse(s2);
		}
		if (s2.isConst())
		{
			if (s2()==1)
				return s1;
		}
		return s1/s2;
	}
	std::function<void()> getEvalFunc(Interval& r, Interval& a1,
	                                  Interval& a2) const
	{	return [&](){	 r = a1/a2;};	}
};

struct PowExprImpl : IBinaryExprImpl
{
	PowExprImpl(const Expr& ae1, const Expr& ae2) :
		IBinaryExprImpl(ae1,ae2) {}
	void print(std::ostream& os) const
	{	e1.print(os);	os << "^";	 e2.print(os);	}
	Interval operator()() const
	{
		Interval ie1 = e1();
		Interval ie2 = e2();
		if (ie2.isSingleton() and
				static_cast<Real>(static_cast<int>(ie2.getLb()))==ie2.getLb())
			return pow(ie1,static_cast<int>(ie2.getLb()));
		return pow(e1(),e2());
	}
	// FIXME: complete
	Expr d(const Var& v) const
	{	PROCURE_UNREACHABLE();	}

	// FIXME: complete
	Expr simplify() const
	{	return pow(e1,e2);	}

	std::function<void()> getEvalFunc(Interval& r, Interval& a1,
	                                  Interval& a2) const
	{	return [&](){	 r = pow(a1,a2);};	}
};

}

#define FUN_DEF_1(Fun,Obj) \
		Expr Fun(const Expr& e1) \
		{	return new Obj(e1);	}

#define FUN_DEF_2(Fun,Obj) \
		Expr Fun(const Expr& e1, const Expr& e2) \
		{	return new Obj(e1,e2);	}

FUN_DEF_1(operator-,Detail::SymExprImpl)
FUN_DEF_1(inverse,Detail::InvExprImpl)

FUN_DEF_2(operator+,Detail::SumExprImpl)
FUN_DEF_2(operator-,Detail::SubExprImpl)
FUN_DEF_2(operator*,Detail::MulExprImpl)
FUN_DEF_2(operator/,Detail::DivExprImpl)
FUN_DEF_2(pow,Detail::PowExprImpl)

#undef FUN_DEF_1
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
	return getImpl().d(v).simplify();
}

bool Expr::isConst() const
{
	return getImpl().isConst();
}

Expr Expr::simplify() const
{
	return getImpl().simplify();
}

std::function<void()> Expr::getEvalFunc(Interval& r) const
{	return getImpl().getEvalFunc(r);	}

std::function<void()> Expr::getEvalFunc(Interval& r, Interval& a1) const
{	return getImpl().getEvalFunc(r,a1);	}

std::function<void()> Expr::getEvalFunc(Interval& r, Interval& a1,
                                  Interval& a2) const
{	return getImpl().getEvalFunc(r,a1,a2);	}

uint Expr::getArity() const
{	return getImpl().getArity();	}

void Expr::flatten(std::set<Expr,Detail::ExprLessId>& s) const
{	getImpl().flatten(*this,s);	}

std::ostream& operator<<(std::ostream& os, const Expr& expr)
{
	expr.print(os);
	return os;
}

} // Procure
