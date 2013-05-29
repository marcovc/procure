#ifndef TENSOR_H
#define TENSOR_H

#include <vector>
#include <Interval.h>
#include <IntervalVector.h>
#include <Functions.h>

using std::vector;

typedef struct _orderS{
    vector<int> polyIndex;
    vector<int *> polyTuple;
    vector<INTERVAL> polyCoefficient;
    vector<INTERVAL> polyDerivative;
    vector<int> errorIndex;
    vector<int *> errorTuple;
    vector<int> errorEven;
    vector<INTERVAL> errorCoefficient;
    vector<INTERVAL> errorDerivative;
} orderS;

class Tensor
{
    public:
        enum OP {CONS, VAR, ADD, SUB, PROD, DIV, POW, SIN, COS, PI, SQRT, SQR, TAN, COT, EXP, LOG, SINH, COSH, TANH, COTH, ARCTAN};
        Tensor(int fN, int nV, vector<int*> fexp);
        virtual ~Tensor();

        void setTuples(vector<pair<int *,INTERVAL> >* ti, int t[], int v, int n, INTERVAL c);
        void setTuples(int order);

        void computeTensor(INTERVAL_VECTOR & x, int n);
        INTERVAL integral(int t[], INTERVAL_VECTOR & r);
        INTERVAL polynomialEval(INTERVAL_VECTOR & x, INTERVAL_VECTOR & mid, INTERVAL_VECTOR & rad, int ord);
        INTERVAL functionPolynomialEval(INTERVAL_VECTOR & midX, int ord);
        INTERVAL error(int t[], INTERVAL_VECTOR & x);
        INTERVAL errorEval(INTERVAL_VECTOR & x, INTERVAL_VECTOR & mid, INTERVAL_VECTOR & rad, int ord);
        INTERVAL errorEval(INTERVAL_VECTOR & x, INTERVAL_VECTOR & mid, INTERVAL_VECTOR & rad, int ord, INTERVAL & P);
        INTERVAL functionErrorEval(INTERVAL_VECTOR & midX, int ord);
        INTERVAL errorDerivatives(int ord);
        INTERVAL quadratureDerivatives(INTERVAL_VECTOR & x, unsigned int ord);
        INTERVAL quadratureEval(INTERVAL_VECTOR & x);
        INTERVAL quadratureError(INTERVAL_VECTOR & x);
        INTERVAL quadratureError(INTERVAL_VECTOR & x, unsigned int ord);
        INTERVAL functionEval(INTERVAL_VECTOR & x);
        INTERVAL functionEval(INTERVAL_VECTOR & x, unsigned int ord);
        INTERVAL quadratureEval(INTERVAL_VECTOR & x, unsigned int ord);
        INTERVAL quadratureBestEval(INTERVAL_VECTOR & x, unsigned int & ord);
        INTERVAL quadratureError(INTERVAL_VECTOR & x, unsigned int ord, INTERVAL & P);
        INTERVAL quadraturePolynomial(INTERVAL_VECTOR & x, unsigned int ord);
        INTERVAL quadratureTaylor(INTERVAL_VECTOR & x, unsigned int ord, INTERVAL & P, INTERVAL & R);
        INTERVAL f(INTERVAL_VECTOR & x);
        void getT(INTERVAL_VECTOR & x, int nObs, int maxObs,int obsIndex[],INTERVAL fwr[]);
        INTERVAL getD(INTERVAL_VECTOR & x, INTERVAL df[]);

        void writeFunction();
        void writeFunctionPaver();
        int getFunctionDef();
        void getFunctionDat();
        void setFunction();
        void setFunction(string functionName);
        bool setOrder(int order);
        void setCurrentSettings();
        unsigned long int setOp(OP op);
        unsigned long int setOp(OP op, unsigned long int arg1);
        unsigned long int setOp(OP op, unsigned long int arg1, unsigned long int arg2);

        void setDerivative();
        void getDerivative();
        unsigned long int setDerivative(unsigned long int i, int t[], int v, int coef);
        unsigned long int setDerivative(unsigned long int i, int v);

        void showTuple(int* t);
        void showConstants();
        void showDifferentiationVector();
        void showTextVector();
        void showIndexes();
        bool isEven(int t[]);
        bool isCorrupted() { return maxOrder<0; }

    protected:
        vector<orderS> orderVec;
        bool changed;
        vector<int*> fDat;
        INTERVAL *x_init;
        vector<pair<OP,unsigned long int *> >* d;
        vector<int>* c;
        INTERVAL *t;
        int fNumber, nVars, nOrder, maxOrder, endIndex;
        vector<int> orderIndex;
        vector<int> taylorIndex;
        vector<INTERVAL> taylorCoefficient;
        vector<int *> taylorTuple;
        vector<bool> evenTuple;
        vector<vector<pair<int *,INTERVAL> >*>* ty;
        vector<int> polyIndex;
        vector<int> fIndex;
        vector<int *> polyTuple;
        vector<INTERVAL> polyCoefficient;
        vector<INTERVAL> polyDerivative;
        vector<INTERVAL> fInterval;
        vector<int> errorIndex;
        vector<int *> errorTuple;
        vector<int> errorEven;
        vector<INTERVAL> errorCoefficient;
        vector<INTERVAL> errorDerivative;
    private:
};

#endif // TENSOR_H
