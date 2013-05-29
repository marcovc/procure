#include "Inverse.h"
#include "chronos.h"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <ctime>    // For time()

InverseProblem::InverseProblem(INTERVAL_VECTOR & x, Tensor* dd)
{
    srand((unsigned)time(0));
    nDiv = 100;
    //nMC = 100000000;
    //nMC = 10000000;
    //nMC = 1000000;
    //nMC = 100000;
    //nMC = 10000;
    //nMC = 1000;
    //nMC = 100;
    //nMC = 50;
    //nMC = 20;
    //nMC = 10;
    nMC = 5;
    MH = 1000;
    //total_wid_max = 0.00001;
    total_wid_max = 0.01;
    d = dd;
    nVars = Dimension(x);
    x_init = x;
    firstBlack = true;
    lbox = new double[nVars];
    eps = new double[nVars];
    h = new INTERVAL[nDiv];
    minh = new INTERVAL[nDiv];
    maxh = new INTERVAL[nDiv];
    ch = new INTERVAL[nDiv];
    gh = new int[nDiv];
    ge = new double[nDiv];
    maxe = new double[nDiv];
    for (unsigned int i=0;i<nVars;i++) {
        lbox[i] = Inf(x_init(i+1));
        eps[i] = 0.01;
    }
    /*
lbox[0]=0.01;
lbox[1]=0.0;
lbox[2]=0.00305176;
eps[0]=0.0324642;
eps[1]=0.00876617;
eps[2]=0.000244141;
*/
    nObs = 6;
    fwr = new INTERVAL[nObs];
    obs = new INTERVAL[nObs];
    double *mean;
    mean = new double[nObs];
    /* case 1
    mean[0] = 0.005638;
    mean[1] = 0.005588;
    mean[2] = 0.006337;
    mean[3] = 0.005571;
    mean[4] = 0.004364;
    mean[5] = 0.000733;
    */
    ///* case 2
    mean[0] = 0.005476;
    mean[1] = 0.005836;
    mean[2] = 0.007615;
    mean[3] = 0.007749;
    mean[4] = 0.007777;
    mean[5] = 0.001811;
    //*/
    /*
    t[6] = 5476;
    t[7] = 5836;
    t[8] = 7615;
    t[9] = 7749;
    t[10] = 7777;
    t[11] = 1811;
    */
    /* case 3
    mean[0] = 0.005361;
    mean[1] = 0.005857;
    mean[2] = 0.007925;
    mean[3] = 0.008250;
    mean[4] = 0.008777;
    mean[5] = 0.002502;
    */
    /* case 4
    mean[0] = 0.006181;
    mean[1] = 0.006927;
    mean[2] = 0.006421;
    mean[3] = 0.004184;
    mean[4] = 0.002628;
    mean[5] = 0.000426;
    */
    /* case 5
    mean[0] = 0.006598;
    mean[1] = 0.007962;
    mean[2] = 0.008760;
    mean[3] = 0.006626;
    mean[4] = 0.004675;
    mean[5] = 0.000840;
    */
    /* case 6
    mean[0] = 0.006981;
    mean[1] = 0.009057;
    mean[2] = 0.012403;
    mean[3] = 0.012501;
    mean[4] = 0.012504;
    mean[5] = 0.003823;
    */
    /* case 7
    mean[0] = 0.001210;
    mean[1] = 0.001809;
    mean[2] = 0.002860;
    mean[3] = 0.002631;
    mean[4] = 0.002227;
    mean[5] = 0.000424;
    */
    /* case 7  Chla = 0.02
    mean[0] = 0.001335;
    mean[1] = 0.001960;
    mean[2] = 0.003060;
    mean[3] = 0.002859;
    mean[4] = 0.002438;
    mean[5] = 0.000462;
    */
    /* case 8
    mean[0] = 0.000287;
    mean[1] = 0.000457;
    mean[2] = 0.000888;
    mean[3] = 0.001059;
    mean[4] = 0.001382;
    mean[5] = 0.000416;
    */
    /* case 9
    mean[0] = 0.000074;
    mean[1] = 0.000120;
    mean[2] = 0.000248;
    mean[3] = 0.000327;
    mean[4] = 0.000571;
    mean[5] = 0.000390;
    */
    /* case 10
    mean[0] = 0.000156;
    mean[1] = 0.000242;
    mean[2] = 0.000455;
    mean[3] = 0.000540;
    mean[4] = 0.000693;
    mean[5] = 0.000190;
    */
    /* case 11
    mean[0] = 0.000069;
    mean[1] = 0.000110;
    mean[2] = 0.000223;
    mean[3] = 0.000298;
    mean[4] = 0.000558;
    mean[5] = 0.000685;
    */
    /* case 12
    mean[0] = 0.002015;
    mean[1] = 0.002753;
    mean[2] = 0.004564;
    mean[3] = 0.005297;
    mean[4] = 0.006842;
    mean[5] = 0.002554;
    */


    double delta;
    double open = 3.0;
    for (unsigned int i=0;i<nObs;i++) {
        if (i==5) delta = 0.06;
        else delta = 0.05;
        //if (i==5) delta = 0.03;
        //else delta = 0.025;
        //if (i==5) delta = 0.005;
        //else delta = 0.006;
        //if (i==5) delta = 0.003;
        //else delta = 0.0025;
        //if (i==5) delta = 0.0005;
        //else delta = 0.0006;
        obs[i] = INTERVAL(mean[i]-open*delta*mean[i],mean[i]+open*delta*mean[i]);
    }
    obsIndex = new int[nObs];
    obsIndex[0] = 315;
    obsIndex[1] = 316;
    obsIndex[2] = 317;
    obsIndex[3] = 318;
    obsIndex[4] = 319;
    obsIndex[5] = 320;
    maxObs = 347;

    INTERVAL_VECTOR tst(nVars);
    /* Case 1
    tst(1) = INTERVAL(1.0);
    tst(2) = INTERVAL(0.373971);
    tst(3) = INTERVAL(0.011946);
    */
    /* Case 2
    tst(1) = INTERVAL(5.0);
    tst(2) = INTERVAL(1.140851);
    tst(3) = INTERVAL(0.034792);
    */
    /* Case 3
    tst(1) = INTERVAL(10.0);
    tst(2) = INTERVAL(1.843495);
    tst(3) = INTERVAL(0.055134);
    */
    /* Case 4
    tst(1) = INTERVAL(0.01);
    tst(2) = INTERVAL(0.5);
    tst(3) = INTERVAL(0.0);
    */
    /* Case 5
    tst(1) = INTERVAL(0.01);
    tst(2) = INTERVAL(1.0);
    tst(3) = INTERVAL(0.0);
    */
    /* Case 6
    tst(1) = INTERVAL(0.01);
    tst(2) = INTERVAL(5.0);
    tst(3) = INTERVAL(0.0);
    */
    /* Case 7
    tst(1) = INTERVAL(0.01);
    tst(2) = INTERVAL(0.5);
    tst(3) = INTERVAL(0.1);
    */
    /* Case 8
    tst(1) = INTERVAL(0.01);
    tst(2) = INTERVAL(0.5);
    tst(3) = INTERVAL(0.5);
    */
    /* Case 9
    tst(1) = INTERVAL(0.01);
    tst(2) = INTERVAL(0.5);
    tst(3) = INTERVAL(2.0);
    */
    /* Case 10
    tst(1) = INTERVAL(0.1);
    tst(2) = INTERVAL(0.1);
    tst(3) = INTERVAL(0.5);
    */
    /* Case 11
    tst(1) = INTERVAL(1.0);
    tst(2) = INTERVAL(0.5);
    tst(3) = INTERVAL(5.0);
    */
    ///* Case 12
    tst(1) = INTERVAL(10.0);
    tst(2) = INTERVAL(2.0);
    tst(3) = INTERVAL(0.5);
    //*/
    cout << "tst(1) = " << tst(1) << endl;
    cout << "tst(2) = " << tst(2) << endl;
    cout << "tst(3) = " << tst(3) << endl;
    d->getT(tst,nObs,maxObs,obsIndex,fwr);
    for (int i=0;i<nObs;i++) cout << "R" << i+1 << " = " << fwr[i] << endl;
    cout << "f(x) = " << d->f(tst) << endl;
    system("PAUSE");
}

InverseProblem::~InverseProblem()
{
    //dtor
}

bool InverseProblem::pruneMid(INTERVAL_VECTOR & x)
{
    INTERVAL_VECTOR mid=Hull(Mid(x));
    d->getT(mid,nObs,maxObs,obsIndex,fwr);
    //cout << "x = " << x << endl;
    for (int i=0;i<nObs;i++) {
        /*
        cout << "obs[" << i << "] = " << obs[i] << endl;
        cout << "fwr[" << i << "] = " << fwr[i] << endl;
        cout << "Inf(obs[i]) = " << Inf(obs[i]) << endl;
        cout << "Sup(fwr[i]) = " << Sup(fwr[i]) << endl;
        cout << "(Inf(obs[i])>Sup(fwr[i])) = " << (Inf(obs[i])>Sup(fwr[i])) << endl;
        cout << "Sup(obs[i]) = " << Sup(obs[i]) << endl;
        cout << "Inf(fwr[i]) = " << Inf(fwr[i]) << endl;
        cout << "(Sup(obs[i])<Inf(fwr[i])) = " << (Sup(obs[i])<Inf(fwr[i])) << endl;
        */
        if ((Inf(obs[i])>Sup(fwr[i])) || (Sup(obs[i])<Inf(fwr[i]))) return false;
    }
    return true;
}

bool InverseProblem::prune(INTERVAL_VECTOR & x, int & n)
{
    n = nObs;
    d->getT(x,nObs,maxObs,obsIndex,fwr);
    //cout << "x = " << x << endl;
    for (int i=0;i<nObs;i++) {
        /*
        cout << "obs[" << i << "] = " << obs[i] << endl;
        cout << "fwr[" << i << "] = " << fwr[i] << endl;
        cout << "Inf(obs[i]) = " << Inf(obs[i]) << endl;
        cout << "Sup(fwr[i]) = " << Sup(fwr[i]) << endl;
        cout << "(Inf(obs[i])>Sup(fwr[i])) = " << (Inf(obs[i])>Sup(fwr[i])) << endl;
        cout << "Sup(obs[i]) = " << Sup(obs[i]) << endl;
        cout << "Inf(fwr[i]) = " << Inf(fwr[i]) << endl;
        cout << "(Sup(obs[i])<Inf(fwr[i])) = " << (Sup(obs[i])<Inf(fwr[i])) << endl;
        */
        if ((Inf(obs[i])>Sup(fwr[i])) || (Sup(obs[i])<Inf(fwr[i]))) return false;
        if ((Inf(obs[i])<=Inf(fwr[i])) && (Sup(obs[i])>=Sup(fwr[i]))) n--;
    }
    return true;
}

void InverseProblem::searchBlack(int v)
{
    //multimap<double,INTERVAL_VECTOR> UB;
    UB.insert(pair<double,INTERVAL_VECTOR>(-Max(Diam(x_init)),x_init));
    bool black = false;
    int n, nbisection = 0;
    INTERVAL x_old;
    int xk;
    while ((!black) && (UB.size()>0)) {
        INTERVAL_VECTOR x = UB.begin()->second;
        UB.erase(UB.begin());
        if (prune(x,n)) {
            if (n==0) {
                black = true;
                black_hull = x;
            }
            else {
                nbisection++;
                //cout << nbisection << ":" << UB.size() << ":" << n << "  " << Max(Diam(x)) << endl;
                INTERVAL_VECTOR xx=BisectIntervalVector(x,xk,x_old);
                UB.insert(pair<double,INTERVAL_VECTOR>(-Max(Diam(x)),x));
                UB.insert(pair<double,INTERVAL_VECTOR>(-Max(Diam(xx)),xx));
            }
        }
    }
    cout << "Final " << nbisection << ":" << UB.size() << endl;
    if (black) {
        cout << "black_hull = " << black_hull << endl;
        //UB_black.insert(pair<double,INTERVAL_VECTOR>(-Max(Diam(black_hull)),black_hull));
        UB_black.insert(pair<double,INTERVAL_VECTOR>(Inf(black_hull(v)),black_hull));
    }
    else cout << "There are no solutions!" << endl;
}


void InverseProblem::splitUB(int v)
{
    INTERVAL_VECTOR xx;
    while (UB_left.size()>0) {
        xx = UB_left.begin()->second;
        UB_left.erase(UB_left.begin());
        UB.insert(pair<double,INTERVAL_VECTOR>(-Max(Diam(xx)),xx));
    }
    while (UB_right.size()>0) {
        xx = UB_right.begin()->second;
        UB_right.erase(UB_right.begin());
        UB.insert(pair<double,INTERVAL_VECTOR>(-Max(Diam(xx)),xx));
    }
    while (UB_inside.size()>0) {
        xx = UB_inside.begin()->second;
        UB_inside.erase(UB_inside.begin());
        UB.insert(pair<double,INTERVAL_VECTOR>(-Max(Diam(xx)),xx));
    }
    double leftValue = Inf(black_hull(v));
    double rightValue = Sup(black_hull(v));
    global_hull = black_hull;
    while (UB.size()>0) {
        INTERVAL_VECTOR x = UB.begin()->second;
        UB.erase(UB.begin());
        global_hull = Hull(global_hull,x);
        if ((Inf(x(v))>=leftValue) && (Sup(x(v))<=rightValue)) UB_inside.insert(pair<double,INTERVAL_VECTOR>(-Max(Diam(x)),x));
        else if (Sup(x(v))<=rightValue) UB_left.insert(pair<double,INTERVAL_VECTOR>(-Max(Diam(x)),x));
        else if (Inf(x(v))>=leftValue) UB_right.insert(pair<double,INTERVAL_VECTOR>(-Max(Diam(x)),x));
        else {
            INTERVAL_VECTOR xx=x;
            double a = Inf(x(v));
            double b = Sup(x(v));
            double c = Mid(black_hull(v));
            x(v)=INTERVAL(a,c);
            xx(v)=INTERVAL(c,b);
            UB_left.insert(pair<double,INTERVAL_VECTOR>(-Max(Diam(x)),x));
            UB_right.insert(pair<double,INTERVAL_VECTOR>(-Max(Diam(xx)),xx));
        }
    }
    cout << "UB_inside -> " << UB_inside.size() << endl;
    cout << "UB_black -> " << UB_black.size() << endl;
    cout << "UB_left -> " << UB_left.size() << endl;
    cout << "UB_right -> " << UB_right.size() << endl;
    cout << "black_hull = " << black_hull << endl;
    cout << "global_hull = " << global_hull << endl;
    leftGlobal = Inf(global_hull(v));
    rightGlobal = Sup(global_hull(v));
}

