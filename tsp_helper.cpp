#include <tsp_helper.h>
#include <random.h>
#include <stdio.h>
#include <math.h>

int sq(int x)
{
    return(x * x);
}

double TSP_helper::distance(tsp_solution* s, int x, int y, tsp_instance* t)
{
    int i = x, j = y;
    if (i == t->n + 1)
    {
        i = 1;
    }
    if (j == t->n + 1)
    {
        j = 1;
    }
    if (i == 0)
    {
        i = t->n;
    }
    if (j == 0)
    {
        j = t->n;
    }
    return sqrt((double)(sq(t->p[s->p[i]].x - t->p[s->p[j]].x) + sq(t->p[s->p[i]].y - t->p[s->p[j]].y)));
}

double TSP_helper::solution_cost(tsp_solution* s, tsp_instance* t)
{
    double cost = distance(s, t->n, 1, t);
    for (int i = 1; i < t->n; i++)
    {
        cost += distance(s, i, i + 1, t);
    }
    return(cost);
}

void TSP_helper::initialize_solution(int n, tsp_solution* s)
{
    s->n = n;
    for (int i = 1; i <= n; i++)
    {
        s->p[i] = i;
    }
}

void TSP_helper::copy_solution(tsp_solution* s, tsp_solution* t)
{
    t->n = s->n;
    for (int i = 1; i <= s->n; i++)
    {
        t->p[i] = s->p[i];
    }
}

void TSP_helper::random_solution(tsp_solution* s)
{
    random_permutation(&(s->p[1]), (s->n) - 1);
}

double TSP_helper::transition(tsp_solution* s, tsp_instance* t, int i, int j)
{
    if (i == j)
    {
        return(0.0);
    }
    if (i > j)
    {
        return(transition(s, t, j, i));
    }

    bool neighbors = false;
    if (i == (j - 1))
    {
        neighbors = true;
    }
    if ((i == 1) && (j == s->n))
    {
        swap(&i, &j);
        neighbors = true;
    }

    double beforeSwap = neighbors
        ? distance(s, i - 1, i, t) + distance(s, j, j + 1, t)
        : distance(s, i - 1, i, t) + distance(s, i, i + 1, t) + distance(s, j - 1, j, t) + distance(s, j, j + 1, t);

    swap(&(s->p[i]), &(s->p[j]));

    double afterSwap = neighbors
        ? distance(s, i - 1, i, t) + distance(s, j, j + 1, t)
        : distance(s, i - 1, i, t) + distance(s, i, i + 1, t) + distance(s, j - 1, j, t) + distance(s, j, j + 1, t);

    return(afterSwap - beforeSwap);
}
