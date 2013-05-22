/*
 * procure.cpp
 *
 *  Created on: May 16, 2013
 *      Author: marco
 */

#include "procure.hpp"

namespace Procure {
namespace Detail {

StaticInitializer::StaticInitializer() 
{
	Interval::initLib();
}

static StaticInitializer _initLib = StaticInitializer();

} // Detail
} // Procure