void InverseProblem::leftProcess(int v)
{
    //cout << "<";
    INTERVAL x_old;
    int xk;
    int n;
    double oldLeftGlobal = leftGlobal;
    double leftBlack = Inf(black_hull(v));
    bool recompute = false;
    bool possible = false;
    INTERVAL_VECTOR x = UB_left.begin()->second;
    UB_left.erase(UB_left.begin());
    //cout << "x = " << x << endl;
    if (Inf(x(v))>=leftBlack) {
        UB_inside.insert(pair<double,INTERVAL_VECTOR>(-Max(Diam(x)),x));
        //cout << "inside" << endl;
        //cin.get();
    }
    else {
        if (Inf(x(v))==leftGlobal) possible = true;
        if (prune(x,n)) {
            if (n==0) {
                black_hull = Hull(black_hull,x);
                //cout << "black" << endl;
                cout << "left -> [ "<< leftGlobal << " <-> " << Inf(black_hull(v)) << " ]" << endl;
                //cout << "(leftGlobal+(Sup(black_hull(v))-leftGlobal)/nDiv) = " << (leftGlobal+(Sup(black_hull(v))-leftGlobal)/nDiv) << endl;
                //cout << "Inf(black_hull(v))" << Inf(black_hull(v)) << endl;
        //cin.get();
                //UB_black.insert(pair<double,INTERVAL_VECTOR>(-Max(Diam(x)),x));
                UB_black.insert(pair<double,INTERVAL_VECTOR>(Inf(x(v)),x));
            }
            else {
                //if (pruneMid(x)) cout << "******** " << Hull(Mid(x(v))) << endl;
                if (pruneMid(x)) {
                    black_hull = Hull(black_hull,Hull(Mid(x)));
                    leftBlack = Inf(black_hull(v));
                }
                if ((possible)&&(Inf(x(v))>leftGlobal)) recompute = true;
                //nbisection++;
                //cout << nbisection << ":" << UB.size() << ":" << n << "  " << Max(Diam(x)) << endl;
                INTERVAL_VECTOR xx=BisectIntervalVector(x,xk,x_old);
                //cout << "bisect" << endl;
                if (Inf(x(v))<leftBlack) {
                    //cout << x << " -> left" << endl;
                    UB_left.insert(pair<double,INTERVAL_VECTOR>(-Max(Diam(x)),x));
                }
                else {
                    //cout << x << " -> inside" << endl;
        //cin.get();
                    UB_inside.insert(pair<double,INTERVAL_VECTOR>(-Max(Diam(x)),x));
                }
                if (Inf(xx(v))<leftBlack)  {
                    //cout << xx << " -> left" << endl;
                    UB_left.insert(pair<double,INTERVAL_VECTOR>(-Max(Diam(xx)),xx));
                }
                else {
                    //cout << xx << " -> inside" << endl;
        //cin.get();
                    UB_inside.insert(pair<double,INTERVAL_VECTOR>(-Max(Diam(xx)),xx));
                }
            }
        }
        else if (possible) recompute = true;
    }
    if (recompute) {
        //cout << "recompute" << endl;
        //cout << "UB_left.size() = " << UB_left.size() << endl;
        multimap<double,INTERVAL_VECTOR>::iterator it;
        leftGlobal = leftBlack;
        for (it=UB_left.begin();it!=UB_left.end();it++) {
            x = it->second;
            if (Inf(x(v))<leftGlobal) leftGlobal = Inf(x(v));
        }
        if (oldLeftGlobal<leftGlobal) {
            cout << "left -> [ " << leftGlobal << " <-> " << Inf(black_hull(v)) << " ]" << endl;
            //cout << "(leftGlobal+(Sup(black_hull(v))-leftGlobal)/nDiv) = " << (leftGlobal+(Sup(black_hull(v))-leftGlobal)/nDiv) << endl;
            //cout << "Inf(black_hull(v))" << Inf(black_hull(v)) << endl;
        }
        //cout << "leftGlobal = " << leftGlobal << endl;
        //cin.get();
    }
}

void InverseProblem::rightProcess(int v)
{
    //cout << ">";
    //cout << "UB_right.size() = " << UB_left.size() << endl;
    //cout << "black_hull(v) = " << black_hull(v) << endl;
    INTERVAL x_old;
    int xk;
    int n;
    double rightBlack = Sup(black_hull(v));
    bool recompute = false;
    bool possible = false;
    double oldRightGlobal=rightGlobal;
    //cout << "!";
    INTERVAL_VECTOR x = UB_right.begin()->second;
    //cout << "$";
    //cout << "x = " << x << endl;
    //cout << "A";
    //delete UB_right.begin()->second;
    UB_right.erase(UB_right.begin());
    //cout << "%";
    if (Sup(x(v))<=rightBlack) {
        UB_inside.insert(pair<double,INTERVAL_VECTOR>(-Max(Diam(x)),x));
        //cout << "inside" << endl;
        //cin.get();
    }
    else {
        if (Sup(x(v))==rightGlobal) possible = true;
        if (prune(x,n)) {
            if (n==0) {
                black_hull = Hull(black_hull,x);
                //cout << "black" << endl;
                cout << "right -> [ "<< Sup(black_hull(v)) << " <-> " << rightGlobal << " ]" << endl;
                //cout << "(rightGlobal-(rightGlobal-Inf(black_hull(v)))/nDiv) = " << (rightGlobal-(rightGlobal-Inf(black_hull(v)))/nDiv) << endl;
                //cout << "Sup(black_hull(v))" << Sup(black_hull(v)) << endl;
        //cin.get();
                //UB_black.insert(pair<double,INTERVAL_VECTOR>(-Max(Diam(x)),x));
                UB_black.insert(pair<double,INTERVAL_VECTOR>(Inf(x(v)),x));
            }
            else {
                //if (pruneMid(x)) cout << "******** " << Hull(Mid(x(v))) << endl;
                if (pruneMid(x)) {
                    black_hull = Hull(black_hull,Hull(Mid(x)));
                    rightBlack = Sup(black_hull(v));
                }
                if ((possible)&&(Sup(x(v))<rightGlobal)) recompute = true;
                //nbisection++;
                //cout << nbisection << ":" << UB.size() << ":" << n << "  " << Max(Diam(x)) << endl;
                INTERVAL_VECTOR xx=BisectIntervalVector(x,xk,x_old);
                if (Sup(x(v))>rightBlack) UB_right.insert(pair<double,INTERVAL_VECTOR>(-Max(Diam(x)),x));
                else UB_inside.insert(pair<double,INTERVAL_VECTOR>(-Max(Diam(x)),x));
                if (Sup(xx(v))>rightBlack) UB_right.insert(pair<double,INTERVAL_VECTOR>(-Max(Diam(xx)),xx));
                else UB_inside.insert(pair<double,INTERVAL_VECTOR>(-Max(Diam(xx)),xx));
            }
        }
        else if (possible) recompute = true;
    }
    if (recompute) {
        //cout << "recompute" << endl;
        //cout << "UB_right.size() = " << UB_right.size() << endl;
        multimap<double,INTERVAL_VECTOR>::iterator it;
        rightGlobal = rightBlack;
        for (it=UB_right.begin();it!=UB_right.end();it++) {
            x = it->second;
            if (Sup(x(v))>rightGlobal) rightGlobal = Sup(x(v));
        }

        if (oldRightGlobal>rightGlobal) {
            cout << "right -> [ "<< Sup(black_hull(v)) << " <-> " << rightGlobal << " ]" << endl;
            //cout << "(rightGlobal-(rightGlobal-Inf(black_hull(v)))/nDiv) = " << (rightGlobal-(rightGlobal-Inf(black_hull(v)))/nDiv) << endl;
            //cout << "Sup(black_hull(v))" << Sup(black_hull(v)) << endl;
        }
    }
}

void InverseProblem::stepProcess(int v)
{
    bool recompute = true;
    while (recompute) {
        recompute = false;
        //if ((leftGlobal+5*(Sup(black_hull(v))-leftGlobal)/nDiv)<=Inf(black_hull(v))) {
        if ((leftGlobal+5*(rightGlobal-leftGlobal)/nDiv)<=Inf(black_hull(v))) {
            leftProcess(v);
            recompute = true;
        }
        //if ((rightGlobal-5*(rightGlobal-Inf(black_hull(v)))/nDiv)>=Sup(black_hull(v))) {
        if ((rightGlobal-5*(rightGlobal-leftGlobal)/nDiv)>=Sup(black_hull(v))) {
            rightProcess(v);
            recompute = true;
        }
        for (int i=0;i<v;i++) if (((rightGlobal-leftGlobal)/nDiv)<=(eps[i-1])/100.0) recompute = false;
        //cout << "left -> [ "<< leftGlobal << " <-> " << Inf(black_hull(v)) << " ]" << endl;
        //cout << "right -> [ "<< Sup(black_hull(v)) << " <-> " << rightGlobal << " ]" << endl;
        //cin.get();
        //cout << ".";
    }
    eps[v-1] = (rightGlobal-leftGlobal)/nDiv;
    lbox[v-1] = leftGlobal;
    cout << "UB_inside -> " << UB_inside.size() << endl;
    cout << "UB_black -> " << UB_black.size() << endl;
    cout << "UB_left -> " << UB_left.size() << endl;
    cout << "UB_right -> " << UB_right.size() << endl;
    cout << "Total Boxes = " << UB_inside.size()+UB_black.size()+UB_left.size()+UB_right.size() << endl;
    cout << "black_hull = " << black_hull << endl;
    cout << "global_hull = " << global_hull << endl;
    cout << "leftGlobal = " << leftGlobal << endl;
    cout << "rightGlobal = " << rightGlobal << endl;
    cout << "eps[" << v-1 << "] = " << eps[v-1] << endl;
    cout << "lbox[" << v-1 << "] = " << lbox[v-1] << endl;
}

