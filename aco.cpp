#include <iostream>
#include <fstream>
#include "aco.h"
#include "ant.cpp"
#include <vector>
#include "tsp_helper.h"
#include "random.h"
#include <math.h>

using namespace std;

tsp_instance* t;
tsp_solution* s;
ant ants[MAX_ANTS];
ant rankAnts[MAX_ANTS];
vector<vector<double>> dist(1, std::vector<double>(1));
vector<vector<double>> phero(1, std::vector<double>(1));
int bestIndex;
int maxTime, curTime = 0;
double bestCost;

void init()
{
	maxTime = t->n * MAX_TOURS;
	cout << "MaxTime=" << maxTime;
	int from, to;
    bestCost = t->n * MAX_DIST;
	for (from = 1; from <= t->n; from++)
	{
		vector<double> rowD = { 0.0 }, rowP = { INIT_PHER };
		for (to = 0; to < t->n; to++)
		{
			rowD.push_back(0.0);
			rowP.push_back(INIT_PHER);
		}
		dist.push_back(rowD);
		phero.push_back(rowP);
	}

	for (from = 1; from <= t->n; from++)
	{
		for (to = 1; to <= t->n; to++)
		{
			if (to != from && dist[from][to] == 0.0)
			{
				dist[from][to] = TSP_helper::distance(s, from, to, t);
				dist[to][from] = dist[from][to];
			}
		}
	}

	to = 1;
	for (int ant = 0; ant < MAX_ANTS; ant++)
	{
		if (to == t->n + 1)
		{
			to = 1;
		}

		ants[ant].curCity = to++;

		for (from = 1; from <= t->n; from++)
		{
			ants[ant].tabu[from] = 0;
			ants[ant].path[from] = -1;
		}

		ants[ant].pathIndex = 1;
		ants[ant].path[1] = ants[ant].curCity;
		ants[ant].nextCity = -1;
		ants[ant].tourLength = 0;
		ants[ant].tabu[ants[ant].curCity] = 1; //loading first city into tabu list
	}
}

void restartAnts()
{
	int i, to = 1;

	for (int ant = 0; ant < MAX_ANTS; ant++)
	{
        if (ants[ant].tourLength < bestCost)
		{
            bestCost = ants[ant].tourLength;
			bestIndex = ant;
		}

		ants[ant].nextCity = -1;
		ants[ant].tourLength = 0.0;

		for (i = 1; i <= t->n; i++)
		{
			ants[ant].tabu[i] = 0;
			ants[ant].path[i] = -1;
		}

		if (to == t->n + 1)
		{
			to = 1;
		}

		ants[ant].curCity = to++;
		ants[ant].pathIndex = 1;
		ants[ant].path[1] = ants[ant].curCity;
		ants[ant].tabu[ants[ant].curCity] = 1;
	}
}

double antProduct(int from, int to)
{
	return pow(phero[from][to], ALPHA) * pow((1.0 / dist[from][to]), BETA);
}

int selectNextCity(int ant)
{
	int from, to;
	double denom = 0.0;

	from = ants[ant].curCity;

	for (to = 1; to <= t->n; to++)
	{
		if (ants[ant].tabu[to] == 0)
		{
			denom += antProduct(from, to);
		}
	}

	do
	{
		double p;
		to++;

		if (to >= t->n + 1)
		{
			to = 1;
		}
		if (ants[ant].tabu[to] == 0)
		{
			p = antProduct(from, to) / denom;

			double x = random_float(0, 1);
			if (x < p)
			{
				break;
			}
		}
	} while (true);

	return to;
}

int simulateAnts()
{
	int moving = 0;
	for (int k = 0; k < MAX_ANTS; k++)
	{
		if (ants[k].pathIndex < t->n)
		{
			ants[k].nextCity = selectNextCity(k);
			ants[k].tabu[ants[k].nextCity] = 1;
			ants[k].path[ants[k].pathIndex+++1] = ants[k].nextCity;
			ants[k].tourLength += dist[ants[k].curCity][ants[k].nextCity];
			if (ants[k].pathIndex == t->n)
			{
				ants[k].tourLength += dist[ants[k].path[t->n]][ants[k].path[1]];
			}
			ants[k].curCity = ants[k].nextCity;
			moving++;
		}
	}

	return moving;
}

