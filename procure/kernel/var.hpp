/*
 * var.hpp
 *
 *  Created on: May 15, 2013
 *      Author: marco
 */

#ifndef PROCURE_KERNEL_VAR_HPP_
#define PROCURE_KERNEL_VAR_HPP_

#include <procure/kernel/common.hpp>
#include <procure/kernel/interval.hpp>
#include <procure/util/pimpl.hpp>

namespace Procure {

namespace Detail {

struct VarImpl
{
	VarImpl(const std::string& name);

	Interval& getValue();
	const Interval& getValue() const;

	void setValue(const Interval& value);

	const std::string& getName() const;

	private:
	Interval value;
	std::string name;
};

struct VarExpr;
struct Func;
struct TaylorModel;

} // Detail

struct Var: Util::SPImplIdiom<Detail::VarImpl>
{
	typedef Util::SPImplIdiom<Detail::VarImpl> Super;

	Var(const Var& var) : Super(var)
	{}

	Var(const std::string& name = "") :
		Super(new Detail::VarImpl(name))
	{}

	Var(Detail::VarImpl* t) : Super(t)
	{}

	const std::string& getName() const
	{	return getImpl().getName();	}

	bool operator==(const Var& s) const
	{	return getPImpl() == s.getPImpl();	}

	private:
	friend struct Detail::VarExpr;
	friend struct Detail::Func;
	friend struct Detail::TaylorModel;

	Interval& getValue()
	{	return getImpl().getValue();	}
	const Interval& getValue() const
	{	return getImpl().getValue();	}
	void setValue(const Interval& i)
	{	getImpl().setValue(i);	}
};

std::ostream& operator<<(std::ostream&, const Var& v);

} // PROCURE

#endif /* PROCURE_KERNEL_VAR_HPP_ */
