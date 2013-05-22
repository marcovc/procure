/*
 * interval.cpp
 *
 *  Created on: May 19, 2013
 *      Author: marco
 */

#include <procure/procure.hpp>
#include <iostream>

#include <random>
#include <vector>

#include <fstream>
#include <chrono>
#include <ctime>

using namespace Procure;

namespace Procure {
template<class Impl>
Detail::IntervalWrapper<Impl>
min(const Real&,const Detail::IntervalWrapper<Impl>&)
{	throw Procure::Exception::Unsupported("min(real,interval)"); }

template<class Impl>
Detail::IntervalWrapper<Impl>
min(const Detail::IntervalWrapper<Impl>&,const Real&)
{	throw Procure::Exception::Unsupported("min(interval,real)"); }

template<class Impl>
Detail::IntervalWrapper<Impl>
max(const Real&,const Detail::IntervalWrapper<Impl>&)
{	throw Procure::Exception::Unsupported("max(real,interval)"); }

template<class Impl>
Detail::IntervalWrapper<Impl>
max(const Detail::IntervalWrapper<Impl>&,const Real&)
{	throw Procure::Exception::Unsupported("max(interval,real)"); }

template<class Impl>
Detail::IntervalWrapper<Impl>
pow(const Real&,const Detail::IntervalWrapper<Impl>&)
{	throw Procure::Exception::Unsupported("pow(real,interval)"); }

template<class Impl>
Detail::IntervalWrapper<Impl>
pow(const Detail::IntervalWrapper<Impl>&,const Real&)
{	throw Procure::Exception::Unsupported("pow(interval,real)"); }
};

std::vector<Interval> genRandomIntervals(unsigned int n, Real lb, Real ub)
{
	std::default_random_engine generator;
	std::uniform_real_distribution<Real> distribution(lb,ub);
	std::vector<Interval> r;
	for (unsigned int i = 0; i < n; ++i)
	{
		Real b1 = distribution(generator);
		Real b2 = distribution(generator);
		if (b1<=b2)
			r.push_back(Interval(b1,b2));
		else
			r.push_back(Interval(b2,b1));
	}
	return r;
}

bool isCorrect(const std::function<Interval(Interval)>& fn,
               const std::function<Real(Real)>& ptfn,
               const Interval& in, int nbSamples)
{
	Interval out = fn(in);
	std::default_random_engine generator;
	std::uniform_real_distribution<Real> distribution(in.getLb(),in.getUb());
	std::vector<Real> xx(nbSamples);
	xx[0] = in.getLb();
	xx[1] = in.getUb();
	for (unsigned int i = 2; i < nbSamples; ++i)
		xx[i] = distribution(generator);
	for (unsigned int i = 0; i < nbSamples; ++i)
	{
		Real x = xx[i];
		Real y = ptfn(x);
		if (not std::isnan(y) and not out.contains(y))
		{
			std::cout << "failed correctness test:\n";
			std::cout << "fn(" << in << ")=" << out << std::endl;
			std::cout << "fn(" << x << ")=" << y << std::endl;
			std::cout << "(" << y << " NOT in " << out << ")" << std::endl;
			return false;
		}
	}
	return true;
}

bool isCorrect(const std::function<Interval(Interval,Interval)>& fn,
               const std::function<Real(Real,Real)>& ptfn,
               const Interval& in1, const Interval& in2,	int nbSamples)
{
	Interval out = fn(in1,in2);
	std::default_random_engine generator;
	std::uniform_real_distribution<Real> distribution1(in1.getLb(),in1.getUb());
	std::uniform_real_distribution<Real> distribution2(in2.getLb(),in2.getUb());
	const unsigned int sqrtNbSamples = ::sqrt(nbSamples);
	std::vector<Real> xx1(sqrtNbSamples);
	std::vector<Real> xx2(sqrtNbSamples);
	xx1[0] = in1.getLb();
	xx1[1] = in1.getUb();
	xx2[0] = in2.getLb();
	xx2[1] = in2.getUb();
	for (unsigned int i = 2; i < sqrtNbSamples; ++i)
	{
		xx1[i] = distribution1(generator);
		xx2[i] = distribution2(generator);
	}
	for (unsigned int i1 = 0; i1 < sqrtNbSamples; ++i1)
		for (unsigned int i2 = 0; i2 < sqrtNbSamples; ++i2)
		{
			Real x1 = xx1[i1];
			Real x2 = xx2[i2];
			Real y = ptfn(x1,x2);
			if (not std::isnan(y) and not out.contains(y))
			{
				std::cout << "failed correctness test:\n";
				std::cout << "fn(" << in1 << "," << in2 << ")=" << out << std::endl;
				std::cout << "fn(" << x1 << "," << x2 << ")=" << y << std::endl;
				std::cout << "(" << y << " NOT in " << out << ")" << std::endl;
				return false;
			}
		}
	return true;
}

