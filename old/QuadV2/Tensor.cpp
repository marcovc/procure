#include "Tensor.h"
#include "function.tsr"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <Utilities.h>
#include <stdlib.h>


Tensor::Tensor(int fN, int nV, vector<int*> fexp)
{
    //cout << "Tensor" << endl;
    nVars = nV;
    c = new vector<int>();
    d = new vector<pair<OP,unsigned long int *> >();
    fNumber = fN;
    stringstream ssi; ssi << fNumber;
    int oldNumber = getFunctionDef();
    if (fNumber!=oldNumber) changed = true;
    else {
        changed = false;
        getFunctionDat();
        if (fDat.size()!=fexp.size()) changed = true;
        else {
            for (unsigned int i=0; i<fDat.size(); i++) {
                int *t0 = fDat[i];
                int *t1 = fexp[i];
                if ((t0[0]!=t1[0])||(t0[1]!=t1[1])||(t0[2]!=t1[2])) { changed = true; break; }
            }
        }
    }
    if (changed) fDat = fexp;
    //cout << "Tensor end" << endl;
}

Tensor::~Tensor()
{
    //dtor
}


void Tensor::setTuples(int order)
{
    ty = new vector<vector<pair<int *,INTERVAL> >*>();
    for (int i=0;i<=order;i++) {
        vector<pair<int *,INTERVAL> >* ti = new vector<pair<int *,INTERVAL> >();
        ty->push_back(ti);
        int t[nVars];
        for (int j=0;j<nVars;j++) t[j] = 0;
        setTuples(ti,t,0,i,1.);
    }
}

unsigned long int Tensor::setOp(OP op, unsigned long int arg1, unsigned long int arg2)
{
    //cout << op << ";" << arg1 << ";" << arg1 << endl;
    if (op==VAR || op==SIN || op==COS || op==SQRT || op==SQR || op==TAN || op==COT || op==EXP || op==LOG || op==SINH || op==COSH || op==TANH || op==COTH || op==ARCTAN ||op==PI) { cout << "Error: operation " << op << " is not binary!" << endl; cin.get(); }
    else {
        unsigned long int j;
        if (op==ADD || op==PROD) {
            j = arg1;
            arg1 = max(arg1,arg2);
            arg2 = min(j,arg2);
        }
        j=d->size()-1;
        if (j==-1) j=0;
        while (j>arg1) {
            if (((*d)[j].first==op) && ((*d)[j].second[0]==arg1) && ((*d)[j].second[1]==arg2)) return j;
            j--;
        }
        unsigned long int *tt = new unsigned long int[nVars+2];
        for (int i=1;i<nVars+2;i++) tt[i] = -1;
        if (op==CONS) {
            op = DIV;
            for (int i=2;i<nVars+2;i++) tt[i] = 0;
        }

        if (op==SUB) {
            if (((*d)[arg1].first==CONS) && ((*d)[arg2].first==CONS)) {
                int value = (*c)[(*d)[arg1].second[0]]-(*c)[(*d)[arg2].second[0]];
                j=d->size()-1;
                if (j==-1) j=0;
                while (j>0) {
                    if ((*d)[j].first==CONS) {
                        if (((*c)[(*d)[j].second[0]]==value) && ((*d)[j].second[1]==-1)) return j;
                    }
                    j--;
                }
                arg1=c->size();
                c->push_back(value);
                arg2 = -1;
                op = CONS;
                for (int i=2;i<nVars+2;i++) tt[i] = 0;
            }
        }
        tt[0]=arg1;
        tt[1]=arg2;
        d->push_back(pair<OP,unsigned long int *>(op,tt));
    }
    return d->size()-1;
}

unsigned long int Tensor::setOp(OP op, unsigned long int arg1)
{
    //cout << op << ";" << arg1 << ";" << arg1 << endl;
    unsigned long int minarg=arg1;
    if (op==CONS || op==VAR || op==SIN || op==COS || op==SQRT || op==SQR || op==TAN || op==COT || op==EXP || op==LOG || op==SINH || op==COSH || op==TANH || op==COTH || op==ARCTAN) {
        unsigned long int j=d->size()-1;
        if (j==-1) j=0;
        if (op==CONS) minarg = -1;
        while (j>minarg) {
            if (((*d)[j].first==op) && ((*d)[j].second[0]==arg1)) return j;
            j--;
        }
        unsigned long int *tt = new unsigned long int[nVars+2];
        tt[1] = -1;
        if (op==CONS || op==VAR ) for (int i=2;i<nVars+2;i++) tt[i] = 0;
        else for (int i=2;i<nVars+2;i++) tt[i] = -1;
        if (op==CONS) {
            tt[0]=c->size();
            c->push_back(arg1);
        }
        else {
            tt[0]=arg1;
            if (op==VAR) tt[arg1+1] = 1;
        }
        d->push_back(pair<OP,unsigned long int *>(op,tt));
    }
    else { cout << "Error: operation " << op << " is not unary!" << endl; cin.get(); }
    return d->size()-1;
}

unsigned long int Tensor::setOp(OP op)
{
    //cout << op << ";" << endl;
    if (op==PI) {
        unsigned long int *tt = new unsigned long int[nVars+2];
        tt[0] = -1;
        tt[1] = -1;
        for (int i=2;i<nVars+2;i++) tt[i] = 0;
        d->push_back(pair<OP,unsigned long int *>(op,tt));
    }
    else { cout << "Error: operation " << op << " is not 0ary!" << endl; cin.get(); }
    return d->size()-1;
}


