/*
 * common.hpp
 *
 *  Created on: May 16, 2013
 *      Author: marco
 */

#ifndef PROCURE_KERNEL_COMMON_HPP_
#define PROCURE_KERNEL_COMMON_HPP_

#include <procure/config.h>

/*
#if HAVE_GAOL
#define PROCURE_INTERVAL_USES_GAOL
#elif HAVE_BOOST
#define PROCURE_INTERVAL_USES_BOOST
#endif
*/

namespace Procure {

typedef double Real;

namespace Detail  {

struct StaticInitializer  
{
	StaticInitializer();
};

} // Detail

} // Procure

#endif /* PROCURE_KERNEL_COMMON_HPP_ */
