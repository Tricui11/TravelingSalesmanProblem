#include <dialog.h>
#include <ui_dialog.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <annealing.h>
#include <random.h>
#include <tsp_helper.h>
#include <QVBoxLayout>
#include <QtWidgets/QGraphicsView>
#include <numbered_text_item.cpp>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
    , scaleFactor(0.1/1.15)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);
    view->setTransform(QTransform::fromScale(scaleFactor, scaleFactor));

    calcAndDrawButton = new QPushButton("Calculate and Draw", this);
    zoomInButton = new QPushButton("Zoom In", this);
    zoomOutButton = new QPushButton("Zoom Out", this);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(view);
    layout->addWidget(calcAndDrawButton);
    layout->addWidget(zoomInButton);
    layout->addWidget(zoomOutButton);

    setLayout(layout);

    connect(calcAndDrawButton, &QPushButton::clicked, this, &Dialog::on_calcAndDrawButton_clicked);
    connect(zoomInButton, &QPushButton::clicked, this, &Dialog::zoomIn);
    connect(zoomOutButton, &QPushButton::clicked, this, &Dialog::zoomOut);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_calcAndDrawButton_clicked()
{
    tsp_instance t;
    tsp_solution s;
    std::vector<int> path;
    extern int solution_count;

    srand(time(NULL));

    TSP_helper::read_tsp(&t);
    TSP_helper::read_solution(&s);
    printf("OPTIMAL SOLUTION COST = %7.1f\n", TSP_helper::solution_cost(&s, &t));
    TSP_helper::print_solution(&s);

    TSP_helper::initialize_solution(t.n, &s);
    printf("solution_cost = %7.1f\n", TSP_helper::solution_cost(&s, &t));
    TSP_helper::print_solution(&s);

    solution_count = 0;
    random_sampling(&t, 1500000, &s, this);
    printf("random sampling %d iterations, cost = %7.1f\n", solution_count, TSP_helper::solution_cost(&s,&t));
    TSP_helper::print_solution(&s);

    solution_count = 0;
    repeated_hill_climbing(&t, 195, &s, this);
    printf("repeated hill climbing %d iterations, cost = %7.1f\n", solution_count, TSP_helper::solution_cost(&s,&t));
    TSP_helper::print_solution(&s);

    solution_count = 0;
    repeated_annealing(&t, 10, &s, this);
    printf("repeated annealing %d iterations, cost = %7.1f\n", solution_count, TSP_helper::solution_cost(&s, &t));
    TSP_helper::print_solution(&s);

    drawSolution(&t, &s);
}

void Dialog::drawSolution(tsp_instance* t, tsp_solution* s)
{
    scene->clear();

    for (int i = 1; i <= t->n; i++)
    {
        QGraphicsEllipseItem* pointItem = new QGraphicsEllipseItem(t->p[i].x - 30, t->p[i].y - 30, 60, 60);
        pointItem->setBrush(Qt::red);
        scene->addItem(pointItem);

        QGraphicsTextItem* textItem = new QGraphicsTextItem(QString::number(i));
        QFont font = textItem->font();
        font.setPointSize(120);
        textItem->setFont(font);
        textItem->setPos(t->p[i].x + 5, t->p[i].y - 15);
        scene->addItem(textItem);
    }

    QPen pen;
    pen.setWidth(10);
    pen.setColor(Qt::black);
    for (int i = 2; i <= s->n; i++)
    {
        const point& startPoint = t->p[s->p[i - 1]];
        const point& endPoint = t->p[s->p[i]];
        scene->addLine(startPoint.x, startPoint.y, endPoint.x, endPoint.y, pen);
    }
    point& startPoint = t->p[s->p[s->n]];
    point& endPoint = t->p[s->p[1]];
    scene->addLine(startPoint.x, startPoint.y, endPoint.x, endPoint.y, pen);
}

void Dialog::zoomIn()
{
    scaleFactor *= 1.2;
    view->setTransform(QTransform::fromScale(scaleFactor, scaleFactor));
}

void Dialog::zoomOut()
{
    scaleFactor /= 1.2;
    view->setTransform(QTransform::fromScale(scaleFactor, scaleFactor));
}
