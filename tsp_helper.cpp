#include "tsp_helper.h"
#include "random.h"
#include <stdio.h>
#include <math.h>

void TSP_helper::read_tsp(tsp_instance* t)
{
    //scanf_s("%d\n", &(t->n));
    //for (int i = 1; i <= (t->n); i++) {
    //    scanf_s("%d %d %d\n", &j, &(t->p[i].x), &(t->p[i].y));
    //}


    t->n = 48;
    t->p[1].x = 6734; t->p[1].y = 1453;
    t->p[2].x = 2233; t->p[2].y = 10;
    t->p[3].x = 5530; t->p[3].y = 1424;
    t->p[4].x = 401; t->p[4].y = 841;
    t->p[5].x = 3082; t->p[5].y = 1644;
    t->p[6].x = 7608; t->p[6].y = 4458;
    t->p[7].x = 7573; t->p[7].y = 3716;
    t->p[8].x = 7265; t->p[8].y = 1268;
    t->p[9].x = 6898; t->p[9].y = 1885;
    t->p[10].x = 1112; t->p[10].y = 2049;
    t->p[11].x = 5468; t->p[11].y = 2606;
    t->p[12].x = 5989; t->p[12].y = 2873;
    t->p[13].x = 4706; t->p[13].y = 2674;
    t->p[14].x = 4612; t->p[14].y = 2035;
    t->p[15].x = 6347; t->p[15].y = 2683;
    t->p[16].x = 6107; t->p[16].y = 669;
    t->p[17].x = 7611; t->p[17].y = 5184;
    t->p[18].x = 7462; t->p[18].y = 3590;
    t->p[19].x = 7732; t->p[19].y = 4723;
    t->p[20].x = 5900; t->p[20].y = 3561;
    t->p[21].x = 4483; t->p[21].y = 3369;
    t->p[22].x = 6101; t->p[22].y = 1110;
    t->p[23].x = 5199; t->p[23].y = 2182;
    t->p[24].x = 1633; t->p[24].y = 2809;
    t->p[25].x = 4307; t->p[25].y = 2322;
    t->p[26].x = 675; t->p[26].y = 1006;
    t->p[27].x = 7555; t->p[27].y = 4819;
    t->p[28].x = 7541; t->p[28].y = 3981;
    t->p[29].x = 3177; t->p[29].y = 756;
    t->p[30].x = 7352; t->p[30].y = 4506;
    t->p[31].x = 7545; t->p[31].y = 2801;
    t->p[32].x = 3245; t->p[32].y = 3305;
    t->p[33].x = 6426; t->p[33].y = 3173;
    t->p[34].x = 4608; t->p[34].y = 1198;
    t->p[35].x = 23; t->p[35].y = 2216;
    t->p[36].x = 7248; t->p[36].y = 3779;
    t->p[37].x = 7762; t->p[37].y = 4595;
    t->p[38].x = 7392; t->p[38].y = 2244;
    t->p[39].x = 3484; t->p[39].y = 2829;
    t->p[40].x = 6271; t->p[40].y = 2135;
    t->p[41].x = 4985; t->p[41].y = 140;
    t->p[42].x = 1916; t->p[42].y = 1569;
    t->p[43].x = 7280; t->p[43].y = 4899;
    t->p[44].x = 7509; t->p[44].y = 3239;
    t->p[45].x = 10; t->p[45].y = 2676;
    t->p[46].x = 6807; t->p[46].y = 2993;
    t->p[47].x = 5185; t->p[47].y = 3258;
    t->p[48].x = 3023; t->p[48].y = 1942;
}

int sq(int x)
{
    return(x * x);
}

double distance(tsp_solution* s, int x, int y, tsp_instance* t)
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