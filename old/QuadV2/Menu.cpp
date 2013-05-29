#include "Menu.h"
#include <iostream>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <Utilities.h>
#include <stdlib.h>
#include <process.h>
#include <ctype.h>


using namespace std;

Menu::Menu()
{
    newFunction = false;
    getFunctionList();
    getCurrentSettings();
    getBoundBox();
    getFDat();
    for (unsigned int i=0;i<7;i++) mainOptions.push_back("");
    int op = -1;
    string menu;
    while (op!=7) {
        updateMainOptions();
        menu = "Choose an option between 1 and 6 to change settings or 7 to execute:\n";
        for (unsigned int i=0;i<mainOptions.size();i++) menu += mainOptions[i] + "\n";
        op = inputMenu(menu);
        switch (op) {
            case 1:
                inputFunction(menu);
                break;
            case 2:
                inputBox(menu);
                break;
            case 3:
                constrained = !constrained;
                saveCurrentSettings();
                break;
            case 4:
                precision = inputPrecision(menu);
                saveCurrentSettings();
                break;
            case 5:
                adaptative = !adaptative;
                saveCurrentSettings();
                break;
            case 6:
                nOrder = inputOrder(menu);
                saveCurrentSettings();
                break;
            case 7:
                break;
        }
    }
}

Menu::~Menu()
{
    //dtor
}

void Menu::updateMainOptions()
{
    string s;
    s = "   1) Integrand function: \n";
    stringstream ss;
    ss << nVars;
    s += "         " + fName;
    mainOptions[0] = s;
    s = "   2) Integration box: \n";
    for (unsigned int i=0;i<nVars-1;i++) {
        stringstream ssi; ssi << i+1;
        s += "         x(" + ssi.str() + ") = [" + fBox[2*i] + "," + fBox[2*i+1]+ "]\n";
    }
    stringstream ssn; ssn << nVars;
    s += "         x(" + ssn.str() + ") = [" + fBox[2*(nVars-1)] + "," + fBox[2*(nVars-1)+1]+ "]";
    mainOptions[1] = s;
    if (constrained) s = "   3) Satisfy innequality constraint";
    else s = "   3) Ignore innequality constraint";
    mainOptions[2] = s;
    s = "   4) Precision requirement: ";
    stringstream ss1;
    ss1 << precision;
    s += ss1.str();
    mainOptions[3] = s;
    if (adaptative) s = "   5) Adaptative strategy: Worst First Box Selection";
    else s = "   5) Adaptative strategy: Fair Box Selection";
    mainOptions[4] = s;
    stringstream ss2;
    ss2 << nOrder;
    s = "   6) Taylor model order: " + ss2.str() ;
    mainOptions[5] = s;
    s = "   7) Compute Integral";
    mainOptions[6] = s;
}


double Menu::inputPrecision(string menu)
{
    string op;
    double p = 0;
    menu += ">> 4 \n";
    menu += ">> Enter the new required precision: ";
    do {
        system("cls");
        cout << menu;
        getline(cin,op);
        p = atof(op.c_str());
    } while (p<=0);
    return p;
}

int Menu::inputOrder(string menu)
{
    string op;
    int n;
    menu += ">> 6 \n";
    menu += ">> Enter the new Taylor model order: ";
    do {
        system("cls");
        cout << menu;
        cin >> op;
        n = atoi(op.c_str());
    } while (n<1);
    return n;
}

