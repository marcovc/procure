/*
 * pimpl.hpp
 *
 *  Created on: May 15, 2013
 *      Author: marco
 */


#ifndef PROCURE_UTIL_PIMPL_HPP_
#define PROCURE_UTIL_PIMPL_HPP_

#include <memory>
#include <assert.h>

namespace Procure {
namespace Util {


/// Pointer to implementation idiom
template<class Iface>
struct PImplIdiom
{
	typedef Iface* PImpl;
	PImplIdiom(Iface* apImpl) : pImpl(apImpl) { }
	PImplIdiom(const PImplIdiom& s) : pImpl(s.getPImpl()) { }
	Iface&				getImpl() 			{ assert(pImpl); return *pImpl;}
	const Iface&		getImpl() const 	{ assert(pImpl); return *pImpl;}
	PImpl&				getPImpl() 			{ return pImpl;}
	const PImpl&		getPImpl() const 	{ return pImpl;}

	const PImplIdiom<Iface>& operator=(const PImplIdiom<Iface>& s)
	{	pImpl = s.pImpl; return *this; 	}

	bool operator==(const PImplIdiom<Iface>& s) const
	{	return pImpl == s.pImpl; }

	PImpl pImpl;
};

/// Pointer to implementation idiom, with automatic garbage collection
template<class Iface>
struct SPImplIdiom
{
	typedef std::shared_ptr<Iface> PImpl;
	SPImplIdiom(Iface* apImpl) : pImpl(apImpl) { }
	SPImplIdiom(PImpl apImpl) : pImpl(apImpl) { }
	SPImplIdiom(const SPImplIdiom& s) : pImpl(s.getPImpl()) { }
	Iface&				getImpl() 			{ assert(pImpl); return *pImpl;}
	const Iface&		getImpl() const 	{ assert(pImpl); return *pImpl;}
	PImpl&				getPImpl() 			{ return pImpl;}
	const PImpl&		getPImpl() const 	{ return pImpl;}

	const SPImplIdiom<Iface>& operator=(const SPImplIdiom<Iface>& s)
	{	pImpl = s.pImpl; return *this; 	}

	bool operator==(const SPImplIdiom<Iface>& s) const
	{	return pImpl == s.pImpl; }

	PImpl pImpl;
};

}
}

#endif /* PROCURE_UTIL_PIMPL_HPP_ */