void merge(ant arr[], int left, int middle, int right)
{
	int n1 = middle - left + 1;
	int n2 = right - middle;

	ant* L = new ant[n1];
	ant* R = new ant[n2];

	for (int i = 0; i < n1; i++)
	{
		L[i] = arr[left + i];
	}
	for (int j = 0; j < n2; j++)
	{
		R[j] = arr[middle + 1 + j];
	}

	int i = 0, j = 0, k = left;

	while (i < n1 && j < n2)
	{
		if (L[i].tourLength <= R[j].tourLength)
		{
			arr[k] = L[i];
			i++;
		}
		else
		{
			arr[k] = R[j];
			j++;
		}
		k++;
	}

	while (i < n1)
	{
		arr[k] = L[i];
		i++;
		k++;
	}

	while (j < n2)
	{
		arr[k] = R[j];
		j++;
		k++;
	}

	delete[] L;
	delete[] R;
}

void mergeSort(ant arr[], int left, int right)
{
	if (left < right)
	{
		int middle = left + (right - left) / 2;

		mergeSort(arr, left, middle);
		mergeSort(arr, middle + 1, right);

		merge(arr, left, middle, right);
	}
}

void sortAnts()
{
	for (int i = 0; i < MAX_ANTS; i++)
	{
		rankAnts[i] = ants[i];
	}

	mergeSort(rankAnts, 0, MAX_ANTS - 1);
}   

void pheromoneEvaporation(int& from, int& to)
{
	for (from = 1; from <= t->n; from++)
	{
		for (to = 1; to <= t->n; to++)
		{
			if (from != to)
			{
				phero[from][to] *= (1.0 - RHO);

				if (phero[from][to] < 0.0)
				{
					phero[from][to] = INIT_PHER;
				}
			}
		}
	}
}

void applyEvaporationRate(int& from, int& to)
{
	for (from = 1; from <= t->n; from++)
	{
		for (to = 1; to <= t->n; to++)
		{
			phero[from][to] *= RHO;
		}
	}
}

void addingTheBestPath(int& from, int& to, int& ant, ACOtype type)
{
	bool flag = false;
	int k;
	for (int i = 1; i <= t->n; i++)
	{
		from = ants[ant].path[i];
		to = i < t->n ? ants[ant].path[i + 1] : ants[ant].path[1];

        phero[from][to] += type == ACOtype::RankBased ? QVAL/bestCost : QVAL/ants[ant].tourLength;
		if (type == ACOtype::Elitist)
		{
			for (k = 1; k < t->n; k++)
			{
				if (from == ants[bestIndex].path[k] && to == ants[bestIndex].path[k + 1])
				{
					flag = true;
				}
			}
			if (flag)
			{
                phero[from][to] += QVAL/bestCost;
			}
		}
		phero[to][from] = phero[from][to];
	}
}

void updateTrails(ACOtype type)
{
	int from, to, ant;

	pheromoneEvaporation(from, to);

	//Add new pheromone to the trails
	if (type == ACOtype::RankBased)
	{
		int i;
		for (ant = 0; ant < RANK_W - 1; ant++)
		{
            for (i = 1; i <= t->n; i++)
			{
				from = rankAnts[ant].path[i];
				to = i < t->n ? rankAnts[ant].path[i + 1] : rankAnts[ant].path[1];

				phero[from][to] += (RANK_W - ant) * (QVAL / rankAnts[ant].tourLength);
				phero[to][from] = phero[from][to];
			}
		}
		addingTheBestPath(from, to, ant, type);
	}
	else
	{
		for (ant = 0; ant < MAX_ANTS; ant++)
		{
			addingTheBestPath(from, to, ant, type);
		}
	}

	applyEvaporationRate(from, to);
}

void fillSolution()
{
	s->n = 0;
	int *bestPath = ants[bestIndex].path;
	int i = 1;
	do
	{
		s->n++;
		s->p[i] = bestPath[i++];
	}
	while(bestPath[i] != 0);
}

void ACO(tsp_instance* inst, tsp_solution* sol, ACOtype type)
{
	t = inst;
	s = sol;
	string typeString = type == ACOtype::Simple ? "S-ACO:" : (type == ACOtype::Elitist ? "E-ACO:" : "R-ACO:");
	cout << typeString;
	init();

	while (curTime++ < maxTime)
	{
		if (simulateAnts() == 0)
		{
			if (type == ACOtype::RankBased) sortAnts();
			updateTrails(type);

			if (curTime != maxTime)
			{
				restartAnts();
			}

            cout << "\n Time is " << curTime << "(" << bestCost << ")";
		}
	}

    cout << "\n" << typeString <<" Best tour = " << bestCost << endl << endl << endl;

	fillSolution();
}
