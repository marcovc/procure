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


VarImpl::VarImpl(const std::string& aname) :
		name(aname)
{}

Interval& VarImpl::getValue()
{	return value;	}

const Interval& VarImpl::getValue() const
{	return value;	}

void VarImpl::setValue(const Interval& avalue)
{	value = avalue;	}

const std::string& VarImpl::getName() const
{	return name;	}

} // Detail

std::ostream& operator<<(std::ostream& os, const Var& v)
{
	if (v.getName()=="")
		os << "Var@" << &v.getImpl();
	else
		os << v.getName();
	return os;
}

} // PROCURE