unsigned long int Tensor::setDerivative(unsigned long int i, int v)
{
    pair<OP,unsigned long int *> p = (*d)[i];
    if (p.second[v+1]==-1) {
        unsigned long int arg1 = p.second[0];
        unsigned long int arg2 = p.second[1];
        unsigned long int ddarg1;
        unsigned long int ddarg2;
        switch ( p.first ) {
            case ADD:
                //cout << "d(" << arg1 << ") + d(" << arg2 << ") -> ";
                ddarg1=setDerivative(arg1,v);
                ddarg2=setDerivative(arg2,v);
                if (ddarg1==0) {
                    if (ddarg2==0) {
                        //cout << "0" << endl;
                        p.second[v+1]=0;
                    }
                    else {
                        //cout << "dd(" << arg2 << "," << v << ")"  << endl;
                        p.second[v+1]=ddarg2;
                    }
                }
                else if (ddarg2==0) {
                    //cout << "dd(" << arg1 << "," << v << ")"  << endl;
                    p.second[v+1]=ddarg1;
                }
                else {
                    //cout << "dd(" << arg1 << "," << v << ") + dd(" << arg2 << "," << v << ")" << endl;
                    p.second[v+1]=setOp(ADD,ddarg1,ddarg2);
                }
                break;
            case SUB:
                //cout << "d(" << arg1 << ") - d(" << arg2 << ") -> ";
                ddarg1=setDerivative(arg1,v);
                ddarg2=setDerivative(arg2,v);
                if (ddarg2==0) {
                    //cout << "dd(" << arg1 << "," << v << ")"  << endl;
                    p.second[v+1]=ddarg1;
                }
                else {
                    //cout << "dd(" << arg1 << "," << v << ") - dd(" << arg2 << "," << v << ")" << endl;
                    p.second[v+1]=setOp(SUB,ddarg1,ddarg2);
                }
                break;
            case PROD:
                //cout << "d(" << arg1 << ") * d(" << arg2 << ") -> ";
                //cin.get();
                ddarg1=setDerivative(arg1,v);
                ddarg2=setDerivative(arg2,v);
                if (ddarg1==0) {
                    if (ddarg2==0) {
                        //cout << "0" << endl;
                        p.second[v+1]=0;
                    }
                    else {
                        if (ddarg2==1) {
                            //cout << "d(" << arg1 << ")" << endl;
                            p.second[v+1]=arg1;
                        }
                        else {
                            //cout << "d(" << arg1 << ") * dd(" << arg2 <<"," << v <<  ")" << endl;
                            p.second[v+1]=setOp(PROD,arg1,ddarg2);
                        }
                    }
                }
                else if (ddarg1==1) {
                    if (ddarg2==0) {
                        //cout << "d(" << arg2 << ")" << endl;
                        p.second[v+1]=arg2;
                    }
                    else {
                        if (ddarg2==1) {
                            //cout << "d(" << arg2 << ") + d(" << arg1 << ")" << endl;
                            p.second[v+1]=setOp(ADD,arg1,arg2);
                        }
                        else {
                            //cout << "d(" << arg2 << ") + d(" << arg1 << ") * dd(" << arg2 <<"," << v <<  ")" << endl;
                            p.second[v+1]=setOp(ADD,setOp(PROD,arg1,ddarg2),arg2);
                        }
                    }
                }
                else if (ddarg2==0) {
                        //cout << "dd(" << arg1 << "," << v << ") * d(" << arg2 << ")" << endl;
                        p.second[v+1]=setOp(PROD,ddarg1,arg2);
                    }
                    else {
                        if (ddarg2==1) {
                            //cout << "dd(" << arg1 << "," << v << ") * d(" << arg2 << ") + d(" << arg1 << ")" << endl;
                            p.second[v+1]=setOp(ADD,arg1,setOp(PROD,arg2,ddarg1));
                        }
                        else {
                            //cout << "dd(" << arg1 << "," << v << ") * d(" << arg2 << ") + d(" << arg1 << ") * dd(" << arg2 <<"," << v <<  ")" << endl;
                            p.second[v+1]=setOp(ADD,setOp(PROD,arg1,ddarg2),setOp(PROD,arg2,ddarg1));
                        }
                    }
                break;
            case DIV:
                //cout << "d(" << arg1 << ") / d(" << arg2 << ") -> ";
                ddarg1=setDerivative(arg1,v);
                ddarg2=setDerivative(arg2,v);
                if (ddarg2==0) {
                    if (ddarg1==0) {
                        //cout << "0" << endl;
                        p.second[v+1]=0;
                    }
                    else {
                        //cout << "(dd(" << arg1 << "," << v << ") / d(" << arg2 << ")" << endl;
                        p.second[v+1]=setOp(PROD,ddarg1,setOp(POW,arg2,setOp(SUB,0,1)));
                    }
                }
                else  if (ddarg2==1) {
                    if (ddarg1==0) {
                        //cout << " - d(" << arg1 << ") / Power(d(" << arg2 << "," << 2 << "))" << endl;
                        if ((*d)[arg2].first==SQRT) p.second[v+1]=setOp(SUB,0,setOp(DIV,arg1,(*d)[arg2].second[0]));
                        //else p.second[v+1]=setOp(SUB,0,setOp(DIV,arg1,setOp(SQR,arg2)));
                        else p.second[v+1]=setOp(SUB,0,setOp(PROD,arg1,setOp(POW,arg2,setOp(SUB,0,2))));
                    }
                    /*
                    else if (ddarg1==1) {
                        //cout << "( d(" << arg2 << ") - d(" << arg1 << ") ) / Power(d(" << arg2 << "," << 2 << "))" << endl;
                        if ((*d)[arg2].first==SQRT) p.second[v+1]=setOp(DIV,setOp(SUB,arg2,arg1),(*d)[arg2].second[0]);
                        else p.second[v+1]=setOp(DIV,setOp(SUB,arg2,arg1),setOp(SQR,arg2));
                    }
                    */
                    else   {
                        //cout << "(dd(" << arg1 << "," << v << ") * d(" << arg2 << ") - d(" << arg1 << ") ) / Power(d(" << arg2 << "," << 2 << "))" << endl;
                        //if ((*d)[arg2].first==SQRT) p.second[v+1]=setOp(DIV,setOp(SUB,setOp(PROD,arg2,ddarg1),arg1),(*d)[arg2].second[0]);
                        //else p.second[v+1]=setOp(DIV,setOp(SUB,setOp(PROD,arg2,ddarg1),arg1),setOp(SQR,arg2));
                        if ((*d)[arg2].first==SQRT) p.second[v+1]=setOp(SUB,setOp(DIV,ddarg1,arg2),setOp(DIV,arg1,(*d)[arg2].second[0]));
                        //else p.second[v+1]=setOp(SUB,setOp(DIV,ddarg1,arg2),setOp(DIV,arg1,setOp(SQR,arg2)));
                        else p.second[v+1]=setOp(PROD,setOp(SUB,ddarg1,setOp(PROD,arg1,setOp(POW,arg2,setOp(SUB,0,1)))),setOp(POW,arg2,setOp(SUB,0,1)));
                    }
                }
                else if (ddarg1==0) {
                        //cout << "- d(" << arg1 << ") * dd(" << arg2 <<"," << v <<  ") / Power(d(" << arg2 << "," << 2 << "))" << endl;
                        if ((*d)[arg2].first==SQRT) p.second[v+1]=setOp(SUB,0,setOp(DIV,setOp(PROD,arg1,ddarg2),(*d)[arg2].second[0]));
                        //else p.second[v+1]=setOp(SUB,0,setOp(DIV,setOp(PROD,arg1,ddarg2),setOp(SQR,arg2)));
                        else p.second[v+1]=setOp(PROD,setOp(SUB,0,setOp(PROD,setOp(PROD,arg1,ddarg2),setOp(POW,arg2,setOp(SUB,0,1)))),setOp(POW,arg2,setOp(SUB,0,1)));
                }
                /*
                else if (ddarg1==1) {
                        //cout << "(d(" << arg2 << ") - d(" << arg1 << ") * dd(" << arg2 <<"," << v <<  ")) / Power(d(" << arg2 << "," << 2 << "))" << endl;
                        if ((*d)[arg2].first==SQRT) p.second[v+1]=setOp(DIV,setOp(SUB,arg2,setOp(PROD,arg1,ddarg2)),(*d)[arg2].second[0]);
                        else p.second[v+1]=setOp(DIV,setOp(SUB,arg2,setOp(PROD,arg1,ddarg2)),setOp(SQR,arg2));
                }
                */
                else {
                    //cout << "(dd(" << arg1 << "," << v << ") * d(" << arg2 << ") - d(" << arg1 << ") * dd(" << arg2 <<"," << v <<  ")) / Power(d(" << arg2 << "," << 2 << "))" << endl;
                    //if ((*d)[arg2].first==SQRT) p.second[v+1]=setOp(DIV,setOp(SUB,setOp(PROD,arg2,ddarg1),setOp(PROD,arg1,ddarg2)),(*d)[arg2].second[0]);
                    //else p.second[v+1]=setOp(DIV,setOp(SUB,setOp(PROD,arg2,ddarg1),setOp(PROD,arg1,ddarg2)),setOp(SQR,arg2));
                    //if ((*d)[arg2].first==SQRT) p.second[v+1]=setOp(SUB,setOp(DIV,ddarg1,arg2),setOp(DIV,setOp(PROD,arg1,ddarg2),(*d)[arg2].second[0]));
                    //else p.second[v+1]=setOp(SUB,setOp(DIV,ddarg1,arg2),setOp(DIV,setOp(PROD,arg1,ddarg2),setOp(SQR,arg2)));
                    //p.second[v+1]=setOp(DIV,setOp(SUB,ddarg1,setOp(DIV,setOp(PROD,arg1,ddarg2),arg2)),arg2);
                    p.second[v+1]=setOp(PROD,setOp(SUB,ddarg1,setOp(PROD,setOp(PROD,arg1,ddarg2),setOp(POW,arg2,setOp(SUB,0,1)))),setOp(POW,arg2,setOp(SUB,0,1)));
                }
                break;
            case POW:
                //cout << "Power(d(" << arg1 << "),d(" << arg2 << ")) -> ";
                ddarg1 = setDerivative(arg1,v);
                if (ddarg1==0) {
                    //cout << "0" << endl;
                    p.second[v+1]=0;
                }
                else if (ddarg1==1) {
                    if (arg2==2) {
                        //cout << "d(" << arg2 << ") * d(" << arg1 << ")" << endl;
                        p.second[v+1]=setOp(PROD,arg1,arg2);
                    }
                    else {
                        //cout << "d(" << arg2 << ") * Power(d(" << arg1 << "),d(" << arg2 << ")-1)" << endl;
                        p.second[v+1]=setOp(PROD,arg2,setOp(POW,arg1,setOp(SUB,arg2,1)));
                    }
                }
                else {
                    if (arg2==2) {
                        //cout << "d(" << arg2 << ") * d(" << arg1 << ") * dd(" << arg1 <<"," << v <<  ")" << endl;
                        p.second[v+1]=setOp(PROD,ddarg1,setOp(PROD,arg1,arg2));
                    }
                    else {
                        //cout << "d(" << arg2 << ") * Power(d(" << arg1 << "),d(" << arg2 << ")-1) * dd(" << arg1 <<"," << v <<  ")" << endl;
                        p.second[v+1]=setOp(PROD,ddarg1,setOp(PROD,setOp(POW,arg1,setOp(SUB,arg2,1)),arg2));
                    }
                }
                break;
            case SIN:
                //cout << "Sin(d(" << arg1 << ")) -> ";
                ddarg1 = setDerivative(arg1,v);
                if (ddarg1==0) {
                    //cout << "0" << endl;
                    p.second[v+1]=0;
                }
                else if (ddarg1==1) {
                    //cout << "Cos(d(" << arg1 << "))" << endl;
                    p.second[v+1]=setOp(COS,arg1);
                }
                else {
                    //cout << "dd(" << arg1 << "," << v << ")*Cos(d(" << arg1 << "))" << endl;
                    p.second[v+1]=setOp(PROD,ddarg1,setOp(COS,arg1));
                }
                break;
            case COS:
                //cout << "Cos(d(" << arg1 << ")) -> ";
                ddarg1 = setDerivative(arg1,v);
                if (ddarg1==0) {
                    //cout << "0" << endl;
                    p.second[v+1]=0;
                }
                else if (ddarg1==1) {
                    //cout << "- Sin(d(" << arg1 << "))" << endl;
                    p.second[v+1]=setOp(SUB,0,setOp(SIN,arg1));
                }
                else {
                    //cout << "- dd(" << arg1 << "," << v << ")*Sin(d(" << arg1 << "))" << endl;
                    p.second[v+1]=setOp(SUB,0,setOp(PROD,ddarg1,setOp(SIN,arg1)));
                }
                break;
            case SQRT:
                //cout << "Sqrt(d(" << arg1 << ")) -> ";
                ddarg1 = setDerivative(arg1,v);
                if (ddarg1==0) {
                    //cout << "0" << endl;
                    p.second[v+1]=0;
                }
                else {
                    //cout << "dd(" << arg1 << "," << v << ")/(2*Sqrt(d(" << arg1 << ")))" << endl;
                    //p.second[v+1]=setOp(DIV,ddarg1,setOp(PROD,2,setOp(SQRT,arg1)));
                    //p.second[v+1]=setOp(PROD,setOp(DIV,ddarg1,setOp(SQRT,arg1)),setOp(DIV,1,2));
                    p.second[v+1]=setOp(PROD,ddarg1,setOp(PROD,setOp(POW,arg1,setOp(SUB,setOp(DIV,1,2),1)),setOp(DIV,1,2)));
                }
                break;
            case SQR:
                //cout << "Sqrt(d(" << arg1 << ")) -> ";
                ddarg1 = setDerivative(arg1,v);
                if (ddarg1==0) {
                    //cout << "0" << endl;
                    p.second[v+1]=0;
                }
                else if (ddarg1==1) {
                    //cout << "Cos(d(" << arg1 << "))" << endl;
                    p.second[v+1]=setOp(PROD,2,arg1);
                }
                else {
                    //cout << "dd(" << arg1 << "," << v << ")*Cos(d(" << arg1 << "))" << endl;
                    p.second[v+1]=setOp(PROD,2,setOp(PROD,ddarg1,arg1));
                }
                break;
            case TAN:
                //cout << "Tan(d(" << arg1 << ")) -> ";
                ddarg1 = setDerivative(arg1,v);
                if (ddarg1==0) {
                    //cout << "0" << endl;
                    p.second[v+1]=0;
                }
                else if (ddarg1==1) {
                    //cout << "Cos(d(" << arg1 << "))^(-2)" << endl;
                    p.second[v+1]=setOp(POW,setOp(COS,arg1),setOp(SUB,0,2));
                }
                else {
                    //cout << "dd(" << arg1 << "," << v << ")*Cos(d(" << arg1 << "))^(-2)" << endl;
                    p.second[v+1]=setOp(PROD,ddarg1,setOp(POW,setOp(COS,arg1),setOp(SUB,0,2)));
                }
                break;
            case COT:
                //cout << "Cot(d(" << arg1 << ")) -> ";
                ddarg1 = setDerivative(arg1,v);
                if (ddarg1==0) {
                    //cout << "0" << endl;
                    p.second[v+1]=0;
                }
                else if (ddarg1==1) {
                    //cout << "-Sin(d(" << arg1 << "))^(-2)" << endl;
                    p.second[v+1]=setOp(SUB,0,setOp(POW,setOp(SIN,arg1),setOp(SUB,0,2)));
                }
                else {
                    //cout << "- dd(" << arg1 << "," << v << ")*Sin(d(" << arg1 << "))^(-2)" << endl;
                    p.second[v+1]=setOp(SUB,0,setOp(PROD,ddarg1,setOp(POW,setOp(SIN,arg1),setOp(SUB,0,2))));
                }
                break;
            case EXP:
                //cout << "Exp(d(" << arg1 << ")) -> ";
                ddarg1 = setDerivative(arg1,v);
                if (ddarg1==0) {
                    //cout << "0" << endl;
                    p.second[v+1]=0;
                }
                else if (ddarg1==1) {
                    //cout << "Exp(d(" << arg1 << "))" << endl;
                    p.second[v+1]=i;
                }
                else {
                    //cout << "dd(" << arg1 << "," << v << ")*Exp(d(" << arg1 << "))" << endl;
                    p.second[v+1]=setOp(PROD,ddarg1,i);
                }
                break;
            case LOG:
                //cout << "Log(d(" << arg1 << ")) -> ";
                ddarg1 = setDerivative(arg1,v);
                if (ddarg1==0) {
                    //cout << "0" << endl;
                    p.second[v+1]=0;
                }
                else if (ddarg1==1) {
                    //cout << "d(" << arg1 << ")^(-1)" << endl;
                    p.second[v+1]=setOp(POW,arg1,setOp(SUB,0,1));
                }
                else {
                    //cout << "dd(" << arg1 << "," << v << ")*d(" << arg1 << ")^(-1)" << endl;
                    p.second[v+1]=setOp(PROD,ddarg1,setOp(POW,arg1,setOp(SUB,0,1)));
                }
                break;
            case SINH:
                //cout << "Sinh(d(" << arg1 << ")) -> ";
                ddarg1 = setDerivative(arg1,v);
                if (ddarg1==0) {
                    //cout << "0" << endl;
                    p.second[v+1]=0;
                }
                else if (ddarg1==1) {
                    //cout << "Cosh(d(" << arg1 << "))" << endl;
                    p.second[v+1]=setOp(COSH,arg1);
                }
                else {
                    //cout << "dd(" << arg1 << "," << v << ")*Cosh(d(" << arg1 << "))" << endl;
                    p.second[v+1]=setOp(PROD,ddarg1,setOp(COSH,arg1));
                }
                break;
            case COSH:
                //cout << "Cosh(d(" << arg1 << ")) -> ";
                ddarg1 = setDerivative(arg1,v);
                if (ddarg1==0) {
                    //cout << "0" << endl;
                    p.second[v+1]=0;
                }
                else if (ddarg1==1) {
                    //cout << "Sinh(d(" << arg1 << "))" << endl;
                    p.second[v+1]=setOp(SINH,arg1);
                }
                else {
                    //cout << "dd(" << arg1 << "," << v << ")*Sinh(d(" << arg1 << "))" << endl;
                    p.second[v+1]=setOp(PROD,ddarg1,setOp(SINH,arg1));
                }
                break;
            case TANH:
                //cout << "Tanh(d(" << arg1 << ")) -> ";
                ddarg1 = setDerivative(arg1,v);
                if (ddarg1==0) {
                    //cout << "0" << endl;
                    p.second[v+1]=0;
                }
                else if (ddarg1==1) {
                    //cout << "Cosh(d(" << arg1 << "))^(-2)" << endl;
                    p.second[v+1]=setOp(POW,setOp(COSH,arg1),setOp(SUB,0,2));
                }
                else {
                    //cout << "dd(" << arg1 << "," << v << ")*Cosh(d(" << arg1 << "))^(-2)" << endl;
                    p.second[v+1]=setOp(PROD,ddarg1,setOp(POW,setOp(COSH,arg1),setOp(SUB,0,2)));
                }
                break;
            case COTH:
                //cout << "Coth(d(" << arg1 << ")) -> ";
                ddarg1 = setDerivative(arg1,v);
                if (ddarg1==0) {
                    //cout << "0" << endl;
                    p.second[v+1]=0;
                }
                else if (ddarg1==1) {
                    //cout << "-Sinh(d(" << arg1 << "))^(-2)" << endl;
                    p.second[v+1]=setOp(SUB,0,setOp(POW,setOp(SINH,arg1),setOp(SUB,0,2)));
                }
                else {
                    //cout << "- dd(" << arg1 << "," << v << ")*Sinh(d(" << arg1 << "))^(-2)" << endl;
                    p.second[v+1]=setOp(SUB,0,setOp(PROD,ddarg1,setOp(POW,setOp(SINH,arg1),setOp(SUB,0,2))));
                }
                break;
            case ARCTAN:
                //cout << "ArcTan(d(" << arg1 << ")) -> ";
                ddarg1 = setDerivative(arg1,v);
                if (ddarg1==0) {
                    //cout << "0" << endl;
                    p.second[v+1]=0;
                }
                else if (ddarg1==1) {
                    //cout << "(d(" << arg1 << ")^2+1)^(-1)" << endl;

                    p.second[v+1]=setOp(POW,setOp(ADD,setOp(POW,arg1,2),1),setOp(SUB,0,1));
                }
                else {
                    //cout << "dd(" << arg1 << "," << v << ")*(d(" << arg1 << ")^2+1)^(-1)" << endl;
                    p.second[v+1]=setOp(PROD,ddarg1,setOp(POW,setOp(ADD,setOp(POW,arg1,2),1),setOp(SUB,0,1)));
                }
                break;
            case PI:
                p.second[v+1]=0;
                break;
            default:   cout << "Error: unknown operation " << p.first  << endl; cin.get();
        }
    }
    else {
        //cout << "Derivative already computed!" << endl;
    }
    return p.second[v+1];
}