/*
void InverseProblem::splitStep(int v)
{
    int n;
    int numDeleted = 0;
    INTERVAL_VECTOR x;
    while (UB_right.size()>0) {
        x = UB_right.begin()->second;
        UB_right.erase(UB_right.begin());
        if (prune(x,n)) {
            if (n==0) UB_black.insert(pair<double,INTERVAL_VECTOR>(Inf(x(v)),x));
            else UB.insert(pair<double,INTERVAL_VECTOR>(Inf(x(v)),x));
        }
        else numDeleted++;
    }
    while (UB_inside.size()>0) {
        x = UB_inside.begin()->second;
        UB_inside.erase(UB_inside.begin());
        if (prune(x,n)) {
            if (n==0) UB_black.insert(pair<double,INTERVAL_VECTOR>(Inf(x(v)),x));
            else UB.insert(pair<double,INTERVAL_VECTOR>(Inf(x(v)),x));
        }
        else numDeleted++;
    }
    while (UB_left.size()>0) {
        x = UB_left.begin()->second;
        UB_left.erase(UB_left.begin());
        if (prune(x,n)) {
            if (n==0) UB_black.insert(pair<double,INTERVAL_VECTOR>(Inf(x(v)),x));
            else UB.insert(pair<double,INTERVAL_VECTOR>(Inf(x(v)),x));
        }
        else numDeleted++;
    }
    cout << "UB -> " << UB.size() << endl;
    cout << "UB_black -> " << UB_black.size() << endl;
    cout << "Deleted -> " << numDeleted << endl;
    //cin.get();cin.get();
    INTERVAL xv,q;
    double leftValue = lbox[v-1];
    bool look;
    int numBlack, numGray;
    global_hull = black_hull;
    for (int i=0;i<nDiv;i++) {
        INTERVAL_VECTOR x_hull;
        int numBoxes = 0;
        //h[i] = INTERVAL(0.0);
        xv = INTERVAL(leftValue+i*eps[v-1],leftValue+(i+1)*eps[v-1]);
        //cout << "x_" << i << " = " << xv << " wid = " << Diam(xv) << endl;
        look = true;
        numBlack = 0;
        numGray = 0;
        //cout << "Gray boxes" << endl;
        while ((UB.size()>0) && look) {
            //if (UB.size()>0) cout <<  "UB.begin()->first = " << UB.begin()->first << endl;
            if (UB.begin()->first < Sup(xv)) {
                x = UB.begin()->second;
                //cout << "x = " << x << endl;
                UB.erase(UB.begin());
                if (Sup(xv)<Sup(x(v))) {
                    INTERVAL_VECTOR xx=x;
                    double a = Inf(x(v));
                    double b = Sup(x(v));
                    double c = Sup(xv);
                    x(v)=INTERVAL(a,c);
                    //cout << "split x = " << x << endl;
                    xx(v)=INTERVAL(c,b);
                    //cout << "split xx = " << xx << endl;
                    UB.insert(pair<double,INTERVAL_VECTOR>(c,xx));
                    if (prune(x,n)) {
                        numBoxes++;
                        if (numBoxes>1) x_hull = Hull(x_hull,x);
                        else x_hull = x;
                        if (n==0) {
                            //numBlack++;
                            //cout << "i = " << i << " black = " << numBlack << endl;
                            black_hull = Hull(black_hull,x);

                            //q = d->quadratureEval(x);
                            //if (Inf(q)<0.0) q = INTERVAL(0.0,Sup(q));
                            //q = integrate(x,total_wid_max);
                            //pair<INTERVAL_VECTOR,INTERVAL> p = pair<INTERVAL_VECTOR,INTERVAL>(x,q);
                            //blackBoxes.insert(pair<double,pair<INTERVAL_VECTOR,INTERVAL> >(-Diam(q),p));

                        }
                        else {
                            //cout << "gray" << endl;
                            global_hull = Hull(global_hull,x);

                            //q = d->quadratureEval(x);
                            //if (Inf(q)<0.0) q = INTERVAL(0.0,Sup(q));
                            //numGray++;
                            //pair<INTERVAL_VECTOR,INTERVAL> p = pair<INTERVAL_VECTOR,INTERVAL>(x,q);
                            //grayBoxes.insert(pair<double,pair<INTERVAL_VECTOR,INTERVAL> >(-Diam(q),p));
                            //grayBoxes.insert(pair<double,pair<INTERVAL_VECTOR,INTERVAL> >(-Sup(q),p));

                        }
                    }
                }
                else {
                    global_hull = Hull(global_hull,x);
                    numBoxes++;
                    if (numBoxes>1) x_hull = Hull(x_hull,x);
                    else x_hull = x;

                    //q = d->quadratureEval(x);
                    //if (Inf(q)<0.0) q = INTERVAL(0.0,Sup(q));
                    //numGray++;
                    //pair<INTERVAL_VECTOR,INTERVAL> p = pair<INTERVAL_VECTOR,INTERVAL>(x,q);
                    //grayBoxes.insert(pair<double,pair<INTERVAL_VECTOR,INTERVAL> >(-Diam(q),p));
                    //grayBoxes.insert(pair<double,pair<INTERVAL_VECTOR,INTERVAL> >(-Sup(q),p));

                }
            }
            else look = false;
        }
        look = true;
        //cout << "Black boxes" << endl;
        while ((UB_black.size()>0) && look) {
            if (UB_black.begin()->first < Sup(xv)) {
                x = UB_black.begin()->second;
                //cout << "x = " << x << endl;
                UB_black.erase(UB_black.begin());
                if (Sup(xv)<Sup(x(v))) {
                    INTERVAL_VECTOR xx=x;
                    double a = Inf(x(v));
                    double b = Sup(x(v));
                    double c = Sup(xv);
                    x(v)=INTERVAL(a,c);
                    //cout << "split x = " << x << endl;
                    xx(v)=INTERVAL(c,b);
                    //cout << "split xx = " << xx << endl;
                    UB_black.insert(pair<double,INTERVAL_VECTOR>(c,xx));
                }
                numBoxes++;
                if (numBoxes>1) x_hull = Hull(x_hull,x);
                else x_hull = x;
                black_hull = Hull(black_hull,x);

                //numBlack++;
                //cout << "i = " << i << " black = " << numBlack << endl;
                //q = d->quadratureEval(x);
                //if (Inf(q)<0.0) q = INTERVAL(0.0,Sup(q));
                //q = integrate(x,total_wid_max);
                //pair<INTERVAL_VECTOR,INTERVAL> p = pair<INTERVAL_VECTOR,INTERVAL>(x,q);
                //blackBoxes.insert(pair<double,pair<INTERVAL_VECTOR,INTERVAL> >(-Diam(q),p));

            }
            else look = false;
        }
        //cout << "numBlack = " << numBlack << endl;
        //cout << "numGray = " << numGray << endl;
        cout << i << "> x_hull = " << x_hull << endl;
        q = d->quadratureEval(x_hull);
        if (Inf(q)<0.0) q = INTERVAL(0.0,Sup(q));
        pair<INTERVAL_VECTOR,INTERVAL> p = pair<INTERVAL_VECTOR,INTERVAL>(x_hull,q);
        grayBoxes.insert(pair<double,pair<INTERVAL_VECTOR,INTERVAL> >(-Diam(q),p));
    }

    global_hull = Hull(global_hull,black_hull);
    cout << "grayBoxes -> " << grayBoxes.size() << endl;
    cout << "blackBoxes -> " << blackBoxes.size() << endl;
    cout << "black_hull = " << black_hull << endl;
    cout << "global_hull = " << global_hull << endl;
    //cout << "H = " << H << endl;
    //for (int i=0;i<nDiv;i++) cout << "h[" << i << "] = " << h[i] << endl;
}
*/
/*
double InverseProblem::improveGraphic(int v, int iter, ofstream & file)
{
    chronos chrono2;
    chrono2.start();
    bool black;
    INTERVAL x_old;
    int xk, n;
    //for (int i=0;i<iter;i++) {
    int i=0;
    while (chrono2.duration()<iter) {
        //Boxes box;
        //cout << i << endl;
        i++;
        pair<INTERVAL_VECTOR,INTERVAL> p;
        INTERVAL_VECTOR x;
        INTERVAL q;
        if (grayBoxes.begin()->first<blackBoxes.begin()->first) {
            //cout << "w = " << -grayBoxes.begin()->first << " (gray)" << endl;
            black = false;
            p = grayBoxes.begin()->second;
            x = p.first;
            //cout << "x = " << x << " (gray)" << endl;
            grayBoxes.erase(grayBoxes.begin());
        }
        else {
            //cout << "w = " << -blackBoxes.begin()->first << " (black)" << endl;
            black = true;
            p = blackBoxes.begin()->second;
            x = p.first;
            //cout << "x = " << x << " (black)" << endl;
            blackBoxes.erase(blackBoxes.begin());
        }
        //q = p.second;
        //cout << "q = " << q << endl;
        //int k = p.second;
        //cout << "k = " << k << endl;
        //cout << "h[k] = " << h[k] << " -> ";
        //h[k] = INTERVAL(Inf(h[k])-Inf(box.q),Sup(h[k])-Sup(box.q));
        //cout << h[k] << endl;
        //cout << "H = " << H << " -> ";
        //H = INTERVAL(Inf(H)-Inf(box.q),Sup(H)-Sup(box.q));
        //cout << H << endl;
        INTERVAL_VECTOR xx=BisectIntervalVector(x,xk,x_old);
        if (black) {
            // box x
            //box.x = x;
            //h[k] += box.q;
            //H += box.q;
            //cout << "new x = " << x << " (black)" << endl;
            //cout << "new q = " << q << endl;
            //cout << "new h[k] = " << h[k] << endl;
            //cout << "new H = " << H << endl;
            //if (box.vars[v-1]==-1) { cout << "ERROR" << endl; cin.get();}
            //blackBoxes.insert(pair<double,Boxes>(-Diam(box.q),box));
            q = d->quadratureEval(x);
            //q = integrate(x,total_wid_max);
            if (Inf(q)<0.0) q = INTERVAL(0.0,Sup(q));
            //cout << "q = " << q << endl;
            pair<INTERVAL_VECTOR,INTERVAL> px = pair<INTERVAL_VECTOR,INTERVAL>(x,q);
            blackBoxes.insert(pair<double,pair<INTERVAL_VECTOR,INTERVAL> >(-Diam(q),px));
            // box xx
            //Boxes box1;
            //box1.x = xx;
            //box1.vars = new int[nVars];
            //for (unsigned int j=0;j<nVars;j++) box.vars[j] = -1;
            //box1.vars[v-1] = k;
            //h[k] += box1.q;
            //H += box1.q;
            //cout << "new xx = " << xx << " (black)" << endl;
            //cout << "new q = " << q << endl;
            //cout << "new h[k] = " << h[k] << endl;
            //cout << "new H = " << H << endl;
            //if (box1.vars[v-1]==-1) { cout << "ERROR" << endl; cin.get();}
            //blackBoxes.insert(pair<double,Boxes>(-Diam(box1.q),box1));
            q = d->quadratureEval(xx);
            //q = integrate(xx,total_wid_max);
            if (Inf(q)<0.0) q = INTERVAL(0.0,Sup(q));
            //cout << "q = " << q << endl;
            pair<INTERVAL_VECTOR,INTERVAL> pxx = pair<INTERVAL_VECTOR,INTERVAL>(xx,q);
            blackBoxes.insert(pair<double,pair<INTERVAL_VECTOR,INTERVAL> >(-Diam(q),pxx));
        }
        else {
            // box x
            if (prune(x,n)) {
                //box.x = x;
                if (n==0) {
                    //cout << "black" << endl;
                    //h[k] += box.q;
                    //H += box.q;
                    //cout << "new x = " << x << " (black)" << endl;
                    //cout << "new q = " << q << endl;
                    black_hull = Hull(black_hull,x);
                    //if (box.vars[v-1]==-1) { cout << "ERROR" << endl; cin.get();}
                    //blackBoxes.insert(pair<double,Boxes>(-Diam(box.q),box));
                    q = d->quadratureEval(x);
                    //q = integrate(x,total_wid_max);
                    if (Inf(q)<0.0) q = INTERVAL(0.0,Sup(q));
                    //cout << "q = " << q << endl;
                    pair<INTERVAL_VECTOR,INTERVAL> px = pair<INTERVAL_VECTOR,INTERVAL>(x,q);
                    blackBoxes.insert(pair<double,pair<INTERVAL_VECTOR,INTERVAL> >(-Diam(q),px));
                }
                else {
                    //h[i] += INTERVAL(0.0,Sup(box.q));
                    //H += INTERVAL(0.0,Sup(box.q));
                    //cout << "gray" << endl;
                    //q0 = -Diam(box.q);
                    //box.q = INTERVAL(0.0,Sup(box.q));
                    //cout << "new x = " << x << " (gray)" << endl;
                    //cout << "new q = " << q << endl;
                    //if (box.vars[v-1]==-1) { cout << "ERROR" << endl; cin.get();}
                    //grayBoxes.insert(pair<double,Boxes>(-Diam(box.q),box));
                    q = d->quadratureEval(x);
                    if (Inf(q)<0.0) q = INTERVAL(0.0,Sup(q));
                    pair<INTERVAL_VECTOR,INTERVAL> px = pair<INTERVAL_VECTOR,INTERVAL>(x,q);
//grayBoxes.insert(pair<double,pair<INTERVAL_VECTOR,INTERVAL> >(-Diam(q),px));
                    //grayBoxes.insert(pair<double,pair<INTERVAL_VECTOR,INTERVAL> >(-Sup(q),px));
                    //grayBoxes.insert(pair<double,Boxes>(q0,box));
                }
                //h[k] += box.q;
                //H += box.q;
                //cout << "new h[k] = " << h[k] << endl;
                //cout << "new H = " << H << endl;
            }
            // box xx
            if (prune(xx,n)) {
                //Boxes box1;
                //box1.x = xx;
                //box1.vars = new int[nVars];
                //for (unsigned int j=0;j<nVars;j++) box.vars[j] = -1;
                //box1.vars[v-1] = k;
                if (n==0) {
                    //cout << "black" << endl;
                    //h[k] += box1.q;
                    //H += box1.q;
                    black_hull = Hull(black_hull,xx);
                    //cout << "new xx = " << xx << " (black)" << endl;
                    //cout << "new q = " << q << endl;
                    //if (box1.vars[v-1]==-1) { cout << "ERROR" << endl; cin.get();}
                    //blackBoxes.insert(pair<double,Boxes>(-Diam(box1.q),box1));
                    q = d->quadratureEval(xx);
                    //q = integrate(xx,total_wid_max);
                    if (Inf(q)<0.0) q = INTERVAL(0.0,Sup(q));
                    //cout << "q = " << q << endl;
                    pair<INTERVAL_VECTOR,INTERVAL> pxx = pair<INTERVAL_VECTOR,INTERVAL>(xx,q);
                    blackBoxes.insert(pair<double,pair<INTERVAL_VECTOR,INTERVAL> >(-Diam(q),pxx));
                }
                else {
                    //cout << "gray" << endl;
                    //h[i] += INTERVAL(0.0,Sup(box.q));
                    //H += INTERVAL(0.0,Sup(box.q));
                    //q0 = -Diam(box1.q);
                    //box1.q = INTERVAL(0.0,Sup(box1.q));
                    //cout << "new xx = " << xx << " (gray)" << endl;
                    //cout << "new q = " << q << endl;
                    //if (box1.vars[v-1]==-1) { cout << "ERROR" << endl; cin.get();}
                    //grayBoxes.insert(pair<double,Boxes>(-Diam(box1.q),box1));
                    q = d->quadratureEval(xx);
                    if (Inf(q)<0.0) q = INTERVAL(0.0,Sup(q));
                    pair<INTERVAL_VECTOR,INTERVAL> pxx = pair<INTERVAL_VECTOR,INTERVAL>(xx,q);
//grayBoxes.insert(pair<double,pair<INTERVAL_VECTOR,INTERVAL> >(-Diam(q),pxx));
                    //grayBoxes.insert(pair<double,pair<INTERVAL_VECTOR,INTERVAL> >(-Sup(q),pxx));
                    //grayBoxes.insert(pair<double,Boxes>(q0,box1));
                }
                //h[k] += box1.q;
                //H += box1.q;
                //cout << "new h[k] = " << h[k] << endl;
                //cout << "new H = " << H << endl;
            }
        }
        //cin.get();
    }

    cout << "H = " << H << endl;
    for (int i=0;i<nDiv;i++) {
        //cout << "h[" << i << "] = " << h[i] << endl;
        h[i] = INTERVAL(0.0);
    }
    cout << "grayBoxes.size() = " << grayBoxes.size() << endl;
    cout << "grayBoxes.max_size() = " << grayBoxes.max_size() << endl;
    cout << "blackBoxes.size() = " << blackBoxes.size() << endl;
    cout << "blackBoxes.max_size() = " << blackBoxes.max_size() << endl;
    H = INTERVAL(0.0);
    //Boxes box;
    INTERVAL q;
    pair<INTERVAL_VECTOR,INTERVAL> p;
    INTERVAL_VECTOR x;
    int ix;
    */
    /*
    multimap<double,pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> > >::iterator it;
    cout << "first gray -> w = " << -grayBoxes.begin()->first << " q = " << grayBoxes.begin()->second.second << endl;
    cout << "first black -> w = " << -blackBoxes.begin()->first << " q = " << blackBoxes.begin()->second.second << endl;
    for (it=grayBoxes.begin();it!=grayBoxes.end();it++) {
        p = it->second;
        x = p.first;
        q = p.second;
        q = INTERVAL(0.0,Sup(q));
        ix = (Mid(x(v))-lbox[v-1])/eps[v-1];
        h[ix] += q;
        H += q;
    }
    for (it=blackBoxes.begin();it!=blackBoxes.end();it++) {
        p = it->second;
        x = p.first;
        q = p.second;
        ix = (Mid(x(v))-lbox[v-1])/eps[v-1];
        h[ix] += q;
        H += q;
    }
    */
    /*
    double maxminh = 0.0;
    double maxch = 0.0;
    for (int i=0;i<nDiv;i++) {
        minh[i] = Inf(h[i])/(Sup(H)-Diam(h[i]));
        maxh[i] = Sup(h[i])/(Inf(H)+Diam(h[i]));
        ch[i] = (maxh[i]+minh[i])/2.0;
        if (Inf(minh[i])>maxminh) maxminh = Inf(minh[i]);
        if (Sup(ch[i])>maxch) maxch = Sup(ch[i]);
    }
    MH = 1000;
    INTERVAL maxk = MH/maxminh;
    double k = MH/maxch;
    double maxge = 0;
    cout << "MH = " << MH << endl;
    cout << "maxminh = " << maxminh << endl;
    cout << "maxch = " << maxch << endl;
    cout << "maxk = " << maxk << endl;
    cout << "k = " << k << endl;
    for (int i=0;i<nDiv;i++) {
        maxe[i] = Sup(maxk*(maxh[i]-minh[i])/2);
        gh[i] = (int)(k*Sup(ch[i])+0.5);
        ge[i] = max((Sup(maxh[i])*k-gh[i]),(gh[i]-Inf(minh[i])*k));
        if (ge[i]>maxge) maxge = ge[i];
    }
    cout << "maxge = " << maxge << endl;

    for (int i=0;i<nDiv;i++) {
        cout << "h[" << i << "] = " << h[i] << endl;
        */
        /*
        cout << "minh[" << i << "] = " << minh[i] << endl;
        cout << "maxh[" << i << "] = " << maxh[i] << endl;
        cout << "ch[" << i << "] = " << ch[i] << endl;
        cout << "maxe[" << i << "] = " << maxe[i] << endl;
        cout << "gh[" << i << "] = " << gh[i] << endl;
        cout << "ge[" << i << "] = " << ge[i] << endl;
        cin.get();
        */
        /*
    }

    cout << "gh -> " << endl;
    file << "gh -> " << endl;
    for (int i=0;i<nDiv;i++) {
        cout << " " << gh[i];
        file << " " << gh[i];
    }
    cout << endl;
    file << endl;
    cout << "ge -> " << endl;
    file << "ge -> " << endl;
    for (int i=0;i<nDiv;i++) {
        cout << " " << ge[i];
        file << " " << ge[i];
    }
    cout << endl;
    file << endl;
    //cin.get();
    return maxge;
}
*/

unsigned long int InverseProblem::getIndex(INTERVAL_VECTOR & x)
{
    int ix;
    unsigned long int index = 0;
    for (int v=1;v<=nVars;v++) {

        ix = (Inf(x(v))-lbox[v-1])/eps[v-1];
        //ix = (Mid(x(v))-lbox[v-1])/eps[v-1];
/*
        cout << "v = " << v << endl;
        cout << "x(v) = " << x(v) << endl;
        cout << "Mid(x(v)) = " << Mid(x(v)) << endl;
        cout << "Inf(x(v)) = " << Inf(x(v)) << endl;
        cout << "lbox[v-1] = " << lbox[v-1] << endl;
        cout << "eps[v-1] = " << eps[v-1] << endl;
        cout << "(Mid(x(v))-lbox[v-1]) = " << (Mid(x(v))-lbox[v-1]) << endl;
        cout << "(Inf(x(v))-lbox[v-1]) = " << (Inf(x(v))-lbox[v-1]) << endl;
        cout << "(Mid(x(v))-lbox[v-1])/eps[v-1] = " << (Mid(x(v))-lbox[v-1])/eps[v-1] << endl;
        cout << "(Inf(x(v))-lbox[v-1])/eps[v-1] = " << (Inf(x(v))-lbox[v-1])/eps[v-1] << endl;
        cout << "ix = " << ix << endl;
*/
        index += ix * pow(nDiv,(nVars-v));
/*
        cout << "nDiv = " << nDiv << endl;
        cout << "nVars = " << nVars << endl;
        cout << "pow(nDiv,(nVars-v)) = " << pow(nDiv,(nVars-v)) << endl;
        cout << "ix * (nDiv^(nVars-v)) = " << ix * pow(nDiv,(nVars-v)) << endl;
        cout << "index = " << index << endl;
*/
    }
    return index;
}


void InverseProblem::getCoordinates(unsigned long int index, int t[])
{
    double npow;
    for (int v=1;v<nVars;v++) {
        //cout << "v = " << v << endl;
        //cout << "index = " << index << endl;
        npow = pow(nDiv,(nVars-v));
        //cout << "npow = " << npow << endl;
        t[v-1] = index / (int)npow;
        //cout << "t[v-1] = " << t[v-1] << endl;
        index -= t[v-1] * (int)npow;
        //cout << "new index = " << index << endl;
    }
    t[nVars-1] = index;
    //cout << "t[nVars-1] = " << t[nVars-1] << endl;
}

double InverseProblem::volume(const INTERVAL_VECTOR & x)
{
	double result=1.0;
	for (int i = 1; i <= Dimension(x); i++)
		result *= Diam(x(i));
	return result;
}


INTERVAL_VECTOR InverseProblem::randomSample(const INTERVAL_VECTOR & x)
{
    INTERVAL_VECTOR xi = x;
    double r, r1;
    for (int i = 1; i <= nVars; i++) {
        r1 = rand();
        r=r1/RAND_MAX;
        //cout << "r1 = " << r1 << " RAND_MAX = " << RAND_MAX << " r = " << r << endl;
        //cin.get();
        xi(i) = INTERVAL(Inf(x(i))+r*Diam(x(i)));
    }
    //cout << "sample x  = " << x << endl;
    //cout << "result xi = " << xi << endl;
    //cin.get();
    return xi;
}

