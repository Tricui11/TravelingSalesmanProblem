#define MAX_DIST 10000
#define MAX_ANTS 300

#define ALPHA 1.0
#define BETA 5.0 //This parameter raises the weight of distance over pheromone
#define RHO 0.5 //Evapouration rate
#define QVAL 100
#define MAX_TOURS 60
#define INIT_PHER (1.0/NMAX)
#define RANK_W MAX_ANTS/2.0 //rank_based only

#include "tsp_instance.cpp"
#include "tsp_solution.cpp"
#include "aco_type.cpp"

void ACO(tsp_instance* inst, tsp_solution* s, ACOtype type);