unsigned long int Tensor::setDerivative(unsigned long int i, int t[], int v, int coef)
{
    //cout << "i = " << i << " v = " << v << " <" << t[0] << "," << t[1] << "," << t[2] << ">" << endl;
    if (v>nVars) return i;
    if (t[v-1]>0) {
        unsigned long int dv = (*d)[i].second[v+1];
        //cout << "dv = " << dv << endl;
        if (dv==0) {
            //cout << "Derivative is 0!" << endl;
            return 0;
        }
        if (dv==-1) {
            //cout << "Derive: " << i << " -> " << v << endl;
            return setDerivative(i,v);
            /*
            dv = setDerivative(i,v);
            cout << "coef = " << coef << endl;
            unsigned long int dcons = setOp(CONS,coef);
            cout << "dcons = " << dcons << endl;
            unsigned long int ddd = setOp(PROD,dv,setOp(POW,dcons,setOp(SUB,0,1)));
            pair<OP,unsigned long int *> p = (*d)[i];
            cout << "p.second[v+1] = " << p.second[v+1] << " -> ";
            p.second[v+1] = ddd;
            cout << "p.second[v+1] = " << p.second[v+1] << endl;
            return ddd;
            */
        }
        else {
            t[v-1] = t[v-1]-1;
            unsigned long int dd = setDerivative(dv,t,v,coef);
            t[v-1] = t[v-1]+1;
            return dd;
        }
    }
    else {
        if (v<nVars) return setDerivative(i,t,v+1,t[v]);
        else return setDerivative(i,t,v+1,0);
    }
}