INTERVAL_VECTOR InverseProblem::monteCarlo(const INTERVAL_VECTOR & x, int iter)
{
    INTERVAL_VECTOR q(2+2*nVars);
    INTERVAL_VECTOR xi;
    INTERVAL f, s;
    for (int i = 1; i <= 2+2*nVars; i++) q(i) = INTERVAL(0.0);
    for (int i = 1; i <= iter; i++) {
        xi = randomSample(x);
        f = d->f(xi);
        q(1) += f;
        q(2) += Sqr(f);
        for (int j = 1; j <= nVars; j++) {
            q(2+j) += xi(j)*f;
            q(2+nVars+j) += Sqr(xi(j))*f;
        }
    }
    return q;
}

INTERVAL_VECTOR InverseProblem::monteCarloHull(const INTERVAL_VECTOR & x, int iter)
{
    INTERVAL_VECTOR q(2+2*nVars);
    INTERVAL_VECTOR xi;
    INTERVAL f, s;
    int n;
    for (int i = 1; i <=2+2*nVars; i++) q(i) = INTERVAL(0.0);
    for (int i = 1; i <= iter; i++) {
        xi = randomSample(x);
        if (prune(xi,n)) {
            if (firstBlack) {
                black_hull = xi;
                firstBlack = false;
            }
            else black_hull = Hull(black_hull,xi);
            f = d->f(xi);
            q(1) += f;
            q(2) += Sqr(f);
            for (int j = 1; j <= nVars; j++) {
                q(2+j) += xi(j)*f;
                q(2+nVars+j) += Sqr(xi(j))*f;
            }
            //cout << "iter = " << i << " xi =" << xi << endl;
            solutions++;
        }
    }
    return q;
}

INTERVAL InverseProblem::monteCarloMean(const INTERVAL_VECTOR & x, int N, const INTERVAL_VECTOR & q)
{
    return INTERVAL(volume(x))*q(1)/N;
}

double InverseProblem::monteCarloSTD(const INTERVAL_VECTOR & x, int N, const INTERVAL_VECTOR & q)
{
    INTERVAL ef = q(1)/N;
    INTERVAL ef2 = q(2)/N;
    double std=volume(x)*Sup(Sqrt((ef2-Sqr(ef))/N));
    return std;
}

INTERVAL_VECTOR InverseProblem::monteCarloIntegrals(const INTERVAL_VECTOR & x, int N, const INTERVAL_VECTOR & q)
{
    INTERVAL_VECTOR r(2+2*nVars);
    INTERVAL ef = q(1)/N;
    INTERVAL ef2 = q(2)/N;
    INTERVAL V = INTERVAL(volume(x));
    r(1) = V*ef;
    r(2) = V*Sqrt((ef2-Sqr(ef))/N);
    for (int j = 1; j <= nVars; j++) {
        r(2+j) = V*q(2+j)/N;
        //r(2+nVars+j) = (V*q(2+nVars+j)/N)-Sqr(r(2+j));
        r(2+nVars+j) = V*q(2+nVars+j)/N;
    }
    return r;
}
void InverseProblem::improveGraphic(int nf, int iter)
{
    chronos chrono2;
    chrono2.start();
    bool black;
    INTERVAL_VECTOR x(nVars);
    INTERVAL_VECTOR xx(nVars);
    INTERVAL q, q0, qindex;
    pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> > p;
    pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> > px;
    pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> > pxx;
    pair<INTERVAL,unsigned long int> qx;
    pair<INTERVAL,unsigned long int> qxx;
    unsigned long int index;
    int n;
    bool canonical;
    while (chrono2.duration()<iter) {
        if (grayBoxes.begin()->first<blackBoxes.begin()->first) {
            //cout << "w = " << -grayBoxes.begin()->first << " (gray)" << endl;
            black = false;
            p = grayBoxes.begin()->second;
            grayBoxes.erase(grayBoxes.begin());
        }
        else {
            //cout << "w = " << -blackBoxes.begin()->first << " (black)" << endl;
            black = true;
            p = blackBoxes.begin()->second;
            blackBoxes.erase(blackBoxes.begin());
        }
        x = p.first;
        q0 = p.second.first;
        index = p.second.second;
        qindex = joint[index];
        qindex = INTERVAL(Inf(qindex)-Inf(q0),Sup(qindex)-Sup(q0));
        if (Inf(qindex)<0.0) cout << "Error: x = " << x << " index = " << index << " joint[index] = " << joint[index] << " q0 = " << q0 << " qindex = " << qindex << endl;
        xx=BisectIntervalVector(x, canonical);
        /*
        if (canonical) {
            cout << "canonical box:" << endl;
            if (black) cout << "black" << endl;
            else cout << "gray" << endl;
            cout << "x = " << x << endl;
            cout << "xx = " << xx << endl;
            cin.get();
        }*/
        if (black) {
            q = d->quadratureEval(x);
            if (Inf(q)<0.0) q = INTERVAL(0.0,Sup(q));
            qx = pair<INTERVAL,unsigned long int>(q,index);
            px = pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> >(x,qx);
            blackBoxes.insert(pair<double,pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> > >(-Diam(q),px));
            qindex += q;
            q = d->quadratureEval(xx);
            if (Inf(q)<0.0) q = INTERVAL(0.0,Sup(q));
            qxx = pair<INTERVAL,unsigned long int>(q,index);
            pxx = pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> >(xx,qxx);
            blackBoxes.insert(pair<double,pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> > >(-Diam(q),pxx));
            qindex += q;
        }
        else {
            if (prune(x,n)) {
                if (n==0) {
                    black_hull = Hull(black_hull,x);
                    q = d->quadratureEval(x);
                    if (Inf(q)<0.0) q = INTERVAL(0.0,Sup(q));
                    qx = pair<INTERVAL,unsigned long int>(q,index);
                    px = pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> >(x,qx);
                    blackBoxes.insert(pair<double,pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> > >(-Diam(q),px));
                }
                else {
                    q = d->quadratureEval(x,1);
                    q = INTERVAL(0.0,Sup(q));
                    qx = pair<INTERVAL,unsigned long int>(q,index);
                    px = pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> >(x,qx);
                    grayBoxes.insert(pair<double,pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> > >(-Diam(q),px));
                }
                qindex += q;
            }
            if (prune(xx,n)) {
                if (n==0) {
                    black_hull = Hull(black_hull,xx);
                    q = d->quadratureEval(xx);
                    if (Inf(q)<0.0) q = INTERVAL(0.0,Sup(q));
                    qxx = pair<INTERVAL,unsigned long int>(q,index);
                    pxx = pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> >(xx,qxx);
                    blackBoxes.insert(pair<double,pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> > >(-Diam(q),pxx));
                }
                else {
                    q = d->quadratureEval(xx,1);
                    q = INTERVAL(0.0,Sup(q));
                    qxx = pair<INTERVAL,unsigned long int>(q,index);
                    pxx = pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> >(xx,qxx);
                    grayBoxes.insert(pair<double,pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> > >(-Diam(q),pxx));
                }
                qindex += q;
            }
        }
        joint[index] = qindex;
    }
    qtotal = INTERVAL(0.0);
    maxqwidth = 0.0;
    cout << "grayBoxes.size() = " << grayBoxes.size() << endl;
    cout << "blackBoxes.size() = " << blackBoxes.size() << endl;
    cout << "first gray -> w = " << -grayBoxes.begin()->first << " q = " << grayBoxes.begin()->second.second.first << endl;
    cout << "first black -> w = " << -blackBoxes.begin()->first << " q = " << blackBoxes.begin()->second.second.first << endl;
    map<unsigned long int,INTERVAL>::iterator it;

    stringstream ssn; ssn << nf;
    string filename = "output" + ssn.str() + ".txt";
    ofstream file (filename.c_str());
    int t[nVars];
    if (file.is_open()){
        for (it=joint.begin();it!=joint.end();it++) {
            index = it->first;
            q = it->second;
            getCoordinates(index,t);
            file << Mid(q);
            for (int v=0;v<nVars;v++) file << " " << t[v];
            file << " " << q << " " << Diam(q) << endl;
            qtotal += q;
            if (Diam(q)>maxqwidth) maxqwidth = Diam(q);
        }
        file.close();
    }


    INTERVAL E[nVars];
    INTERVAL E1[nVars];
    for (int v=0;v<nVars;v++) E[v] = INTERVAL(0.0);
    for (int v=0;v<nVars;v++) E1[v] = INTERVAL(0.0);
    multimap<double,pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> > >::iterator it0;
    for (it0=blackBoxes.begin();it0!=blackBoxes.end();it0++) {
        p = it0->second;
        x = p.first;
        q = p.second.first;
        for (int v=0;v<nVars;v++) {
            E[v] += x(v+1)*q;
            E1[v] += Sqr(x(v+1))*q;
        }
    }
    for (it0=grayBoxes.begin();it0!=grayBoxes.end();it0++) {
        p = it0->second;
        x = p.first;
        q = p.second.first;
        for (int v=0;v<nVars;v++) {
            E[v] += x(v+1)*q;
            E1[v] += Sqr(x(v+1))*q;
            //E1[v] += x(v+1)*INTERVAL(0.0,Sup((d->f(x))*volume(x)));
        }
    }
    for (int v=0;v<nVars;v++) cout << "E[" << v << "] = " << E[v] << endl;
    for (int v=0;v<nVars;v++) cout << "E[" << v << "]/L = " << E[v]/qtotal << endl;
    for (int v=0;v<nVars;v++) cout << "E1[" << v << "]/L = " << E1[v]/qtotal << endl;
    for (int v=0;v<nVars;v++) cout << "mid E[" << v << "] = " << Mid(E[v]) << endl;
    for (int v=0;v<nVars;v++) cout << "mid E[" << v << "] / mid L = " << Mid(E[v])/Mid(qtotal) << endl;

    INTERVAL V[nVars];
    for (int v=0;v<nVars;v++) V[v] = INTERVAL(0.0);
    INTERVAL V1[nVars];
    for (int v=0;v<nVars;v++) V1[v] = INTERVAL(0.0);
    for (it0=blackBoxes.begin();it0!=blackBoxes.end();it0++) {
        p = it0->second;
        x = p.first;
        q = p.second.first;
        for (int v=0;v<nVars;v++) {
            V[v] += Power((x(v+1)-E[v]/qtotal),2)*q;
            V1[v] += Power((x(v+1)-Mid(E[v]/qtotal)),2)*q;
        }
    }
    for (it0=grayBoxes.begin();it0!=grayBoxes.end();it0++) {
        p = it0->second;
        x = p.first;
        q = p.second.first;
        for (int v=0;v<nVars;v++) {
            V[v] += Power((x(v+1)-E[v]/qtotal),2)*q;
            V1[v] += Power((x(v+1)-Mid(E[v]/qtotal)),2)*q;
        }
    }
    for (int v=0;v<nVars;v++) cout << "V[" << v << "] = " << V[v] << endl;
    for (int v=0;v<nVars;v++) cout << "V[" << v << "]/L = " << V[v]/qtotal << endl;
    for (int v=0;v<nVars;v++) cout << "sqrt V[" << v << "]/L = " << Sqrt(V[v]/qtotal) << endl;
    for (int v=0;v<nVars;v++) cout << "mid V[" << v << "] = " << Mid(V[v]) << endl;
    for (int v=0;v<nVars;v++) cout << "mid V[" << v << "] / mid L = " << Mid(V[v])/Mid(qtotal) << endl;
    for (int v=0;v<nVars;v++) cout << "mid sqrt V[" << v << "]/L = " << Mid(Sqrt(V[v]/qtotal)) << endl;

    for (int v=0;v<nVars;v++) cout << "V1[" << v << "] = " << V1[v] << endl;
    for (int v=0;v<nVars;v++) cout << "V1[" << v << "]/L = " << V1[v]/qtotal << endl;
    for (int v=0;v<nVars;v++) cout << "sqrt V1[" << v << "] = " << Sqrt(V1[v]) << endl;
    for (int v=0;v<nVars;v++) cout << "sqrt V1[" << v << "]/L = " << Sqrt(V1[v]/qtotal) << endl;
    for (int v=0;v<nVars;v++) cout << "sigma[" << v << "] = " << (E1[v]/qtotal)-Sqr(E[v]/qtotal) << endl;
    for (int v=0;v<nVars;v++) cout << "mid V1[" << v << "] = " << Mid(V1[v]) << endl;
    for (int v=0;v<nVars;v++) cout << "mid sqrt V1[" << v << "] = " << Mid(Sqrt(V1[v])) << endl;
    for (int v=0;v<nVars;v++) cout << "mid sqrt V1[" << v << "]/L = " << Mid(Sqrt(V1[v]/qtotal)) << endl;

    stringstream ssn1; ssn1 << nf;
    string filename1 = "expected" + ssn1.str() + ".txt";
    ofstream file1 (filename1.c_str());
    if (file1.is_open()){
        for (int v=0;v<nVars;v++) file1 << "E[" << v << "] = " << E[v] << endl;
        for (int v=0;v<nVars;v++) file1 << "E[" << v << "]/L = " << E[v]/qtotal << endl;
        for (int v=0;v<nVars;v++) file1 << "E1[" << v << "]/L = " << E1[v]/qtotal << endl;
        for (int v=0;v<nVars;v++) file1 << "mid E[" << v << "] = " << Mid(E[v]) << endl;
        for (int v=0;v<nVars;v++) file1 << "mid E[" << v << "] / mid L = " << Mid(E[v])/Mid(qtotal) << endl;

        for (int v=0;v<nVars;v++) file1 << "V[" << v << "] = " << V[v] << endl;
        for (int v=0;v<nVars;v++) file1 << "V[" << v << "]/L = " << V[v]/qtotal << endl;
        for (int v=0;v<nVars;v++) file1 << "sqrt V[" << v << "]/L = " << Sqrt(V[v]/qtotal) << endl;
        for (int v=0;v<nVars;v++) file1 << "mid V[" << v << "] = " << Mid(V[v]) << endl;
        for (int v=0;v<nVars;v++) file1 << "mid V[" << v << "] / mid L = " << Mid(V[v])/Mid(qtotal) << endl;
        for (int v=0;v<nVars;v++) file1 << "mid sqrt V[" << v << "]/L = " << Mid(Sqrt(V[v]/qtotal)) << endl;

        for (int v=0;v<nVars;v++) file1 << "V1[" << v << "] = " << V1[v] << endl;
        for (int v=0;v<nVars;v++) file1 << "V1[" << v << "]/L = " << V1[v]/qtotal << endl;
        for (int v=0;v<nVars;v++) file1 << "sqrt V1[" << v << "] = " << Sqrt(V1[v]) << endl;
        for (int v=0;v<nVars;v++) file1 << "sqrt V1[" << v << "]/L = " << Sqrt(V1[v]/qtotal) << endl;
        for (int v=0;v<nVars;v++) file1 << "sigma[" << v << "] = " << (E1[v]/qtotal)-Sqr(E[v]/qtotal) << endl;
        for (int v=0;v<nVars;v++) file1 << "mid V1[" << v << "] = " << Mid(V1[v]) << endl;
        for (int v=0;v<nVars;v++) file1 << "mid sqrt V1[" << v << "] = " << Mid(Sqrt(V1[v])) << endl;
        for (int v=0;v<nVars;v++) file1 << "mid sqrt V1[" << v << "]/L = " << Mid(Sqrt(V1[v]/qtotal)) << endl;
        file1.close();
    }

}

