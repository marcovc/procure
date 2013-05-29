/*
 * taylor.hpp
 *
 *  Created on: May 27, 2013
 *      Author: marco
 */

#include "taylor.hpp"
#include <util/util.hpp>

namespace Procure {
namespace Detail {

struct ProxyForFactAlpha
{
	ProxyForFactAlpha(uint maxK) : fact(maxK+1)
	{
		fact[0] = 1;
		for(uint i = 1; i <= maxK; ++i)
				fact[i] = i*fact[i-1];
	}
	// first is the value of FactAlpha, second is the first elem in seq
	typedef std::pair<uint,uint> Seq;
	Seq k0(uint n) const		{	return {1,0};	}
	Seq n1(uint k) const		{	return {fact[k],k};	}
	void incN(Seq& s) const	{	s.second = 0;	}
	void incK(Seq& s) const
	{
		s.first*=s.second+1;
		++s.second;
	}
	std::vector<uint> fact;
};

std::list<uint> factAlpha(uint n,uint k)
{
	ProxyForFactAlpha p(k);
	auto m = multichoose(n,k,p);
	std::list<uint> r;
	for (auto v: m)
		r.push_back(v.first);
	return r;
}

struct ProxyForDeriv
{
	ProxyForDeriv(const Func& f,uint maxK) :
		vars(f.getArity()),deriv(maxK+1)
	{
		for (uint i = 0; i < vars.size(); ++i)
			vars[i] = f.getVar(i);
		deriv[0] = f.getExpr();
		for(uint i = 1; i <= maxK; ++i)
			deriv[i] = deriv[i-1].d(vars[vars.size()-1]);
	}
	// first is the value of Deriv, second is the var idx of the first elem
	// in seq
	typedef std::pair<Expr,uint> Seq;
	Seq k0(uint n) const		{	return {deriv[0],vars.size()-n};	}
	Seq n1(uint k) const		{	return {deriv[k],vars.size()-1};	}
	void incN(Seq& s) const
	{	--s.second;	}
	void incK(Seq& s) const
	{
		s.first = s.first.d(vars[s.second]);
	}
	std::vector<Var>	vars;
	std::vector<Expr> deriv;
};

std::list<Expr> deriv(const Func& f, uint k)
{
	ProxyForDeriv p(f,k);
	auto m = multichoose(f.getArity(),k,p);
	std::list<Expr> r;
	for (auto v: m)
		r.push_back(v.first);
	return r;
}

Expr TaylorModel::buildContribExpr(uint ha)
{
	const uint a = ha*2;
	const uint n = f.getArity();
	std::list<std::list<uint> > t = multichoose(n,a);
	std::list<uint> fa = factAlpha(n,a);
	std::list<Expr> d = deriv(f,a);
	assert(t.size()==fa.size());
	assert(t.size()==d.size());

	Expr ret(0.0);
	auto tit = t.begin();
	auto fait = fa.begin();
	auto dit = d.begin();
	for ( ; tit != t.end(); ++tit, ++fait, ++dit)
	{
		// if any component of t is odd then don't insert any term factors
		// since its contribution to the integral is 0.
		auto titit = tit->begin();
		for ( ; titit != tit->end(); ++titit)
			if (*titit % 2 == 1)
				break;
		if (titit != tit->end())
			continue;

		// if derivative is 0 then don't insert any term factors (for the
		// same reason).
		if (dit->isConst() and (*dit)()==0)
			continue;

		// otherwise insert term contribution into expr
		uint pai1 = 1;
		Expr p = 1;
		titit = tit->begin();
		for (uint i = 0; i < n; ++i,++titit)
		{
			uint ai = *titit;
			pai1 *= ai+1;
			p = p*pow(r[i],ai);
		}
		ret = ret + Interval(*fait).inverse()*
						(*dit)*p/static_cast<Real>(pai1);
	}
	return ret;
}

TaylorModel::TaylorModel(const Func& af, unsigned int order) :
		f(af),
		r(f.getArity())
{
	const uint n = f.getArity();

	expr1 = buildContribExpr(0);
	for (uint ha = 1; ha <= order/2; ++ha)
		expr1 = expr1 + buildContribExpr(ha);
	if ((order+1)%2 == 0)
		expr2 = buildContribExpr((order+1)/2);
	else
		expr2 = 0.0;
}

Interval TaylorModel::operator()(const std::vector<Interval>& x)
{
	const uint n = f.getArity();
	assert(n==x.size());

	Interval w = 1;	// volume of x
	for (uint i = 0; i < n; ++i)
	{
		Interval wi = x[i].getWidth();
		w *= wi;
		r[i].setValue(wi/2.0);
	}

	Interval ret(0);

	// compute expr1 (with m)
	for (uint i = 0; i < n; ++i)
		f.getVar(i).setValue(x[i].getMedian());
	ret += expr1();

	// compute expr2 (with x)
	for (uint i = 0; i < n; ++i)
		f.getVar(i).setValue(x[i]);
	ret += expr2();

	return ret*w;
}

} // Detail
} // Procure