void Tensor::setDerivative()
{
    //cout << "setDerivative" << endl;
    taylorIndex.erase(taylorIndex.begin(),taylorIndex.end());
    unsigned long int dd;
    taylorIndex.push_back(endIndex-1);
    for (unsigned int p=1;p<ty->size();p++) {
        vector<pair<int *,INTERVAL> >* ti = (*ty)[p];
        for (unsigned int i=0;i<ti->size();i++) {
            int* tup = (*ti)[i].first;
            //showTuple(tup);
            dd = setDerivative(endIndex-1,tup,1,tup[0]);
            //cout << "dd = " << dd << endl;
            taylorIndex.push_back(dd);
        }
    }
    //cout << "setDerivative end" << endl;
}


void Tensor::getDerivative()
{
    orderIndex.erase(orderIndex.begin(),orderIndex.end());
    taylorCoefficient.erase(taylorCoefficient.begin(),taylorCoefficient.end());
    taylorTuple.erase(taylorTuple.begin(),taylorTuple.end());
    evenTuple.erase(evenTuple.begin(),evenTuple.end());
    unsigned long int dd, maxdd;
    maxdd = 0;
    int j = 0;
    for (unsigned int p=0;p<ty->size();p++) {
        vector<pair<int *,INTERVAL> >* ti = (*ty)[p];
        for (unsigned int i=0;i<ti->size();i++) {
            int* tup = (*ti)[i].first;
            dd = taylorIndex[j];
            maxdd = max(maxdd,dd);
            j++;
            taylorCoefficient.push_back((*ti)[i].second);
            taylorTuple.push_back(tup);
            evenTuple.push_back(isEven(tup));
        }
        orderIndex.push_back(maxdd);
    }
    t = new INTERVAL[maxdd+1];

}

void Tensor::setFunction()
{
    //cout << "setFunction" << endl;
    int op, arg1, arg2;
    for (unsigned int j=0;j<fDat.size();j++) {
        op = (fDat[j])[0];
        arg1 = (fDat[j])[1];
        arg2 = (fDat[j])[2];
        switch ( op ) {
            case CONS: setOp(CONS,arg1); break;
            case VAR:  setOp(VAR,arg1); break;
            case ADD:  setOp(ADD,arg1,arg2); break;
            case SUB:  if (arg1==-1) setOp(SUB,0,arg2);
                       else if (arg2==-1) setOp(SUB,0,setOp(CONS,arg1));
                       else setOp(SUB,arg1,arg2); break;
            case PROD: setOp(PROD,arg1,arg2); break;
            case DIV:  setOp(DIV,arg1,arg2); break;
            case POW:  setOp(POW,arg1,arg2); break;
            case SIN:  setOp(SIN,arg1); break;
            case COS:  setOp(COS,arg1);  break;
            case SQRT: setOp(SQRT,arg1); break;
            case SQR:  setOp(SQR,arg1); break;
            case TAN:  setOp(TAN,arg1); break;
            case COT:  setOp(COT,arg1); break;
            case EXP:  setOp(EXP,arg1); break;
            case LOG:  setOp(LOG,arg1); break;
            case SINH: setOp(SINH,arg1); break;
            case COSH: setOp(COSH,arg1); break;
            case TANH: setOp(TANH,arg1); break;
            case COTH: setOp(COTH,arg1); break;
            case ARCTAN: setOp(ARCTAN,arg1); break;
            case PI:   setOp(PI); break;
        }
    }
    //cout << "setFunction end" << endl;
    showDifferentiationVector();
}

bool Tensor::setOrder(int order)
{
    setTuples(order);
    if (changed) cout << "The integrand function as been changed! " << endl;
    else if (order>maxOrder) {
        if (maxOrder==0) cout << "The integrand function is not precompiled!" << endl;
        else cout << "The integrand function is only precompiled for derivatives up to order " << maxOrder << "!" << endl;
    }
    if ((changed)||(order>maxOrder)) {
        setFunction();
        endIndex = d->size();
        setDerivative();
        getDerivative();
        maxOrder = order;
        writeFunction();
        writeFunctionPaver();
        cout << "Please, compile your code and run again!" << endl;
        return false;
    }
    getDerivative();
    nOrder = order;
    orderS aux;
    orderVec.clear();
    fIndex.clear();
    fIndex.push_back(taylorIndex[0]);
    fInterval.push_back(0);
    for (unsigned ord=1;ord<=order;ord++) {
        aux.polyIndex.clear();
        aux.polyTuple.clear();
        aux.polyCoefficient.clear();
        aux.polyDerivative.clear();
        aux.errorIndex.clear();
        aux.errorTuple.clear();
        aux.errorEven.clear();
        aux.errorCoefficient.clear();
        aux.errorDerivative.clear();
        int j = 0;
        for (unsigned int p=0;p<ord;p++) {
            vector<pair<int *,INTERVAL> >* ti = (*ty)[p];
            for (unsigned int i=0;i<ti->size();i++) {
                if ((evenTuple[j]) && (taylorIndex[j]>0)) {
                    aux.polyIndex.push_back(taylorIndex[j]);
                    aux.polyTuple.push_back(taylorTuple[j]);
                    aux.polyCoefficient.push_back(taylorCoefficient[j]);
                    aux.polyDerivative.push_back(0);
                }
                j++;
            }
        }
        vector<pair<int *,INTERVAL> >* ti = (*ty)[ord];
        for (unsigned int i=0;i<ti->size();i++) {
            if (taylorIndex[j]>0) {
                aux.errorIndex.push_back(taylorIndex[j]);
                aux.errorTuple.push_back(taylorTuple[j]);
                aux.errorEven.push_back(evenTuple[j]);
                aux.errorCoefficient.push_back(taylorCoefficient[j]);
                aux.errorDerivative.push_back(0);
            }
            j++;
        }
        orderVec.push_back(aux);
    }
    return true;
}