void InverseProblem::improveGraphicMC(int nf, int iter)
{
    chronos chrono2;
    chrono2.start();
    bool black;
    INTERVAL_VECTOR x(nVars);
    INTERVAL_VECTOR xx(nVars);
    INTERVAL qindex, x_old;
    INTERVAL_VECTOR q(2+2*nVars);
    INTERVAL_VECTOR q0(2+2*nVars);
    double std;
    pair<INTERVAL_VECTOR,pair<INTERVAL_VECTOR,unsigned long int> > p;
    pair<INTERVAL_VECTOR,pair<INTERVAL_VECTOR,unsigned long int> > px;
    pair<INTERVAL_VECTOR,pair<INTERVAL_VECTOR,unsigned long int> > pxx;
    pair<INTERVAL_VECTOR,unsigned long int> qx;
    pair<INTERVAL_VECTOR,unsigned long int> qxx;
    unsigned long int index = 0;
    int n, xk;
    double r1;
    bool canonical;
    while (chrono2.duration()<iter) {
        p = grayBoxesMC.begin()->second;
        grayBoxesMC.erase(grayBoxesMC.begin());
        x = p.first;
        q0 = p.second.first;
        xx=BisectIntervalVector(x,xk,x_old);
        q = monteCarloHull(x,nMC);
        std = monteCarloSTD(x,nMC,q);
        //r1 = rand();
        //std=r1/RAND_MAX;
        //cout << "std = " << std << endl;
        qx = pair<INTERVAL_VECTOR, unsigned long int>(q,index);
        px = pair<INTERVAL_VECTOR,pair<INTERVAL_VECTOR, unsigned long int> >(x,qx);
        grayBoxesMC.insert(pair<double,pair<INTERVAL_VECTOR,pair<INTERVAL_VECTOR, unsigned long int> > >(-std,px));
        globalMC  += q;
        nglobalMC += nMC;
        /*
        cout << "x ******************************************" << endl;
        cout << "q = " << q << endl;
        cout << "globalMC = " << globalMC << endl;
        cout << "nglobalMC = " << nglobalMC << endl;
        cout << "solutions = " << solutions << endl;
        cout << "******************************************" << endl;
        */
        //cin.get();

        q = monteCarloHull(xx,nMC);
        std = monteCarloSTD(xx,nMC,q);
        //r1 = rand();
        //std=r1/RAND_MAX;
        //cout << "std = " << std << endl;
        qxx = pair<INTERVAL_VECTOR, unsigned long int>(q,index);
        pxx = pair<INTERVAL_VECTOR,pair<INTERVAL_VECTOR, unsigned long int> >(xx,qxx);
        grayBoxesMC.insert(pair<double,pair<INTERVAL_VECTOR,pair<INTERVAL_VECTOR, unsigned long int> > >(-std,pxx));
        globalMC  += q;
        nglobalMC += nMC;
        /*
        cout << "xx ******************************************" << endl;
        cout << "q = " << q << endl;
        cout << "globalMC = " << globalMC << endl;
        cout << "nglobalMC = " << nglobalMC << endl;
        cout << "solutions = " << solutions << endl;
        cout << "******************************************" << endl;
        */
        //cin.get();
    }
    qtotal = INTERVAL(0.0);
    maxqwidth = 0.0;
    cout << "BoxesMC.size() = " << grayBoxesMC.size() << endl;
    cout << "first gray -> w = " << -grayBoxesMC.begin()->first << " q = " << grayBoxes.begin()->second.second.first << endl;
    map<unsigned long int,INTERVAL>::iterator it;

    /*
    stringstream ssn; ssn << nf;
    string filename = "output" + ssn.str() + ".txt";
    ofstream file (filename.c_str());
    int t[nVars];
    if (file.is_open()){
        for (it=joint.begin();it!=joint.end();it++) {
            index = it->first;
            q = it->second;
            getCoordinates(index,t);
            file << Mid(q);
            for (int v=0;v<nVars;v++) file << " " << t[v];
            file << " " << q << " " << Diam(q) << endl;
            qtotal += q;
            if (Diam(q)>maxqwidth) maxqwidth = Diam(q);
        }
        file.close();
    }
    */

    INTERVAL_VECTOR qtotalMC(2+2*nVars);
    for (int i = 1; i <= 2+2*nVars; i++) qtotalMC(i) = INTERVAL(0.0);
    multimap<double,pair<INTERVAL_VECTOR,pair<INTERVAL_VECTOR,unsigned long int> > >::iterator it0;
    for (it0=grayBoxesMC.begin();it0!=grayBoxesMC.end();it0++) {
        p = it0->second;
        x = p.first;
        q = p.second.first;
        qtotalMC += monteCarloIntegrals(x,nMC,q);
    }
    cout << "qtotalMC = " << qtotalMC << endl;
    cout << "L = " << qtotalMC(1) << endl;
    cout << "ErrL = " << qtotalMC(2) << endl;
    cout << "E(1) = " << qtotalMC(3)/qtotalMC(1) << endl;
    cout << "E(2) = " << qtotalMC(4)/qtotalMC(1) << endl;
    cout << "E(3) = " << qtotalMC(5)/qtotalMC(1) << endl;
    cout << "STD(1) = " << Sqrt((qtotalMC(6)/qtotalMC(1))- Sqr(qtotalMC(3)/qtotalMC(1))) << endl;
    cout << "STD(2) = " << Sqrt((qtotalMC(7)/qtotalMC(1))- Sqr(qtotalMC(4)/qtotalMC(1))) << endl;
    cout << "STD(3) = " << Sqrt((qtotalMC(8)/qtotalMC(1))- Sqr(qtotalMC(5)/qtotalMC(1))) << endl;
    if (!firstBlack) cout << "black_hull = " << black_hull << endl;
    else cout << "black_hull = empty " << endl;

    stringstream ssn1; ssn1 << nf;
    string filename1 = "expected" + ssn1.str() + ".txt";
    ofstream file1 (filename1.c_str());
    if (file1.is_open()){
        file1 << "nMC = " << nMC << endl;
        file1 << "BoxesMC.size() = " << grayBoxesMC.size() << endl;
        file1 << "qtotalMC = " << qtotalMC << endl;
        file1 << "L = " << qtotalMC(1) << endl;
        file1 << "ErrL = " << qtotalMC(2) << endl;
        file1 << "E(1) = " << qtotalMC(3)/qtotalMC(1) << endl;
        file1 << "E(2) = " << qtotalMC(4)/qtotalMC(1) << endl;
        file1 << "E(3) = " << qtotalMC(5)/qtotalMC(1) << endl;
        file1 << "STD(1) = " << Sqrt((qtotalMC(6)/qtotalMC(1))- Sqr(qtotalMC(3)/qtotalMC(1))) << endl;
        file1 << "STD(2) = " << Sqrt((qtotalMC(7)/qtotalMC(1))- Sqr(qtotalMC(4)/qtotalMC(1))) << endl;
        file1 << "STD(3) = " << Sqrt((qtotalMC(8)/qtotalMC(1))- Sqr(qtotalMC(5)/qtotalMC(1))) << endl;
        if (!firstBlack) file1 << "black_hull = " << black_hull << endl;
        else file1 << "black_hull = empty " << endl;
        qtotalMC = monteCarloIntegrals(x_init,nglobalMC,globalMC);
        file1 << "******************************************" << endl;
        file1 << "globalMC = " << globalMC << endl;
        file1 << "nglobalMC = " << nglobalMC << endl;
        file1 << "qtotalMC = " << qtotalMC << endl;
        file1 << "L = " << qtotalMC(1) << endl;
        file1 << "ErrL = " << qtotalMC(2) << endl;
        file1 << "E(1) = " << qtotalMC(3)/qtotalMC(1) << endl;
        file1 << "E(2) = " << qtotalMC(4)/qtotalMC(1) << endl;
        file1 << "E(3) = " << qtotalMC(5)/qtotalMC(1) << endl;
        file1 << "STD(1) = " << Sqrt((qtotalMC(6)/qtotalMC(1))- Sqr(qtotalMC(3)/qtotalMC(1))) << endl;
        file1 << "STD(2) = " << Sqrt((qtotalMC(7)/qtotalMC(1))- Sqr(qtotalMC(4)/qtotalMC(1))) << endl;
        file1 << "STD(3) = " << Sqrt((qtotalMC(8)/qtotalMC(1))- Sqr(qtotalMC(5)/qtotalMC(1))) << endl;
        file1 << "solutions = " << solutions << endl;
        file1 << "******************************************" << endl;
        file1.close();
    }
    qtotalMC = monteCarloIntegrals(x_init,nglobalMC,globalMC);
    cout << "******************************************" << endl;
    cout << "globalMC = " << globalMC << endl;
    cout << "nglobalMC = " << nglobalMC << endl;
    cout << "qtotalMC = " << qtotalMC << endl;
    cout << "L = " << qtotalMC(1) << endl;
    cout << "ErrL = " << qtotalMC(2) << endl;
    cout << "E(1) = " << qtotalMC(3)/qtotalMC(1) << endl;
    cout << "E(2) = " << qtotalMC(4)/qtotalMC(1) << endl;
    cout << "E(3) = " << qtotalMC(5)/qtotalMC(1) << endl;
    cout << "STD(1) = " << Sqrt((qtotalMC(6)/qtotalMC(1))- Sqr(qtotalMC(3)/qtotalMC(1))) << endl;
    cout << "STD(2) = " << Sqrt((qtotalMC(7)/qtotalMC(1))- Sqr(qtotalMC(4)/qtotalMC(1))) << endl;
    cout << "STD(3) = " << Sqrt((qtotalMC(8)/qtotalMC(1))- Sqr(qtotalMC(5)/qtotalMC(1))) << endl;
    cout << "solutions = " << solutions << endl;
    cout << "******************************************" << endl;
}


void InverseProblem::improveGraphicMax(int nf, int iter)
{
    chronos chrono2;
    chrono2.start();
    bool black;
    INTERVAL_VECTOR x(nVars);
    INTERVAL_VECTOR xx(nVars);
    INTERVAL q, q0, qindex;
    pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> > p;
    pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> > px;
    pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> > pxx;
    pair<INTERVAL,unsigned long int> qx;
    pair<INTERVAL,unsigned long int> qxx;
    unsigned long int index;
    int n;
    bool canonical;
    while (chrono2.duration()<iter) {
        if (grayBoxes.begin()->first<blackBoxes.begin()->first) {
            //cout << "w = " << -grayBoxes.begin()->first << " (gray)" << endl;
            black = false;
            p = grayBoxes.begin()->second;
            grayBoxes.erase(grayBoxes.begin());
        }
        else {
            //cout << "w = " << -blackBoxes.begin()->first << " (black)" << endl;
            black = true;
            p = blackBoxes.begin()->second;
            blackBoxes.erase(blackBoxes.begin());
        }
        x = p.first;
        q0 = p.second.first;
        index = p.second.second;
        qindex = joint[index];
        if (Sup(q0)>Inf(qindex)) {
            xx=BisectIntervalVector(x, canonical);
            if (canonical) {

                cout << "canonical box:" << endl;
                if (black) cout << "black" << endl;
                else cout << "gray" << endl;
                cout << "x = " << x << endl;
                cout << "xx = " << xx << endl;

                q = reduceX(x,canonical);
                qx = pair<INTERVAL,unsigned long int>(q,index);
                px = pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> >(x,qx);
                //grayCanonical.insert(pair<double,pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> > >(-Diam(q),px));
                blackBoxes.insert(pair<double,pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> > >(-Diam(q),px));
                if (Sup(q)>Inf(qindex)) qindex = INTERVAL(Sup(q),Sup(qindex));
                //cin.get();
            }
            else {
                if (black) {
                    //q = d->quadratureEval(x);
                    q = reduceX(x,canonical);
                    //q = d->functionEval(x,0);
                    if (Sup(q)>Inf(qindex)) {
                        if (Inf(q)<0.0) q = INTERVAL(0.0,Sup(q));
                        qx = pair<INTERVAL,unsigned long int>(q,index);
                        px = pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> >(x,qx);
                        blackBoxes.insert(pair<double,pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> > >(-Diam(q),px));
                        if (canonical) qindex = INTERVAL(Sup(q),Sup(qindex));
                        else if (Inf(q)>Inf(qindex)) qindex = INTERVAL(Inf(q),Sup(qindex));
                    }
                    //q = d->quadratureEval(xx);
                    q = reduceX(xx,canonical);
                    //q = d->functionEval(xx,0);
                    if (Sup(q)>Inf(qindex)) {
                        if (Inf(q)<0.0) q = INTERVAL(0.0,Sup(q));
                        qxx = pair<INTERVAL,unsigned long int>(q,index);
                        pxx = pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> >(xx,qxx);
                        blackBoxes.insert(pair<double,pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> > >(-Diam(q),pxx));
                        if (canonical) qindex = INTERVAL(Sup(q),Sup(qindex));
                        else if (Inf(q)>Inf(qindex)) qindex = INTERVAL(Inf(q),Sup(qindex));
                    }
                }
                else {
                    if (prune(x,n)) {
                        if (n==0) {
                            black_hull = Hull(black_hull,x);
                            //q = d->quadratureEval(x);
                            q = reduceX(x,canonical);
                            //q = d->functionEval(x,0);
                            if (Sup(q)>Inf(qindex)) {
                                if (Inf(q)<0.0) q = INTERVAL(0.0,Sup(q));
                                qx = pair<INTERVAL,unsigned long int>(q,index);
                                px = pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> >(x,qx);
                                blackBoxes.insert(pair<double,pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> > >(-Diam(q),px));                            if (Inf(q)>Inf(qindex)) qindex = INTERVAL(Inf(q),Sup(qindex));
                                if (canonical) qindex = INTERVAL(Sup(q),Sup(qindex));
                                else if (Inf(q)>Inf(qindex)) qindex = INTERVAL(Inf(q),Sup(qindex));
                            }
                        }
                        else {
                            //q = d->quadratureEval(x,1);
                            q = reduceGray(x);
                            //q = d->functionEval(x,0);
                            if (Sup(q)>Inf(qindex)) {
                                //q = INTERVAL(0.0,Sup(q));
                                qx = pair<INTERVAL,unsigned long int>(q,index);
                                px = pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> >(x,qx);
                                grayBoxes.insert(pair<double,pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> > >(-Diam(q),px));
                            }
                        }
                    }
                    if (prune(xx,n)) {
                        if (n==0) {
                            black_hull = Hull(black_hull,xx);
                            //q = d->quadratureEval(xx);
                            q = reduceX(xx,canonical);
                            //q = d->functionEval(xx,0);
                            if (Sup(q)>Inf(qindex)) {
                                if (Inf(q)<0.0) q = INTERVAL(0.0,Sup(q));
                                qxx = pair<INTERVAL,unsigned long int>(q,index);
                                pxx = pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> >(xx,qxx);
                                blackBoxes.insert(pair<double,pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> > >(-Diam(q),pxx));
                                if (canonical) qindex = INTERVAL(Sup(q),Sup(qindex));
                                else if (Inf(q)>Inf(qindex)) qindex = INTERVAL(Inf(q),Sup(qindex));
                            }
                        }
                        else {
                            //q = d->quadratureEval(xx,1);
                            q = reduceGray(xx);
                            //q = d->functionEval(xx,0);
                            if (Sup(q)>Inf(qindex)) {
                                //q = INTERVAL(0.0,Sup(q));
                                qxx = pair<INTERVAL,unsigned long int>(q,index);
                                pxx = pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> >(xx,qxx);
                                grayBoxes.insert(pair<double,pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> > >(-Diam(q),pxx));
                            }
                        }
                    }
                }
                joint[index] = qindex;
            }
        }
    }
    qtotal = INTERVAL(0.0);
    maxqwidth = 0.0;

    map<unsigned long int,INTERVAL>::iterator it;
    for (it=joint.begin();it!=joint.end();it++) {
        //joint[it->first]=INTERVAL(0.0);
        it->second = INTERVAL(0.0);
    }

    multimap<double,pair<INTERVAL_VECTOR,pair<INTERVAL,unsigned long int> > >::iterator it0;
    for (it0=blackBoxes.begin();it0!=blackBoxes.end();it0++) {
        p = it0->second;
        x = p.first;
        q = p.second.first;
        index = p.second.second;
        qindex = joint[index];
        //if (Sup(q)<=Inf(qindex)) blackBoxes.erase(it0);
        //else {
            if (Sup(q)>Sup(qindex)) qindex = INTERVAL(Inf(qindex),Sup(q));
            if (Inf(q)>Inf(qindex)) qindex = INTERVAL(Inf(q),Sup(qindex));
            joint[index] = qindex;
        //}
    }
    for (it0=grayBoxes.begin();it0!=grayBoxes.end();it0++) {
        p = it0->second;
        x = p.first;
        q = p.second.first;
        index = p.second.second;
        qindex = joint[index];
        //if (Sup(q)<=Inf(qindex)) grayBoxes.erase(it0);
        //else {
            if (Sup(q)>Sup(qindex)) {
                qindex = INTERVAL(Inf(qindex),Sup(q));
                joint[index] = qindex;
            }
        //}
    }
    for (it0=grayCanonical.begin();it0!=grayCanonical.end();it0++) {
        p = it0->second;
        x = p.first;
        q = p.second.first;
        index = p.second.second;
        qindex = joint[index];
        //if (Sup(q)<=Inf(qindex)) grayBoxes.erase(it0);
        //else {
            if (Sup(q)>Sup(qindex)) {
                qindex = INTERVAL(Inf(qindex),Sup(q));
                joint[index] = qindex;
            }
        //}
    }




    cout << "joint.size() = " << joint.size() << endl;
    cout << "grayCanonical.size() = " << grayCanonical.size() << endl;
    cout << "grayBoxes.size() = " << grayBoxes.size() << endl;
    cout << "blackBoxes.size() = " << blackBoxes.size() << endl;
    cout << "first gray -> w = " << -grayBoxes.begin()->first << " q = " << grayBoxes.begin()->second.second.first << " x = " << grayBoxes.begin()->second.first << endl;
    cout << "first black -> w = " << -blackBoxes.begin()->first << " q = " << blackBoxes.begin()->second.second.first << " x = " << blackBoxes.begin()->second.first << endl;


    stringstream ssn; ssn << nf;
    string filename = "output" + ssn.str() + ".txt";
    ofstream file (filename.c_str());
    int t[nVars];
    if (file.is_open()){
        for (it=joint.begin();it!=joint.end();it++) {
            index = it->first;
            q = it->second;
            getCoordinates(index,t);
            file << Mid(q);
            for (int v=0;v<nVars;v++) file << " " << t[v];
            file << " " << q << " " << Diam(q) << endl;
            qtotal += q;
            if (Diam(q)>maxqwidth) maxqwidth = Diam(q);
        }
        file.close();
    }
}