int Menu::getOpNumber(string op)
{
    //cout << "operator = " << op << endl;
    if (op=="cons") return CONS;
    if (op=="var") return VAR;
    if (op=="+") return ADD;
    if (op=="-") return SUB;
    if (op=="*") return PROD;
    if (op=="/") return DIV;
    if (op=="^") return POW;
    if (op=="sin") return SIN;
    if (op=="cos") return COS;
    if (op=="pi") return PI;
    if (op=="sqrt") return SQRT;
    if (op=="sqr") return SQR;
    if (op=="tan") return TAN;
    if (op=="cot") return COT;
    if (op=="exp") return EXP;
    if (op=="log") return LOG;
    if (op=="sinh") return SINH;
    if (op=="cosh") return COSH;
    if (op=="tanh") return TANH;
    if (op=="coth") return COTH;
    if (op=="arctan") return ARCTAN;
    //cout << "Invalid operator!" << endl;
    return -1;
}
string Menu::extractArgument(string str, bool &isOp, int &num)
{
    isOp = false;
    num = -1;
    string::iterator It = str.begin();
    bool ok = true;
    int first = 0;
    while ( (It != str.end()) && ok ) {
        if ( (*It != ' ') && (*It != '(') && (*It != ')') && (*It != ',')) {
            ok = false;
        }
        else {
            *It++;
            first++;
        }
    }
    int last = first;
    int next = 0;
    if (It==str.end()) {
        return str;
    }
    if (!isdigit(*It) && !isalpha(*It)) {
        if ( (*It == '+') || (*It == '-') || (*It == '*') || (*It == '/') || (*It == '^')) {
            string s1 (str,first,1);
            num = getOpNumber(s1);
            string s2(str,last+1,str.length());
            isOp = true;
            return s2;
        }
        else return str;
    }
    bool isDigit = isdigit(*It);
    ok = true;
    while ( (It != str.end()) && ok ) {
    	if ( (isDigit && !isdigit(*It)) || (!isDigit && !isalpha(*It))  ) {
    	    ok = false;
    	}
    	else {
    	    *It = tolower(*It);
            *It++;
            last++;
    	}
    }
    string s1 (str,first,last-first);
    string s2;
    if (str.length()>=last) s2.assign(str,last,str.length());
    else s2 = "";
    if (isDigit) {
        num = atoi(s1.c_str());
    }
    else {
        num = getOpNumber(s1);
        isOp = true;
    }
    return s2;
}

bool Menu::verifyOperator(int op, int arg1, int arg2, int n)
{
    string str = "";
    int n1;
    if (n==-1) n1 = fDat.size();
    else n1 = (boundBox[n]).size();
    if ((op!=CONS)&&(((op!=SUB)&& (arg1>=n1))||(arg2>=n1))) return false;
    stringstream ss1;
    ss1 << arg1;
    string sarg1=ss1.str();
    stringstream ss2;
    ss2 << arg2;
    string sarg2=ss2.str();
    bool ok = true;
    switch ( op ) {
        case CONS:{
            if (arg2!=-1) ok = false;
            str += sarg1;
            break;
        }
        case VAR: {
            if (arg2!=-1) ok = false;
            str += "x(";
            str += sarg1;
            str += ")";
            if ((arg1<1)||(arg1>nVars)) ok = false;
            break;
        }
        case ADD: {
            str += "<";
            str += sarg1;
            str += "> + <";
            str += sarg2;
            str += ">";
            if ((arg1==-1)||(arg2==-1)) ok = false;
            break;
        }
        case SUB: {
            str += "<";
            str += sarg1;
            str += "> - <";
            str += sarg2;
            str += ">";
            //if (arg2==-1) ok = false;
            break;
        }
        case PROD: {
            str += "<";
            str += sarg1;
            str += "> * <";
            str += sarg2;
            str += ">";
            if ((arg1==-1)||(arg2==-1)) ok = false;
            break;
        }
        case DIV:  {
            str += "<";
            str += sarg1;
            str += "> / <";
            str += sarg2;
            str += ">";
            if ((arg1==-1)||(arg2==-1)) ok = false;
            break;
        }
        case POW:  {
            str += "Power(<";
            str += sarg1;
            str += ">,<";
            str += sarg2;
            str += ">)";
            if ((arg1==-1)||(arg2==-1)) ok = false;
            break;
        }
        case SIN:  {
            if (arg2!=-1) ok = false;
            str += "Sin(<";
            str += sarg1;
            str += ">)";
            if (arg1==-1) ok = false;
            break;
        }
        case COS:  {
            if (arg2!=-1) ok = false;
            str += "Cos(<";
            str += sarg1;
            str += ">)";
            if (arg1==-1) ok = false;
            break;
        }
        case PI:  {
            if ((arg1!=-1)||(arg2!=-1)) ok = false;
            str += "Pi";
            break;
        }
        case SQRT:  {
            if (arg2!=-1) ok = false;
            str += "Sqrt(<";
            str += sarg1;
            str += ">)";
            if (arg1==-1) ok = false;
            break;
        }
        case SQR:  {
            if (arg2!=-1) ok = false;
            str += "Sqr(<";
            str += sarg1;
            str += ">)";
            if (arg1==-1) ok = false;
            break;
        }
        case TAN:  {
            if (arg2!=-1) ok = false;
            str += "Tan(<";
            str += sarg1;
            str += ">)";
            if (arg1==-1) ok = false;
            break;
        }
        case COT:  {
            if (arg2!=-1) ok = false;
            str += "Cot(<";
            str += sarg1;
            str += ">)";
            if (arg1==-1) ok = false;
            break;
        }
        case EXP:  {
            if (arg2!=-1) ok = false;
            str += "Exp(<";
            str += sarg1;
            str += ">)";
            if (arg1==-1) ok = false;
            break;
        }
        case LOG:  {
            if (arg2!=-1) ok = false;
            str += "Log(<";
            str += sarg1;
            str += ">)";
            if (arg1==-1) ok = false;
            break;
        }
        case SINH:  {
            if (arg2!=-1) ok = false;
            str += "Sinh(<";
            str += sarg1;
            str += ">)";
            if (arg1==-1) ok = false;
            break;
        }
        case COSH:  {
            if (arg2!=-1) ok = false;
            str += "Cosh(<";
            str += sarg1;
            str += ">)";
            if (arg1==-1) ok = false;
            break;
        }
        case TANH:  {
            if (arg2!=-1) ok = false;
            str += "Tanh(<";
            str += sarg1;
            str += ">)";
            if (arg1==-1) ok = false;
            break;
        }
        case COTH:  {
            if (arg2!=-1) ok = false;
            str += "Coth(<";
            str += sarg1;
            str += ">)";
            if (arg1==-1) ok = false;
            break;
        }
        case ARCTAN:  {
            if (arg2!=-1) ok = false;
            str += "ArcTan(<";
            str += sarg1;
            str += ">)";
            if (arg1==-1) ok = false;
            break;
        }
        default: ok = false;
    }
    //cout << "str = **" << str << "**" << endl;
    //cout << "ok = " << ok << endl;
    if (ok) {
        int *tt = new int[3];
        tt[0] = op;
        tt[1] = arg1;
        tt[2] = arg2;
        if (n==-1) fDat.push_back(tt);
        else (boundBox[n]).push_back(tt);
    }
    return ok;
}

