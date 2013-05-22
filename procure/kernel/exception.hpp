/*
 * exception.hpp
 *
 *  Created on: May 16, 2013
 *      Author: marco
 */

#ifndef PROCURE_KERNEL_EXCEPTION_HPP_
#define PROCURE_KERNEL_EXCEPTION_HPP_

#include <stdexcept>

namespace Procure {
namespace Exception {

struct Unsupported : std::logic_error
{
	Unsupported(const std::string& what) :
		std::logic_error(what+" is unsupported") {}
};

} // Exception
} // Procure


#endif /* PROCURE_KERNEL_EXCEPTION_HPP_ */
