#include "tsp_helper.h"
#include "random.h"
#include <stdio.h>
#include <math.h>

int sq(int x)
{
    return(x * x);
}

double TSP_helper::distance(tsp_solution* s, int x, int y, tsp_instance* t)
{
    int i, j;
    i = x;
    j = y;
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

void TSP_helper::print_solution(tsp_solution* s)
{
    for (int i = 1; i <= (s->n); i++)
    {
        printf(" %d", s->p[i]);
    }
    printf("\n------------------------------------------------------\n");
}

void TSP_helper::read_solution(tsp_solution* s)
{
    //scanf_s("%d\n", &(s->n));

    //for (i = 1; i <= (s->n); i++) {
    //    scanf_s("%d", &(s->p[i]));
    //}

    s->n = 48;
    s->p[1] = 1;
    s->p[2] = 8;
    s->p[3] = 38;
    s->p[4] = 31;
    s->p[5] = 44;
    s->p[6] = 18;
    s->p[7] = 7;
    s->p[8] = 28;
    s->p[9] = 6;
    s->p[10] = 37;
    s->p[11] = 19;
    s->p[12] = 27;
    s->p[13] = 17;
    s->p[14] = 43;
    s->p[15] = 30;
    s->p[16] = 36;
    s->p[17] = 46;
    s->p[18] = 33;
    s->p[19] = 20;
    s->p[20] = 47;
    s->p[21] = 21;
    s->p[22] = 32;
    s->p[23] = 39;
    s->p[24] = 48;
    s->p[25] = 5;
    s->p[26] = 42;
    s->p[27] = 24;
    s->p[28] = 10;
    s->p[29] = 45;
    s->p[30] = 35;
    s->p[31] = 4;
    s->p[32] = 26;
    s->p[33] = 2;
    s->p[34] = 29;
    s->p[35] = 34;
    s->p[36] = 41;
    s->p[37] = 16;
    s->p[38] = 22;
    s->p[39] = 3;
    s->p[40] = 23;
    s->p[41] = 14;
    s->p[42] = 25;
    s->p[43] = 13;
    s->p[44] = 11;
    s->p[45] = 12;
    s->p[46] = 15;
    s->p[47] = 40;
    s->p[48] = 9;
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