INTERVAL Tensor::f(INTERVAL_VECTOR & x)
{
    computeTensor(x,fIndex[0]);
    return t[fIndex[0]];
}

void Tensor::getT(INTERVAL_VECTOR & x, int nObs, int maxObs,int obsIndex[],INTERVAL fwr[])
{
    computeTensor(x,maxObs);
    for (int i=0;i<nObs;i++) fwr[i] = t[obsIndex[i]];
}

INTERVAL Tensor::getD(INTERVAL_VECTOR & x, INTERVAL df[])
{
    computeTensor(x,orderIndex[1]);
    for (int i=0;i<nVars;i++) df[i] = t[orderVec[0].errorIndex[i]];
    return t[fIndex[0]];
}

INTERVAL Tensor::integral(int t[], INTERVAL_VECTOR & r)
{
    INTERVAL I = 1;
    for (int i=0;i<nVars;i++) {
        if (t[i] % 2 == 1) return 0;
        if (t[i]>0) I *= Power(r(i+1),t[i])/(t[i]+1);
    }
    return I;
}

INTERVAL Tensor::polynomialEval(INTERVAL_VECTOR & x, INTERVAL_VECTOR & mid, INTERVAL_VECTOR & rad, int ord)
{
    //cout << "polynomialEval" << " mid = " << mid << " rad = " << rad << " ord = " << ord << endl;
    INTERVAL I = 0.0;
    for (unsigned int i=0;i<orderVec[ord-1].polyIndex.size();i++) {
        /*
        showTuple(orderVec[ord-1].polyTuple[i]);
        cout << " polyCoefficient = " << orderVec[ord-1].polyCoefficient[i] << endl;
        cout << " integral = " << integral(orderVec[ord-1].polyTuple[i],rad) << endl;
        cout << " polyDerivative = " << t[orderVec[ord-1].polyIndex[i]] << endl;
        cout << " term -> " << orderVec[ord-1].polyCoefficient[i] * integral(orderVec[ord-1].polyTuple[i],rad) * t[orderVec[ord-1].polyIndex[i]] << endl;
        */
        //cout << " polyDerivative = t[" << orderVec[ord-1].polyIndex[i] << "] = " << t[orderVec[ord-1].polyIndex[i]] << endl;
        I += orderVec[ord-1].polyCoefficient[i] * integral(orderVec[ord-1].polyTuple[i],rad) * t[orderVec[ord-1].polyIndex[i]];
        //I += integral(orderVec[ord-1].polyTuple[i],rad) * t[orderVec[ord-1].polyIndex[i]];
    }
    return I;
}

INTERVAL Tensor::error(int t[], INTERVAL_VECTOR & x)
{
    //cout << "--- error " << endl;
    //showTuple(t);
    //cout << endl;
    //cout << "x = " << x << endl;
    INTERVAL I = 1;
    for (int i=0;i<nVars;i++) if (t[i]>0) I *= Power(x(i+1),t[i]);
    //cout << "I = " << I << endl;
    //cout << "--- error end" << endl;
    return I;
}

INTERVAL Tensor::errorEval(INTERVAL_VECTOR & x, INTERVAL_VECTOR & mid, INTERVAL_VECTOR & rad, int ord)
{
    //cout << "errorEval" << " mid = " << mid << " rad = " << rad << " ord = " << ord << endl;
    INTERVAL I = 0.0;
    INTERVAL D, midD;
    INTERVAL_VECTOR midX = x-mid;
    for (unsigned int i=0;i<orderVec[ord-1].errorIndex.size();i++) {
        D = t[orderVec[ord-1].errorIndex[i]];
        //cout << " errorDerivative = t[" << orderVec[ord-1].errorIndex[i] << "] = " << t[orderVec[ord-1].errorIndex[i]] << endl;
        if (D.unbounded()) {midD = INTERVAL(0.); }
        else midD = Hull(Mid(D));
        //showTuple(orderVec[ord-1].errorTuple[i]); cout << " D = " << D << endl;
        //cout << " midD = " << midD << endl;
        //cout << " (D-midD) = " << (D-midD) << endl;
        if (orderVec[ord-1].errorEven[i]) {
            /*
            cout << "polynomial contribution" << endl;
            cout << " errorCoefficient = " << orderVec[ord-1].errorCoefficient[i] << endl;
            cout << " integral = " << integral(orderVec[ord-1].errorTuple[i],rad) << endl;
            cout << " midD = " << midD << endl;
            cout << " term -> " << orderVec[ord-1].errorCoefficient[i] * integral(orderVec[ord-1].errorTuple[i],rad) * midD << endl;
            */
            I += orderVec[ord-1].errorCoefficient[i] * integral(orderVec[ord-1].errorTuple[i],rad) * midD;
        }
/*
        cout << "error contribution" << endl;
        INTERVAL IDmidD = (D-midD);
        cout << " (D-midD) = " << IDmidD << " wid  = " << Diam(IDmidD) << endl;
        INTERVAL IerrorCoefficient = orderVec[ord-1].errorCoefficient[i];
        cout << " errorCoefficient = " << IerrorCoefficient << " wid  = " << Diam(IerrorCoefficient) << endl;
        INTERVAL Ierror = error(orderVec[ord-1].errorTuple[i],midX);
        cout << " error = " << Ierror << " wid  = " << Diam(Ierror) << endl;

        INTERVAL Iterm = IDmidD;
        cout << " Iterm = " << Iterm << " wid  = " << Diam(Iterm) << endl;
        Iterm *= Ierror;
        cout << " Iterm = " << Iterm << " wid  = " << Diam(Iterm) << endl;
        Iterm *= IerrorCoefficient;
        cout << " Iterm = " << Iterm << " wid  = " << Diam(Iterm) << endl;

        cout << " term -> " << orderVec[ord-1].errorCoefficient[i] * error(orderVec[ord-1].errorTuple[i],midX) * (D-midD) << endl;
*/
        I += orderVec[ord-1].errorCoefficient[i] * error(orderVec[ord-1].errorTuple[i],midX) * (D-midD);
        //I += error(orderVec[ord-1].errorTuple[i],x-mid) * (D-midD);
        //cin.get();
    }
    return I;
}

INTERVAL Tensor::functionErrorEval(INTERVAL_VECTOR & midX, int ord)
{
    //cout << "errorEval" << " mid = " << mid << " rad = " << rad << " ord = " << ord << endl;
    INTERVAL I = 0.0;
    INTERVAL D;
    for (unsigned int i=0;i<orderVec[ord-1].errorIndex.size();i++) {
        D = t[orderVec[ord-1].errorIndex[i]];
        //cout << " errorDerivative = t[" << orderVec[ord-1].errorIndex[i] << "] = " << t[orderVec[ord-1].errorIndex[i]] << endl;
        I += orderVec[ord-1].errorCoefficient[i] * error(orderVec[ord-1].errorTuple[i],midX) * D;
    }
    return I;
}

