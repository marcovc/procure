/*
 * interval_profil.hpp
 *
 *  Created on: May 22, 2013
 *      Author: marco
 */

#include <procure/kernel/common.hpp>
#include <procure/kernel/exception.hpp>
#include <procure/kernel/rounding.hpp>

#include <Interval.h>
#include <BIAS/BiasF.h>
#include <Functions.h>

namespace Procure {
namespace Detail {

typedef INTERVAL ProfilInterval;

template<>
inline
void IntervalWrapper<ProfilInterval>::initLib()
{
	BiasInit();
	BiasFuncInit();
}

template<>
inline
IntervalWrapper<ProfilInterval>::IntervalWrapper(const ProfilInterval& aimpl) :
		impl(aimpl)
{
}

template<>
inline
IntervalWrapper<ProfilInterval>::IntervalWrapper() :
	impl(BiasNegInf,BiasPosInf)
{
}

template<>
inline
IntervalWrapper<ProfilInterval>::IntervalWrapper(const Real& val) :
		impl(val)
{
}

template<>
inline
IntervalWrapper<ProfilInterval>::IntervalWrapper(const Real& lb, const Real& ub) :
		impl(lb, ub)
{
}

template<>
inline
IntervalWrapper<ProfilInterval>::IntervalWrapper(const std::string& s)
{
	throw Procure::Exception::Unsupported(
			"creating interval from string argument");
}

template<>
inline
IntervalWrapper<ProfilInterval>::IntervalWrapper(const IntervalWrapper& i) :
		impl(i.impl)
{
}

template<>
inline
Real IntervalWrapper<ProfilInterval>::getLb() const
{
	return Inf(impl);
}

template<>
inline
Real IntervalWrapper<ProfilInterval>::getUb() const
{
	return Sup(impl);
}

template<>
inline
Real IntervalWrapper<ProfilInterval>::getWidth() const
{
	return Diam(impl);
}

template<>
inline
Real IntervalWrapper<ProfilInterval>::getMig() const
{
	return Mig(impl);
}

template<>
inline
Real IntervalWrapper<ProfilInterval>::getMag() const
{
	return Abs(impl);
}

template<>
inline
bool IntervalWrapper<ProfilInterval>::isEmpty() const
{
	return false;	// Profil does not support empty intervals
}

template<>
inline
bool IntervalWrapper<ProfilInterval>::contains(const IntervalWrapper& i) const
{
	return i.impl <= impl;
}

template<>
inline
bool IntervalWrapper<ProfilInterval>::contains(const Real& r) const
{
	return r <= impl;
}

template<>
inline
Real IntervalWrapper<ProfilInterval>::getSignedMig() const
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
Real IntervalWrapper<ProfilInterval>::getMedian() const
{
	return Mid(impl);
}

template<>
inline
bool IntervalWrapper<ProfilInterval>::isSymmetric() const
{
	return -getLb() == getUb();
}

template<>
inline
IntervalWrapper<ProfilInterval> IntervalWrapper<ProfilInterval>::empty()
{
	throw Procure::Exception::Unsupported("Profil empty interval");
}

template<>
inline
IntervalWrapper<ProfilInterval> IntervalWrapper<ProfilInterval>::getMedianInterval() const
{
	if (isEmpty())
		return empty();

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
	return ProfilInterval(lb,ub);
}

template<>
inline
bool IntervalWrapper<ProfilInterval>::isSingleton() const
{
	return getLb()==getUb();
}

template<>
inline
bool IntervalWrapper<ProfilInterval>::isFinite() const
{
	return getLb()!=-std::numeric_limits<Real>::infinity() and
			getUb()!=std::numeric_limits<Real>::infinity() and
			getLb()!=std::numeric_limits<Real>::infinity() and
			getUb()!=-std::numeric_limits<Real>::infinity();
}

template<>
inline
bool IntervalWrapper<ProfilInterval>::isCanonical() const
{
	return Pred(impl)==impl;	// FIXME: not sure this is correct
}

template<>
inline
bool IntervalWrapper<ProfilInterval>::strictlyContains(
    const IntervalWrapper& i) const
{
	return i.impl < impl;
}

template<>
inline
bool IntervalWrapper<ProfilInterval>::isDisjoint(const IntervalWrapper& i) const
{
	return getUb() < i.getLb() or getLb() > i.getUb();
}

template<>
inline
IntervalWrapper<ProfilInterval>& IntervalWrapper<ProfilInterval>::operator=(
    const IntervalWrapper& i)
{
	impl = i.impl;
	return *this;
}

template<>
inline
IntervalWrapper<ProfilInterval>& IntervalWrapper<ProfilInterval>::operator+=(
    const IntervalWrapper& i)
{
	impl += i.impl;
	return *this;
}

template<>
inline
IntervalWrapper<ProfilInterval>& IntervalWrapper<ProfilInterval>::operator-=(
    const IntervalWrapper& i)
{
	impl -= i.impl;
	return *this;
}

template<>
inline
IntervalWrapper<ProfilInterval>& IntervalWrapper<ProfilInterval>::operator*=(
    const IntervalWrapper& i)
{
	impl *= i.impl;
	return *this;
}

template<>
inline
IntervalWrapper<ProfilInterval>& IntervalWrapper<ProfilInterval>::operator/=(
    const IntervalWrapper &i)
{
	impl /= i.impl;
	return *this;
}

template<>
inline
IntervalWrapper<ProfilInterval>& IntervalWrapper<ProfilInterval>::operator&=(
    const IntervalWrapper &i)
{
	INTERVAL res;
	if (not Intersection(res,impl,i.impl))
		impl = empty().impl;
	else
		impl = res;
	return *this;
}

template<>
inline
IntervalWrapper<ProfilInterval>& IntervalWrapper<ProfilInterval>::operator|=(
    const IntervalWrapper &i)
{
	impl = Hull(impl,i.impl);
	return *this;
}

template<>
inline
const IntervalWrapper<ProfilInterval>& IntervalWrapper<ProfilInterval>::operator+() const
{
	return *this;
}

template<>
inline
IntervalWrapper<ProfilInterval> IntervalWrapper<ProfilInterval>::operator-() const
{
	return -impl;
}

template<>
inline
IntervalWrapper<ProfilInterval> IntervalWrapper<ProfilInterval>::inverse() const
{
	return 1.0/impl;
}

template<>
inline
IntervalWrapper<ProfilInterval> IntervalWrapper<ProfilInterval>::whole()
{
	return IntervalWrapper(-std::numeric_limits<Real>::infinity(),
	                       std::numeric_limits<Real>::infinity());
}

template<>
inline
std::pair<IntervalWrapper<ProfilInterval>,IntervalWrapper<ProfilInterval>> IntervalWrapper<
    ProfilInterval>::bisect() const
{
	Real mid = getMedian();
	IntervalWrapper lh(getLb(),mid);
	IntervalWrapper uh(mid,getUb());
	return std::make_pair(lh,uh);
}

template<>
inline
IntervalWrapper<ProfilInterval> IntervalWrapper<ProfilInterval>::getLowerHalf() const
{
	return IntervalWrapper(getLb(),getMedian());
}

template<>
inline
IntervalWrapper<ProfilInterval> IntervalWrapper<ProfilInterval>::getUpperHalf() const
{
	return IntervalWrapper(getMedian(),getUb());
}

} // Detail

#define OP_DEF_2(Ret,Op)	\
		inline Ret operator Op(const Detail::IntervalWrapper<Detail::ProfilInterval>& i1,\
		                       const Detail::IntervalWrapper<Detail::ProfilInterval>& i2)\
		{\
			return i1.getImpl() Op i2.getImpl(); \
		}\

#define OP_DEF_2_R(Ret,Op)	\
		OP_DEF_2(Ret,Op)\
		inline Ret operator Op(const Real& r,const Detail::IntervalWrapper<Detail::ProfilInterval>& i)\
		{\
			return r Op i.getImpl(); \
		}\
		inline Ret operator Op(const Detail::IntervalWrapper<Detail::ProfilInterval>& i,const Real& r)\
		{\
			return i.getImpl() Op r; \
		}

#define OP_UNDEF_2(Ret,Op)	\
		inline Ret operator Op(const Detail::IntervalWrapper<Detail::ProfilInterval>&,\
													const Detail::IntervalWrapper<Detail::ProfilInterval>&)\
		{	throw Procure::Exception::Unsupported(#Op);	}

#define OP_UNDEF_2_R(Ret,Op)	\
		OP_UNDEF_2(Ret,Op)\
		inline Ret operator Op(const Real& r,const Detail::IntervalWrapper<Detail::ProfilInterval>& i)\
		{	throw Procure::Exception::Unsupported(#Op);	}\
		inline Ret operator Op(const Detail::IntervalWrapper<Detail::ProfilInterval>& i,const Real& r)\
		{	throw Procure::Exception::Unsupported(#Op);	}

/// Comparison operators involving intervals.
/// @{

OP_DEF_2	(bool,==)
OP_DEF_2	(bool,!=)
OP_UNDEF_2	(bool,>)
OP_UNDEF_2	(bool,>=)
OP_UNDEF_2	(bool,<)
OP_UNDEF_2	(bool,<=)

/// @}

/// Arithmetic operators involving intervals.
/// @{

OP_DEF_2_R	(Detail::IntervalWrapper<Detail::ProfilInterval>,+)
OP_DEF_2_R	(Detail::IntervalWrapper<Detail::ProfilInterval>,-)
OP_DEF_2_R	(Detail::IntervalWrapper<Detail::ProfilInterval>,*)
OP_DEF_2_R	(Detail::IntervalWrapper<Detail::ProfilInterval>,/)

/// @}

#define FUN_DEF_1(Op,BiasFunc)\
	inline\
	Detail::IntervalWrapper<Detail::ProfilInterval> Op(const Detail::IntervalWrapper<Detail::ProfilInterval>& i)\
	{\
		return BiasFunc(i.getImpl());	\
	}

/// Algebraic operators involving intervals.
/// @{

FUN_DEF_1(abs,IAbs)
FUN_DEF_1(sqrt,Sqrt)
FUN_DEF_1(sqr,Sqr)

inline
Detail::IntervalWrapper<Detail::ProfilInterval> pow(const Detail::IntervalWrapper<Detail::ProfilInterval>& i, int y)
{	
	return Power(i.getImpl(),y);
}

inline
Detail::IntervalWrapper<Detail::ProfilInterval> pow(const Detail::IntervalWrapper<Detail::ProfilInterval>& i1,
                                  const Detail::IntervalWrapper<Detail::ProfilInterval>& i2)
{	
	return Power(i1.getImpl(),i2.getImpl());
}

inline
Detail::IntervalWrapper<Detail::ProfilInterval> nth_root(const Detail::IntervalWrapper<Detail::ProfilInterval>& i,
                                       unsigned int y)
{	
	return Root(i.getImpl(),y);
}


/// @}

/// Transcendental operators involving intervals.
/// @{

FUN_DEF_1(exp,Exp)
FUN_DEF_1(log,Log)


/// @}

/// Modulo operator involving intervals.
/// @{

OP_UNDEF_2_R(Detail::IntervalWrapper<Detail::ProfilInterval>,%)

/// @}

/// Trignometric functions involving intervals.
/// @{

FUN_DEF_1(sin,Sin)
FUN_DEF_1(cos,Cos)
FUN_DEF_1(tan,Tan)

FUN_DEF_1(asin,ArcSin)
FUN_DEF_1(acos,ArcCos)
FUN_DEF_1(atan,ArcTan)

FUN_DEF_1(sinh,Sinh)
FUN_DEF_1(cosh,Cosh)
FUN_DEF_1(tanh,Tanh)

FUN_DEF_1(asinh,ArSinh)
FUN_DEF_1(acosh,ArCosh)
FUN_DEF_1(atanh,ArTanh)

/// @}

/// min/max functions involving intervals.
/// @{

inline
Detail::IntervalWrapper<Detail::ProfilInterval> min(const Detail::IntervalWrapper<Detail::ProfilInterval>& i1,
                                  const Detail::IntervalWrapper<Detail::ProfilInterval>& i2)
{	throw Procure::Exception::Unsupported("min");	}

inline
Detail::IntervalWrapper<Detail::ProfilInterval> max(const Detail::IntervalWrapper<Detail::ProfilInterval>& i1,
                                  const Detail::IntervalWrapper<Detail::ProfilInterval>& i2)
{	throw Procure::Exception::Unsupported("max");	}


/// @}

#undef OP_DEF_2
#undef OP_DEF_2_R
#undef OP_UNDEF_2
#undef OP_UNDEF_2_R

} // Procure
