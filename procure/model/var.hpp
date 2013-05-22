/*
 * var.hpp
 *
 *  Created on: May 15, 2013
 *      Author: marco
 */

#ifndef PROCURE_MODEL_VAR_HPP_
#define PROCURE_MODEL_VAR_HPP_

#include <procure/kernel/common.hpp>
#include <procure/kernel/interval.hpp>
#include <procure/util/pimpl.hpp>

namespace Procure {

namespace Detail {

struct VarImpl
{
	VarImpl(const Interval& domain, const std::string& name);
	VarImpl(const Real& r, const std::string& name);
	VarImpl(const Real& lb, const Real& ub, const std::string& name);

	Interval& getDomain();
	const Interval& getDomain() const;

	const std::string& getName() const;

	private:
	Interval domain;
	std::string name;
};

} // Detail

struct Var: Util::SPImplIdiom<Detail::VarImpl>
{
	typedef Util::SPImplIdiom<Detail::VarImpl> Super;

	Var(const Var& var) : Super(var)
	{}

	Var(const Interval& i, const std::string& name = "") :
		Super(new Detail::VarImpl(i,name))
	{}

	Var(const Real& r, const std::string& name = "") :
		Super(new Detail::VarImpl(r,name))
	{}

	Var(const Real& lb, const Real& ub, const std::string& name = "") :
		Super(new Detail::VarImpl(lb,ub,name))
	{}

	Var(Detail::VarImpl* t) : Super(t)
	{}

	Interval& getDomain()
	{	return getImpl().getDomain();	}
	const Interval& getDomain() const
	{	return getImpl().getDomain();	}

	const std::string& getName() const
	{	return getImpl().getName();	}

	bool operator==(const Var& s) const
	{	return getPImpl() == s.getPImpl();	}
};

std::ostream& operator<<(std::ostream&, const Var& v);

} // PROCURE

#endif /* PROCURE_MODEL_VAR_HPP_ */
