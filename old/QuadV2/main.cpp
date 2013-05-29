#include <cstdlib>
#include <vector>
#include <map>

#include <cmath>

#include "chronos.h"
#include "io.h"
#include "Tensor.h"
#include "Menu.h"
#include "Inverse.h"

#include <Interval.h>
#include <LongInterval.h>
#include <IntervalVector.h>
#include <Utilities.h>
#include <Functions.h>

#include <process.h>

#include <sstream>
#include <iomanip>

//#define __G_DISK_2D__
//#define __G_DISK_3D__
//#define __G_A__
//#define __G_B__
//#define __G_C__
//#define __G_D__
//#define __G_H1__
//#define __G_H2__
//#define __G_H3__
//#define __G_H3b__
//#define __G_H4__
//#define __G_H5__
//#define __G_H6__
//#define __G_H7__
//#define __G_H8__
//#define __G_H9__
//#define __G_H10__
//#define __G_H11__
//#define __G_H12__
//#define __G_H13__
//#define __G_H14__
#define __G_H15__
//#define __G_H16__

using namespace std;

Tensor* d;
bool constrained;

int step =500;            // shows data every step bisections (set step = 1000000000 to disable this function)
double stop = -1.0;         // prevents to split boxes with: w(q) < stop  (set stop = -1.0 to disable this function)
bool timePlot = false;       // prints to file _timing_Ord(w/f) every time steps
double maxTime = 5000;        // if timeplot then interrupts when time >= maxTime (set timeplot = false to disable this function)
bool divisionPlot = false;  // prints to file _timing_Ord(w/f) every division steps
int maxDiv = 6;             // if divisionplot then interrupts when nDiv >= maxDiv (set timeplot = false to disable this function)
bool saveBoxes = false;     // saves inner and boundary boxes

double volume(const INTERVAL_VECTOR & x)
{
	double result=1.0;
	for (int i = 1; i <= Dimension(x); i++)
		result *= Diam(x(i));
	return result;
}

INTERVAL_VECTOR BisectIntervalVector(INTERVAL_VECTOR & X1)
{
	INTERVAL_VECTOR X2=X1;
	int kmax=Dimension(X1);
	double dmax=Diam(X1(kmax));
	for(int k=kmax-1; k>=1; k--)
		if(Diam(X1(k))>dmax)
		{
			kmax=k;
			dmax=Diam(X1(k));
		}
    double a = Inf(X1(kmax));
    double b = Sup(X1(kmax));
    double c = Mid(X1(kmax));
	X1(kmax)=INTERVAL(a,c);
	X2(kmax)=INTERVAL(c,b);
	return X2;
}

//*****************************************************************************

double key(pair<INTERVAL_VECTOR,INTERVAL> p, int adapt)
{
	if(adapt)
		return -Diam(p.second);
	else
		return -Max(Diam(p.first));
}

//*****************************************************************************

