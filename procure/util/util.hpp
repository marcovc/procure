/*
 * permutation.hpp
 *
 *  Created on: May 26, 2013
 *      Author: marco
 */

#ifndef PROCURE_UTIL_UTIL_HPP_
#define PROCURE_UTIL_UTIL_HPP_

#include <procure/kernel/common.hpp>

#include <list>
#include <vector>
#include <assert.h>
#include <iostream>

namespace Procure {
namespace Detail {

/*
 *		Generalization of the multichoose algorithm (tabled version).
 *
 *		The algorithm abstracts the sequence object to any object that maps to
 *		a sequence. For this, template parameter Proxy must implement:
 *		Seq	-> type of the sequence
 *		Seq k0(uint n) -> returns the sequence object of length n and sum 0.
 *		Seq n1(uint k) -> returns the sequence object of length 1 and sum k.
 *		void incN(Seq& r) -> prepend 0 to sequence object r
 *		void incK(Seq& r) -> transpose by 1 the first element of sequence
 *												object r.
 *		\pre n>0
 *
 *		Let S(n,k) be the cost of computing the sequence object
 *		<k-n+1,k-n+2,...,k> of size n (i.e. the most expensive sequence).
 *
 *		Complexity for this algorithm is O((k+1)n(S(k,n))).
 *		The iterative version (stars and bars) would be O(binom(n+k-1,k)*S(k,n)).
 *
 *		See also:
 *
 *		Greg Kuperberg (mathoverflow.net/users/1450), Uniquely generate all
 *		permutations of three digits that sum to a particular value?,
 *		http://mathoverflow.net/questions/9494 (version: 2009-12-21)
 *
 *		http://en.wikipedia.org/wiki/Stars_and_bars_(combinatorics)
 *		http://en.wikipedia.org/wiki/Multiset.
 *
 */
template<class Proxy>
std::list<typename Proxy::Seq> multichoose(uint n, uint k, Proxy& p)
{
	typedef	std::list<typename Proxy::Seq> Ret;
	std::vector<std::vector<Ret> > table(n+1,std::vector<Ret>(k+1));
	multichoose(n,k,p,table);
	return table[n][k];
}

template<class Proxy>
void multichoose(uint n, uint k, Proxy& p,
        std::vector<std::vector<std::list<typename Proxy::Seq> > >& table)
{
	assert(n>0);
	if (not table[n][k].empty())
		return;
	if (k==0)	// if sum is 0 then there is only the sequence {0,...,0}
		table[n][k].push_back(p.k0(n));
	else
	if (n==1)	// if sequence length is 1 then there is only sequence {k}
		table[n][k].push_back(p.n1(k));
	else
	{
		// get all sequences with length n-1 and sum k (set A)
		multichoose(n-1,k,p,table);
		auto sa = table[n-1][k];
		// extend with 0 all sequences of set A
		for (auto it = sa.begin(); it != sa.end(); ++it)
			p.incN(*it);

		// get all sequences with length n and sum k-1	(set B)
		multichoose(n,k-1,p,table);
		auto sb = table[n][k-1];
		// add 1 to the first element of all sequences of set B
		for (auto it = sb.begin(); it != sb.end(); ++it)
			p.incK(*it);

		// r = l1 U l2
		table[n][k].insert(table[n][k].end(),sa.begin(),sa.end());
		table[n][k].insert(table[n][k].end(),sb.begin(),sb.end());
	}
	return;
}

// specialization of the above algorithm to generate the set of all
// n-length sequences of non-negative integers whose sum is k.
std::list<std::list<uint> > multichoose(uint n,uint k);

} // Detail
} // Procure


#endif /* PROCURE_UTIL_UTIL_HPP_ */
