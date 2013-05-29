#ifndef MENU_H
#define MENU_H

#include <vector>
#include <Interval.h>
#include <IntervalVector.h>
#include <Functions.h>

#include <iostream>
#include <fstream>
using namespace std;


using std::vector;

class Menu
{
    public:
        Menu();
        virtual ~Menu();

        enum OP {CONS, VAR, ADD, SUB, PROD, DIV, POW, SIN, COS, PI, SQRT, SQR, TAN, COT, EXP, LOG, SINH, COSH, TANH, COTH, ARCTAN};

        INTERVAL getBound(int i, bool side);
        INTERVAL getInterval(int i);
        INTERVAL evalOp(vector<int*> e ,int j, vector<INTERVAL> &d);
        vector<int*> getFExp() {return fDat;}
        int getFNumber() {return fNumber;}
        int getnVars() {return nVars;}
        int getnOrder() {return nOrder;}
        double getPrecision() {return precision;}
        bool getAdaptative() {return adaptative;}
        bool getConstrained() {return constrained;}
        void getFDat();
        void saveFDat();
        void getBoundBox();
        void saveBoundBox();
        void saveDefaultBoundBox();
        int inputMenu(string menu);
        int inputOrder(string menu);
        void inputFunction(string menu);
        void editFunction(string menu);
        void selectFunction(string menu);
        void createFunction(string menu);
        void inputBox(string menu);
        double inputPrecision(string menu);
        int getOpNumber(string op);
        void getFunctionList();
        void saveFunctionList();
        void getCurrentSettings();
        void saveCurrentSettings();
        void updateMainOptions();
        void showBoundingExpression(int bound);
        string getExpression(vector<int*> e, bool show);
        string extractArgument(string str, bool &isOp, int &num);
        bool verifyOperator(int op, int arg1, int arg2, int bound);

    protected:
        vector<string> mainOptions;
        bool constrained, adaptative, newFunction;
        double precision;
        int fNumber, nVars, nOrder, newVars, maxOrder;
        string fName;
        vector<int*> fDat;
        vector<string> fNameList;
        vector<int> nVarsList;
        vector<string> fBox;
        vector<vector<int*> > boundBox;
    private:
};

#endif // MENU_H
