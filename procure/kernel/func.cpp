/*
 * func.hpp
 *
 *  Created on: May 26, 2013
 *      Author: marco
 */

#include <procure/kernel/func.hpp>

#include <assert.h>

namespace Procure {
namespace Detail {

Interval Func::operator()(const std::vector<Interval>& vals)
{
	assert(vals.size() == vars.size());
	for (unsigned int i = 0; i < vars.size(); ++i)
		vars[i].setValue(vals[i]);
	return expr();
}

std::ostream& operator<<(std::ostream& os, const Func& f)
{
	os << "Func(";
	if (f.getArity()>0)
	{
		os << f.getVar(0);
		for (unsigned int i = 1; i < f.getArity(); ++i)
			os << "," << f.getVar(i);
	}
	os << "):=" << f.getExpr();
	return os;
}

} // Detail
}	// Procure