bool isMonotonic(const std::function<Interval(Interval)>& fn,
                 const Interval& i1, int nbSplits)
{
	Interval in = i1;
	Interval out = fn(in);
	for (unsigned int i = 0; i < nbSplits; ++i)
	{
		Interval newIn = (i%2)?in.getLowerHalf():in.getUpperHalf();
		Interval newOut = fn(newIn);
		if (not out.contains(newOut))
		{
			std::cout << "failed monotonicity test:\n";
			std::cout << newIn << " is a subset of " << in << std::endl;
			std::cout << "fn(" << newIn << ")=" << newOut << std::endl;
			std::cout << "fn(" << in << ")=" << out << std::endl;
			std::cout << "and " << newOut << " is NOT a subset of " << out
					<< std::endl;
			return false;
		}
		in = newIn;
		out = newOut;
	}
	return true;
}

bool isMonotonic(const std::function<Interval(Interval,Interval)>& fn,
                 const Interval& i1, const Interval& i2, int nbSplits)
{
	Interval in1 = i1;
	Interval in2 = i2;
	Interval out = fn(in1,in2);
	for (unsigned int i = 0; i < nbSplits; ++i)
	{
		Interval newIn1(in1),newIn2(in2);
		switch (i%4)
		{
			case 0:
				newIn1 = in1.getLowerHalf();
				break;
			case 1:
				newIn2 = in2.getLowerHalf();
				break;
			case 2:
				newIn1 = in1.getUpperHalf();
				break;
			case 3:
				newIn2 = in2.getUpperHalf();
				break;
		}
		Interval newOut = fn(newIn1,newIn2);
		if (not out.contains(newOut))
		{
			std::cout << "failed monotonicity test:\n";
			std::cout << newIn1 << " is a subset of " << in1 << std::endl;
			std::cout << newIn2 << " is a subset of " << in2 << std::endl;
			std::cout << "fn(" << newIn1 << "," << newIn2 << ")=" << newOut
					<< std::endl;
			std::cout << "fn(" << in1 << "," << in2 << ")=" << out << std::endl;
			std::cout << "and " << newOut << " is NOT a subset of " << out
					<< std::endl;
			return false;
		}
		in1 = newIn1;
		in2 = newIn2;
		out = newOut;
	}
	return true;
}


const Real inf = std::numeric_limits<Real>::infinity();
const Real maxd = std::numeric_limits<Real>::max();

template<class Func>
struct Collect;

template<>
struct Collect<std::function<Real(Real)>>
{
	static const std::vector<std::function<Real(Real)>> fn;
	std::function<Real(Real)> operator[](unsigned int i)
	{	return fn[i];	}
	static unsigned int size() {	return fn.size();	}
	static const std::vector<Interval> dom;
	static const std::vector<std::string> name;
};

template<class Impl>
struct Collect<std::function<Detail::IntervalWrapper<Impl>(
														 Detail::IntervalWrapper<Impl>)>>
{
	typedef Detail::IntervalWrapper<Impl> Interval;
	typedef Interval (*funtype)(const Interval&);
	static const std::vector<std::function<Interval(Interval)>> fn;
	static unsigned int size() {	return fn.size();	}

	std::function<Interval(Interval)> operator[](unsigned int i)
	{	return fn[i];	}
};


