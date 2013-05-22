/*
 * rounding.hpp
 *
 *  Created on: May 21, 2013
 *      Author: marco
 */

#ifndef PROCURE_KERNEL_ROUNDING_HPP_
#define PROCURE_KERNEL_ROUNDING_HPP_

#include <fenv.h>

/*
 * 	This is not portable (does not compile with msvc, for example).
 */

namespace Procure {
namespace Detail {

/**
 *		This class gives access to rounding direction in the fpu.
 *		The current rounding direction is always restored upon destruction
 *		since the underlying interval library may require a fixed rounding
 *		direction (e.g. gaol).
 */
struct Rounding
{
	Rounding() : state(fegetround())
	{}
	~Rounding()
	{	fesetround(state);	}
	void setDownward()
	{	fesetround	(FE_DOWNWARD);	}
	void setUpward()
	{	fesetround	(FE_UPWARD);	}
	void setNearest()
	{	fesetround	(FE_TONEAREST);	}
	int state;
};

} // Detail
} // Procure



#endif /* PROCURE_KERNEL_ROUNDING_HPP_ */
