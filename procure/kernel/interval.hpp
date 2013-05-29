/*
 * interval.hpp
 *
 *  Created on: May 15, 2013
 *      Author: marco
 */

#ifndef PROCURE_KERNEL_INTERVAL_HPP_
#define PROCURE_KERNEL_INTERVAL_HPP_

#include <procure/kernel/common.hpp>
#include <procure/kernel/rounding.hpp>

#include <string>
#include <iostream>
#include <sstream>
#include <assert.h>
#include <limits>
#include <iterator>
#include <utility>

namespace Procure {
namespace Detail {

template<class Impl>
struct IntervalWrapper
{
  enum OutputFormat {
  		bounds, // [lb,ub]
  		width,		// median (+/- width/2)
  		prefix // common_prefix[ltail,rtail]
   };

	/// Initializes an interval to the universe [-inf, +inf].
	IntervalWrapper();

	/// Initializes an interval to [val,val].
	IntervalWrapper(const Real& val);

	/// Initializes an interval to [lb,ub].
	IntervalWrapper(const Real& lb,const Real& ub);

	/// Initializes an interval to [p.first,p.second].
	IntervalWrapper(const std::pair<Real,Real>& p) :
		IntervalWrapper(p.first,p.second) {}

	/// Initializes an interval from an interval of the wrapped type.
	IntervalWrapper(const Impl&);

	/**
	 * 	Initializes an interval the enclosure of the real number
	 * 	represented by a string in some floating-point format.
	 */
	IntervalWrapper(const std::string& s);

	/// Copy constructor.
	IntervalWrapper(const IntervalWrapper& i);

	/// Get lower bound.
	Real getLb() const;

	/// Get upper bound.
	Real getUb() const;

	/// Get size of interval.
	Real getWidth() const;

	/// Get mignitude of interval.
	Real getMig() const;

	/// Get magnitude of interval.
	Real getMag() const;

	/// Get signed mignitude of interval.
	Real getSignedMig() const;

	/// Get median of interval.
	Real getMedian() const;

	/// Get an interval enclosure of the median of interval.
	IntervalWrapper getMedianInterval() const;

	/// Check whether interval is empty.
	bool isEmpty() const;

	/// Check whether interval is a point.
	bool isSingleton() const;

	/// Check whether interval has finite bounds.
	bool isFinite() const;

	/// Check whether interval contains at most two floating point numbers.
	bool isCanonical() const;

	/// Check whether the lower bound is the opposite of the upper bound.
	bool isSymmetric() const;

	/// Check whether interval \a i is a subset of this interval.
	bool 	contains(const IntervalWrapper& i) const;

	/// Check whether real \a r is a member of this interval.
	bool 	contains(const Real& r) const;

	/// Check whether interval \a i is a proper subset of this interval.
	bool 	strictlyContains(const IntervalWrapper& i) const;

	/// Check whether this interval and interval \a i are disjoint.
	bool 	isDisjoint (const IntervalWrapper& i) const;

	/// Assignment operator
	IntervalWrapper& operator=(const IntervalWrapper& s);

	/// Addition with assignment.
	IntervalWrapper& 	operator+=(const IntervalWrapper& i);

	/// Subtraction with assignment.
	IntervalWrapper& 	operator-=(const IntervalWrapper& i);

	/// Multiplication with assignment.
	IntervalWrapper& 	operator*=(const IntervalWrapper& i);

	/// Division with assignment.
 	IntervalWrapper& 	operator/=(const IntervalWrapper& i);

	/// Intersection with assignment.
 	IntervalWrapper& 	operator&=(const IntervalWrapper& i);

	/// Hull of the union with assignment.
 	IntervalWrapper& 	operator|=(const IntervalWrapper& i);

 	/// Unary plus (identity).
 	const IntervalWrapper& operator+() const;

 	/// Symmetric.
 	IntervalWrapper operator-() const;

 	/// Inverse.
 	IntervalWrapper inverse() const;

 	/// Returns an empty interval.
 	static IntervalWrapper empty();

 	/// Returns the interval [-inf,+inf].
 	static IntervalWrapper whole();

 	/// Bisects interval using median().
 	std::pair<IntervalWrapper,IntervalWrapper> bisect() const;

  	/// Bisects interval using median() and returns lower interval.
 	IntervalWrapper getLowerHalf() const;

	/// Bisects interval using median() and returns upper interval.
 	IntervalWrapper getUpperHalf() const;

	/// Set output format
	static void setOutputFormat(const OutputFormat& format);

	/// Get output format
	static OutputFormat getOutputFormat();

	/// Set output precision
	static void setOutputPrecision(unsigned int precision);

	/// Get output precision
	static unsigned int getOutputPrecision();

	/// Return wrapped interval
	Impl& getImpl();

	/// Return wrapped interval
	const Impl& getImpl() const;

	private:
	friend struct Detail::StaticInitializer;

	/// Used to initialize wrapped library
	static void initLib();

