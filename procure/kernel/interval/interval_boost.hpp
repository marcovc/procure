/*
 * interval_boost.hpp
 *
 *  Created on: May 16, 2013
 *      Author: marco
 */

#include <procure/kernel/common.hpp>
#include <procure/kernel/exception.hpp>
#include <procure/kernel/rounding.hpp>

#include <boost/numeric/interval.hpp>

#include <limits>

#include <limits>
#include <cmath>

#if PROCURE_HAVE_CRLIBM
#include <crlibm.h>
#elif PROCURE_HAVE_MPFR
#include <mpfr.h>
#endif

namespace Procure {
namespace Detail {

/*
 * 	Transcendental and trignometric functions in std namespace do not
 * 	follow rounding flag.
 * 	The following class redirects to functions with correct
 * 	rounding. It relies on crlibm, or mpfr libraries (in this order).
 * 	If neither crlibm nor mpfr are present then it uses an approximation.
 */

#if PROCURE_HAVE_CRLIBM

struct RoundedMath :
		boost::numeric::interval_lib::rounded_arith_opp<Real>
{
	private:
	#define GENR_FUNC(name) \
		Real name##_down(Real x) \
		{ \
			this->to_nearest(); \
			Real y = this->force_rounding(name##_rd(x));	\
			this->upward();\
			return y; \
		} \
		Real name##_up(Real x) \
		{	\
			this->to_nearest(); \
			Real y = this->force_rounding(name##_ru(x));	\
			this->upward(); \
			return y;\
		}
	#define UNSUP_FUNC(name) \
		Real name##_down(Real x) \
		{ \
			this->to_nearest();	\
			Real y = std::nextafter(::name(x), -std::numeric_limits<Real>::infinity());	\
			this->upward();\
			return y;\
		} \
		Real name##_up(Real x) \
		{ \
			this->to_nearest();	\
			Real y = std::nextafter(::name(x), std::numeric_limits<Real>::infinity());	\
			this->upward();\
			return y;\
		}
	public:

	GENR_FUNC(exp);
	GENR_FUNC(log);
	GENR_FUNC(cos);
	GENR_FUNC(tan);
	GENR_FUNC(asin);
	GENR_FUNC(acos);
	GENR_FUNC(atan);
	GENR_FUNC(sinh);
	GENR_FUNC(cosh);
	UNSUP_FUNC(tanh);	// these are not supported by crlibm
	UNSUP_FUNC(asinh);
	UNSUP_FUNC(acosh);
	UNSUP_FUNC(atanh);

	#undef GENR_FUNC
	#undef UNSUP_FUNC
};

#elif PROCURE_HAVE_MPFR

struct RoundedMath :
		boost::numeric::interval_lib::rounded_arith_opp<Real>
{
	private:

	typedef int mpfr_func(mpfr_t,const mpfr_t, mp_rnd_t);
	static double invoke_mpfr(double x, mpfr_func f, mp_rnd_t r) {
		mpfr_t xx;
		mpfr_init_set_d(xx, x, GMP_RNDN);
		f(xx, xx, r);
		double res = mpfr_get_d(xx, r);
		mpfr_clear(xx);
		return res;
	}
	#define GENR_FUNC(name) \
		Real name##_down(Real x) { return invoke_mpfr(x, mpfr_##name, GMP_RNDD); } \
		Real name##_up  (Real x) { return invoke_mpfr(x, mpfr_##name, GMP_RNDU); }

	public:

	GENR_FUNC(exp);
	GENR_FUNC(log);
	GENR_FUNC(cos);
	GENR_FUNC(tan);
	GENR_FUNC(asin);
	GENR_FUNC(acos);
	GENR_FUNC(atan);
	GENR_FUNC(sinh);
	GENR_FUNC(cosh);
	GENR_FUNC(tanh);
	GENR_FUNC(asinh);
	GENR_FUNC(acosh);
	GENR_FUNC(atanh);

	#undef GENR_FUNC
};
#else

struct RoundedMath :
		boost::numeric::interval_lib::rounded_arith_opp<Real>
{
	private:

	#define UNSUP_FUNC(name) \
		Real name##_down(Real x) \
		{ this->to_nearest();	return std::nextafter(name(x), -std::numeric_limits<Real>::infinity());	} \
		Real name##_up(Real x) \
		{ this->to_nearest();	return std::nextafter(name(x), std::numeric_limits<Real>::infinity());	}
	public:

	UNSUP_FUNC(exp);
	UNSUP_FUNC(log);
	UNSUP_FUNC(cos);
	UNSUP_FUNC(tan);
	UNSUP_FUNC(asin);
	UNSUP_FUNC(acos);
	UNSUP_FUNC(atan);
	UNSUP_FUNC(sinh);
	UNSUP_FUNC(cosh);
	UNSUP_FUNC(tanh);
	UNSUP_FUNC(asinh);
	UNSUP_FUNC(acosh);
	UNSUP_FUNC(atanh);

	#undef UNSUP_FUNC
};

#endif

typedef boost::numeric::interval<Real,
		boost::numeric::interval_lib::policies<
			boost::numeric::interval_lib::save_state<RoundedMath>,
			boost::numeric::interval_lib::checking_base<Real> > > BoostInterval;

template<>
inline
void IntervalWrapper<BoostInterval>::initLib()
{
}

template<>
inline
IntervalWrapper<BoostInterval>::IntervalWrapper(const BoostInterval& aimpl) :
	impl(aimpl)
{
}

template<>
inline
IntervalWrapper<BoostInterval>::IntervalWrapper() :
		impl(-std::numeric_limits < Real > ::infinity(),
		     std::numeric_limits < Real > ::infinity())
{
}

template<>
inline
IntervalWrapper<BoostInterval>::IntervalWrapper(const Real& val) :
		impl(val)
{
}

template<>
inline
IntervalWrapper<BoostInterval>::IntervalWrapper(const Real& lb, const Real& ub) :
		impl(lb, ub)
{
}

template<>
inline
IntervalWrapper<BoostInterval>::IntervalWrapper(const std::string& s) :
		impl()
{
	throw Procure::Exception::Unsupported(
	    "creating interval from string argument");
}

template<>
inline
IntervalWrapper<BoostInterval>::IntervalWrapper(const IntervalWrapper& i) :
		impl(i.impl)
{
}

template<>
inline
Real IntervalWrapper<BoostInterval>::getLb() const
{
	return impl.lower();
}

template<>
inline
Real IntervalWrapper<BoostInterval>::getUb() const
{
	return impl.upper();
}

template<>
inline
Real IntervalWrapper<BoostInterval>::getWidth() const
{
	return boost::numeric::width(impl);
}

template<>
inline
Real IntervalWrapper<BoostInterval>::getMedian() const
{
	return  boost::numeric::median(impl);
}

template<>
inline
bool IntervalWrapper<BoostInterval>::isEmpty() const
{
	return boost::numeric::empty(impl);
}

template<>
inline
bool IntervalWrapper<BoostInterval>::isSingleton() const
{
	return boost::numeric::singleton(impl);
}

template<>
inline
bool IntervalWrapper<BoostInterval>::contains(const IntervalWrapper& i) const
{
	return boost::numeric::subset(i.impl, impl);
}

template<>
inline
bool IntervalWrapper<BoostInterval>::contains(const Real& r) const
{
	return boost::numeric::in(r, impl);
}

template<>
inline
bool IntervalWrapper<BoostInterval>::strictlyContains(
    const IntervalWrapper& i) const
{
	return boost::numeric::proper_subset(i.impl, impl);
}

template<>
inline
IntervalWrapper<BoostInterval>& IntervalWrapper<BoostInterval>::
	operator=(const IntervalWrapper& i)
{
	impl = i.impl;
	return *this;
}

template<>
inline
IntervalWrapper<BoostInterval>& IntervalWrapper<BoostInterval>::
	operator+=(const IntervalWrapper& i)
{
	impl += i.impl;
	return *this;
}

template<>
inline
IntervalWrapper<BoostInterval>& IntervalWrapper<BoostInterval>::
	operator-=(const IntervalWrapper& i)
{
	impl -= i.impl;
	return *this;
}

template<>
inline
IntervalWrapper<BoostInterval>& IntervalWrapper<BoostInterval>::
	operator*=(const IntervalWrapper& i)
{
	impl *= i.impl;
	return *this;
}


template<>
inline
IntervalWrapper<BoostInterval>& IntervalWrapper<BoostInterval>::
	operator/=(const IntervalWrapper &i)
{
	impl /= i.impl;
	return *this;
}

template<>
inline
const IntervalWrapper<BoostInterval>& IntervalWrapper<BoostInterval>::
	operator+() const
{
	return *this;
}

template<>
inline
IntervalWrapper<BoostInterval> IntervalWrapper<BoostInterval>::operator-() const
{	return boost::numeric::operator-(impl);	}

template<>
inline
IntervalWrapper<BoostInterval> IntervalWrapper<BoostInterval>::inverse() const
{	return boost::numeric::interval_lib::multiplicative_inverse(impl);	}

template<>
inline
IntervalWrapper<BoostInterval> IntervalWrapper<BoostInterval>::empty()
{	return BoostInterval::empty();	}

template<>
inline
IntervalWrapper<BoostInterval> IntervalWrapper<BoostInterval>::whole()
{	return BoostInterval::whole();	}

template<>
inline
std::pair<IntervalWrapper<BoostInterval>,IntervalWrapper<BoostInterval>>
	IntervalWrapper<BoostInterval>::bisect() const
{
	auto p = boost::numeric::bisect(impl);
	return std::pair<IntervalWrapper<BoostInterval>,
										IntervalWrapper<BoostInterval>>(p.first,p.second);
}

// not directly supported

template<>
inline
bool IntervalWrapper<BoostInterval>::isFinite() const
{
	return not std::isinf(getLb()) and not std::isinf(getUb());
}

template<>
inline
bool IntervalWrapper<BoostInterval>::isCanonical() const
{
	return std::nextafter(getLb(), std::numeric_limits < Real > ::infinity())
	    >= getUb();
}

template<>
inline
bool IntervalWrapper<BoostInterval>::isSymmetric() const
{
	return not isEmpty() and -getLb() == getUb();
}

template<>
inline
Real IntervalWrapper<BoostInterval>::getMig() const
{
	if (isEmpty())
		return std::numeric_limits < Real > ::quiet_NaN();

	if (contains(0))
		return 0.0;

	if (getUb() < 0.0)
		return -getUb();
	// getLb()>0.0
	return getLb();
}

template<>
inline
Real IntervalWrapper<BoostInterval>::getMag() const
{
	return std::max(std::fabs(getLb()), std::fabs(getUb()));
}

template<>
inline
Real IntervalWrapper<BoostInterval>::getSignedMig() const
{
	if (isEmpty())
		return std::numeric_limits<Real>::quiet_NaN();

	if (contains(0))
		return 0.0;

	if (getUb() < 0.0)
		return getUb();
	// getLb()>0.0
	return getLb();
}

template<>
inline
IntervalWrapper<BoostInterval> IntervalWrapper<BoostInterval>::getMedianInterval() const
{
	if (isEmpty())
		return BoostInterval::empty();

 	if (isSymmetric())  // symmetric case handles [-oo, +oo]
 		return 0.0;

 	if (getLb() == -std::numeric_limits<Real>::infinity())
 		return -std::numeric_limits<Real>::max();

	if (getUb() == std::numeric_limits<Real>::infinity())
		return std::numeric_limits<Real>::max();

	Rounding rnd;
	rnd.setDownward();
	Real lb = .5*(getLb()+getUb());
	rnd.setUpward();
	Real ub = .5*(getLb()+getUb());
	return BoostInterval(lb,ub);
}

template<>
inline
bool IntervalWrapper<BoostInterval>::isDisjoint(const IntervalWrapper& i) const
{
	return getUb() < i.getLb() or getLb() > i.getUb() or isEmpty() or
		i.isEmpty();
}

// FIXME: optimize (avoid copy)
template<>
inline
IntervalWrapper<BoostInterval>& IntervalWrapper<BoostInterval>::
	operator&=(const IntervalWrapper &i)
{
	impl = boost::numeric::intersect(impl,i.impl);
	return *this;
}

// FIXME: optimize (avoid copy)
template<>
inline
IntervalWrapper<BoostInterval>& IntervalWrapper<BoostInterval>::
	operator|=(const IntervalWrapper &i)
{
	impl = boost::numeric::hull(impl,i.impl);
	return *this;
}

template<>
inline
IntervalWrapper<BoostInterval> IntervalWrapper<BoostInterval>::
	getLowerHalf() const
{
	return boost::numeric::bisect(impl).first;
}

template<>
inline
IntervalWrapper<BoostInterval> IntervalWrapper<BoostInterval>::
	getUpperHalf() const
{
	return boost::numeric::bisect(impl).second;
}

} // Detail

// operators

#define OP_DEF_2(Ret,Op)	\
		inline Ret operator Op(const Detail::IntervalWrapper<Detail::BoostInterval>& i1,\
		                const Detail::IntervalWrapper<Detail::BoostInterval>& i2)\
		{\
			return i1.getImpl() Op i2.getImpl(); \
		}

#define OP_DEF_2_R(Ret,Op)	\
		OP_DEF_2(Ret,Op)\
		inline Ret operator Op(const Real& r,const Detail::IntervalWrapper<Detail::BoostInterval>& i)\
		{\
			return r Op i.getImpl(); \
		}\
		inline Ret operator Op(const Detail::IntervalWrapper<Detail::BoostInterval>& i,const Real& r)\
		{\
			return i.getImpl() Op r; \
		}

/// Comparison operators involving intervals.
/// @{

OP_DEF_2	(bool,==)
OP_DEF_2	(bool,!=)
OP_DEF_2	(bool,>)
OP_DEF_2	(bool,>=)
OP_DEF_2	(bool,<)
OP_DEF_2	(bool,<=)

/// @}

/// Arithmetic operators involving intervals.
/// @{

OP_DEF_2_R	(Detail::IntervalWrapper<Detail::BoostInterval>,+)
OP_DEF_2_R	(Detail::IntervalWrapper<Detail::BoostInterval>,-)
OP_DEF_2_R	(Detail::IntervalWrapper<Detail::BoostInterval>,*)
OP_DEF_2_R	(Detail::IntervalWrapper<Detail::BoostInterval>,/)

/// @}

/// Algebraic operators involving intervals.
/// @{

inline Detail::IntervalWrapper<Detail::BoostInterval> abs(const Detail::IntervalWrapper<Detail::BoostInterval>& i)
{	return boost::numeric::abs(i.getImpl());	}

inline Detail::IntervalWrapper<Detail::BoostInterval> sqrt(const Detail::IntervalWrapper<Detail::BoostInterval>& i)
{	return boost::numeric::sqrt(i.getImpl());	}

inline Detail::IntervalWrapper<Detail::BoostInterval> sqr(const Detail::IntervalWrapper<Detail::BoostInterval>& i)
{	return boost::numeric::square(i.getImpl());	}

inline Detail::IntervalWrapper<Detail::BoostInterval> pow(const Detail::IntervalWrapper<Detail::BoostInterval>& i, int y)
{	return boost::numeric::pow(i.getImpl(),y);	}

inline Detail::IntervalWrapper<Detail::BoostInterval> pow(const Detail::IntervalWrapper<Detail::BoostInterval>& i1,
                                  const Detail::IntervalWrapper<Detail::BoostInterval>& i2)
{	throw Procure::Exception::Unsupported("operation power(interval,interval)");	}

inline Detail::IntervalWrapper<Detail::BoostInterval> nth_root(const Detail::IntervalWrapper<Detail::BoostInterval>& i,
                                       unsigned int y)
{	return boost::numeric::nth_root(i.getImpl(),static_cast<int>(y));	}

/// @}

/// Transcendental operators involving intervals.
/// @{

inline Detail::IntervalWrapper<Detail::BoostInterval> exp(const Detail::IntervalWrapper<Detail::BoostInterval>& i)
{	return boost::numeric::exp(i.getImpl());	}

inline Detail::IntervalWrapper<Detail::BoostInterval> log(const Detail::IntervalWrapper<Detail::BoostInterval>& i)
{	return boost::numeric::log(i.getImpl());	}

/// @}

/// Modulo operator involving intervals.
/// @{

OP_DEF_2_R	(Detail::IntervalWrapper<Detail::BoostInterval>,%)

/// @}

/// Trignometric functions involving intervals.
/// @{

inline Detail::IntervalWrapper<Detail::BoostInterval> sin(const Detail::IntervalWrapper<Detail::BoostInterval>& i)
{	return boost::numeric::sin(i.getImpl());	}

inline Detail::IntervalWrapper<Detail::BoostInterval> cos(const Detail::IntervalWrapper<Detail::BoostInterval>& i)
{	return boost::numeric::cos(i.getImpl());	}

inline Detail::IntervalWrapper<Detail::BoostInterval> tan(const Detail::IntervalWrapper<Detail::BoostInterval>& i)
{	return boost::numeric::tan(i.getImpl());	}

inline Detail::IntervalWrapper<Detail::BoostInterval> asin(const Detail::IntervalWrapper<Detail::BoostInterval>& i)
{	return boost::numeric::asin(i.getImpl());	}

inline Detail::IntervalWrapper<Detail::BoostInterval> acos(const Detail::IntervalWrapper<Detail::BoostInterval>& i)
{	return boost::numeric::acos(i.getImpl());	}

inline Detail::IntervalWrapper<Detail::BoostInterval> atan(const Detail::IntervalWrapper<Detail::BoostInterval>& i)
{	return boost::numeric::atan(i.getImpl());	}

inline Detail::IntervalWrapper<Detail::BoostInterval> sinh(const Detail::IntervalWrapper<Detail::BoostInterval>& i)
{	return boost::numeric::sinh(i.getImpl());	}

inline Detail::IntervalWrapper<Detail::BoostInterval> cosh(const Detail::IntervalWrapper<Detail::BoostInterval>& i)
{	return boost::numeric::cosh(i.getImpl());	}

inline Detail::IntervalWrapper<Detail::BoostInterval> tanh(const Detail::IntervalWrapper<Detail::BoostInterval>& i)
{	return boost::numeric::tanh(i.getImpl());	}

inline Detail::IntervalWrapper<Detail::BoostInterval> asinh(const Detail::IntervalWrapper<Detail::BoostInterval>& i)
{	return boost::numeric::asinh(i.getImpl());	}

inline Detail::IntervalWrapper<Detail::BoostInterval> acosh(const Detail::IntervalWrapper<Detail::BoostInterval>& i)
{	return boost::numeric::acosh(i.getImpl());	}

inline Detail::IntervalWrapper<Detail::BoostInterval> atanh(const Detail::IntervalWrapper<Detail::BoostInterval>& i)
{	return boost::numeric::atanh(i.getImpl());	}

/// @}

/// min/max functions involving intervals.
/// @{

inline Detail::IntervalWrapper<Detail::BoostInterval> min(const Detail::IntervalWrapper<Detail::BoostInterval>& i1,
                                  const Detail::IntervalWrapper<Detail::BoostInterval>& i2)
{	return boost::numeric::min(i1.getImpl(),i2.getImpl());	}

inline Detail::IntervalWrapper<Detail::BoostInterval> max(const Detail::IntervalWrapper<Detail::BoostInterval>& i1,
                                  const Detail::IntervalWrapper<Detail::BoostInterval>& i2)
{	return boost::numeric::max(i1.getImpl(),i2.getImpl());	}

/// @}

#undef OP_DEF_2
#undef OP_DEF_2_R

} // Procure