INTERVAL InverseProblem::reduceX(INTERVAL_VECTOR & x, bool & canonical)
{
    INTERVAL_VECTOR x0 = x;
    INTERVAL df[nVars];
    bool reduced = false;
    canonical = true;
    INTERVAL q = d->getD(x,df);
    for (int i=0;i<nVars;i++) {
        if (Inf(df[i])>=0.0) {
            x(i+1) = Hull(Sup(x(i+1))) & x0(i+1);
        }
        else if (Sup(df[i])<=0.0) {
            x(i+1) = Hull(Inf(x(i+1))) & x0(i+1);
        }
        else canonical = false;
        if ((Inf(x(i+1))>Inf(x0(i+1)))||(Sup(x(i+1))<Sup(x0(i+1)))) reduced = true;
    }

    if (reduced) {
        /*
        cout << "before reduceX:" << endl;
        cout << "   x = " << x0 << endl;
        cout << "   q = " << q << endl;
        for (int i=0;i<nVars;i++) cout << "   df[" << i << "] = " << df[i] << endl;
        cout << "after reduceX:" << endl;
        cout << "   x = " << x << endl;
        */
        q = reduceX(x, canonical);
        /*
        cout << "   q = " << q << endl;
        cin.get();
        */
    }

    return q;
}

INTERVAL InverseProblem::reduceGray(INTERVAL_VECTOR & x)
{
    INTERVAL_VECTOR x0 = x;
    INTERVAL df[nVars];
    bool reduced = false;
    INTERVAL q = d->getD(x0,df);
    for (int i=0;i<nVars;i++) {
        if (Inf(df[i])>=0.0) {
            x0(i+1) = Hull(Sup(x0(i+1))) & x(i+1);
        }
        else if (Sup(df[i])<=0.0) {
            x0(i+1) = Hull(Inf(x0(i+1))) & x(i+1);
        }
        if ((Inf(x0(i+1))>Inf(x(i+1)))||(Sup(x0(i+1))<Sup(x(i+1)))) reduced = true;
    }

    if (reduced) {
        /*
        cout << "before reduceX:" << endl;
        cout << "   x = " << x0 << endl;
        cout << "   q = " << q << endl;
        for (int i=0;i<nVars;i++) cout << "   df[" << i << "] = " << df[i] << endl;
        cout << "after reduceX:" << endl;
        cout << "   x = " << x << endl;
        */
        q = reduceGray(x0);
        /*
        cout << "   q = " << q << endl;
        cin.get();
        */
    }

    return q;
}

void InverseProblem::solve()
{
    chronos chrono1;
    chrono1.start();
    int v = 1;
    searchBlack(v);
    cout << "time searchBlack -> " << chrono1.duration() << endl;
    splitUB(v);
    cout << "time splitUB -> " << chrono1.duration() << endl;
    stepProcess(v);
    cout << "time stepProcess -> " << chrono1.duration() << endl;
    v = 2;
    splitUB(v);
    cout << "time splitUB -> " << chrono1.duration() << endl;
    stepProcess(v);
    cout << "time stepProcess -> " << chrono1.duration() << endl;
    v = 3;
    splitUB(v);
    cout << "time splitUB -> " << chrono1.duration() << endl;
    stepProcess(v);
    cout << "time stepProcess -> " << chrono1.duration() << endl;
    INTERVAL_VECTOR x(3);
    INTERVAL q;
    qtotal=INTERVAL(0.0);
    maxqwidth = 0.0;
    double t = 0.0;
    int n, nBlack=0, nGray=0, nDelete=0;


    cout << "joint.size() = " << joint.size() << endl;
    cout << "grayBoxes.size() = " << grayBoxes.size() << endl;
    cout << "blackBoxes.size() = " << blackBoxes.size() << endl;

    string filename = "output0.txt";
    ofstream file (filename.c_str());
    unsigned long int index = 0;
    INTERVAL I0, I1, I2, I3, I4, I5;
    bool canonical;
    if (file.is_open()){
        for (int i=0;i<nDiv;i++) {
            I0 = INTERVAL(lbox[0]+i*eps[0]);
            I1 = INTERVAL(lbox[0]+(i+1)*eps[0]);
            //x(1) = INTERVAL(lbox[0]+i*eps[0],lbox[0]+(i+1)*eps[0]);
            for (int j=0;j<nDiv;j++) {
                I2 = INTERVAL(lbox[1]+j*eps[1]);
                I3 = INTERVAL(lbox[1]+(j+1)*eps[1]);
                //x(2) = INTERVAL(lbox[1]+j*eps[1],lbox[1]+(j+1)*eps[1]);
                for (int k=0;k<nDiv;k++) {
                    I4 = INTERVAL(lbox[2]+k*eps[2]);
                    I5 = INTERVAL(lbox[2]+(k+1)*eps[2]);
                    x(1) = INTERVAL(Inf(I0),Sup(I1));
                    x(2) = INTERVAL(Inf(I2),Sup(I3));
                    x(3) = INTERVAL(Inf(I4),Sup(I5));
                    //x(3) = INTERVAL(lbox[2]+k*eps[2],lbox[2]+(k+1)*eps[2]);
                    if (prune(x,n)) {
                        //cout << "<" << i << "," << j << "," << k << "> -> ";
                        if (n==0) {
                    //cout << "black" << endl;
                            nBlack++;
                            //t = chrono1.duration();
                            q = d->quadratureEval(x);
                    //cout << "q = " << q << endl;
                            //q = reduceX(x,canonical);
                            //q = d->functionEval(x,0);
                            //q = integrate(x,total_wid_max);
                            if (Inf(q)<0.0) q = INTERVAL(0.0,Sup(q));
                            pair<INTERVAL, unsigned long int> qx = pair<INTERVAL, unsigned long int>(q,index);
                            pair<INTERVAL_VECTOR,pair<INTERVAL, unsigned long int> > px = pair<INTERVAL_VECTOR,pair<INTERVAL, unsigned long int> >(x,qx);
                            blackBoxes.insert(pair<double,pair<INTERVAL_VECTOR,pair<INTERVAL, unsigned long int> > >(-Diam(q),px));
                            file << Mid(q) << " " << i << " " << j << " " << k << " " << q << " " << Diam(q) << endl;
                            //cout << chrono1.duration() << " -> nBlack = " << nBlack << " (" << chrono1.duration()-t << ")" << endl;
                        }
                        else {
                    //cout << "gray" << endl;
                            nGray++;
                            //t = chrono1.duration();
                            q = d->quadratureEval(x,1);

                    //cout << "q = " << q << endl;
                            //q = reduceGray(x);
                            //q = d->functionEval(x,0);
                            q = INTERVAL(0.0,Sup(q));
                            pair<INTERVAL, unsigned long int> qx = pair<INTERVAL, unsigned long int>(q,index);
                            pair<INTERVAL_VECTOR,pair<INTERVAL, unsigned long int> > px = pair<INTERVAL_VECTOR,pair<INTERVAL, unsigned long int> >(x,qx);
                            grayBoxes.insert(pair<double,pair<INTERVAL_VECTOR,pair<INTERVAL, unsigned long int> > >(-Diam(q),px));
                            file << Mid(q) << " " << i << " " << j << " " << k << " " << q << " " << Diam(q) << endl;
                            //cout << chrono1.duration() << " -> nGray = " << nGray << " (" << chrono1.duration()-t << ")" << endl;
                        }
                        qtotal += q;
                        if (Diam(q)>maxqwidth) maxqwidth = Diam(q);
                        joint.insert(pair<unsigned long int,INTERVAL>(index,q));
                        //cin.get();
                    }
                    else nDelete++;
                    index++;
                }
            }
        }
        file.close();
    }
    cout << chrono1.duration() << " -> nGray = " << nGray << endl;
    cout << chrono1.duration() << " -> nBlack = " << nBlack << endl;
    cout << chrono1.duration() << " -> nDelete = " << nDelete << endl;


    cout << "joint.size() = " << joint.size() << endl;
    cout << "grayBoxes.size() = " << grayBoxes.size() << endl;
    cout << "blackBoxes.size() = " << blackBoxes.size() << endl;
    cout << "first gray -> w = " << -grayBoxes.begin()->first << " q = " << grayBoxes.begin()->second.second.first << " x = " << grayBoxes.begin()->second.first << endl;
    cout << "first black -> w = " << -blackBoxes.begin()->first << " q = " << blackBoxes.begin()->second.second.first << " x = " << blackBoxes.begin()->second.first << endl;

    graphicsOutputN(0);
    string fname = "timeLog.txt";
    ofstream f2 (fname.c_str());
    if (f2.is_open()) {
        cout << "graphicsOutput 0 -> time = " << chrono1.duration() << "s qtotal = " << qtotal << " wid = " << Diam(qtotal) << " maxqwidth = " << maxqwidth << endl;
        f2 << "graphicsOutput 0 -> time = " << chrono1.duration() << "s qtotal = " << qtotal << " wid = " << Diam(qtotal) << " maxqwidth = " << maxqwidth << endl;
        int period = 600;
        int sec = period - (int)chrono1.duration() % period;
        int i = (int)chrono1.duration() / period;
        while (true) {
            i++;
            improveGraphic(i,sec);
            //improveGraphicMax(i,sec);
            graphicsOutputN(i);
            cout << "graphicsOutput " << i << " -> time = " << chrono1.duration() << "s qtotal = " << qtotal << " wid = " << Diam(qtotal) << " maxqwidth = " << maxqwidth << endl;
            f2 << "graphicsOutput " << i << " -> time = " << chrono1.duration() << "s qtotal = " << qtotal << " wid = " << Diam(qtotal) << " maxqwidth = " << maxqwidth << endl;
            sec = period - (int)chrono1.duration() % period;
        }
        f2.close();
    }
}

