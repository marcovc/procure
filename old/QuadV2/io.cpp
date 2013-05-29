#include "io.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <math.h>

//***************************************************

void write_LIV_2D(list<INTERVAL_VECTOR> l, string filename)
{
    double volume=0;
	int nboxes=0;
    INTERVAL_VECTOR I;
    int v1=1;
    int v2=2;

	ofstream file (filename.c_str());
	file << std::setiosflags(ios::fixed) << std::setprecision(10);
    if (file.is_open())
    {
        file << "{";
        while( !(l.empty()) ){
            I=l.front();
            l.pop_front();
            volume+= Diam(I(v1))*Diam(I(v2));
			nboxes++;
			//******************************************************************
			//** box
			file << "{";
            file << "{" << Inf(I(v1)) << "," << Sup(I(v1)) << "}";
            file << ",";
            file << "{" << Inf(I(v2)) << "," << Sup(I(v2)) << "}";
			file << "}";
			//******************************************************************
            if(!(l.empty())) file << ",";
        };
        file << "}";
        file.close();
    }
    cout << "--boxes written in file " << filename;
    cout << " (nboxes: " << nboxes << "; volume: " << volume << ")" << endl;
}

//***************************************************

void write_LDD(list<pair<double,double> > l, string filename)
{
	ofstream file (filename.c_str());
    if (file.is_open())
    {
		//file << std::setiosflags(ios::fixed) << std::setprecision(16);
		file << std::setiosflags(ios::scientific) << std::setprecision(16);
        file << "{";
        while( !(l.empty()) ){
            file << "{" << l.front().first << "," << l.front().second << "}";
            l.pop_front();
            if(!(l.empty())) file << ",";
        };
        file << "}";
        file.close();
    }
	cout << "--timings written in file " << filename << endl;
}
