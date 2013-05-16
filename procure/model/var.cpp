/*
 * var.hpp
 *
 *  Created on: May 15, 2013
 *      Author: marco
 */

#include "var.hpp"

namespace Procure {

namespace Detail {

VarImpl::VarImpl(const Interval& adomain) : domain(adomain)
{}

Interval& VarImpl::getDomain()
{	return domain;	}

const Interval& VarImpl::getDomain() const
{	return domain;	}

#ifdef HAVE_REALPAVER
Realpaver::RealVariable VarImpl::toRPVar() const
{	assert(0);	}
#endif

	private:
	Interval domain;
};

} // Detail


} // PROCURE

#endif /* PROCURE_VAR_HPP_ */