void InverseProblem::solveMC()
{
    chronos chrono1;
    chrono1.start();
    int v = 1;
    searchBlack(v);
    cout << "time searchBlack -> " << chrono1.duration() << endl;
    splitUB(v);
    cout << "time splitUB -> " << chrono1.duration() << endl;
    stepProcess(v);
    cout << "time stepProcess -> " << chrono1.duration() << endl;
    v = 2;
    splitUB(v);
    cout << "time splitUB -> " << chrono1.duration() << endl;
    stepProcess(v);
    cout << "time stepProcess -> " << chrono1.duration() << endl;
    v = 3;
    splitUB(v);
    cout << "time splitUB -> " << chrono1.duration() << endl;
    stepProcess(v);
    cout << "time stepProcess -> " << chrono1.duration() << endl;
    INTERVAL_VECTOR x(nVars);
    INTERVAL_VECTOR q(2+2*nVars);
    qtotal=INTERVAL(0.0);
    INTERVAL_VECTOR qtotalMC(2+2*nVars);
    for (int i = 1; i <= 2+2*nVars; i++) qtotalMC(i) = INTERVAL(0.0);
    maxqwidth = 0.0;
    double t = 0.0;
    int n, nBlack=0, nGray=0, nDelete=0;


    cout << "joint.size() = " << joint.size() << endl;
    cout << "grayBoxesMC.size() = " << grayBoxesMC.size() << endl;
    cout << "blackBoxesMC.size() = " << blackBoxesMC.size() << endl;

    string filename = "output0.txt";
    ofstream file (filename.c_str());
    unsigned long int index = 0;
    INTERVAL I0, I1, I2, I3, I4, I5;
    bool canonical;
    double std;
    INTERVAL mean;
    INTERVAL qMC;
    if (file.is_open()){
        for (int i=0;i<nDiv;i++) {
            I0 = INTERVAL(lbox[0]+i*eps[0]);
            I1 = INTERVAL(lbox[0]+(i+1)*eps[0]);
            for (int j=0;j<nDiv;j++) {
                I2 = INTERVAL(lbox[1]+j*eps[1]);
                I3 = INTERVAL(lbox[1]+(j+1)*eps[1]);
                for (int k=0;k<nDiv;k++) {
                    I4 = INTERVAL(lbox[2]+k*eps[2]);
                    I5 = INTERVAL(lbox[2]+(k+1)*eps[2]);
                    x(1) = INTERVAL(Inf(I0),Sup(I1));
                    x(2) = INTERVAL(Inf(I2),Sup(I3));
                    x(3) = INTERVAL(Inf(I4),Sup(I5));
                    if (prune(x,n)) {
                        if (n==0) {
                            //cout << "black" << endl;
                            nBlack++;
                            //q = d->quadratureEval(x);
                            q = monteCarlo(x,nMC);
                            std = monteCarloSTD(x,nMC,q);
                            mean = monteCarloMean(x,nMC,q);
                            pair<INTERVAL_VECTOR, unsigned long int> qx = pair<INTERVAL_VECTOR, unsigned long int>(q,index);
                            pair<INTERVAL_VECTOR,pair<INTERVAL_VECTOR, unsigned long int> > px = pair<INTERVAL_VECTOR,pair<INTERVAL_VECTOR, unsigned long int> >(x,qx);
                            blackBoxesMC.insert(pair<double,pair<INTERVAL_VECTOR,pair<INTERVAL_VECTOR, unsigned long int> > >(-std,px));
                            qMC = INTERVAL(Inf(mean)-std,Sup(mean)+std);
                            file << Mid(mean) << " " << i << " " << j << " " << k << " " << qMC << " " << Diam(qMC) << endl;
                            //cout << chrono1.duration() << " -> nBlack = " << nBlack << " (" << chrono1.duration()-t << ")" << endl;
                        }
                        else {
                            //cout << "gray" << endl;
                            nGray++;
                            //q = d->quadratureEval(x,1);
                            //q = INTERVAL(0.0,Sup(q));
                            q = monteCarlo(x,nMC);
                            std = monteCarloSTD(x,nMC,q);
                            mean = monteCarloMean(x,nMC,q);
                            pair<INTERVAL_VECTOR, unsigned long int> qx = pair<INTERVAL_VECTOR, unsigned long int>(q,index);
                            pair<INTERVAL_VECTOR,pair<INTERVAL_VECTOR, unsigned long int> > px = pair<INTERVAL_VECTOR,pair<INTERVAL_VECTOR, unsigned long int> >(x,qx);
                            grayBoxesMC.insert(pair<double,pair<INTERVAL_VECTOR,pair<INTERVAL_VECTOR, unsigned long int> > >(-std,px));
                            qMC = INTERVAL(Inf(mean)-std,Sup(mean)+std);
                            file << Mid(qMC) << " " << i << " " << j << " " << k << " " << qMC << " " << Diam(qMC) << endl;
                            //cout << chrono1.duration() << " -> nGray = " << nGray << " (" << chrono1.duration()-t << ")" << endl;
                        }
                        qtotal += mean;

                        qtotalMC += monteCarloIntegrals(x,nMC,q);
                        if (Diam(qMC)>maxqwidth) maxqwidth = Diam(qMC);
                        joint.insert(pair<unsigned long int,INTERVAL>(index,mean));
                        //cin.get();
                    }
                    else nDelete++;
                    index++;
                }
            }
        }
        file.close();
    }
    cout << chrono1.duration() << " -> nGray = " << nGray << endl;
    cout << chrono1.duration() << " -> nBlack = " << nBlack << endl;
    cout << chrono1.duration() << " -> nDelete = " << nDelete << endl;
    cout << "qtotalMC = " << qtotalMC << endl;
    cout << "L = " << qtotalMC(1) << endl;
    cout << "ErrL = " << qtotalMC(2) << endl;
    cout << "E(1) = " << qtotalMC(3)/qtotalMC(1) << endl;
    cout << "E(2) = " << qtotalMC(4)/qtotalMC(1) << endl;
    cout << "E(3) = " << qtotalMC(5)/qtotalMC(1) << endl;
    cout << "STD(1) = " << Sqrt((qtotalMC(6)/qtotalMC(1))- Sqr(qtotalMC(3)/qtotalMC(1))) << endl;
    cout << "STD(2) = " << Sqrt((qtotalMC(7)/qtotalMC(1))- Sqr(qtotalMC(4)/qtotalMC(1))) << endl;
    cout << "STD(3) = " << Sqrt((qtotalMC(8)/qtotalMC(1))- Sqr(qtotalMC(5)/qtotalMC(1))) << endl;


    cout << "joint.size() = " << joint.size() << endl;
    cout << "grayBoxesMC.size() = " << grayBoxesMC.size() << endl;
    cout << "blackBoxesMC.size() = " << blackBoxesMC.size() << endl;
    cout << "first gray -> w = " << -grayBoxesMC.begin()->first << " q = " << grayBoxesMC.begin()->second.second.first << " x = " << grayBoxesMC.begin()->second.first << endl;
    cout << "first black -> w = " << -blackBoxesMC.begin()->first << " q = " << blackBoxesMC.begin()->second.second.first << " x = " << blackBoxesMC.begin()->second.first << endl;

    graphicsOutputN(0);
    string fname = "timeLog.txt";
    ofstream f2 (fname.c_str());
    if (f2.is_open()) {
        cout << "graphicsOutput 0 -> time = " << chrono1.duration() << "s qtotal = " << qtotal << " wid = " << Diam(qtotal) << " maxqwidth = " << maxqwidth << endl;
        f2 << "graphicsOutput 0 -> time = " << chrono1.duration() << "s qtotal = " << qtotal << " wid = " << Diam(qtotal) << " maxqwidth = " << maxqwidth << endl;
        f2 << "L = " << qtotalMC(1) << endl;
        f2 << "ErrL = " << qtotalMC(2) << endl;
        f2 << "E(1) = " << qtotalMC(3)/qtotalMC(1) << endl;
        f2 << "E(2) = " << qtotalMC(4)/qtotalMC(1) << endl;
        f2 << "E(3) = " << qtotalMC(5)/qtotalMC(1) << endl;
        f2 << "STD(1) = " << Sqrt((qtotalMC(6)/qtotalMC(1))- Sqr(qtotalMC(3)/qtotalMC(1))) << endl;
        f2 << "STD(2) = " << Sqrt((qtotalMC(7)/qtotalMC(1))- Sqr(qtotalMC(4)/qtotalMC(1))) << endl;
        f2 << "STD(3) = " << Sqrt((qtotalMC(8)/qtotalMC(1))- Sqr(qtotalMC(5)/qtotalMC(1))) << endl;


        /*
        system("PAUSE");
        int period = 600;
        int sec = period - (int)chrono1.duration() % period;
        int i = (int)chrono1.duration() / period;
        while (true) {
            i++;
            improveGraphic(i,sec);
            //improveGraphicMax(i,sec);
            graphicsOutputN(i);
            cout << "graphicsOutput " << i << " -> time = " << chrono1.duration() << "s qtotal = " << qtotal << " wid = " << Diam(qtotal) << " maxqwidth = " << maxqwidth << endl;
            f2 << "graphicsOutput " << i << " -> time = " << chrono1.duration() << "s qtotal = " << qtotal << " wid = " << Diam(qtotal) << " maxqwidth = " << maxqwidth << endl;
            sec = period - (int)chrono1.duration() % period;
        }
        */
        f2.close();
    }
}


void InverseProblem::solveMC0()
{
    chronos chrono1;
    chrono1.start();

    INTERVAL_VECTOR x(nVars);
    INTERVAL_VECTOR q(2+2*nVars);
    INTERVAL_VECTOR qtotalMC(2+2*nVars);
    for (int i = 1; i <= 2+2*nVars; i++) qtotalMC(i) = INTERVAL(0.0);
    globalMC=INTERVAL_VECTOR(2+2*nVars);
    for (int i = 1; i <= 2+2*nVars; i++) globalMC(i) = INTERVAL(0.0);
    int nBoxes=0;
    solutions = 0;
    cout << "grayBoxesMC.size() = " << grayBoxesMC.size() << endl;
    unsigned long int index = 0;
    double std;
    INTERVAL mean;
    INTERVAL qMC;

    x = x_init;
    /*
    x(1) = INTERVAL(0.01,33.0);
    x(2) = INTERVAL(0.0,2.5);
    x(3) = INTERVAL(0.0,0.09);
    */
    //cout << "gray" << endl;
    nBoxes++;
    q = monteCarloHull(x,nMC);
    std = monteCarloSTD(x,nMC,q);
    mean = monteCarloMean(x,nMC,q);
    pair<INTERVAL_VECTOR, unsigned long int> qx = pair<INTERVAL_VECTOR, unsigned long int>(q,index);
    pair<INTERVAL_VECTOR,pair<INTERVAL_VECTOR, unsigned long int> > px = pair<INTERVAL_VECTOR,pair<INTERVAL_VECTOR, unsigned long int> >(x,qx);
    grayBoxesMC.insert(pair<double,pair<INTERVAL_VECTOR,pair<INTERVAL_VECTOR, unsigned long int> > >(-std,px));
    qtotalMC = monteCarloIntegrals(x,nMC,q);
    globalMC  = q;
    nglobalMC = nMC;

    cout << chrono1.duration() << " -> nBoxes = " << nBoxes << endl;
    cout << "qtotalMC = " << qtotalMC << endl;
    cout << "L = " << qtotalMC(1) << endl;
    cout << "ErrL = " << qtotalMC(2) << endl;
    cout << "E(1) = " << qtotalMC(3)/qtotalMC(1) << endl;
    cout << "E(2) = " << qtotalMC(4)/qtotalMC(1) << endl;
    cout << "E(3) = " << qtotalMC(5)/qtotalMC(1) << endl;
    cout << "STD(1) = " << Sqrt((qtotalMC(6)/qtotalMC(1))- Sqr(qtotalMC(3)/qtotalMC(1))) << endl;
    cout << "STD(2) = " << Sqrt((qtotalMC(7)/qtotalMC(1))- Sqr(qtotalMC(4)/qtotalMC(1))) << endl;
    cout << "STD(3) = " << Sqrt((qtotalMC(8)/qtotalMC(1))- Sqr(qtotalMC(5)/qtotalMC(1))) << endl;

    qtotalMC = monteCarloIntegrals(x_init,nglobalMC,globalMC);
    cout << "******************************************" << endl;
    cout << "globalMC = " << globalMC << endl;
    cout << "nglobalMC = " << nglobalMC << endl;
    cout << "qtotalMC = " << qtotalMC << endl;
    cout << "L = " << qtotalMC(1) << endl;
    cout << "ErrL = " << qtotalMC(2) << endl;
    cout << "E(1) = " << qtotalMC(3)/qtotalMC(1) << endl;
    cout << "E(2) = " << qtotalMC(4)/qtotalMC(1) << endl;
    cout << "E(3) = " << qtotalMC(5)/qtotalMC(1) << endl;
    cout << "STD(1) = " << Sqrt((qtotalMC(6)/qtotalMC(1))- Sqr(qtotalMC(3)/qtotalMC(1))) << endl;
    cout << "STD(2) = " << Sqrt((qtotalMC(7)/qtotalMC(1))- Sqr(qtotalMC(4)/qtotalMC(1))) << endl;
    cout << "STD(3) = " << Sqrt((qtotalMC(8)/qtotalMC(1))- Sqr(qtotalMC(5)/qtotalMC(1))) << endl;
    cout << "solutions = " << solutions << endl;
    cout << "******************************************" << endl;

    cout << "boxesMC.size() = " << grayBoxesMC.size() << endl;
    cout << "first box -> w = " << -grayBoxesMC.begin()->first << " q = " << grayBoxesMC.begin()->second.second.first << " x = " << grayBoxesMC.begin()->second.first << endl;
    if (!firstBlack) cout << "black_hull = " << black_hull << endl;
    else cout << "black_hull = empty " << endl;
    string fname = "timeLog.txt";
    ofstream f2 (fname.c_str());
    if (f2.is_open()) {
        cout << "graphicsOutput 0 -> time = " << chrono1.duration() << endl;
        f2 << "graphicsOutput 0 -> time = " << chrono1.duration() << endl;
        f2 << "L = " << qtotalMC(1) << endl;
        f2 << "ErrL = " << qtotalMC(2) << endl;
        f2 << "E(1) = " << qtotalMC(3)/qtotalMC(1) << endl;
        f2 << "E(2) = " << qtotalMC(4)/qtotalMC(1) << endl;
        f2 << "E(3) = " << qtotalMC(5)/qtotalMC(1) << endl;
        f2 << "STD(1) = " << Sqrt((qtotalMC(6)/qtotalMC(1))- Sqr(qtotalMC(3)/qtotalMC(1))) << endl;
        f2 << "STD(2) = " << Sqrt((qtotalMC(7)/qtotalMC(1))- Sqr(qtotalMC(4)/qtotalMC(1))) << endl;
        f2 << "STD(3) = " << Sqrt((qtotalMC(8)/qtotalMC(1))- Sqr(qtotalMC(5)/qtotalMC(1))) << endl;
        if (!firstBlack) f2 << "black_hull = " << black_hull << endl;
        else f2 << "black_hull = empty " << endl;


        //system("PAUSE");
        int period = 600;
        //int period = 180;
        int sec = period - (int)chrono1.duration() % period;
        int i = (int)chrono1.duration() / period;
        while (true) {
            i++;
            improveGraphicMC(i,sec);
            //graphicsOutputN(i);
            cout << "graphicsOutput " << i << " -> time = " << chrono1.duration() << endl;
            f2 << "graphicsOutput " << i << " -> time = " << chrono1.duration() << endl;
            sec = period - (int)chrono1.duration() % period;
        }

        f2.close();
    }
}
/*
void InverseProblem::solve(int v)
{
    string filename = "outputG.txt";
    ofstream file (filename.c_str());
    if (file.is_open()){
        chronos chrono1;
        chrono1.start();
        searchBlack(v);
        splitUB(v);
        stepProcess(v);
        splitStep(v);
        int i = 0;
        cout << "time[" << i << "] -> " << chrono1.duration() << endl;
        file << "time[" << i << "] -> " << chrono1.duration() << endl;
        int sec = 60 - (int)chrono1.duration() % 60;
        double maxge = 1000;
        while (maxge>10) {
            i++;
            maxge = improveGraphic(v, sec, file);
            cout << "time[" << i << "] -> " << chrono1.duration() << endl;
            file << "time[" << i << "] -> " << chrono1.duration() << endl;
            sec = 60 - (int)chrono1.duration() % 60;
        }
        file.close();
    }
}
*/
/*
bool InverseProblem::prune(INTERVAL_VECTOR & x)
{
    d->getT(x,nObs,maxObs,obsIndex,fwr);
    //cout << "x = " << x << endl;
    for (int i=0;i<nObs;i++) {
        //
        cout << "obs[" << i << "] = " << obs[i] << endl;
        cout << "fwr[" << i << "] = " << fwr[i] << endl;
        cout << "Inf(obs[i]) = " << Inf(obs[i]) << endl;
        cout << "Sup(fwr[i]) = " << Sup(fwr[i]) << endl;
        cout << "(Inf(obs[i])>Sup(fwr[i])) = " << (Inf(obs[i])>Sup(fwr[i])) << endl;
        cout << "Sup(obs[i]) = " << Sup(obs[i]) << endl;
        cout << "Inf(fwr[i]) = " << Inf(fwr[i]) << endl;
        cout << "(Sup(obs[i])<Inf(fwr[i])) = " << (Sup(obs[i])<Inf(fwr[i])) << endl;
        //
        if ((Inf(obs[i])>Sup(fwr[i])) || (Sup(obs[i])<Inf(fwr[i]))) return false;
    }
    return true;
}
*/
/*
bool InverseProblem::split(INTERVAL_VECTOR x, int lb[], int w[],int & v, double & a, double & b, double & c, int & wl, int & wr)
{
    int best = 0, wbest = w[0];
    for (int i=1;i<Dimension(x);i++)
        if (w[i]>wbest) {
            best = i;
            wbest = w[i];
        }
    if (wbest>1) {
        v = best+1;
        a = Inf(x(v));
        b = Sup(x(v));
        wl = wbest / 2;
        wr = wbest - wl;
        c = lbox[best]+eps[best]*(lb[best]+wl);
        return true;
    }
    return false;
}
*/
/*
void InverseProblem::search(INTERVAL_VECTOR & x, int lb[], int w[], double total_wid_max, ofstream & file)
{
	int v, wl, wr;
	double a, b, c;
	//bool canonical = false;
	//cout << "> Before prune:" << endl;
	//cout << x << endl;
	if (prune(x)) {
		//cout << "> Ok prune!" << endl;
		//cin.get();
		if (split(x,lb,w,v,a,b,c,wl,wr)) {
			//cout << "> Split x" << v << " left " << c << ":" << endl;
			x(v) = INTERVAL(a,c);
			w[v-1] = wl;
			search(x,lb,w,total_wid_max,file);
			//cout << "> Split x" << v << " right " << c << ":" << endl;
			x(v) = INTERVAL(c,b);
			w[v-1] = wr;
			lb[v-1] += wl;
			search(x,lb,w,total_wid_max,file);
			x(v) = INTERVAL(a,b);
			w[v-1] = wl+wr;
			lb[v-1] -= wl;
		}
		else {
            INTERVAL q=integrate(x,total_wid_max);
            int value = 10000*Mid(q);
            if (value>0) {
                cout << "> Canonical Solution:" << endl;
                cout << "x = " << x << endl;
                cout << "lb =";
                for (int i = 0; i < Dimension(x); i++) cout << " " << lb[i]; cout << endl;
                cout << "q = " << q << endl;
                //q = INTERVAL(max(Inf(q),0.0),Sup(q));
                //cout << "q = " << q << endl;
                cout << "value = " << value << endl;
                file << value ;
                for (int i = 0; i < Dimension(x); i++) file << " " << lb[i]; file << endl;
                //cin.get();
            }
		}
	}
	//else cout << "> Fail prune" << endl;
}
*/