template<>
struct Collect<std::function<Real(Real,Real)>>
{
	typedef Real (*funtype)(Real,Real);
	static const std::vector<std::function<Real(Real,Real)>> fn;
	static unsigned int size() {	return fn.size();	}

	std::function<Real(Real,Real)> operator[](unsigned int i)
	{	return fn[i];	}

	static const std::vector<std::string> name;
};

template<class T1,class T2, class T3>
struct Collect<std::function<T1(const T2&, const T3&)>>
{
	typedef T1 (*funtype)(const T2&, const T3&);
	static const std::vector<std::function<T1(const T2&,const T3&)>> fn;
	static unsigned int size() {	return fn.size();	}

	std::function<T1(const T2&, const T3&)> operator[](unsigned int i)
	{	return fn[i];	}
};

const std::vector<std::function<Real(Real)>>
Collect<std::function<Real(Real)>>::fn =
{
	::fabs,::sqrt,[](Real x){return x*x;},::exp,
	::log,::sin,::cos,::tan,::asin,::acos,::atan,::sinh,::cosh,
	 ::tanh,::asinh,::acosh,::atanh
};

const std::vector<Interval>
Collect<std::function<Real(Real)>>::dom =
{
	Interval(-inf,inf),	//abs
	Interval(0,inf),			//sqrt
	Interval(-inf,inf),	//sqr
	Interval(-inf,inf),	//exp
	Interval(std::numeric_limits<Real>::epsilon(),inf),	//log
	Interval(-maxd,maxd),	//sin
	Interval(-maxd,maxd),	//cos
	Interval(-maxd,maxd),	//tan
	Interval(-1.0,1.0),	//asin
	Interval(-1.0,1.0),	//acos
	Interval(-inf,inf),	//atan
	Interval(-inf,inf),	//sinh
	Interval(-inf,inf),	//cosh
	Interval(-inf,inf),	//tanh
	Interval(-inf,inf),	//asinh
	Interval(1.0,inf),		//acosh
	Interval(-1.0,1.0),	//atanh
};

const std::vector<std::string>
Collect<std::function<Real(Real)>>::name =
{
	"abs","sqrt","sqr","exp","log","sin","cos","tan",
	"asin","acos","atan","sinh","cosh","tanh","asinh","acosh","atanh"
};

template<class Impl>
const std::vector<std::function<Detail::IntervalWrapper<Impl>(
		Detail::IntervalWrapper<Impl>)>>
		Collect<std::function<Detail::IntervalWrapper<Impl>(
														 Detail::IntervalWrapper<Impl>)>>::fn =
{
	static_cast<funtype>(abs),
	static_cast<funtype>(sqrt),
	static_cast<funtype>(sqr),
	static_cast<funtype>(exp),
	static_cast<funtype>(log),
	static_cast<funtype>(sin),
	static_cast<funtype>(cos),
	static_cast<funtype>(tan),
	static_cast<funtype>(asin),
	static_cast<funtype>(acos),
	static_cast<funtype>(atan),
	static_cast<funtype>(sinh),
	static_cast<funtype>(cosh),
	static_cast<funtype>(tanh),
	static_cast<funtype>(asinh),
	static_cast<funtype>(acosh),
	static_cast<funtype>(atanh)
};

const std::vector<std::function<Real(Real,Real)>>
Collect<std::function<Real(Real,Real)>>::fn =
{
	[](Real x,Real y){return x+y;},
	[](Real x,Real y){return x-y;},
	[](Real x,Real y){return x*y;},
	[](Real x,Real y){return x/y;},
//	static_cast<funtype>(std::pow),
	[](Real x,Real y){return std::min(x,y);},
	[](Real x,Real y){return std::max(x,y);}
};

const std::vector<std::string>
Collect<std::function<Real(Real,Real)>>::name =
{
	"+","-","*","/","min","max"
};

template<class T1,class T2, class T3>
const std::vector<std::function<T1(const T2&,const T3&)>>
Collect<std::function<T1(const T2&, const T3&)>>::fn =
{
	static_cast<funtype>(Procure::operator+),
	static_cast<funtype>(Procure::operator-),
	static_cast<funtype>(Procure::operator*),
	static_cast<funtype>(Procure::operator/),
	//static_cast<funtype>(Procure::pow),
	static_cast<funtype>(Procure::min),
	static_cast<funtype>(Procure::max)
};