void Menu::showBoundingExpression(int n)
{
    fBox[n] = getExpression(boundBox[n],true);
}

string Menu::getExpression(vector<int*> e, bool show)
{
    int op, arg1, arg2;
    vector<string> vstr;
    string str;
    for (int i=0; i<e.size();i++) {
        op = (e[i])[0];
        arg1 = (e[i])[1];
        arg2 = (e[i])[2];
        str = "";
        stringstream ss1;
        ss1 << arg1;
        string sarg1=ss1.str();
        stringstream ss2;
        ss2 << arg2;
        string sarg2=ss2.str();
        switch ( op ) {
            case CONS: str += sarg1; break;
            case VAR:  str += "x(" + sarg1 + ")"; break;
            case ADD:  str += "(" + vstr[arg1] + "+" + vstr[arg2] + ")"; break;
            case SUB:  if (arg1==-1) str += "-" + vstr[arg2];
                       else if (arg2==-1) str += "-" + sarg1;
                       else str += "(" + vstr[arg1] + "-" + vstr[arg2] + ")"; break;
            case PROD: str += "(" + vstr[arg1] + "*" + vstr[arg2] + ")"; break;
            case DIV:  str += "(" + vstr[arg1] + "/" + vstr[arg2] + ")"; break;
            case POW:  str += "(" + vstr[arg1] + "^" + vstr[arg2] + ")"; break;
            case SIN:  str += "Sin(" + vstr[arg1] + ")"; break;
            case COS:  str += "Cos(" + vstr[arg1] + ")"; break;
            case PI:   str += "Pi"; break;
            case SQRT:  str += "Sqrt(" + vstr[arg1] + ")"; break;
            case SQR:  str += "Sqr(" + vstr[arg1] + ")"; break;
            case TAN:  str += "Tan(" + vstr[arg1] + ")"; break;
            case COT:  str += "Cot(" + vstr[arg1] + ")"; break;
            case EXP:  str += "Exp(" + vstr[arg1] + ")"; break;
            case LOG:  str += "Log(" + vstr[arg1] + ")"; break;
            case SINH: str += "Sinh(" + vstr[arg1] + ")"; break;
            case COSH: str += "Cosh(" + vstr[arg1] + ")"; break;
            case TANH: str += "Tanh(" + vstr[arg1] + ")"; break;
            case COTH: str += "Coth(" + vstr[arg1] + ")"; break;
            case ARCTAN: str += "ArcTan(" + vstr[arg1] + ")"; break;
        }
        vstr.push_back(str);
        if (show) cout << "   <" << i << "> " << str  << endl;
    }
    return str;
}