INTERVAL g(INTERVAL_VECTOR x)
{
	INTERVAL y, y1, y2;
#ifdef __G_NO_2D__
	y=-1.;
#endif
#ifdef __G_DISK_2D__
	y=(Sqr(x(1)-1)+Sqr(x(2)-1)-1.);
#endif
#ifdef __G_DISK_3D__
	y=(Sqr(x(1)-1)+Sqr(x(2)-1)+Sqr(x(3)-1)-1.);
#endif
#ifdef __G_LINE_2D__
	y=x(1)+x(2)-1.;
#endif
#ifdef __G_A__
	y1=Sqr(x(1))+Sqr(x(2))-4.;
	if (Inf(y1)>=0.0) return y1;
	y2=x(2)-Sin(x(1));
	if (Sup(y1)<=0.0) return y2;
	if (Inf(y2)>=0.0) return y2;
	return y1;
#endif
#ifdef __G_B__
	y1=x(1)*(x(1)+1)-x(2);
	if (Inf(y1)>=0.0) return y1;
	y2=x(2)-Cos(x(1));
	if (Sup(y1)<=0.0) return y2;
	if (Inf(y2)>=0.0) return y2;
	return y1;
#endif
#ifdef __G_C__
	y1=(Sqr(x(1))+Sqr(x(2))+Sqr(x(3))-4.);
	if (Inf(y1)>=0.0) return y1;
	y2=(Sqr(x(1)-1)+Sqr(x(2))-x(3));
	if (Sup(y1)<=0.0) return y2;
	if (Inf(y2)>=0.0) return y2;
	return y1;
#endif

#ifdef __G_D__
	y=(Sqr(x(1))+Sqr(x(2))-1.);
#endif

#ifdef __G_H1__
	y=Cos(3*x(1)) + Cos(3*x(2))-0.5;
#endif

#ifdef __G_H2__
	y=Sqr(x(1))*x(2) + Sqr(x(2))*x(1)-0.5;
#endif

#ifdef __G_H3__
    y1=(Exp(x(1))*Power(x(2),3) + Exp(x(2))*Power(x(1),3) -8.0);
	if (Inf(y1)>=0.0) return y1;
	y2=(-Exp(x(1))*Power(x(2),3) - Exp(x(2))*Power(x(1),3) -1.5);
	if (Sup(y1)<=0.0) return y2;
	if (Inf(y2)>=0.0) return y2;
	return y1;
#endif

#ifdef __G_H3b__
    y1=(Exp(x(1)+1.5)*Power(x(2)+1.5,3) + Exp(x(2)+1.5)*Power(x(1)+1.5,3) -8.0);
	if (Inf(y1)>=0.0) return y1;
	y2=(-Exp(x(1)+1.5)*Power(x(2)+1.5,3) - Exp(x(2)+1.5)*Power(x(1)+1.5,3) -1.5);
	if (Sup(y1)<=0.0) return y2;
	if (Inf(y2)>=0.0) return y2;
	return y1;
#endif



#ifdef __G_H4__
	y= -Sin(3*x(1)) - Sqr(Sin(3*x(2)))+0.5;
#endif

#ifdef __G_H5__
	y=Sin(Exp(x(1))*(Log(Sqr(x(1)))+Log(Sqr(x(2)))));
#endif

#ifdef __G_H6__
    y1=(Sqr(Cos(5*x(1))) + Sqr(Cos(5*x(2)))  - Sqr(Sqr(x(1)) + Sqr(x(2)))/10.0);
	if (Inf(y1)>=0.0) return y1;
	y2=(Sqr(x(1))+Sqr(x(2))-4.0);
	if (Sup(y1)<=0.0) return y2;
	if (Inf(y2)>=0.0) return y2;
	return y1;
#endif

#ifdef __G_H7__
	y=Sqr(x(1))+Sqr(x(2))-2.5;
#endif

#ifdef __G_H8__
	y= -Sqr(x(1))-Sqr(x(2))+2.5;
#endif

#ifdef __G_H9__
	//y=2*x(1) + x(2) - 5*Log(Sqr(x(1)))*Sin(x(2));
	y1=(2*x(1) + x(2) - 5*Log(Sqr(x(1)))*Sin(x(2))-1.0);
	if (Inf(y1)>=0.0) return y1;
	y2=(-2*x(1) - x(2) + 5*Log(Sqr(x(1)))*Sin(x(2)));
	if (Sup(y1)<=0.0) return y2;
	if (Inf(y2)>=0.0) return y2;
	return y1;
#endif

#ifdef __G_H10__
	//y=Cos(10*x(1)) + Cos(10*x(2));
	y1=(Cos(10*x(1)) + Cos(10*x(2))-1.0);
	if (Inf(y1)>=0.0) return y1;
	y2=(-Cos(10*x(1)) - Cos(10*x(2))+0.5);
	if (Sup(y1)<=0.0) return y2;
	if (Inf(y2)>=0.0) return y2;
	return y1;
#endif

#ifdef __G_H11__
	y1=(Cos(x(1))*Power(Exp(Cos(x(2))) - Cos(x(1)) - Sin(x(2)),5)-1.0);
	if (Inf(y1)>=0.0) return y1;
	y2=(-Cos(x(1))*Power(Exp(Cos(x(2))) - Cos(x(1)) - Sin(x(2)),5)+0.0);
	if (Sup(y1)<=0.0) return y2;
	if (Inf(y2)>=0.0) return y2;
	return y1;
#endif

#ifdef __G_H12__
	y1=(Cos(x(1))*Power(Exp(Cos(x(2))) - Cos(x(1)) - Sin(x(2)),5)-6.0);
	if (Inf(y1)>=0.0) return y1;
	y2=(-Cos(x(1))*Power(Exp(Cos(x(2))) - Cos(x(1)) - Sin(x(2)),5)+1.0);
	if (Sup(y1)<=0.0) return y2;
	if (Inf(y2)>=0.0) return y2;
	return y1;
#endif

#ifdef __G_H13__
	y1=(Cos(x(1))*Power(Exp(Cos(x(2))) - Cos(x(1)) - Sin(x(2)),5)-10.0);
	if (Inf(y1)>=0.0) return y1;
	y2=(-Cos(x(1))*Power(Exp(Cos(x(2))) - Cos(x(1)) - Sin(x(2)),5)+1.0);
	if (Sup(y1)<=0.0) return y2;
	if (Inf(y2)>=0.0) return y2;
	return y1;
#endif

#ifdef __G_H14__
	y1=(Sqr(x(1))*Sin(20*x(1)) - x(2));
	if (Inf(y1)>=0.0) return y1;
	y2=(Sqr(x(2))*Cos(20*x(2)) - x(1));
	if (Sup(y1)<=0.0) return y2;
	if (Inf(y2)>=0.0) return y2;
	return y1;
#endif


#ifdef __G_H15__
	//INTERVAL h = INTERVAL(15.0);
	//INTERVAL b = INTERVAL(5.0);
	INTERVAL h = INTERVAL(15.0);
	INTERVAL b = INTERVAL(1.7);
	//INTERVAL h = INTERVAL(6.0);
	//INTERVAL b = INTERVAL(1.0);
	//INTERVAL h = INTERVAL(4.0);
	//INTERVAL b = INTERVAL(4.0);
	//INTERVAL h = INTERVAL(25.0);
	//INTERVAL b = INTERVAL(8.668);
	//INTERVAL h = INTERVAL(25.0);
	//INTERVAL b = INTERVAL(0.69);
	INTERVAL M = x(1);
	INTERVAL P = x(2);
	INTERVAL Y = x(3);
	y= -1+4*M/(b*Sqr(h)*Y)+Sqr(P/(b*h*Y));
	//y= -1.0+4.0*x(1)/(5.0*(225)*x(3))+Sqr(x(2)/(5.0*15.0*x(3)));
	//y= -1+4*x(1)/(8.668*(25^2)*x(3))+Sqr(x(2)/(8.668*25*x(3)));
#endif

#ifdef __G_H16__
	INTERVAL h = INTERVAL(25.0);
	INTERVAL b = INTERVAL(8.668);
	//INTERVAL h = INTERVAL(15.0);
	//INTERVAL b = INTERVAL(5.0);
	INTERVAL M = INTERVAL(2000.0)+INTERVAL(400.0)*x(1);
	INTERVAL P = INTERVAL(500.0)+INTERVAL(100.0)*x(2);
	INTERVAL Y = INTERVAL(5.0)+INTERVAL(0.5)*x(3);
	y= -1+4*M/(b*Sqr(h)*Y)+Sqr(P/(b*h*Y));
#endif
	return y;
}




