#include <annealing.h>
#include <stdio.h>
#include <random.h>
#include <cmath>
#include <tsp_helper.h>
#include <dialog.h>
#include <QCoreApplication>

int solution_count;
double e = exp(1.0);
Dialog* dialog;

void solution_count_update(tsp_solution *s, tsp_instance *t)
{
    solution_count++;
    if ((solution_count % PRINT_FREQUENCY) == 0)
    {
        dialog->drawSolution(t, s);
        QCoreApplication::processEvents();
    }
}

void random_sampling(tsp_instance *t, int nsamples, tsp_solution *bestsol, Dialog* inDialog)
{
    dialog = inDialog;
    tsp_solution s;
    double best_cost, curr_cost;

    TSP_helper::initialize_solution(t->n, &s);
    best_cost = TSP_helper::solution_cost(&s, t);
    TSP_helper::copy_solution(&s, bestsol);

    for (int i = 1; i <= nsamples; i++)
    {
        TSP_helper::random_solution(&s);
        curr_cost = TSP_helper::solution_cost(&s, t);

        if (curr_cost < best_cost)
        {
            best_cost = curr_cost;
            dialog->resCostLineEdit->setText(QString::number(curr_cost));
            TSP_helper::copy_solution(&s, bestsol);
        }

        solution_count_update(&s, t);
    }
}

void hill_climbing(tsp_instance *t, tsp_solution *s)
{
    TSP_helper::initialize_solution(t->n, s);
    TSP_helper::random_solution(s);

    double delta;
    bool isGetBetterSol;
    do
    {
        isGetBetterSol = false;
        for (int i = 1; i < t->n; i++)
        {
            for (int j = i+1; j <= t->n; j++)
            {
                delta = TSP_helper::transition(s, t, i, j);
                if (delta < 0)
                {
                    isGetBetterSol = true;
                }
                else
                {
                    TSP_helper::transition(s, t, j, i);
                }
                solution_count_update(s, t);
            }
        }
    }
    while(isGetBetterSol);
}

void repeated_hill_climbing(tsp_instance *t, int nsamples, tsp_solution *bestsol, Dialog* inDialog)
{
    dialog = inDialog;
    tsp_solution s;

    TSP_helper::initialize_solution(t->n, &s);
    double best_cost = TSP_helper::solution_cost(&s, t);
    TSP_helper::copy_solution(&s, bestsol);

    double curr_cost;
    for (int i = 1; i <= nsamples; i++)
    {
        hill_climbing(t, &s);
        curr_cost = TSP_helper::solution_cost(&s, t);
        if (curr_cost < best_cost)
        {
            best_cost = curr_cost;
            dialog->resCostLineEdit->setText(QString::number(curr_cost));
            TSP_helper::copy_solution(&s, bestsol);
        }
    }
}

//Pairs of components of the same type will be swapped at random, and the new arrangment
//accepted either if (1) it is an improvement, or (2) the penalty is
//less than a random number, which is a function of the temperature of the system.
//We are seeking to *minimize* the current_value.
void anneal(tsp_instance *t, tsp_solution *s)
{
    int i1, i2; //pair of items to swap 
    double start_value; 
    double delta; //value after swap
    double merit, flip; //hold swap accept conditions
    double exponent; //exponent for energy funct

    double temperature = initialTemperature;
    TSP_helper::initialize_solution(t->n, s);
    double current_value = TSP_helper::solution_cost(s, t);

    for (int i = 1; i <= coolingSteps; i++)
    {
        temperature *= coolingFraction;

        start_value = current_value;		

        for (int j = 1; j <= stepsPerTemp; j++)
        {
            i1 = random_int(1, t->n);
            i2 = random_int(1, t->n);

            delta = TSP_helper::transition(s, t, i1, i2);

            flip = random_float(0, 1);
            exponent = (-delta/current_value) / (K*temperature);
            merit = pow(e, exponent);
            //if (merit >= 1.0)  merit = 0.0; //don't do unchanging swaps
            if (delta < 0)
            {    
                //ACCEPT-WIN
                current_value += delta;
            }
            else
            { 
                if (merit > flip)
                {   
                    //ACCEPT-LOSS
                    current_value += delta;
                }
                else
                {   
                    //REJECT
                    TSP_helper::transition(s, t, i1, i2);
                } 
            }
            solution_count_update(s, t);
        }
        if ((current_value-start_value) < 0.0)
        {
            //rerun at this temp
            temperature /= coolingFraction;
        }
    }
}

void repeated_annealing(tsp_instance *t, int nsamples, tsp_solution *bestsol, Dialog* inDialog)
{
    dialog = inDialog;
    tsp_solution s;

    TSP_helper::initialize_solution(t->n, &s);
    double best_cost = TSP_helper::solution_cost(&s, t);
    TSP_helper::copy_solution(&s, bestsol);

    double curr_cost;
    for (int i = 1; i <= nsamples; i++)
    {
        anneal(t, &s);
        curr_cost = TSP_helper::solution_cost(&s, t);
        if (curr_cost < best_cost)
        {
            best_cost = curr_cost;
            dialog->resCostLineEdit->setText(QString::number(curr_cost));
            TSP_helper::copy_solution(&s, bestsol);
        }
    }
}
