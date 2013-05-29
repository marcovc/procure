/*
 * permutation.hpp
 *
 *  Created on: May 26, 2013
 *      Author: marco
 */

#include <procure/util/util.hpp>

#include <list>
#include <vector>
#include <iterator>

namespace Procure {
namespace Detail {

struct Proxy
{
	typedef std::list<uint> Seq;
	Seq k0(uint n) const
	{
		std::list<uint> r;
		std::fill_n(std::back_inserter(r),n,0);
		return r;
	}
	Seq n1(uint k) const
	{	return {k};	}
	void incN(Seq& s) const
	{	s.push_front(0);	}
	void incK(Seq& s) const
	{	++s.front();	}
};

std::list<std::list<uint> > multichoose(uint n,uint k)
{
	Proxy p;
	return multichoose(n,k,p);
}


} // Detail
} // Procure