INTERVAL_VECTOR InverseProblem::BisectIntervalVector(INTERVAL_VECTOR & X1, int & kmax, INTERVAL & Xkmax)
{
	INTERVAL_VECTOR X2=X1;
	kmax=Dimension(X1);
	double dmax=Diam(X1(kmax));
	for(int k=kmax-1; k>=1; k--)
		if(Diam(X1(k))>dmax)
		{
			kmax=k;
			dmax=Diam(X1(k));
		}
    Xkmax = X1(kmax);
    double a = Inf(X1(kmax));
    double b = Sup(X1(kmax));
    double c = Mid(X1(kmax));
	X1(kmax)=INTERVAL(a,c);
	X2(kmax)=INTERVAL(c,b);
	return X2;
}

INTERVAL_VECTOR InverseProblem::BisectIntervalVector(INTERVAL_VECTOR & X1, bool & canonical)
{
	INTERVAL_VECTOR X2=X1;
	canonical = false;
	int kmax=Dimension(X1);
	double dmax=Diam(X1(kmax))/eps[kmax-1];
	for(int k=kmax-1; k>=1; k--)
		if(Diam(X1(k))/eps[k-1]>dmax)
		{
			kmax=k;
			dmax=Diam(X1(k))/eps[k-1];
		}
    double a = Inf(X1(kmax));
    double b = Sup(X1(kmax));
    double c = Mid(X1(kmax));
	X1(kmax)=INTERVAL(a,c);
	X2(kmax)=INTERVAL(c,b);
	if (!(c>a) || !(c<b)) {
	    /*
	    cout << "Error spliting:" << endl;
	    cout << " a = " << a << endl;
	    cout << " b = " << b << endl;
	    cout << " c = " << c << endl;
	    */
	    canonical = true;
	    X1(kmax)=INTERVAL(a,b);
        X2(kmax)=INTERVAL(a,b);
	}
	return X2;
}
/*
void InverseProblem::insert(multimap<double,pair<INTERVAL_VECTOR,INTERVAL> > & UB, INTERVAL_VECTOR & x, INTERVAL q)
{
    pair<INTERVAL_VECTOR,INTERVAL> p = pair<INTERVAL_VECTOR,INTERVAL>(x,q);
    UB.insert(pair<double,pair<INTERVAL_VECTOR,INTERVAL> >(-Diam(p.second),p));
}
*/

/*
INTERVAL InverseProblem::integrate(INTERVAL_VECTOR x_init, double total_wid_max)
{
    multimap<double,pair<INTERVAL_VECTOR,INTERVAL> > UB;
    INTERVAL q=d->quadratureEval(x_init);
    insert(UB,x_init,q);
    INTERVAL total_upper=INTERVAL(Sup(UB.begin()->second.second));
    INTERVAL total_lower=INTERVAL(Inf(UB.begin()->second.second));
    INTERVAL upper_lower=total_upper-total_lower;
    double total_wid = Sup(upper_lower);
    unsigned int nbisection=0;
    int xk;
    INTERVAL x_old, q_old, q1, q2, q_new;
    INTERVAL quad = q;
    INTERVAL best = q;
    int step = 1;
    double stop = 5.0e-17;
    while(total_wid>total_wid_max)
    {
        Intersection (best, best, quad);
        if (Sup(best)<0.0001) return best;
        nbisection++;
        if (nbisection%step==0) {
            multimap<double,pair<INTERVAL_VECTOR,INTERVAL> >::iterator it;
            total_upper = INTERVAL(0.);
            total_lower = INTERVAL(0.);
            quad = INTERVAL(0.);
            for (it=UB.begin();it!=UB.end();it++) {
                pair<double,pair<INTERVAL_VECTOR,INTERVAL> > p = (*it);
                q = (p.second).second;
                total_upper += INTERVAL(Sup(q));
                total_lower += INTERVAL(Inf(q));
                quad += q;
            }
            upper_lower = total_upper - total_lower;
            total_wid = Sup(upper_lower);
            Intersection (best, best, quad);
            //cout << nbisection << " " << UB.size() << ":" << " " << best << " " << Diam(best) << endl;
        }
        INTERVAL_VECTOR x = UB.begin()->second.first;
        q = UB.begin()->second.second;
        q_old = q;
        if (Diam(q_old)>stop) {
            UB.erase(UB.begin());
            quad -= q_old;
            total_upper -= INTERVAL(Sup(q));
            total_lower -= INTERVAL(Inf(q));
            INTERVAL_VECTOR xx=BisectIntervalVector(x,xk,x_old);
            q1 = d->quadratureEval(x);
            q2 = d->quadratureEval(xx);
            q_new = q1+q2;
            insert(UB,x,q1);
            insert(UB,xx,q2);
            total_upper += INTERVAL(Sup(q_new));
            total_lower += INTERVAL(Inf(q_new));
            quad += q_new;
            upper_lower = total_upper - total_lower;
            total_wid = Sup(upper_lower);
        }
        else return best;
        //cin.get();
    }
    return best;
}
*/
/*
void InverseProblem::solve(double total_wid_max)
{
    INTERVAL_VECTOR x = x_init;
    int *lb;
    lb = new int[nVars];
    int *w;
    w = new int[nVars];
    for (unsigned int i=0;i<nVars;i++) {
        lb[i] = 0;
        w[i] = ceil(Diam(x_init(i+1))/eps[i]);
    }
    string filename = "output.txt";
    ofstream file (filename.c_str());
    if (file.is_open()){
        search(x,lb,w,total_wid_max,file);
        file.close();
    }
}
*/


void InverseProblem::createOutput(int v1, int v2)
{
    //cout << "x_init -> " << x_init <<endl;
    //int w1 = ceil(Diam(x_init(v1))/eps[v1-1]);
    //int w2 = ceil(Diam(x_init(v2))/eps[v2-1]);
    int w1 = nDiv;
    int w2 = nDiv;
    //int m[w1][w2];
    double m[w1][w2];
    for (unsigned int i=0;i<w1;i++)
        for (unsigned int j=0;j<w2;j++) m[i][j] = 0;

    string line;
    string filename = "output.txt";
    ifstream file (filename.c_str());
    int min1=w1, min2=w2, max1=0, max2=0;
    //int value;
    double value;
    int t[nVars];
    if (file.is_open()) {
        while (!file.eof()) {
            getline(file,line);
            if (line!="") {
                istringstream iss(line,istringstream::in);
                iss >> value;
                for (unsigned int i=0;i<nVars;i++) iss >> t[i];
                m[t[v1-1]][t[v2-1]] += value;
                if (t[v1-1]<min1) min1 = t[v1-1];
                if (t[v1-1]>max1) max1 = t[v1-1];
                if (t[v2-1]<min2) min2 = t[v2-1];
                if (t[v2-1]>max2) max2 = t[v2-1];
            }
        }
        file.close();
    }
    stringstream ssv1; ssv1 << v1;
    stringstream ssv2; ssv2 << v2;
    filename = "output_" + ssv1.str() + "_" + ssv2.str() + ".txt";
    ofstream f2 (filename.c_str());
    double v1min, v1max, v2min, v2max;
    /*
    v1min = Inf(x_init(v1)) + min1*eps[v1-1];
    v1max = Inf(x_init(v1)) + max1*eps[v1-1];
    v2min = Inf(x_init(v2)) + min2*eps[v2-1];
    v2max = Inf(x_init(v2)) + max2*eps[v2-1];
    */
    v1min = lbox[v1-1] + min1*eps[v1-1];
    v1max = lbox[v1-1] + max1*eps[v1-1];
    v2min = lbox[v2-1] + min2*eps[v2-1];
    v2max = lbox[v2-1] + max2*eps[v2-1];
    if (f2.is_open())
    {
        f2 << v1min << " ";
        f2 << v1max << " ";
        f2 << v2min << " ";
        f2 << v2max << " ";
        f2 << eps[v1-1] << " ";
        f2 << eps[v2-1] << " ";
        f2 << max1-min1+1 << " ";
        f2 << max2-min2+1 << endl;
        for (unsigned int i=min1;i<=max1;i++) {
            for (unsigned int j=min2;j<=max2;j++) f2 << m[i][j] << " ";
            f2 << endl;
        }
        f2.close();
    }
}

void InverseProblem::createOutputN(int v1, int v2, int n)
{
    //cout << "x_init -> " << x_init <<endl;
    //int w1 = ceil(Diam(x_init(v1))/eps[v1-1]);
    //int w2 = ceil(Diam(x_init(v2))/eps[v2-1]);
    int w1 = nDiv;
    int w2 = nDiv;
    //int m[w1][w2];
    double m[w1][w2];
    for (unsigned int i=0;i<w1;i++)
        for (unsigned int j=0;j<w2;j++) m[i][j] = 0;

    string line;
    stringstream ssn; ssn << n;
    string filename = "output" + ssn.str() + ".txt";
    ifstream file (filename.c_str());
    int min1=w1, min2=w2, max1=0, max2=0;
    //int value;
    double value;
    int t[nVars];
    if (file.is_open()) {
        while (!file.eof()) {
            getline(file,line);
            if (line!="") {
                istringstream iss(line,istringstream::in);
                iss >> value;
                for (unsigned int i=0;i<nVars;i++) iss >> t[i];
                m[t[v1-1]][t[v2-1]] += value;
                if (t[v1-1]<min1) min1 = t[v1-1];
                if (t[v1-1]>max1) max1 = t[v1-1];
                if (t[v2-1]<min2) min2 = t[v2-1];
                if (t[v2-1]>max2) max2 = t[v2-1];
            }
        }
        file.close();
    }
    stringstream ssv1; ssv1 << v1;
    stringstream ssv2; ssv2 << v2;
    filename = "output" + ssn.str() + "_" + ssv1.str() + "_" + ssv2.str() + ".txt";
    ofstream f2 (filename.c_str());
    double v1min, v1max, v2min, v2max;
    /*
    v1min = Inf(x_init(v1)) + min1*eps[v1-1];
    v1max = Inf(x_init(v1)) + max1*eps[v1-1];
    v2min = Inf(x_init(v2)) + min2*eps[v2-1];
    v2max = Inf(x_init(v2)) + max2*eps[v2-1];
    */
    v1min = lbox[v1-1] + min1*eps[v1-1];
    v1max = lbox[v1-1] + (max1+1)*eps[v1-1];
    v2min = lbox[v2-1] + min2*eps[v2-1];
    v2max = lbox[v2-1] + (max2+1)*eps[v2-1];
    if (f2.is_open())
    {
        f2 << v1min << " ";
        f2 << v1max << " ";
        f2 << v2min << " ";
        f2 << v2max << " ";
        f2 << eps[v1-1] << " ";
        f2 << eps[v2-1] << " ";
        f2 << max1-min1+1 << " ";
        f2 << max2-min2+1 << endl;
        for (unsigned int i=min1;i<=max1;i++) {
            for (unsigned int j=min2;j<=max2;j++) f2 << m[i][j] << " ";
            f2 << endl;
        }
        f2.close();
    }
}


void InverseProblem::createOutput(int v1)
{
    //cout << "x_init -> " << x_init <<endl;
    //int w1 = ceil(Diam(x_init(v1))/eps[v1-1]);
    //int m[w1];
    int w1 = nDiv;
    double m[w1];
    for (unsigned int i=0;i<w1;i++) m[i] = 0;

    string line;
    stringstream ssi;
    string filename = "output.txt";
    ifstream file (filename.c_str());
    int min1=w1, max1=0;
    //int value;
    double value;
    int t[nVars];
    if (file.is_open()) {
        while (!file.eof()) {
            getline(file,line);
            if (line!="") {
                istringstream iss(line,istringstream::in);
                iss >> value;
                for (unsigned int i=0;i<nVars;i++) iss >> t[i];
                m[t[v1-1]] += value;
                if (t[v1-1]<min1) min1 = t[v1-1];
                if (t[v1-1]>max1) max1 = t[v1-1];
            }
        }
        file.close();
    }
    stringstream ssv1; ssv1 << v1;
    filename = "output_" + ssv1.str() + ".txt";
    ofstream f2 (filename.c_str());
    double v1min, v1max;
    //v1min = Inf(x_init(v1)) + min1*eps[v1-1];
    //v1max = Inf(x_init(v1)) + max1*eps[v1-1];
    v1min = lbox[v1-1] + min1*eps[v1-1];
    v1max = lbox[v1-1] + max1*eps[v1-1];
    if (f2.is_open())
    {
        f2 << v1min << " ";
        f2 << v1max << " ";
        f2 << eps[v1-1] << " ";
        f2 << max1-min1+1 << endl;
        for (unsigned int i=min1;i<=max1;i++) f2 << m[i] << " ";
        f2 << endl;
        f2.close();
    }
}


void InverseProblem::createOutputN(int v1, int n)
{
    //cout << "x_init -> " << x_init <<endl;
    //int w1 = ceil(Diam(x_init(v1))/eps[v1-1]);
    //int m[w1];
    int w1 = nDiv;
    double m[w1];
    for (unsigned int i=0;i<w1;i++) m[i] = 0;

    string line;
    stringstream ssi;
    stringstream ssn; ssn << n;
    string filename = "output" + ssn.str() + ".txt";
    ifstream file (filename.c_str());
    string filename1 = "output3D" + ssn.str() + ".txt";
    ofstream f1 (filename1.c_str());
    int min1=w1, max1=0;
    //int value;
    double value;
    int t[nVars];
    if (v1==nVars) {
        if (f1.is_open()) {
            f1 << std::setiosflags(ios::fixed) << std::setprecision(16);
            for (unsigned int i=0;i<nVars;i++) f1 << lbox[i] << " " << lbox[i] + nDiv*eps[i] << " ";
            for (unsigned int i=0;i<nVars;i++) f1 << eps[i] << " ";
            for (unsigned int i=0;i<nVars;i++) f1 << nDiv << " ";
            f1 << endl;
        }
    }
    bool first = true;
    if (file.is_open()) {
        while (!file.eof()) {
            getline(file,line);
            if (line!="") {
                istringstream iss(line,istringstream::in);
                iss >> value;
                for (unsigned int i=0;i<nVars;i++) {
                    iss >> t[i];
                    if (v1==nVars) f1 << t[i] << " ";
                }
                if (v1==nVars) f1 << value << endl;
                m[t[v1-1]] += value;
                if (t[v1-1]<min1) min1 = t[v1-1];
                if (t[v1-1]>max1) max1 = t[v1-1];
            }
        }
        file.close();
    }
    if (v1==nVars) f1.close();
    stringstream ssv1; ssv1 << v1;
    filename = "output" + ssn.str() + "_" + ssv1.str() + ".txt";
    ofstream f2 (filename.c_str());
    double v1min, v1max;
    //v1min = Inf(x_init(v1)) + min1*eps[v1-1];
    //v1max = Inf(x_init(v1)) + max1*eps[v1-1];
    v1min = lbox[v1-1] + min1*eps[v1-1];
    v1max = lbox[v1-1] + (max1+1)*eps[v1-1];
    if (f2.is_open())
    {
        f2 << v1min << " ";
        f2 << v1max << " ";
        f2 << eps[v1-1] << " ";
        f2 << max1-min1+1 << endl;
        for (unsigned int i=min1;i<=max1;i++) f2 << m[i] << " ";
        f2 << endl;
        f2.close();
    }
}


void InverseProblem::graphicsOutput()
{
    //for (unsigned int i=0;i<nVars;i++) x_init(i+1) = m->getInterval(i);
    for (unsigned int v1=1;v1<=nVars;v1++) {
        createOutput(v1);
        for (unsigned int v2=v1+1;v2<=nVars;v2++) createOutput(v1,v2);
    }
}

void InverseProblem::graphicsOutputN(int n)
{
    //for (unsigned int i=0;i<nVars;i++) x_init(i+1) = m->getInterval(i);
    for (unsigned int v1=1;v1<=nVars;v1++) {
        cout << "graphicsOutputN " << v1 << endl;
        createOutputN(v1,n);
        for (unsigned int v2=v1+1;v2<=nVars;v2++) {
            cout << "graphicsOutputN " << v1 << " " << v2 << endl;
            createOutputN(v1,v2,n);
        }
    }
}
