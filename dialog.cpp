#include <dialog.h>
#include <ui_dialog.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <annealing.h>
#include <random.h>
#include <tsp_helper.h>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_pushButton_clicked()
{
    tsp_instance t;
    tsp_solution s;
    extern int solution_count;;

    TSP_helper::read_tsp(&t);
    TSP_helper::read_solution(&s);
    printf("OPTIMAL SOLUTION COST = %7.1f\n", TSP_helper::solution_cost(&s, &t));
    TSP_helper::print_solution(&s);

    TSP_helper::initialize_solution(t.n, &s);
    printf("solution_cost = %7.1f\n", TSP_helper::solution_cost(&s, &t));
    TSP_helper::print_solution(&s);

    solution_count = 0;
    random_sampling(&t,1500000,&s);
    printf("random sampling %d iterations, cost = %7.1f\n", solution_count, TSP_helper::solution_cost(&s,&t));
    TSP_helper::print_solution(&s);

    solution_count = 0;
    repeated_hill_climbing(&t,195,&s);
    printf("repeated hill climbing %d iterations, cost = %7.1f\n", solution_count, TSP_helper::solution_cost(&s,&t));
    TSP_helper::print_solution(&s);

    solution_count = 0;
    repeated_annealing(&t, 3, &s);
    printf("repeated annealing %d iterations, cost = %7.1f\n", solution_count, TSP_helper::solution_cost(&s, &t));
    TSP_helper::print_solution(&s);
}

