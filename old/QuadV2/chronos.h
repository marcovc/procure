#ifndef __CHRONOS__
#define __CHRONOS__

#include <ctime>

class chronos
{
private:
	clock_t _tbegin;
public:
	void start() {_tbegin=clock();}
	double duration() {return ((double)clock()-(double)_tbegin)/(double)CLOCKS_PER_SEC;}
};

#endif // __CHRONOS__