Interval toFn1Dom(unsigned int f, const Interval& i)
{
	Interval r = i;
	typedef	Collect<std::function<Real(Real)> > FnR;
	r &= FnR::dom[f];
	return r;
}

Collect<std::function<Real(Real)> > fr;
Collect<std::function<Interval(Interval)> > fi;
Collect<std::function<Real(Real,Real)> > frr;
Collect<std::function<Interval(const Real&,const Interval&)> > fri;
Collect<std::function<Interval(const Interval&,const Real&)> > fir;
Collect<std::function<Interval(const Interval&,const Interval&)> > fii;

void testCorrectness(const std::vector<Interval>& in,
                    const unsigned int nbSamples = 10)
{
	for (unsigned int f = 0; f < fi.size(); ++f)
	{
		try {
			for (unsigned int i = 0; i < in.size(); ++i)
				if (not toFn1Dom(f,in[i]).isEmpty() and
						not isCorrect(fi[f],fr[f],toFn1Dom(f,in[i]),nbSamples))
				{
					std::cout << "with fn = " << fr.name[f] << std::endl;
					std::cout << "----\n";
					break;
				}
		} catch (const Procure::Exception::Unsupported& e)
		{
			std::cout << "warning: " << e.what() << std::endl;
		}
	}

	for (unsigned int f = 0; f < fii.size(); ++f)
	{
		try {
			for (unsigned int i = 0; i < in.size()-1; ++i)
				if (not isCorrect(fii[f],frr[f],in[i],in[i+1],nbSamples))
				{
					std::cout << "with fn = " << frr.name[f] << std::endl;
					std::cout << "----\n";
					break;
				}
		} catch (const Procure::Exception::Unsupported& e)
		{
			std::cout << "warning: " << e.what() << std::endl;
		}
	}

	using namespace std::placeholders;

	for (unsigned int f = 0; f < fir.size(); ++f)
	{
		try {
			for (unsigned int i = 0; i < in.size()-1; ++i)
				if (not isCorrect(std::bind(fir[f],_1,in[i+1].getLb()),
				                  std::bind(frr[f],_1,in[i+1].getLb()),
				                  in[i],nbSamples))
				{
					std::cout << "with fn = " << frr.name[f] << std::endl;
					std::cout << "----\n";
					break;
				}
		} catch (const Procure::Exception::Unsupported& e)
		{
			std::cout << "warning: " << e.what() << std::endl;
		}
	}

	for (unsigned int f = 0; f < fri.size(); ++f)
	{
		try {
			for (unsigned int i = 0; i < in.size()-1; ++i)
				if (not isCorrect(std::bind(fri[f],in[i+1].getLb(),_1),
				                  std::bind(frr[f],in[i+1].getLb(),_1),
				                  in[i],nbSamples))
				{
					std::cout << "with fn = " << frr.name[f] << std::endl;
					std::cout << "----\n";
					break;
				}
		} catch (const Procure::Exception::Unsupported& e)
		{
			std::cout << "warning: " << e.what() << std::endl;
		}
	}
}

