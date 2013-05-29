#ifndef INVERSE_H
#define INVERSE_H

#include <map>

#include <Interval.h>
#include <IntervalVector.h>
#include "Tensor.h"

#include <vector>

using std::vector;

typedef struct _Boxes{
    INTERVAL_VECTOR x;
    INTERVAL q;
    int * vars;
    //vector<int> vars;
} Boxes;

class InverseProblem
{
    public:
        InverseProblem(INTERVAL_VECTOR & x, Tensor* dd);
        virtual ~InverseProblem();
        bool prune(INTERVAL_VECTOR & x, int & n);
        bool pruneMid(INTERVAL_VECTOR & x);
        void searchBlack(int v);
        void splitUB(int v);
        void leftProcess(int v);
        void rightProcess(int v);
        void stepProcess(int v);
        //void splitStep(int v);
        //double improveGraphic(int v, int iter, ofstream & file);
        INTERVAL_VECTOR randomSample(const INTERVAL_VECTOR & x);
        INTERVAL_VECTOR monteCarlo(const INTERVAL_VECTOR & x, int iter);
        INTERVAL_VECTOR monteCarloHull(const INTERVAL_VECTOR & x, int iter);
        double monteCarloSTD(const INTERVAL_VECTOR & x, int N, const INTERVAL_VECTOR & q);
        INTERVAL monteCarloMean(const INTERVAL_VECTOR & x, int N, const INTERVAL_VECTOR & q);
        INTERVAL_VECTOR monteCarloIntegrals(const INTERVAL_VECTOR & x, int N, const INTERVAL_VECTOR & q);
        void improveGraphic(int nf, int iter);
        void improveGraphicMC(int nf, int iter);
        void improveGraphicMax(int nf, int iter);
        unsigned long int getIndex(INTERVAL_VECTOR & x);
        void getCoordinates(unsigned long int index, int t[]);
        //void solve(int v);
        void solve();
        void solveMC();
        void solveMC0();
        //bool prune(INTERVAL_VECTOR & x);
        INTERVAL reduceX(INTERVAL_VECTOR & x, bool & canonical);
        INTERVAL reduceGray(INTERVAL_VECTOR & x);
        //bool split(INTERVAL_VECTOR x, int lb[], int w[],int & v, double & a, double & b, double & c, int & wl, int & wr);
        //void search(INTERVAL_VECTOR & x, int lb[], int w[], double total_wid_max, ofstream & file);
        INTERVAL_VECTOR BisectIntervalVector(INTERVAL_VECTOR & X1, int & kmax, INTERVAL & Xkmax);
        INTERVAL_VECTOR BisectIntervalVector(INTERVAL_VECTOR & X1, bool & canonical);
        void insert(multimap<double,pair<INTERVAL_VECTOR,INTERVAL> > & UB, INTERVAL_VECTOR & x, INTERVAL q);
        //INTERVAL integrate(INTERVAL_VECTOR x_init, double total_wid_max);
        void createOutput(int v1, int v2);
        void createOutput(int v1);
        void createOutputN(int v1, int v2, int n);
        void createOutputN(int v1, int n);
        //void solve(double total_wid_max);
        void graphicsOutput();
        void graphicsOutputN(int n);
        double volume(const INTERVAL_VECTOR & x);
    protected:
        INTERVAL_VECTOR x_init;
        INTERVAL_VECTOR black_hull;
        INTERVAL_VECTOR global_hull;
        INTERVAL_VECTOR globalMC;
        int nglobalMC, solutions;
        double leftGlobal, rightGlobal;
        multimap<double,INTERVAL_VECTOR> UB;
        multimap<double,INTERVAL_VECTOR> UB_left;
        multimap<double,INTERVAL_VECTOR> UB_right;
        multimap<double,INTERVAL_VECTOR> UB_inside;
        multimap<double,INTERVAL_VECTOR> UB_black;
        map<unsigned long int,INTERVAL> joint;
        INTERVAL qtotal;
        //INTERVAL_VECTOR qtotalMC;
        double maxqwidth;
        //multimap<double,Boxes> grayBoxes;
        //multimap<double,Boxes> blackBoxes;
        multimap<double,pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> > > grayCanonical;
        multimap<double,pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> > > grayBoxes;
        multimap<double,pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> > > blackBoxes;
        multimap<double,pair<INTERVAL_VECTOR,pair<INTERVAL_VECTOR,unsigned long int> > > grayBoxesMC;
        multimap<double,pair<INTERVAL_VECTOR,pair<INTERVAL_VECTOR,unsigned long int> > > blackBoxesMC;
        INTERVAL *h;
        INTERVAL *minh;
        INTERVAL *maxh;
        INTERVAL *ch;
        int *gh;
        double *ge;
        double *maxe;
        INTERVAL H;
        Tensor* d;
        double *lbox;
        double *eps;
        INTERVAL *obs;
        INTERVAL *fwr;
        double total_wid_max;
        int *obsIndex, nObs, maxObs, nVars, nDiv;
        int MH;
        int nMC;
        bool firstBlack;
    private:
};

#endif // INVERSE_H
