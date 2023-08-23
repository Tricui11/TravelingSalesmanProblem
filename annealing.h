#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <dialog.h>
#include <tsp_instance.cpp>
#include <tsp_solution.cpp>

#define PRINT_FREQUENCY 10000

inline double initialTemperature;
inline int coolingSteps;	//change in pair with the COOLING_FRACTION
inline double coolingFraction; //higher to improve quality
inline int stepsPerTemp;
inline double K; //Boltzman's constant

void random_sampling(tsp_instance* t, int nsamples, tsp_solution* s, Dialog* Dialog);

void repeated_hill_climbing(tsp_instance* t, int nsamples, tsp_solution* s, Dialog* Dialog);

void repeated_annealing(tsp_instance *t, int nsamples, tsp_solution *s, Dialog* Dialog);