INTERVAL Tensor::functionPolynomialEval(INTERVAL_VECTOR & midX, int ord)
{
    //cout << "polynomialEval" << " mid = " << mid << " rad = " << rad << " ord = " << ord << endl;
    INTERVAL I = t[fIndex[0]];
    INTERVAL D;
    for (int n=0;n<ord-1;n++)
        for (unsigned int i=0;i<orderVec[n].errorIndex.size();i++) {
            D = t[orderVec[n].errorIndex[i]];
            I += orderVec[n].errorCoefficient[i] * error(orderVec[n].errorTuple[i],midX) * D;
        }
    return I;
}


INTERVAL Tensor::errorEval(INTERVAL_VECTOR & x, INTERVAL_VECTOR & mid, INTERVAL_VECTOR & rad, int ord, INTERVAL & P)
{
    P = 0.0;
    INTERVAL I = 0.0;
    INTERVAL D, midD;
    INTERVAL_VECTOR midX = x-mid;
    for (unsigned int i=0;i<orderVec[ord-1].errorIndex.size();i++) {
        D = t[orderVec[ord-1].errorIndex[i]];
        if (D.unbounded()) {midD = INTERVAL(0.); }
        else midD = Hull(Mid(D));
        if (orderVec[ord-1].errorEven[i]) {
            P += orderVec[ord-1].errorCoefficient[i] * integral(orderVec[ord-1].errorTuple[i],rad) * midD;
        }
        I += orderVec[ord-1].errorCoefficient[i] * error(orderVec[ord-1].errorTuple[i],midX) * (D-midD);
    }
    return I;
}


INTERVAL Tensor::errorDerivatives(int ord)
{
    INTERVAL I = 0.0;
    INTERVAL D;
    for (unsigned int i=0;i<orderVec[ord-1].errorIndex.size();i++) {
        D = t[orderVec[ord-1].errorIndex[i]];
        //showTuple(orderVec[ord-1].errorTuple[i]); cout << " D = " << D << endl;
        //I += D;
        I += orderVec[ord-1].errorCoefficient[i]*D;
    }
    return I;
}

INTERVAL Tensor::quadratureError(INTERVAL_VECTOR & x)
{
    INTERVAL P;
    return quadratureError(x,nOrder,P);
}

INTERVAL Tensor::quadratureError(INTERVAL_VECTOR & x, unsigned int ord)
{
    INTERVAL P;
    return quadratureError(x,ord,P);
}

INTERVAL Tensor::quadratureEval(INTERVAL_VECTOR & x)
{
    INTERVAL vol = 1.0;
	for (int i = 1; i <= Dimension(x); i++) vol *= Diam(x(i));
	INTERVAL_VECTOR mid=Hull(Mid(x));
	INTERVAL_VECTOR rad=Hull(Diam(x))/2;
	int maxInd = 0;
	maxInd = orderIndex[nOrder];
    //cout << "maxInd = " << maxInd << endl;
    //cout << "computeTensor" << endl;
	computeTensor(x,maxInd);
    //cout << "computeTensor end" << endl;
	INTERVAL R, newR;
	unsigned int ord = nOrder;
	R = errorEval(x,mid,rad,ord);
	/*
    //cout << "R(" << ord << ") = " << R << " wid = " << Diam(R) << endl;
	for (ord=nOrder-1;ord>=1;ord--) {
        newR = errorEval(x,mid,rad,ord);
        //cout << "R(" << ord << ") = " << newR << " wid = " << Diam(newR) << endl;
        if ((R.unbounded())||(Diam(newR)<Diam(R))) {
            //cout << "better!" << endl;
            R = newR;
        }
        else {
            //cout << "not better!" << endl;
            ord++;
            break;
        }
	}
	if (ord==0) ord = 1;
	*/
	//INTERVAL D = errorDerivatives(ord);
    //cout << "wid D = " << Diam(D) << endl;
	//cout << "Best order = " << ord << " R = " << R << " wid = " << Diam(R) << endl;
	//maxInd = orderIndex[ord];
	if (ord%2==0) maxInd = orderIndex[ord-2];
	else maxInd = orderIndex[ord-1];
	//cout << "maxInd = " << maxInd << endl;
	computeTensor(mid,maxInd);
	INTERVAL P = polynomialEval(x,mid,rad,ord);
    //cout << "P = " << P << endl;
    //cout << "R = " << R << endl;
	INTERVAL T = vol*(P+R);
	//cout << "x -> " << x << " q -> " << T << " wid = " << Diam(T) << endl;
	//cin.get();
    return T;

}

INTERVAL Tensor::quadratureEval(INTERVAL_VECTOR & x, unsigned int ord)
{
    INTERVAL vol = 1.0;
	for (int i = 1; i <= Dimension(x); i++) vol *= Diam(x(i));
	INTERVAL_VECTOR mid=Hull(Mid(x));
	INTERVAL_VECTOR rad=Hull(Diam(x))/2;
	int maxInd = 0;
	maxInd = orderIndex[ord];
	computeTensor(x,maxInd);
	INTERVAL R, newR;
	R = errorEval(x,mid,rad,ord);
	if (ord%2==0) maxInd = orderIndex[ord-2];
	else maxInd = orderIndex[ord-1];
	computeTensor(mid,maxInd);
    INTERVAL P = polynomialEval(x,mid,rad,ord);
    //cout << "P = " << vol*P << endl;
    //cout << "R = " << vol*R << endl;
    INTERVAL T = vol*(P+R);
	//cout << "x -> " << x << " q -> " << T << " wid = " << Diam(T) << endl;
	//cin.get();
    return T;
}

INTERVAL Tensor::quadratureBestEval(INTERVAL_VECTOR & x, unsigned int & ord)
{
    //cout << "nOrder = " << nOrder << endl;
    //cin.get();
    if (ord==nOrder) return quadratureEval(x,ord);
    INTERVAL vol = 1.0;
	for (int i = 1; i <= Dimension(x); i++) vol *= Diam(x(i));
	INTERVAL_VECTOR mid=Hull(Mid(x));
	INTERVAL_VECTOR rad=Hull(Diam(x))/2;
	int maxInd = 0;
	maxInd = orderIndex[ord+1];
	computeTensor(x,maxInd);
	INTERVAL R, newR;
	R = errorEval(x,mid,rad,ord);
	//cout << "ord = " << ord << endl;
	//cout << "R = " << R << " w = " << Diam(R) << endl;
	newR = errorEval(x,mid,rad,ord+1);
	//cout << "newR = " << newR << " w = " << Diam(newR) << endl;
	//cin.get();
	if (Diam(newR)<Diam(R)) {
        R = newR;
        ord++;
        return quadratureBestEval(x,ord);
	}
	//cout << "ord = " << ord << endl;
	if (ord%2==0) maxInd = orderIndex[ord-2];
	else maxInd = orderIndex[ord-1];
	computeTensor(mid,maxInd);
    INTERVAL P = polynomialEval(x,mid,rad,ord);
    //cout << "P = " << vol*P << endl;
    //cout << "R = " << vol*R << endl;
    INTERVAL T = vol*(P+R);
	//cout << "x -> " << x << " q -> " << T << " wid = " << Diam(T) << endl;
	//cin.get();
    return T;
}

INTERVAL Tensor::functionEval(INTERVAL_VECTOR & x)
{
    return functionEval(x,nOrder);
}

INTERVAL Tensor::functionEval(INTERVAL_VECTOR & x, unsigned int ord)
{
    if (ord == 0) return f(x);
	INTERVAL_VECTOR mid=Hull(Mid(x));
    INTERVAL_VECTOR midX = x-mid;
	int maxInd = 0;
	maxInd = orderIndex[ord];
	computeTensor(x,maxInd);
	INTERVAL F = t[fIndex[0]];
	INTERVAL R = functionErrorEval(midX,ord);
	maxInd = orderIndex[ord-1];
	computeTensor(mid,maxInd);
    INTERVAL P = functionPolynomialEval(midX,ord);
    //cout << "P = " << P << endl;
    //cout << "R = " << R << endl;
    INTERVAL T = P+R;
	//cout << "x -> " << x << " q -> " << T << " wid = " << Diam(T) << endl;
	//cin.get();
    return (T & F);
}

INTERVAL Tensor::quadratureError(INTERVAL_VECTOR & x, unsigned int ord, INTERVAL & P)
{
    INTERVAL vol = 1.0;
	for (int i = 1; i <= Dimension(x); i++) vol *= Diam(x(i));
	INTERVAL_VECTOR mid=Hull(Mid(x));
	INTERVAL_VECTOR rad=Hull(Diam(x))/2;
	computeTensor(x,orderIndex[ord]);
	INTERVAL R = errorEval(x,mid,rad,ord,P);
	P *= vol;
	INTERVAL T = vol*R;
	//cout << "x -> " << x << " q -> " << T << " wid = " << Diam(T) << endl;
	//cin.get();
    return T;
}