void Menu::inputBox(string menu)
{
    string op, vstr, line;
    menu += ">> 2 \n";
    int v, n;
    if (nVars > 1) {
        menu += ">> Enter the number of the variable to change the interval of integration: ";
        do {
            system("cls");
            cout << menu;
            cin >> vstr;
            v = atoi(vstr.c_str());
        } while ((v<1) || (v>nVars));
        menu += vstr + "\n";
    }
    else v = 1;
    menu += ">> Do you want to change the lower bound (1) or the upper bound (2)? ";
    do {
        system("cls");
        cout << menu;
        cin >> op;
        n = atoi(op.c_str());
    } while ((n<1) || (n>2));
    menu += op + "\n";
    menu += ">> Expression for the ";
    if (n==1) menu += "left ";
    else menu += "right ";
    menu += "bound of variable x(" + vstr + "): \n";
    bool ok = false;
    int bound = 2*(v-1)+(n-1);
    int k=0;
    do {
        if (boundBox[bound].size()>0) {
            do {
                system("cls");
                cout << menu;
                showBoundingExpression(bound);
                cout << "1) Add new line; 2) Delete last line; 3) Save expression; 4) Cancel;" << endl;
                cout << ">> ";
                cin >> op;
                k = atoi(op.c_str());
            } while ((k<1) || (k>4));
        }
        else k = 1;
        if (k==1) {
            do {
                system("cls");
                cout << menu;
                showBoundingExpression(bound);
                cout << "   <" << boundBox[bound].size() << "> ";
                cin.sync();
                getline(cin,line);
                //cout << "**" <<line << "**" << endl;
                bool isOp;
                int num,arg1,arg2;
                line = extractArgument(line, isOp, num);
                int numOp;
                if (isOp) {
                    numOp = num;
                    line = extractArgument(line, isOp, num);
                    if (isOp) ok = false;
                    else {
                        arg1 = num;
                        extractArgument(line,isOp, num);
                        if (isOp) ok = false;
                        else {
                            arg2 = num;
                            ok = verifyOperator(numOp,arg1,arg2,bound);
                        }
                    }
                }
                else {
                    if (line=="") {
                    numOp = CONS;
                    arg1 = num;
                    arg2 = -1;
                    ok = verifyOperator(numOp,arg1,arg2,bound);
                    }
                    else {
                        arg1 = num;
                        line = extractArgument(line,isOp, num);
                        if (!isOp) ok = false;
                        else {
                            numOp = num;
                            extractArgument(line,isOp, num);
                            if (isOp) ok = false;
                            else {
                                arg2 = num;
                                ok = verifyOperator(numOp,arg1,arg2,bound);
                            }
                        }
                    }
                }
            } while (!ok);
        }
        if (k==2) {
            cout << "delete line" << endl;
            boundBox[bound].pop_back();
        }
        if (k==3) {
            saveBoundBox();
            getBoundBox();
        }
        if (k==4) {
            getBoundBox();
        }
    } while (k<3);
}

void Menu::inputFunction(string menu)
{
    string vstr;
    int other;
    menu += ">> 1 \n";
    menu += "   1) Edit current integrand function \n";
    menu += "   2) Create a new integrand function \n";
    if (fNameList.size()>1) {
        if (fNameList.size()==2) {
            if (fNumber==0) other = 1;
            else other = 0;
            menu += "   3) Change to integrand function: \n         " + fNameList[other] +"\n";
        }
        else menu += "   3) Select other integrand function \n";
    }
    menu += ">> ";
    int v;
    do {
        system("cls");
        cout << menu;
        cin >> vstr;
        v = atoi(vstr.c_str());
    } while ((v<1) || (v>3));
    switch (v) {
        case 1:
            editFunction(menu);
            break;
        case 2:
            createFunction(menu);
            newFunction = true;
            editFunction(menu);
            break;
        case 3:
            if (fNameList.size()==2) fNumber = other;
            else selectFunction(menu);
            fName = fNameList[fNumber];
            saveFunctionList();
            getCurrentSettings();
            getBoundBox();
            getFDat();
            break;
    }
}

