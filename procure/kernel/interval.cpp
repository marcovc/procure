/*
 * interval.cpp
 *
 *  Created on: May 20, 2013
 *      Author: marco
 */

#include <procure/kernel/interval.hpp>

#include <procure/kernel/common.hpp>
#include <procure/kernel/rounding.hpp>

#include <cmath>
#include <iostream>

namespace Procure {
namespace Detail {

std::ostream& outputLb(std::ostream& os, const Real& lb,
                       unsigned int precision)
{
	Rounding rnd;
	rnd.setDownward();
	os.precision(precision);
	os << lb;
	return os;
}

std::ostream& outputUb(std::ostream& os, const Real& ub,
                       unsigned int precision)
{
	Rounding rnd;
	rnd.setUpward();
	os.precision(precision);
	os << ub;
	return os;
}

} // Detail
} // Procure