INTERVAL Tensor::quadraturePolynomial(INTERVAL_VECTOR & x, unsigned int ord)
{
    INTERVAL vol = 1.0;
	for (int i = 1; i <= Dimension(x); i++) vol *= Diam(x(i));
	INTERVAL_VECTOR mid=Hull(Mid(x));
	INTERVAL_VECTOR rad=Hull(Diam(x))/2;
	int maxInd;
	if (ord%2==0) maxInd = orderIndex[ord-2];
	else maxInd = orderIndex[ord-1];
	computeTensor(mid,maxInd);
    INTERVAL P = polynomialEval(x,mid,rad,ord);
    //cout << "PP = " << vol*P << endl;
    INTERVAL T = vol*P;
	//cout << "x -> " << x << " q -> " << T << " wid = " << Diam(T) << endl;
	//cin.get();
    return T;
}

INTERVAL Tensor::quadratureTaylor(INTERVAL_VECTOR & x, unsigned int ord, INTERVAL & P, INTERVAL & R)
{
    INTERVAL PP;
    R = quadratureError(x,ord,P);
    //cout << "R+PR = " << R+P << endl;
    //cout << "PR = " << P << endl;
    PP = quadraturePolynomial(x,ord);
    //cout << "PP = " << PP << endl;
    P += PP;
    //cout << "P = " << P << endl;
    //cout << "R = " << R << endl;
    INTERVAL T = P+R;
	//cout << "x -> " << x << " q -> " << T << " wid = " << Diam(T) << endl;
	//cin.get();
    return T;
}

INTERVAL Tensor::quadratureDerivatives(INTERVAL_VECTOR & x, unsigned int ord)
{
	int maxInd = orderIndex[ord];
	computeTensor(x,maxInd);
	INTERVAL R = errorDerivatives(ord);
    return R;
}

void Tensor::setTuples(vector<pair<int *,INTERVAL> >* ti, int t[], int v, int n, INTERVAL c)
{
    INTERVAL cc = c;
    if (v>=nVars-1) {
        t[v] = n;
        for (int j=2;j<=n;j++) cc /= j;
        int *tt = new int[nVars];
        for (int i=0;i<nVars;i++) tt[i] = t[i];
		pair<int *,INTERVAL> p = pair<int *,INTERVAL>(tt,cc);
        ti->push_back(p);
    }
    else {
        for (int i=n;i>=0;i--) {
            t[v] = i;
            cc = c;
            for (int j=2;j<=i;j++) cc /= j;
            setTuples(ti,t,v+1,n-i,cc);
        }
    }
    return;
}



void Tensor::showConstants()
{
    cout << "*********** CONSTANTS ***********" << endl;
    for (unsigned int i=0; i<c->size(); i++) {
        cout << "c(" << i << ") = " << (*c)[i] << endl;
    }
}

void Tensor::showDifferentiationVector()
{
    cout << "*********** VECTOR ***********" << endl;
    for (unsigned int i=0; i<d->size(); i++) {
        pair<OP,unsigned long int *> p = (*d)[i];
        OP op = p.first;
        unsigned long int* a = p.second;
        unsigned long int arg1 = a[0];
        unsigned long int arg2 = a[1];
        cout << "d(" << i << ") = " << op << " ; " << arg1 ;
        if (arg2==-1) cout << " ; " << -1 ;
        else cout << " ; " << arg2;
        for (int j = 1; j<=nVars; j++) {
            if (a[j+1]==-1) cout << " ; " << -1 ;
            else cout << " ; " << a[j+1];
        }
        cout << endl;
    }
}

void Tensor::showTextVector()
{
    cout << "*********** TEXT VECTOR ***********" << endl;
    for (unsigned int i=0; i<d->size(); i++) {
        pair<OP,unsigned long int *> p = (*d)[i];
        OP op = p.first;
        unsigned long int* a = p.second;
        unsigned long int arg1 = a[0];
        unsigned long int arg2 = a[1];
        cout << "d(" << i << ") = ";
        switch ( op ) {
            case CONS: cout << "INTERVAL(" << (*c)[arg1] << ")" << endl; break;
            case VAR:  cout << "x(" << arg1 << ")" << endl; break;
            case ADD:  cout << "d(" << arg1 << ") + d(" << arg2 << ")" << endl; break;
            case SUB:  cout << "d(" << arg1 << ") - d(" << arg2 << ")" << endl; break;
            case PROD: cout << "d(" << arg1 << ") * d(" << arg2 << ")" << endl; break;
            case DIV:  cout << "d(" << arg1 << ") / d(" << arg2 << ")" << endl; break;
            case POW:  cout << "Power(d(" << arg1 << "),d(" << arg2 << "))" << endl; break;
            case SIN:  cout << "Sin(d(" << arg1 << "))" << endl; break;
            case COS:  cout << "Cos(d(" << arg1 << "))" << endl; break;
            case SQRT:  cout << "Sqrt(d(" << arg1 << "))" << endl; break;
            case SQR:  cout << "Sqr(d(" << arg1 << "))" << endl; break;
            case TAN:  cout << "Tan(d(" << arg1 << "))" << endl; break;
            case COT:  cout << "Cot(d(" << arg1 << "))" << endl; break;
            case EXP:  cout << "Exp(d(" << arg1 << "))" << endl; break;
            case LOG:  cout << "Log(d(" << arg1 << "))" << endl; break;
            case SINH: cout << "Sinh(d(" << arg1 << "))" << endl; break;
            case COSH: cout << "Cosh(d(" << arg1 << "))" << endl; break;
            case TANH: cout << "Tanh(d(" << arg1 << "))" << endl; break;
            case COTH: cout << "Coth(d(" << arg1 << "))" << endl; break;
            case ARCTAN: cout << "ArcTan(d(" << arg1 << "))" << endl; break;
            case PI:   cout << "Pi" << endl; break;
            default:   cout << "Error: unknown operation " << op  << endl; cin.get();
        }
    }
    showIndexes();
}

void Tensor::writeFunction()
{
    string filename2 = "function.tsr";
    ofstream file (filename2.c_str());
    if (file.is_open())
    {
        file << "#include \"Tensor.h\"" << endl;
        file << endl;
        file << "int Tensor::getFunctionDef()" << endl;
        file << "{" << endl;
        file << "    maxOrder = " << maxOrder << ";" << endl;
        file << "    taylorIndex.clear();" << endl;
        for (unsigned int i=0; i<taylorIndex.size(); i++) {
            file << "    taylorIndex.push_back(" << taylorIndex[i] << ");" << endl;
        }
        file << "    return " << fNumber << ";" << endl;
        file << "}" << endl;
        file << endl;

        file << "void Tensor::getFunctionDat()" << endl;
        file << "{" << endl;
        file << "    fDat.clear();" << endl;
        file << "    int *t;" << endl;
        for (unsigned int i=0; i<fDat.size(); i++) {
            file << "    t = new int[3];" << endl;
            int *t0 = fDat[i];
            file << "    t[0] = " << t0[0] << ";" << endl;
            file << "    t[1] = " << t0[1] << ";" << endl;
            file << "    t[2] = " << t0[2] << ";" << endl;
            file << "    fDat.push_back(t);" << endl;
        }
        file << "}" << endl;
        file << endl;

        file << "void Tensor::computeTensor(INTERVAL_VECTOR & x, int n)" << endl;
        file << "{" << endl;
        int k = 0;
        unsigned long int nextTaylor = orderIndex[k];
        for (unsigned int i=0; i<d->size(); i++) {
            pair<OP,unsigned long int *> p = (*d)[i];
            OP op = p.first;
            unsigned long int* a = p.second;
            unsigned long int arg1 = a[0];
            unsigned long int arg2 = a[1];
            file << "    t[" << i << "] = ";
            switch ( op ) {
                case CONS: file << (*c)[arg1]; break;
                case VAR:  file << "x(" << arg1 << ")"; break;
                case ADD:  file << "t[" << arg1 << "] + t[" << arg2 << "]"; break;
                case SUB:  file << "t[" << arg1 << "] - t[" << arg2 << "]"; break;
                case PROD: file << "t[" << arg1 << "] * t[" << arg2 << "]"; break;
                case DIV:  file << "t[" << arg1 << "] / t[" << arg2 << "]"; break;
                case POW:  {
                    if ((*d)[arg2].first==CONS)
                         file << "Power(t[" << arg1 << "]," << (*c)[(*d)[arg2].second[0]] << ")";
                    else file << "Power(t[" << arg1 << "],t[" << arg2 << "])";
                    break;
                }
                case SIN:  file << "Sin(t[" << arg1 << "])"; break;
                case COS:  file << "Cos(t[" << arg1 << "])"; break;
                case SQRT:  file << "Sqrt(t[" << arg1 << "])"; break;
                case SQR:  file << "Sqr(t[" << arg1 << "])"; break;
                case TAN:  file << "Tan(t[" << arg1 << "])"; break;
                case COT:  file << "Cot(t[" << arg1 << "])"; break;
                case EXP:  file << "Exp(t[" << arg1 << "])"; break;
                case LOG:  file << "Log(t[" << arg1 << "])"; break;
                case SINH: file << "Sinh(t[" << arg1 << "])"; break;
                case COSH: file << "Cosh(t[" << arg1 << "])"; break;
                case TANH: file << "Tanh(t[" << arg1 << "])"; break;
                case COTH: file << "Coth(t[" << arg1 << "])"; break;
                case ARCTAN: file << "ArcTan(t[" << arg1 << "])"; break;
                case PI:   file << "INTERVAL::Pi"; break;
            }
            file << ";" << endl;
            if (i==nextTaylor) {
                file << "    if (n==" << i << ") return;" << endl;
                k++;
                nextTaylor = orderIndex[k];
            }
        }
        file << "    cout << \"Error: cannot compute the required tensor!\" << endl;" << endl;
        file << "}" << endl;
        file.close();
    }
	cout << "--taylor derivatives written in file " << filename2 << endl;
}