bool reduce(INTERVAL_VECTOR & x)
{
    bool fixed = false;
    INTERVAL x1 = x(1);
    INTERVAL x2 = x(2);
    while (!fixed) {
        x(2) &= Hull(INTERVAL(0.),Sin(x(1)));
        if (x(2).empty()) return false;
        x(2) &= Hull(INTERVAL(0.),Sqrt(4-Sqr(x(1))));
        if (x(2).empty()) return false;
        x(1) &= Hull(INTERVAL(0.),Sqrt(4-Sqr(x(2))));
        if (x(1).empty()) return false;
        if (Sup(x(1))<=INTERVAL::Pi/2.0) x(1) &= Hull(INTERVAL(2.),ArcSin(x(2)));
        if (x(1).empty()) return false;
        if ((x1 == x(1)) && (x2 == x(2))) fixed = true;
        x1 = x(1);
        x2 = x(2);
    }
    return true;
}


INTERVAL proceed(INTERVAL_VECTOR & x, unsigned int & ord)
{
	INTERVAL y;
	//if (constrained) if (!reduce(x)) return 0.;
	if (constrained) y=g(x);
	else y = -1;
	INTERVAL q;
	if(Inf(y)<=0)
	{
        if(Sup(y)>0) {
            INTERVAL fx = d->f(x);
            q=fx*volume(x);
            q=Hull(q,INTERVAL(0.));
            if (Inf(fx)*Sup(fx) < 0.0) {
                double c = min(-Inf(fx),Sup(fx));
                q += INTERVAL(-c,c)*volume(x);
            }
            //cout << "Gray" << endl;
        }
        else {
            q=d->quadratureEval(x,ord);
            q &= (d->f(x))*volume(x);
            //q &= INTERVAL(0.,2.)*volume(x);
        }
        return q;
	}
	//else cout << "Fail" << endl;
	return 0.;
}

