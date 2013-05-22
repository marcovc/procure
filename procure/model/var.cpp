/*
 * var.hpp
 *
 *  Created on: May 15, 2013
 *      Author: marco
 */

#include "var.hpp"

#include <sstream>

namespace Procure {

namespace Detail {


VarImpl::VarImpl(const Interval& adomain, const std::string& aname) :
		domain(adomain),name(aname)
{}

VarImpl::VarImpl(const Real& r, const std::string& aname) :
		domain(r),name(aname)
{}

VarImpl::VarImpl(const Real& lb, const Real& ub,
                 const std::string& aname) :
		domain(lb,ub),name(aname)
{}

Interval& VarImpl::getDomain()
{	return domain;	}

const Interval& VarImpl::getDomain() const
{	return domain;	}

const std::string& VarImpl::getName() const
{	return name;	}

} // Detail

std::ostream& operator<<(std::ostream& os, const Var& v)
{
	if (v.getName()=="")
		os << "Var@" << &v.getDomain();
	else
		os << v.getName();
	//os << ":" << v.getDomain();
	return os;
}

} // PROCURE