void Menu::editFunction(string menu)
{
    string op, vstr, line;
    menu += "1 \n";
    bool ok;
    int k=0;
    int minsize;
    if (newFunction) minsize=3+newVars;
    else minsize=3+nVars;
    do {
        if (fDat.size()>minsize) {
            do {
                system("cls");
                cout << menu;
                getExpression(fDat,true);
                cout << "1) Add new line; 2) Delete last line; 3) Save function; 4) Cancel;" << endl;
                cout << ">> ";
                cin >> op;
                k = atoi(op.c_str());
            } while ((k<1) || (k>4));
        }
        else k = 1;
        if (k==1) {
            do {
                system("cls");
                cout << menu;
                getExpression(fDat,true);
                cout << "   <" << fDat.size() << "> ";
                cin.sync();
                getline(cin,line);
                bool isOp;
                int num,arg1,arg2;
                line = extractArgument(line, isOp, num);
                int numOp;
                if (isOp) {
                    numOp = num;
                    line = extractArgument(line, isOp, num);
                    if (isOp) ok = false;
                    else {
                        arg1 = num;
                        extractArgument(line,isOp, num);
                        if (isOp) ok = false;
                        else {
                            arg2 = num;
                            ok = verifyOperator(numOp,arg1,arg2,-1);
                        }
                    }
                }
                else {
                    if (line=="") {
                    numOp = CONS;
                    arg1 = num;
                    arg2 = -1;
                    ok = verifyOperator(numOp,arg1,arg2,-1);
                    }
                    else {
                        arg1 = num;
                        line = extractArgument(line,isOp, num);
                        if (!isOp) ok = false;
                        else {
                            numOp = num;
                            extractArgument(line,isOp, num);
                            if (isOp) ok = false;
                            else {
                                arg2 = num;
                                ok = verifyOperator(numOp,arg1,arg2,-1);
                            }
                        }
                    }
                }
            } while (!ok);
        }
        if (k==2) {
            cout << "delete line" << endl;
            fDat.pop_back();
        }
        if (k==3) {
            if (newFunction) {
                fNumber = fNameList.size();
                nVars = newVars;
                fName = getExpression(fDat, false);
                fNameList.push_back(fName);
                nVarsList.push_back(newVars);
                saveCurrentSettings();
                saveDefaultBoundBox();
                saveFDat();
                newFunction = false;
                getFunctionList();
                getCurrentSettings();
                getBoundBox();
                getFDat();
            }
            else {
                fName = getExpression(fDat, false);
                saveFDat();
                getFDat();
            }
        }
        if (k==4) {
            getFDat();
        }
    } while (k<3);
}
/*
void Menu::selectFunction(string menu)
{
    string vstr;
    menu += ">> 3 \n";
    int j = 1;
    for (unsigned int i=0;i<fNameList.size();i++) {
        stringstream ssi; ssi << i+j;
        if (i!=fNumber) menu += "   "+ ssi.str() +") " + fNameList[i] + "\n";
        else j--;
    }
    menu += ">> ";
    int v;
    do {
        system("cls");
        cout << menu;
        cin >> vstr;
        v = atoi(vstr.c_str());
    } while ((v<1) || (v>fNameList.size()-1));
    if (fNumber>=v) j++;
    fNumber = v-j;
}
*/

void Menu::selectFunction(string menu)
{
    string vstr;
    menu += ">> 3 \n";
    for (unsigned int i=0;i<fNameList.size();i++) {
        stringstream ssi; ssi << i;
        menu += "   "+ ssi.str() +") " + fNameList[i] + "\n";
    }
    menu += ">> ";
    int v;
    do {
        system("cls");
        cout << menu;
        cin >> vstr;
        v = atoi(vstr.c_str());
    } while ((v<0) || (v>fNameList.size()-1));
    fNumber = v;
}

void Menu::createFunction(string menu)
{
    string vstr;
    menu += "2 \n";
    menu += ">> ";
    int v;
    do {
        system("cls");
        cout << menu;
        cout << "How many variables has the new integrand function? ";
        cin >> vstr;
        v = atoi(vstr.c_str());
    } while (v<1);
    fDat.erase(fDat.begin(),fDat.end());
    int *t0 = new int[3];
    t0[0] = 0;
    t0[1] = 0;
    t0[2] = -1;
    fDat.push_back(t0);
    int *t1 = new int[3];
    t1[0] = 0;
    t1[1] = 1;
    t1[2] = -1;
    fDat.push_back(t1);
    int *t2 = new int[3];
    t2[0] = 0;
    t2[1] = 2;
    t2[2] = -1;
    fDat.push_back(t2);
    for (unsigned int i=0;i<v;i++) {
        int *t = new int[3];
        t[0] = 1;
        t[1] = i+1;
        t[2] = -1;
        fDat.push_back(t);
    }
    newVars = v;
}