void testMonotonicity(const std::vector<Interval>& in,
                     const unsigned int nbSplits = 10)
{
	for (unsigned int f = 0; f < fi.size(); ++f)
	{
		try {
			for (unsigned int i = 0; i < in.size(); ++i)
				if (not toFn1Dom(f,in[i]).isEmpty() and
						not isMonotonic(fi[f],in[i],nbSplits))
				{
					std::cout << "with fn = " << fr.name[f] << std::endl;
					std::cout << "----\n";
					break;
				}
		} catch (const Procure::Exception::Unsupported& e)
		{
			std::cout << "warning: " << e.what() << std::endl;
		}
	}

	for (unsigned int f = 0; f < fii.size(); ++f)
	{
		try {
			for (unsigned int i = 0; i < in.size()-1; ++i)
				if (not isMonotonic(fii[f],in[i],in[i+1],nbSplits))
				{
					std::cout << "with fn = " << frr.name[f] << std::endl;
					std::cout << "----\n";
					break;
				}
		} catch (const Procure::Exception::Unsupported& e)
		{
			std::cout << "warning: " << e.what() << std::endl;
		}
	}

	using namespace std::placeholders;

	for (unsigned int f = 0; f < fir.size(); ++f)
	{
		try {
			for (unsigned int i = 0; i < in.size()-1; ++i)
				if (not isMonotonic(std::bind(fir[f],_1,in[i+1].getLb()),
						                in[i],nbSplits))
				{
					std::cout << "with fn = " << frr.name[f] << std::endl;
					std::cout << "----\n";
					break;
				}
		} catch (const Procure::Exception::Unsupported& e)
		{
			std::cout << "warning: " << e.what() << std::endl;
		}
	}

	for (unsigned int f = 0; f < fri.size(); ++f)
	{
		try {
			for (unsigned int i = 0; i < in.size()-1; ++i)
				if (not isMonotonic(std::bind(fri[f],in[i+1].getLb(),_1),
						                in[i],nbSplits))
				{
					std::cout << "with fn = " << frr.name[f] << std::endl;
					std::cout << "----\n";
					break;
				}
		} catch (const Procure::Exception::Unsupported& e)
		{
			std::cout << "warning: " << e.what() << std::endl;
		}
	}
}

template<class Impl2>
int compare(const std::vector<Interval>& in)
{
	typedef Interval	Interval1;
	typedef Detail::IntervalWrapper<Impl2>	Interval2;

	Collect<std::function<Interval2(Interval2)> > fi2;
	Collect<std::function<Interval2(const Real&,const Interval2&)> > fri2;
	Collect<std::function<Interval2(const Interval2&,const Real&)> > fir2;
	Collect<std::function<Interval2(const Interval2&,const Interval2&)> > fii2;

	std::vector<Interval2> in2(in.size());
	for (unsigned int i = 0; i < in.size(); ++i)
		in2[i] = Interval2(in[i].getLb(),in[i].getUb());

	unsigned int b,b2,incomp,count;
	b=b2=incomp=count=0;
	long double bsum = 0;
	long double b2sum = 0;

	auto accum = [&](const Interval& out, const Interval2& out2,
			unsigned int f) {
		if (out.strictlyContains(Interval(out2.getLb(),out2.getUb())))
		{
			++b2;
			b2sum += out2.getLb()-out.getLb() + out.getUb()-out2.getUb();
		}
		else
		if (out2.strictlyContains(Interval2(out.getLb(),out.getUb())))
		{
			++b;
			bsum += out.getLb()-out2.getLb() + out2.getUb()-out.getUb();
		}
		else
		if (out.isDisjoint(Interval(out2.getLb(),out2.getUb())))
			++incomp;
		++count;
	};

	for (unsigned int f = 0; f < fi.size(); ++f)
	{
		try {
			for (unsigned int i = 0; i < in.size(); ++i)
				if (not toFn1Dom(f,in[i]).isEmpty())
				{
					Interval out = fi[f](in[i]);
					Interval2 out2 = fi2[f](in2[i]);
					accum(out,out2,f);
				}
		} catch (const Procure::Exception::Unsupported& e)
		{
			std::cout << "warning: " << e.what() << std::endl;
		}
	}

	for (unsigned int f = 0; f < fii.size(); ++f)
	{
		try {
			for (unsigned int i = 0; i < in.size()-1; ++i)
			{
				Interval out = fii[f](in[i],in[i+1]);
				Interval2 out2 = fii2[f](in2[i],in2[i+1]);
				accum(out,out2,f);
			}
		} catch (const Procure::Exception::Unsupported& e)
		{
			std::cout << "warning: " << e.what() << std::endl;
		}
	}

	using namespace std::placeholders;

	for (unsigned int f = 0; f < fir.size(); ++f)
	{
		try {
			for (unsigned int i = 0; i < in.size()-1; ++i)
			{
				Interval out = std::bind(fir[f],_1,in[i+1].getLb())(in[i]);
				Interval2 out2 = std::bind(fir2[f],_1,in2[i+1].getLb())(in2[i]);
				accum(out,out2,f);
			}
		} catch (const Procure::Exception::Unsupported& e)
		{
			std::cout << "warning: " << e.what() << std::endl;
		}
	}

	for (unsigned int f = 0; f < fri.size(); ++f)
	{
		try {
			for (unsigned int i = 0; i < in.size()-1; ++i)
			{
				Interval out = std::bind(fri[f],in[i+1].getLb(),_1)(in[i]);
				Interval2 out2 = std::bind(fri2[f],in2[i+1].getLb(),_1)(in2[i]);
				accum(out,out2,f);
			}
		} catch (const Procure::Exception::Unsupported& e)
		{
			std::cout << "warning: " << e.what() << std::endl;
		}
	}

	std::cout << "% impl1 strictly contained in impl2: " << b/((double)(count));
	std::cout << " (average diff is " << bsum/((double)b) << ")" << std::endl;
	std::cout << "% impl2 strictly contained in impl1: " << b2/((double)(count));
	std::cout << " (average diff is " << b2sum/((double)b2) << ")" << std::endl;
	std::cout << "% disjoint: " << incomp/((double)(count)) << std::endl;
	return 0;
}

