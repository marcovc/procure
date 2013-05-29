/*
 * common.hpp
 *
 *  Created on: May 16, 2013
 *      Author: marco
 */

#ifndef PROCURE_KERNEL_COMMON_HPP_
#define PROCURE_KERNEL_COMMON_HPP_

#include <procure/procure-config.hpp>

#define PROCURE_UNREACHABLE()\
throw Procure::Exception::Internal()

namespace Procure {

typedef double Real;
typedef unsigned int uint;

namespace Detail  {

struct StaticInitializer  
{
	StaticInitializer();
};

} // Detail

} // Procure

#endif /* PROCURE_KERNEL_COMMON_HPP_ */