int Menu::inputMenu(string menu)
{
    string op;
    int n;
    menu += ">> ";
    do {
        system("cls");
        cout << menu;
        cin >> op;
        n = atoi(op.c_str());
    } while ((n<=0) || (n>mainOptions.size()));
    return n;
}

void Menu::getFunctionList()
{
    fNameList.erase(fNameList.begin(),fNameList.end());
    nVarsList.erase(nVarsList.begin(),nVarsList.end());
    string line;
    ifstream file ("functionsList.dat");
    if (file.is_open()) {
        getline(file,line);
        fNumber = atoi(line.c_str());
        while (!file.eof()) {
            getline(file,line);
            if (line!="") {
                istringstream iss(line,istringstream::in);
                string fN;
                iss >> fN;
                int nV;
                iss >> nV;
                fNameList.push_back(fN);
                nVarsList.push_back(nV);
            }
        }
        file.close();
    }
}

void Menu::saveFunctionList()
{
    string line;
    ofstream file ("functionsList.dat");
    if (file.is_open()) {
        file << fNumber << endl;
        for (unsigned int i=0;i<fNameList.size();i++)
            file << fNameList[i] << " " << nVarsList[i] << endl;
        file.close();
    }
}

void Menu::saveBoundBox()
{
    stringstream ssi; ssi << fNumber;
    string filename = "function" + ssi.str() + ".box";
    ofstream file (filename.c_str());
    int n;
    if (file.is_open()) {
        for (unsigned int i=0;i<nVars;i++) {
            vector<int*> lb = boundBox[2*i];
            file << lb.size() << endl;
            for (unsigned int j=0;j<lb.size();j++) {
                int *t = lb[j];
                for (unsigned int k=0;k<3;k++) file << t[k] << " ";
                file << endl;
            }
            vector<int*> rb = boundBox[2*i+1];
            file << rb.size() << endl;
            for (unsigned int j=0;j<rb.size();j++) {
                int *t = rb[j];
                for (unsigned int k=0;k<3;k++) file << t[k] << " ";
                file << endl;
            }
        }
        file.close();
    }
}

void Menu::saveDefaultBoundBox()
{
    stringstream ssi; ssi << fNumber;
    string filename = "function" + ssi.str() + ".box";
    ofstream file (filename.c_str());
    int n;
    if (file.is_open()) {
        for (unsigned int i=0;i<nVars;i++) {
            file << "1" << endl;
            file << "0 -1 -1" << endl;
            file << "1" << endl;
            file << "0 1 -1" << endl;
        }
        file.close();
    }
}

void Menu::getFDat()
{
    fDat.erase(fDat.begin(),fDat.end());
    string line;
    stringstream ssi; ssi << fNumber;
    string filename = "function" + ssi.str() + ".dat";
    ifstream file (filename.c_str());
    if (file.is_open()) {
        while (!file.eof()) {
            getline(file,line);
            if (line!="") {
                istringstream iss(line,istringstream::in);
                int *t = new int[3];
                iss >> t[0];
                iss >> t[1];
                iss >> t[2];
                fDat.push_back(t);
            }
        }
        file.close();
    }
    fName = getExpression(fDat, false);
}


void Menu::saveFDat()
{
    stringstream ssi; ssi << fNumber;
    string filename = "function" + ssi.str() + ".dat";
    ofstream file (filename.c_str());
    if (file.is_open())
    {
        for (unsigned int i=0;i<fDat.size();i++) {
            int *t = fDat[i];
            for (unsigned int k=0;k<3;k++) file << t[k] << " ";
            file << endl;
        }
        file.close();
    }
    fNameList[fNumber] = fName;
    saveFunctionList();
}

