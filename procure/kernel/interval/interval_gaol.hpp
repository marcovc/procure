/*
 * interval_gaol.hpp
 *
 *  Created on: May 16, 2013
 *      Author: marco
 */

#include <ios> // gaol is missing this header
#include <gaol/gaol_interval.h>

namespace Procure {
namespace Detail {

typedef gaol::interval GaolInterval;

template<>
inline
void IntervalWrapper<GaolInterval>::initLib()
{
}

template<>
inline
IntervalWrapper<GaolInterval>::IntervalWrapper(const GaolInterval& aimpl) :
		impl(aimpl)
{
}

template<>
inline
IntervalWrapper<GaolInterval>::IntervalWrapper() :
		impl(gaol::interval::universe())
{
}

template<>
inline
IntervalWrapper<GaolInterval>::IntervalWrapper(const Real& val) :
		impl(val)
{
}

template<>
inline
IntervalWrapper<GaolInterval>::IntervalWrapper(const Real& lb, const Real& ub) :
		impl(lb, ub)
{
}

template<>
inline
IntervalWrapper<GaolInterval>::IntervalWrapper(const std::string& s) :
		impl(s.c_str())
{
}

template<>
inline
IntervalWrapper<GaolInterval>::IntervalWrapper(const IntervalWrapper& i) :
		impl(i.impl)
{
}

template<>
inline
Real IntervalWrapper<GaolInterval>::getLb() const
{
	return impl.left();
}

template<>
inline
Real IntervalWrapper<GaolInterval>::getUb() const
{
	return impl.right();
}

template<>
inline
Real IntervalWrapper<GaolInterval>::getWidth() const
{
	return impl.width();
}

template<>
inline
Real IntervalWrapper<GaolInterval>::getMig() const
{
	return impl.mig();
}

template<>
inline
Real IntervalWrapper<GaolInterval>::getMag() const
{
	return impl.mag();
}

template<>
inline
Real IntervalWrapper<GaolInterval>::getSignedMig() const
{
	return impl.smig();
}

template<>
inline
Real IntervalWrapper<GaolInterval>::getMedian() const
{
	return impl.midpoint();
}

template<>
inline
IntervalWrapper<GaolInterval> IntervalWrapper<GaolInterval>::getMedianInterval() const
{
	return impl.mid();
}

template<>
inline
bool IntervalWrapper<GaolInterval>::isEmpty() const
{
	return impl.is_empty();
}

template<>
inline
bool IntervalWrapper<GaolInterval>::isSingleton() const
{
	return impl.is_a_double();
}

template<>
inline
bool IntervalWrapper<GaolInterval>::isFinite() const
{
	return impl.is_finite();
}

template<>
inline
bool IntervalWrapper<GaolInterval>::isCanonical() const
{
	return impl.is_canonical();
}

template<>
inline
bool IntervalWrapper<GaolInterval>::isSymmetric() const
{
	return impl.is_symmetric();
}

template<>
inline
bool IntervalWrapper<GaolInterval>::contains(const IntervalWrapper& i) const
{
	return impl.set_contains(i.impl);
}

template<>
inline
bool IntervalWrapper<GaolInterval>::contains(const Real& r) const
{
	return impl.set_contains(r);
}

template<>
inline
bool IntervalWrapper<GaolInterval>::strictlyContains(
    const IntervalWrapper& i) const
{
	return impl.set_strictly_contains(i.impl);
}

template<>
inline
bool IntervalWrapper<GaolInterval>::isDisjoint(const IntervalWrapper& i) const
{
	return impl.set_disjoint(i.impl);
}

template<>
inline
IntervalWrapper<GaolInterval>& IntervalWrapper<GaolInterval>::operator=(
    const IntervalWrapper& i)
{
	impl = i.impl;
	return *this;
}

template<>
inline
IntervalWrapper<GaolInterval>& IntervalWrapper<GaolInterval>::operator+=(
    const IntervalWrapper& i)
{
	impl += i.impl;
	return *this;
}

template<>
inline
IntervalWrapper<GaolInterval>& IntervalWrapper<GaolInterval>::operator-=(
    const IntervalWrapper& i)
{
	impl -= i.impl;
	return *this;
}

template<>
inline
IntervalWrapper<GaolInterval>& IntervalWrapper<GaolInterval>::operator*=(
    const IntervalWrapper& i)
{
	impl *= i.impl;
	return *this;
}

template<>
inline
IntervalWrapper<GaolInterval>& IntervalWrapper<GaolInterval>::operator/=(
    const IntervalWrapper &i)
{
	impl /= i.impl;
	return *this;
}

template<>
inline
IntervalWrapper<GaolInterval>& IntervalWrapper<GaolInterval>::operator&=(
    const IntervalWrapper &i)
{
	impl &= i.impl;
	return *this;
}

template<>
inline
IntervalWrapper<GaolInterval>& IntervalWrapper<GaolInterval>::operator|=(
    const IntervalWrapper &i)
{
	impl |= i.impl;
	return *this;
}

template<>
inline
const IntervalWrapper<GaolInterval>& IntervalWrapper<GaolInterval>::operator+() const
{
	return *this;
}

template<>
inline
IntervalWrapper<GaolInterval> IntervalWrapper<GaolInterval>::operator-() const
{
	return -impl;
}

template<>
inline
IntervalWrapper<GaolInterval> IntervalWrapper<GaolInterval>::inverse() const
{
	return impl.inverse();
}

template<>
inline
IntervalWrapper<GaolInterval> IntervalWrapper<GaolInterval>::empty()
{
	return GaolInterval::emptyset();
}

template<>
inline
IntervalWrapper<GaolInterval> IntervalWrapper<GaolInterval>::whole()
{
	return GaolInterval::universe();
}

template<>
inline
std::pair<IntervalWrapper<GaolInterval>,IntervalWrapper<GaolInterval>> IntervalWrapper<
    GaolInterval>::bisect() const
{
	GaolInterval lh, uh;
	impl.split(lh, uh);
	return std::pair<IntervalWrapper<GaolInterval>,IntervalWrapper<GaolInterval>>(
	    lh, uh);
}

template<>
inline
IntervalWrapper<GaolInterval> IntervalWrapper<GaolInterval>::getLowerHalf() const
{
	return impl.split_left();
}

template<>
inline
IntervalWrapper<GaolInterval> IntervalWrapper<GaolInterval>::getUpperHalf() const
{
	return impl.split_right();
}

} // Detail

#define OP_DEF_2(Ret,Op)	\
		inline Ret operator Op(const Detail::IntervalWrapper<Detail::GaolInterval>& i1,\
		                const Detail::IntervalWrapper<Detail::GaolInterval>& i2)\
		{\
			return i1.getImpl() Op i2.getImpl(); \
		}

#define OP_DEF_2_R(Ret,Op)	\
		OP_DEF_2(Ret,Op)\
		inline Ret operator Op(const Real& r,const Detail::IntervalWrapper<Detail::GaolInterval>& i)\
		{\
			return r Op i.getImpl(); \
		}\
		inline Ret operator Op(const Detail::IntervalWrapper<Detail::GaolInterval>& i,const Real& r)\
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

OP_DEF_2_R	(Detail::IntervalWrapper<Detail::GaolInterval>,+)
OP_DEF_2_R	(Detail::IntervalWrapper<Detail::GaolInterval>,-)
OP_DEF_2_R	(Detail::IntervalWrapper<Detail::GaolInterval>,*)
OP_DEF_2_R	(Detail::IntervalWrapper<Detail::GaolInterval>,/)

/// @}

/// Algebraic operators involving intervals.
/// @{

inline
Detail::IntervalWrapper<Detail::GaolInterval> abs(const Detail::IntervalWrapper<Detail::GaolInterval>& i)
{	return abs(i.getImpl());	}

inline
Detail::IntervalWrapper<Detail::GaolInterval> sqrt(const Detail::IntervalWrapper<Detail::GaolInterval>& i)
{	return sqrt(i.getImpl());	}

inline
Detail::IntervalWrapper<Detail::GaolInterval> sqr(const Detail::IntervalWrapper<Detail::GaolInterval>& i)
{	return sqr(i.getImpl());	}

inline
Detail::IntervalWrapper<Detail::GaolInterval> pow(const Detail::IntervalWrapper<Detail::GaolInterval>& i, int y)
{	return pow(i.getImpl(),y);	}

inline
Detail::IntervalWrapper<Detail::GaolInterval> pow(const Detail::IntervalWrapper<Detail::GaolInterval>& i1,
                                  const Detail::IntervalWrapper<Detail::GaolInterval>& i2)
{	return pow(i1.getImpl(),i2.getImpl());	}

inline
Detail::IntervalWrapper<Detail::GaolInterval> nth_root(const Detail::IntervalWrapper<Detail::GaolInterval>& i,
                                       unsigned int y)
{	return nth_root(i.getImpl(),y);	}

/// @}

/// Transcendental operators involving intervals.
/// @{

inline
Detail::IntervalWrapper<Detail::GaolInterval> exp(const Detail::IntervalWrapper<Detail::GaolInterval>& i)
{	return exp(i.getImpl());	}

inline
Detail::IntervalWrapper<Detail::GaolInterval> log(const Detail::IntervalWrapper<Detail::GaolInterval>& i)
{	return log(i.getImpl());	}

/// @}

/// Modulo operator involving intervals.
/// @{

OP_DEF_2_R	(Detail::IntervalWrapper<Detail::GaolInterval>,%)

/// @}

/// Trignometric functions involving intervals.
/// @{

inline
Detail::IntervalWrapper<Detail::GaolInterval> sin(const Detail::IntervalWrapper<Detail::GaolInterval>& i)
{	return sin(i.getImpl());	}

inline
Detail::IntervalWrapper<Detail::GaolInterval> cos(const Detail::IntervalWrapper<Detail::GaolInterval>& i)
{	return cos(i.getImpl());	}

inline
Detail::IntervalWrapper<Detail::GaolInterval> tan(const Detail::IntervalWrapper<Detail::GaolInterval>& i)
{	return tan(i.getImpl());	}

inline
Detail::IntervalWrapper<Detail::GaolInterval> asin(const Detail::IntervalWrapper<Detail::GaolInterval>& i)
{	return asin(i.getImpl());	}

inline
Detail::IntervalWrapper<Detail::GaolInterval> acos(const Detail::IntervalWrapper<Detail::GaolInterval>& i)
{	return acos(i.getImpl());	}

inline
Detail::IntervalWrapper<Detail::GaolInterval> atan(const Detail::IntervalWrapper<Detail::GaolInterval>& i)
{	return atan(i.getImpl());	}

inline
Detail::IntervalWrapper<Detail::GaolInterval> sinh(const Detail::IntervalWrapper<Detail::GaolInterval>& i)
{	return sinh(i.getImpl());	}

inline
Detail::IntervalWrapper<Detail::GaolInterval> cosh(const Detail::IntervalWrapper<Detail::GaolInterval>& i)
{	return cosh(i.getImpl());	}

inline
Detail::IntervalWrapper<Detail::GaolInterval> tanh(const Detail::IntervalWrapper<Detail::GaolInterval>& i)
{	return tanh(i.getImpl());	}

inline
Detail::IntervalWrapper<Detail::GaolInterval> asinh(const Detail::IntervalWrapper<Detail::GaolInterval>& i)
{	return asinh(i.getImpl());	}

inline
Detail::IntervalWrapper<Detail::GaolInterval> acosh(const Detail::IntervalWrapper<Detail::GaolInterval>& i)
{	return acosh(i.getImpl());	}

inline
Detail::IntervalWrapper<Detail::GaolInterval> atanh(const Detail::IntervalWrapper<Detail::GaolInterval>& i)
{	return atanh(i.getImpl());	}

/// @}

/// min/max functions involving intervals.
/// @{

inline
Detail::IntervalWrapper<Detail::GaolInterval> min(const Detail::IntervalWrapper<Detail::GaolInterval>& i1,
                                  const Detail::IntervalWrapper<Detail::GaolInterval>& i2)
{	return min(i1.getImpl(),i2.getImpl());	}

inline
Detail::IntervalWrapper<Detail::GaolInterval> max(const Detail::IntervalWrapper<Detail::GaolInterval>& i1,
                                  const Detail::IntervalWrapper<Detail::GaolInterval>& i2)
{	return max(i1.getImpl(),i2.getImpl());	}

/// @}

#undef OP_DEF_2
#undef OP_DEF_2_R

} // Procure

