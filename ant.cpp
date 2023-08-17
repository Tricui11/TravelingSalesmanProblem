#include "aco.h"
#include "point.cpp"

struct ant
{
	int curCity, nextCity, pathIndex;
	int tabu[NMAX+1];
	int path[NMAX+1];
	double tourLength;
};