void insert(multimap<double,pair<INTERVAL_VECTOR,INTERVAL> > & UB, INTERVAL_VECTOR & x, INTERVAL q, int adapt)
{
    pair<INTERVAL_VECTOR,INTERVAL> p = pair<INTERVAL_VECTOR,INTERVAL>(x,q);
    UB.insert(pair<double,pair<INTERVAL_VECTOR,INTERVAL> >(key(p,adapt),p));
}

int main(int argc, char *argv[])
{
    INTERVAL q, q_old, q_new, q1, q2;
    Menu* m = new Menu();
    d = new Tensor(m->getFNumber(),m->getnVars(),m->getFExp());
    if (d->isCorrupted()) return 0;
    double total_wid_max = m->getPrecision();
    bool _adapt_ = m->getAdaptative();
    constrained = m->getConstrained();
    int nVars = m->getnVars();
    if (d->setOrder(m->getnOrder())) {
        d->showIndexes();
        //d->showDifferentiationVector();
        INTERVAL_VECTOR x_init(nVars);
        for (unsigned int i=0;i<nVars;i++) x_init(i+1) = m->getInterval(i);
        cout << "x_init -> " << x_init << endl;
        /*
    INTERVAL I1(1,2);
    INTERVAL I2(0,1);
    cout << "I1 = " << I1 << endl;
    cout << "I2 = " << I2 << endl;
    cout << "I1/I2 = " << I1/I2 << endl;
*/
        if (m->getFNumber()==8) {
            InverseProblem* ip = new InverseProblem(x_init,d);
            ip->solve();
            //ip->solveMC();
            //ip->solveMC0();
            //system("PAUSE");
            return 0;
        }

        bool filePlot = (divisionPlot || timePlot);
        unsigned int ord=m->getnOrder();
        cout << "TM " << ord << endl;
        stringstream ssi; ssi << ord;
        string filename;
        if (_adapt_) filename = "_timing_" + ssi.str() + "w";
        else filename = "_timing_" + ssi.str() + "f";
        ofstream file (filename.c_str());
        if (filePlot) {
            file << std::setiosflags(ios::scientific) << std::setprecision(16);
            file << "{" << endl;
        }
        chronos chrono1;
        chrono1.start();
        multimap<double,pair<INTERVAL_VECTOR,INTERVAL> > UB;
        multimap<double,pair<INTERVAL_VECTOR,INTERVAL> > PB;
        {
            q=d->quadratureEval(x_init,ord);
//cout << "Sem dividir: " << q << endl;
            q &= (d->f(x_init))*volume(x_init);
//cout << "Sem dividir: " << q << endl;
//system("PAUSE");
            if (constrained) if(Sup(g(x_init))>0) q=Hull(q,INTERVAL(0.));
            insert(UB,x_init,q,_adapt_);
        }
        INTERVAL total_upper=INTERVAL(Sup(q));
        INTERVAL total_lower=INTERVAL(Inf(q));
        INTERVAL upper_lower=total_upper-total_lower;
        double total_wid = Sup(upper_lower);
        unsigned long int nbisection=0;
        double nDiv;
        INTERVAL quad = q;
        INTERVAL best = q;
        double time = chrono1.duration();
        if (filePlot) file << "{" << time << "," << Diam(best) << "}";
        cout << time << "> " << nbisection << " " << UB.size() << ":" << PB.size() << " " << best << " " << Diam(best) << endl;
        bool interruptTime = false;
        bool interruptDivision = false;
        bool stopQuadrature = false;
        if ((!filePlot) && (total_wid<=total_wid_max)) stopQuadrature = true;
        int nTime = 0;
        //cout << time << ">>>>>>> " << stopQuadrature << endl;

        list<INTERVAL_VECTOR> inner,boundary;
        unsigned long int numInner = 0;
        unsigned long int numBoundary = 0;
        INTERVAL y;
        while((!stopQuadrature) && (UB.size()>0))
        {
            nbisection++;
            nDiv = log2(nbisection+1)/nVars;
            if (divisionPlot && (fmod(nDiv,1.0)==0.0)) interruptDivision = true;
            if (timePlot && ((time+chrono1.duration())>=pow(1.0715,nTime))||(time+chrono1.duration()>=maxTime)) interruptTime = true;
            if ((nbisection%step==0)||interruptDivision||interruptTime) {
                time += chrono1.duration();
                multimap<double,pair<INTERVAL_VECTOR,INTERVAL> >::iterator it;
                total_upper = INTERVAL(0.);
                total_lower = INTERVAL(0.);
                quad = INTERVAL(0.);
                numInner = 0;
                numBoundary = 0;
                for (it=UB.begin();it!=UB.end();it++) {
                    pair<double,pair<INTERVAL_VECTOR,INTERVAL> > p = (*it);
                    q = (p.second).second;
                    total_upper += INTERVAL(Sup(q));
                    total_lower += INTERVAL(Inf(q));
                    quad += q;
                    if (nTime>=0) {
                        if (constrained) y=g((p.second).first);
                        else y = -1;
                        if(Inf(y)<=0)
                        {
                            if(Sup(y)>0) {
                                numBoundary++;
                            }
                            else {
                                numInner++;
                            }
                        }
                    }
                }
                for (it=PB.begin();it!=PB.end();it++) {
                    pair<double,pair<INTERVAL_VECTOR,INTERVAL> > p = (*it);
                    q = (p.second).second;
                    total_upper += INTERVAL(Sup(q));
                    total_lower += INTERVAL(Inf(q));
                    quad += q;
                }
                upper_lower = total_upper - total_lower;
                total_wid = Sup(upper_lower);
                Intersection (best, best, quad);
                cout << time << "> " << nbisection << " " << numBoundary << "+" << numInner << "=" << UB.size() << ":" << PB.size() << " " << best << " " << Diam(best) << endl;
                if (interruptDivision) {
                    interruptDivision = false;
                    file << "," << endl;
                    file << "{" << time << "," << Diam(best) << "}";
                }
                if (interruptTime) {
                    interruptTime = false;
                    nTime++;
                    file << "," << endl;
                    file << "{" << time << "," << Diam(best) << "}";
                }
                chrono1.start();
            }
            INTERVAL_VECTOR x = UB.begin()->second.first;
            q = UB.begin()->second.second;
            UB.erase(UB.begin());
            total_upper -= INTERVAL(Sup(q));
            total_lower -= INTERVAL(Inf(q));
            if (Diam(q)<stop) {
                total_upper += INTERVAL(Sup(q));
                total_lower += INTERVAL(Inf(q));
                insert(PB,x,q,_adapt_);
            }
            else {
                INTERVAL_VECTOR xx=BisectIntervalVector(x);
                q1 = proceed(x,ord);
                q2 = proceed(xx,ord);
                if (q1!=0) {
                    insert(UB,x,q1,_adapt_);
                    total_upper += INTERVAL(Sup(q1));
                    total_lower += INTERVAL(Inf(q1));
                }
                if (q2!=0) {
                    insert(UB,xx,q2,_adapt_);
                    total_upper += INTERVAL(Sup(q2));
                    total_lower += INTERVAL(Inf(q2));
                }
            }
            upper_lower = total_upper - total_lower;
            total_wid = Sup(upper_lower);
            if ((!filePlot) && (total_wid<=total_wid_max)) stopQuadrature = true;
            if (divisionPlot && (nDiv>=maxDiv)) stopQuadrature = true;
            if (timePlot && (time>=maxTime)) stopQuadrature = true;
            if ((_adapt_) && (Diam(q)<stop)) stopQuadrature = true;
            //D3
            //if ((Inf(total_lower) >= 15.994402890) && (Sup(total_upper) <= 15.994402893)) {
            //if ((Inf(total_lower) >= 15.9943) && (Sup(total_upper) <= 15.9945)) {
            //if ((Inf(total_lower) >= 15.9942) && (Sup(total_upper) <= 15.9946)) {
            //D2
            //if ((Inf(total_lower) >= 7.99626468194) && (Sup(total_upper) <= 7.99626468196)) {
            //if ((Inf(total_lower) >= 7.996264681) && (Sup(total_upper) <= 7.996264683)) {
            //I2
            //if ((Inf(total_lower) >= -1.5242783655029) && (Sup(total_upper) <= -1.5242783655024)) {
            //if ((Inf(total_lower) >= -1.524278365504) && (Sup(total_upper) <= -1.524278365501)) {
            //if ((Inf(total_lower) >= -1.5242783656) && (Sup(total_upper) <= -1.5242783654)) {
            //I1
            //if ((Inf(total_lower) >= 0.74997436851) && (Sup(total_upper) <= 0.74997436854)) {
            //if ((Inf(total_lower) >= 0.74997436851) && (Sup(total_upper) <= 0.74997436855)) {
            //if ((Inf(total_lower) >= 0.7499743671) && (Sup(total_upper) <= 0.7499743699)) {
            //if ((Inf(total_lower) >= 0.74997436) && (Sup(total_upper) <= 0.74997438)) {
            //C2
            //if ((Inf(total_lower) >= 6.30011) && (Sup(total_upper) <= 6.30013)) {
            //if ((Inf(total_lower) >= 6.296) && (Sup(total_upper) <= 6.304)) {
            //C3
            //if ((Inf(total_lower) >= 8.34) && (Sup(total_upper) <= 8.41)) {
            //if ((Inf(total_lower) >= 8.25) && (Sup(total_upper) <= 8.54)) {
            //if ((Inf(total_lower) >= 1.255) && (Sup(total_upper) <= 1.257)) {
            //if ((Inf(total_lower) >= 1.473) && (Sup(total_upper) <= 1.476)) {
            //if ((Inf(total_lower) >= 1.474821) && (Sup(total_upper) <= 1.474825)) {
            /*if ((Inf(total_lower) >= 1.791) && (Sup(total_upper) <= 1.799)) {
                stopQuadrature = true;
                time += chrono1.duration();
                cout << "time : " << time << " --> " <<INTERVAL(Inf(total_lower),Sup(total_upper)) << endl;
            }*/

            //if (nbisection==3443) stopQuadrature = true;
            //if (nbisection==4300) stopQuadrature = true;
            if (stopQuadrature) {
                time += chrono1.duration();
                cout << "Final time : " << time << " total_wid = " << total_wid << endl;

                //cout << time << "> " << nbisection << " " << numBoundary << "+" << numInner << "=" << UB.size() << ":" << PB.size() << " " << best << " " << Diam(best) << endl;
            }
        }
        cout << "time : " << time << endl;

        //******************************************************************

        q=INTERVAL(0.);
        numInner = 0;
        numBoundary = 0;
        while(!UB.empty()) {
            q += UB.begin()->second.second;
            INTERVAL_VECTOR x=UB.begin()->second.first;
            if (constrained) {
                INTERVAL y=g(x);
                if(Sup(y)<=0) {
                    numInner++;
                    if (saveBoxes) inner.push_back(x);
                }
                else {
                    numBoundary++;
                    if (saveBoxes) boundary.push_back(x);
                }
            }
            else {
                numInner++;
                if (saveBoxes) inner.push_back(x);
            }
            UB.erase(UB.begin());
        }
        while(!PB.empty())
        {
            q += PB.begin()->second.second;
            INTERVAL_VECTOR x=PB.begin()->second.first;
            if (constrained) {
                INTERVAL y=g(x);
                if(Sup(y)<=0) {
                    numInner++;
                    if (saveBoxes) inner.push_back(x);
                }
                else {
                    numBoundary++;
                    if (saveBoxes) boundary.push_back(x);
                }
            }
            else {
                numInner++;
                if (saveBoxes) inner.push_back(x);
            }
            PB.erase(PB.begin());
        }

        Intersection (q, best, q);
        cout << "quadrature : " << q << " " << Diam(q) << endl;
        //cout << "out : " << INTERVAL(1.0)-q << " " << Diam(1.0-Sqr(q)) << endl;
        INTERVAL result = INTERVAL(BiasNegInf,BiasPosInf);
        switch ( m->getFNumber() ) {
            case 0: result = INTERVAL(0.682689492136,0.682689492138); break;
            case 1: result = INTERVAL::Pi; break;
            case 2: break;
            case 3: break;
            case 4: result = INTERVAL::Pi / (2.*Sqrt(1-(INTERVAL(INTERVAL(1.0)/INTERVAL(10.0))))); break;
            case 5: result = Power(INTERVAL::Pi,3) / (4.*Sqrt(1-(INTERVAL(INTERVAL(1.0)/INTERVAL(10.0))))); break;
            case 6: result = 3.0*Power(INTERVAL::Pi,5) / (32.*Sqrt(1-(INTERVAL(INTERVAL(1.0)/INTERVAL(10.0))))); break;
            case 7: result = Power(INTERVAL::Pi,7) / (32.*Sqrt(1-(INTERVAL(INTERVAL(1.0)/INTERVAL(10.0))))); break;
        }
        if (!result.unbounded()) cout << "result     : " << result << endl;
        cout << "inner boxes: " << numInner << endl;
        if (constrained) cout << "boundary boxes: " << numBoundary << endl;
        if (saveBoxes) {
            write_LIV_2D(inner,"_inner");
            write_LIV_2D(boundary,"_boundary");
            cout << endl;
        }
        if (filePlot) {
            file << endl;
            file << "}";
            file.close();
        }

    };
    return 0;
}