	Impl impl;
	static OutputFormat outputFormat;
	static unsigned int outputPrecision;
};

// default output format
template<class Impl>
typename IntervalWrapper<Impl>::OutputFormat IntervalWrapper<Impl>::
	outputFormat =	IntervalWrapper<Impl>::bounds;

// default output precision
template<class Impl>
unsigned int IntervalWrapper<Impl>::outputPrecision =
		std::numeric_limits<Real>::digits10;

template<class Impl>
void IntervalWrapper<Impl>::setOutputFormat(const OutputFormat& format)
{
	outputFormat = format;
}

template<class Impl>
typename IntervalWrapper<Impl>::OutputFormat IntervalWrapper<Impl>::
	getOutputFormat()
{
	return outputFormat;
}

template<class Impl>
void IntervalWrapper<Impl>::setOutputPrecision(unsigned int precision)
{
	outputPrecision = precision;
}

template<class Impl>
unsigned int IntervalWrapper<Impl>::getOutputPrecision()
{
	return outputPrecision;
}

template<class Impl>
Impl& IntervalWrapper<Impl>::getImpl()
{	return impl;	}

template<class Impl>
const Impl& IntervalWrapper<Impl>::getImpl() const
{	return impl;	}


std::ostream& outputLb(std::ostream& os, const Real& lb,
                       unsigned int precision);

std::ostream& outputUb(std::ostream& os, const Real& ub,
                       unsigned int precision);

template<class Impl>
std::ostream& outputBounds(std::ostream& os, const IntervalWrapper<Impl>& i)
{
	assert(not i.isEmpty() and not i.isSingleton());
	os << "[";
	outputLb(os,i.getLb(),IntervalWrapper<Impl>::getOutputPrecision());
	os << ", ";
	outputUb(os,i.getUb(),IntervalWrapper<Impl>::getOutputPrecision());
	os << "]";
	return os;
}

template<class Impl>
std::ostream& outputWidth(std::ostream& os, const IntervalWrapper<Impl>& i)
{
	//round_nearest();
	assert(not i.isEmpty() and not i.isSingleton());
	os.precision(IntervalWrapper<Impl>::getOutputPrecision());
	os << i.getMedian() << " (+/- " << i.getWidth()/2.0 << ")";
	return os;
}

template<class Impl>
std::ostream& outputPrefix(std::ostream& os, const IntervalWrapper<Impl>& i)
{
	assert(not i.isEmpty() and not i.isSingleton());
	std::ostringstream oslb,osub;
	oslb.precision(IntervalWrapper<Impl>::getOutputPrecision());
	osub.precision(IntervalWrapper<Impl>::getOutputPrecision());
	Rounding rnd;
	rnd.setDownward();
	oslb << i.getLb();
	std::string slb = oslb.str();
	rnd.setUpward();
	osub << i.getUb();
	std::string sub = osub.str();
	auto mit = std::mismatch(slb.begin(),slb.end(),sub.begin());
	os.precision(IntervalWrapper<Impl>::getOutputPrecision());
	std::copy(slb.begin(),mit.first,std::ostream_iterator<char>(os));
	os << "[";
	std::copy(mit.first,slb.end(),std::ostream_iterator<char>(os));
	os << ", ";
	std::copy(mit.second,sub.end(),std::ostream_iterator<char>(os));
	os << "]";
	return os;
}

template<class Impl>
std::ostream& operator<<(std::ostream& os, const IntervalWrapper<Impl>& i)
{
	if (i.isEmpty())
    os << "[]";
	else
	if (i.isSingleton())
		os << i.getLb();
	else
	{
		switch (IntervalWrapper<Impl>::getOutputFormat())
		{
			case IntervalWrapper<Impl>::bounds:
				return outputBounds(os,i);
			case IntervalWrapper<Impl>::width:
				return outputWidth(os,i);
			case IntervalWrapper<Impl>::prefix:
				return outputPrefix(os,i);
		}
	}
	return os;
}

} // Detail
} // Procure

#if PROCURE_HAVE_GAOL
#include <procure/kernel/interval/interval_gaol.hpp>
#endif

#if PROCURE_HAVE_BOOSTINTERVAL
#include <procure/kernel/interval/interval_boost.hpp>
#endif

#if PROCURE_HAVE_PROFIL
#include <procure/kernel/interval/interval_profil.hpp>
#endif

namespace Procure	{
	typedef Procure::Detail::IntervalWrapper<
#if defined(PROCURE_INTERVAL_USE_GAOL)
	Procure::Detail::GaolInterval
#elif defined(PROCURE_INTERVAL_USE_BOOST)
	Procure::Detail::BoostInterval
#elif defined(PROCURE_INTERVAL_USE_PROFIL)
	Procure::Detail::ProfilInterval
#endif
	>	Interval;

} // Procure

#endif /* PROCURE_KERNEL_INTERVAL_HPP_ */
