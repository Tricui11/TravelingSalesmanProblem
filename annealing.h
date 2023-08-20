#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <dialog.h>
#include "tsp_instance.cpp"
#include "tsp_solution.cpp"

#define TRACE_OUTPUT false
#define PRINT_FREQUENCY 10000
#define ERROR "ERROR"
#define ERROR_VALUE	0.0

//Simulated Annealing Constants
#define REPEAT_COUNT 1
#define	INITIAL_TEMPERATURE	1
#define COOLING_STEPS 500	//change in pair with the COOLING_FRACTION
#define COOLING_FRACTION 0.97 //higher to improve quality
#define STEPS_PER_TEMP 1000
#define K 0.01 //Boltzman's constant

void random_sampling(tsp_instance* t, int nsamples, tsp_solution* bestsol, Dialog* Dialog);

void repeated_hill_climbing(tsp_instance* t, int nsamples, tsp_solution* bestsol, Dialog* Dialog);

void repeated_annealing(tsp_instance *t, int nsamples, tsp_solution *bestsol, Dialog* Dialog);