void Tensor::writeFunctionPaver()
{
    string filename2 = "functionPaver.tsr";
    ofstream file (filename2.c_str());
    if (file.is_open())
    {
        file << "#include \"Tensor.h\"" << endl;
        file << endl;
        file << "int Tensor::getFunctionDef()" << endl;
        file << "{" << endl;
        file << "    maxOrder = " << maxOrder << ";" << endl;
        file << "    taylorIndex.clear();" << endl;
        for (unsigned int i=0; i<taylorIndex.size(); i++) {
            file << "    taylorIndex.push_back(" << taylorIndex[i] << ");" << endl;
        }
        file << "    return " << fNumber << ";" << endl;
        file << "}" << endl;
        file << endl;

        file << "void Tensor::computeTensor(INTERVAL x[], int n)" << endl;
        file << "{" << endl;
        int k = 0;
        unsigned long int nextTaylor = orderIndex[k];
        for (unsigned int i=0; i<d->size(); i++) {
            pair<OP,unsigned long int *> p = (*d)[i];
            OP op = p.first;
            unsigned long int* a = p.second;
            unsigned long int arg1 = a[0];
            unsigned long int arg2 = a[1];
            file << "    t[" << i << "] = ";
            switch ( op ) {
                case CONS: file << "INTERVAL("<< (*c)[arg1] << ".0)"; break;
                case VAR:  file << "x[" << arg1-1 << "]"; break;
                case ADD:  file << "t[" << arg1 << "] + t[" << arg2 << "]"; break;
                case SUB:  file << "t[" << arg1 << "] - t[" << arg2 << "]"; break;
                case PROD: file << "t[" << arg1 << "] * t[" << arg2 << "]"; break;
                case DIV:  file << "t[" << arg1 << "] / t[" << arg2 << "]"; break;
                case POW:  {
                    if ((*d)[arg2].first==CONS)
                         file << "pow(t[" << arg1 << "]," << (*c)[(*d)[arg2].second[0]] << ")";
                    else file << "pow(t[" << arg1 << "],t[" << arg2 << "])";
                    break;
                }
                case SIN:  file << "sin(t[" << arg1 << "])"; break;
                case COS:  file << "cos(t[" << arg1 << "])"; break;
                case SQRT:  file << "sqrt(t[" << arg1 << "])"; break;
                case SQR:  file << "sqr(t[" << arg1 << "])"; break;
                case TAN:  file << "tan(t[" << arg1 << "])"; break;
                //case COT:  file << "cot(t[" << arg1 << "])"; break;
                case EXP:  file << "exp(t[" << arg1 << "])"; break;
                case LOG:  file << "log(t[" << arg1 << "])"; break;
                case SINH: file << "sinh(t[" << arg1 << "])"; break;
                case COSH: file << "cosh(t[" << arg1 << "])"; break;
                case TANH: file << "tanh(t[" << arg1 << "])"; break;
                //case COTH: file << "coth(t[" << arg1 << "])"; break;
                case ARCTAN: file << "atan(t[" << arg1 << "])"; break;
                case PI:   file << "INTERVAL::pi"; break;
            }
            file << ";" << endl;
            if (i==nextTaylor) {
                file << "    if (n==" << i << ") return;" << endl;
                k++;
                nextTaylor = orderIndex[k];
            }
        }
        file << "    cout << \"Error: cannot compute the required tensor!\" << endl;" << endl;
        file << "}" << endl;
        file.close();
    }
	cout << "--taylor derivatives written in file " << filename2 << endl;
}

bool Tensor::isEven(int t[])
{
    for (int i=0;i<nVars;i++) if (t[i] % 2 == 1) return false;
    return true;
}

void Tensor::showTuple(int* t)
{
    if (nVars==1) cout << "<" << t[0] << ">";
    else {cout << "<"; for (int i=0;i<nVars-1;i++) cout << t[i] << ","; cout << t[nVars-1] << ">";}
}

void Tensor::showIndexes()
{
    cout << "nOrder = " << nOrder << endl;
    cout << "orderIndex: ";
    for (unsigned int i=0; i<orderIndex.size(); i++) cout << orderIndex[i] << " ";
    cout << endl;
    cout << "taylorIndex: "  << endl;
    for (unsigned int i=0; i<taylorTuple.size(); i++) {
        int* tup = taylorTuple[i];
        if (nVars==1) cout << "<" << tup[0] << ">";
        else {cout << "<"; for (int ii=0;ii<nVars-1;ii++) cout << tup[ii] << ","; cout << tup[nVars-1] << ">";}
        cout << " -> ";
        cout << taylorIndex[i] << " " << taylorCoefficient[i];
        if (evenTuple[i]) cout << " (even)";
        else cout << " (odd)";
        cout << endl;
    }
    cout << "polyIndex: " << endl;
    for (unsigned int i=0; i<orderVec[nOrder-1].polyIndex.size(); i++) {
        int* tup = orderVec[nOrder-1].polyTuple[i];
        if (nVars==1) cout << "<" << tup[0] << ">";
        else {cout << "<"; for (int ii=0;ii<nVars-1;ii++) cout << tup[ii] << ","; cout << tup[nVars-1] << ">";}
        cout << " -> ";
        cout << orderVec[nOrder-1].polyIndex[i] << " " << orderVec[nOrder-1].polyCoefficient[i];
        //cout << " " << orderVec[nOrder-1].polyDerivative[i];
        cout << " " << t[orderVec[nOrder-1].polyIndex[i]];
        cout << endl;
    }
    cout << "errorIndex: " << endl;
    for (unsigned int i=0; i<orderVec[nOrder-1].errorIndex.size(); i++) {
        int* tup = orderVec[nOrder-1].errorTuple[i];
        if (nVars==1) cout << "<" << tup[0] << ">";
        else {cout << "<"; for (int ii=0;ii<nVars-1;ii++) cout << tup[ii] << ","; cout << tup[nVars-1] << ">";}
        cout << " -> ";
        cout << orderVec[nOrder-1].errorIndex[i] << " " << orderVec[nOrder-1].errorCoefficient[i];
        //cout << " " << orderVec[nOrder-1].errorDerivative[i];
        cout << " " << t[orderVec[nOrder-1].errorIndex[i]];
        if (orderVec[nOrder-1].errorEven[i]) cout << " (even)";
        else cout << " (odd)";
        cout << endl;
    }

}