void Menu::getBoundBox()
{
    boundBox.erase(boundBox.begin(),boundBox.end());
    fBox.erase(fBox.begin(),fBox.end());
    string line;
    stringstream ssi; ssi << fNumber;
    string filename = "function" + ssi.str() + ".box";
    ifstream file (filename.c_str());
    if (file.is_open()) {
        for (unsigned int i=0;i<nVars;i++) {
            vector<int*> lb;
            getline(file,line);
            int nlb = atoi(line.c_str());
            for (unsigned int j=0;j<nlb;j++) {
                getline(file,line);
                if (line!="") {
                    istringstream iss(line,istringstream::in);
                    int *t = new int[3];
                    iss >> t[0];
                    iss >> t[1];
                    iss >> t[2];
                    lb.push_back(t);
                }
            }
            boundBox.push_back(lb);
            vector<int*> rb;
            getline(file,line);
            int nrb = atoi(line.c_str());
            for (unsigned int j=0;j<nrb;j++) {
                getline(file,line);
                if (line!="") {
                    istringstream iss(line,istringstream::in);
                    int *t = new int[3];
                    iss >> t[0];
                    iss >> t[1];
                    iss >> t[2];
                    rb.push_back(t);
                }
            }
            boundBox.push_back(rb);
        }
        file.close();
    }
    for (unsigned int i=0;i<nVars;i++) {
        fBox.push_back(getExpression(boundBox[2*i], false));
        fBox.push_back(getExpression(boundBox[2*i+1], false));
    }
}

void Menu::saveCurrentSettings()
{
    stringstream ssi; ssi << fNumber;
    string filename = "currentSettings" + ssi.str() + ".dat";
    ofstream file (filename.c_str());
    if (file.is_open())
    {
        file << constrained << endl;
        file << precision << endl;
        file << adaptative << endl;
        file << nOrder << endl;
        file << maxOrder << endl;
        file.close();
    }
}

void Menu::getCurrentSettings()
{
    string line;
    stringstream ssi; ssi << fNumber;
    string filename = "currentSettings" + ssi.str() + ".dat";
    ifstream file (filename.c_str());
    if (file.is_open()) {
        fName = fNameList[fNumber];
        nVars = nVarsList[fNumber];
        getline(file,line);
        constrained = atoi(line.c_str());
        getline(file,line);
        precision = atof(line.c_str());
        getline(file,line);
        adaptative = atoi(line.c_str());
        getline(file,line);
        nOrder = atoi(line.c_str());
        getline(file,line);
        maxOrder = atoi(line.c_str());
        file.close();
    }
}

INTERVAL Menu::evalOp(vector<int*> e ,int j, vector<INTERVAL> &d)
{

}

INTERVAL Menu::getBound(int i, bool side)
{
    int n, op, arg1, arg2;
    if (side) n = 2*i;
    else n = 2*i+1;
    vector<int*> e = boundBox[n];
    vector<INTERVAL> d;
    int k;
    for (unsigned int j=0;j<e.size();j++) {
        op = (e[j])[0];
        arg1 = (e[j])[1];
        arg2 = (e[j])[2];
        switch ( op ) {
            case CONS: d.push_back(arg1); break;
            case ADD:  d.push_back(d[arg1]+d[arg2]); break;
            case SUB:  if (arg1==-1) d.push_back(-d[arg2]);
                       else if (arg2==-1) d.push_back(-arg1);
                       else d.push_back(d[arg1]-d[arg2]); break;
            case PROD: d.push_back(d[arg1]*d[arg2]); break;
            case DIV:  d.push_back(d[arg1]/d[arg2]); break;
            case POW:  //d.push_back(Power(d[arg1],d[arg2])); break;
                       k = round(Sup(d[arg2]));
                       d.push_back(Power(d[arg1],k)); break;
            case SIN:  d.push_back(Sin(d[arg1])); break;
            case COS:  d.push_back(Cos(d[arg1]));  break;
            case PI:   d.push_back(INTERVAL::Pi); break;
            case SQRT:  d.push_back(Sqrt(d[arg1])); break;
            case SQR:  d.push_back(Sqr(d[arg1])); break;
            case TAN:  d.push_back(Tan(d[arg1])); break;
            case COT:  d.push_back(Cot(d[arg1])); break;
            case EXP:  d.push_back(Exp(d[arg1])); break;
            case LOG:  d.push_back(Log(d[arg1])); break;
            case SINH: d.push_back(Sinh(d[arg1])); break;
            case COSH: d.push_back(Sinh(d[arg1])); break;
            case TANH: d.push_back(Tanh(d[arg1])); break;
            case COTH: d.push_back(Coth(d[arg1])); break;
            case ARCTAN: d.push_back(ArcTan(d[arg1])); break;
        }
    }
    return d[e.size()-1];
}

INTERVAL Menu::getInterval(int i)
{
    return Hull(getBound(i,true),getBound(i,false));
}

