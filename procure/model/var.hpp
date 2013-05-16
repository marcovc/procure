/*
 * var.hpp
 *
 *  Created on: May 15, 2013
 *      Author: marco
 */

#ifndef PROCURE_MODEL_VAR_HPP_
#define PROCURE_MODEL_VAR_HPP_

#include <procure/config.h>

namespace Procure {

namespace Detail {

struct VarImpl
{
	VarImpl(const Interval& domain);

	Interval& getDomain();
	const Interval& getDomain() const;

#ifdef HAVE_REALPAVER
	Realpaver::RealVariable toRPVar() const = 0;
#endif

	private:
	Interval domain;
};

} // Detail

struct Var: Util::SPImplIdiom<VarImpl>
{
	typedef Casper::Util::SPImplIdiom<Detail::VarImpl> Super;

	Var(const Var& var) : Super(var)
	{}

	Var(const Interval& i) : Super(new VarImpl(i))
	{}

	Var(Detail::VarImpl* t) : Super(t)
	{}

	Interval& getDomain()
	{	return getImpl().getDomain();	}
	const Interval& getDomain() const
	{	return getImpl().getDomain();	}

#ifdef HAVE_REALPAVER
	Realpaver::RealVariable toRPVar() const
	{	return getImpl().toRPVar();	}
#endif
};

} // PROCURE

#endif /* PROCURE_MODEL_VAR_HPP_ */