void time(const std::vector<Interval>& in, unsigned int nbSamples)
{
	std::clock_t c_start = std::clock();
	unsigned int count = 0;

	for (unsigned int f = 0; f < fi.size(); ++f)
	{
		try {
			for (unsigned int i = 0; i < in.size(); ++i)
				if (not toFn1Dom(f,in[i]).isEmpty())
				{
					for (unsigned int s = 0; s < nbSamples; ++s)
						fi[f](in[i]);
					count += 1;
				}
		} catch (const Procure::Exception::Unsupported& e)
		{
			std::cout << "warning: " << e.what() << std::endl;
		}
	}

	for (unsigned int f = 0; f < fii.size(); ++f)
	{
		try {
			for (unsigned int i = 0; i < in.size()-1; ++i)
				for (unsigned int s = 0; s < nbSamples; ++s)
					fii[f](in[i],in[i+1]);
		} catch (const Procure::Exception::Unsupported& e)
		{
			std::cout << "warning: " << e.what() << std::endl;
		}
	}
	count += (in.size()-1)*fii.size();

	using namespace std::placeholders;

	for (unsigned int f = 0; f < fir.size(); ++f)
	{
		try {
			for (unsigned int i = 0; i < in.size()-1; ++i)
				for (unsigned int s = 0; s < nbSamples; ++s)
					std::bind(fir[f],_1,in[i+1].getLb())(in[i]);
		} catch (const Procure::Exception::Unsupported& e)
		{
			std::cout << "warning: " << e.what() << std::endl;
		}
	}
	count += (in.size()-1)*fir.size();

	for (unsigned int f = 0; f < fri.size(); ++f)
	{
		try {
			for (unsigned int i = 0; i < in.size()-1; ++i)
				for (unsigned int s = 0; s < nbSamples; ++s)
					std::bind(fri[f],in[i+1].getLb(),_1)(in[i]);
		} catch (const Procure::Exception::Unsupported& e)
		{
			std::cout << "warning: " << e.what() << std::endl;
		}
	}
	count += (in.size()-1)*fri.size();
	count *= nbSamples;

	std::clock_t c_end = std::clock();
	double ms = 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC;
	std::cout << "CPU time per interval operation: " << ms / count << " ms\n";
	std::cout << "Total CPU time: " << ms << " ms\n";
}

int main(int argc, char** argv)
{
	Interval::setOutputFormat(Interval::OutputFormat::bounds);
	Interval::setOutputPrecision(17);

	auto in = genRandomIntervals(10000,-5,5);

	try {
		testCorrectness(in,10);
		testMonotonicity(in,10);
		compare<Detail::GaolInterval>(in);
		time(in,100);
	}
	catch (const std::exception& e)
	{
		std::cout << "error: " << e.what() << std::endl;
	}
}



