#include <tsp_instance.cpp>
#include <tsp_solution.cpp>

#ifndef TSP_HELPER
#define TSP_HELPER

class TSP_helper
{
public:
    
    static double distance(tsp_solution* s, int x, int y, tsp_instance* t);

    static double solution_cost(tsp_solution* s, tsp_instance* t);

    static void initialize_solution(int n, tsp_solution* s);

    static void copy_solution(tsp_solution* s, tsp_solution* t);

    static void random_solution(tsp_solution* s);

    static double transition(tsp_solution* s, tsp_instance* t, int i, int j);
};

#endif //TSP_HELPER
