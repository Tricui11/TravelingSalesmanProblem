#include <dialog.h>
#include <ui_dialog.h>
#include <QFileDialog>
#include <stdio.h>
#include <string.h>
#include <annealing.h>
#include <random.h>
#include <tsp_helper.h>
#include <QVBoxLayout>
#include <QtWidgets/QGraphicsView>
#include <numbered_text_item.cpp>
#include <QFileDialog>
#include <QMessageBox>
#include <fstream>
#include <QLineEdit>
#include <QLabel>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
    , scaleFactor(0.1/1.15)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);
    view->setTransform(QTransform::fromScale(scaleFactor, scaleFactor));

    calcButton = new QPushButton("Calculate", this);
    loadFromFileButton = new QPushButton("Load from File", this);
    zoomInButton = new QPushButton("Zoom In", this);
    zoomOutButton = new QPushButton("Zoom Out", this);

    repeatCountSpinBox = new QSpinBox(this);
    initialTemperatureSpinBox = new QDoubleSpinBox(this);
    coolingStepsSpinBox = new QSpinBox(this);
    coolingFractionSpinBox = new QDoubleSpinBox(this);
    stepsPerTempSpinBox = new QSpinBox(this);
    kSpinBox = new QDoubleSpinBox(this);
    repeatCountSpinBox->setMinimum(1);
    initialTemperatureSpinBox->setMinimum(0.0);
    coolingStepsSpinBox->setMinimum(1);
    coolingFractionSpinBox->setMinimum(0.0);
    stepsPerTempSpinBox->setMinimum(1);
    kSpinBox->setMinimum(0.0);

    QHBoxLayout* optimalDistanceLayout = new QHBoxLayout();
    QLabel* optimalDistanceLabel = new QLabel("Optimal = ", this);
    optimalDistanceLineEdit = new QLineEdit(this);
    optimalDistanceLayout->addWidget(optimalDistanceLabel);
    optimalDistanceLayout->addWidget(optimalDistanceLineEdit);

    QHBoxLayout* resDistanceLayout = new QHBoxLayout();
    QLabel* resDistanceLabel = new QLabel("Result = ", this);
    resDistanceLineEdit = new QLineEdit(this);
    resDistanceLayout->addWidget(resDistanceLabel);
    resDistanceLayout->addWidget(resDistanceLineEdit);

    QHBoxLayout* pathLayout = new QHBoxLayout();
    QLabel* pathLabel = new QLabel("Path: ", this);
    pathLineEdit = new QLineEdit(this);
    pathLayout->addWidget(pathLabel);
    pathLayout->addWidget(pathLineEdit);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(repeatCountSpinBox);
    layout->addWidget(initialTemperatureSpinBox);
    layout->addWidget(coolingStepsSpinBox);
    layout->addWidget(coolingFractionSpinBox);
    layout->addWidget(stepsPerTempSpinBox);
    layout->addWidget(kSpinBox);
    layout->addLayout(optimalDistanceLayout);
    layout->addWidget(view);
    layout->addWidget(calcButton);
    layout->addWidget(loadFromFileButton);
    layout->addWidget(zoomInButton);
    layout->addWidget(zoomOutButton);
    layout->addLayout(resDistanceLayout);
    layout->addLayout(pathLayout);

    setLayout(layout);

    connect(calcButton, &QPushButton::clicked, this, &Dialog::on_calcButton_clicked);
    connect(loadFromFileButton, &QPushButton::clicked, this, &Dialog::on_loadFromFileButton_clicked);
    connect(zoomInButton, &QPushButton::clicked, this, &Dialog::zoomIn);
    connect(zoomOutButton, &QPushButton::clicked, this, &Dialog::zoomOut);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_loadFromFileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", QString(), "All Files (*)");

    if (!fileName.isEmpty())
    {
        QFile qFile(fileName);
        if (!qFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::critical(this, "Error", "Failed to open file.");
            return;
        }

        QTextStream in(&qFile);

        QString firstLine = in.readLine();
        QString optimalSolution = firstLine.split(" ").last();
        optimalDistanceLineEdit->setText(optimalSolution);

        t.n = 0;
        while (!in.atEnd())
        {
            QString line = in.readLine();
            QStringList parts = line.split(" ");
            if (parts.size() != 3)
            {
                continue;
            }

            int index = parts[0].toInt();
            int x = parts[1].toInt();
            int y = parts[2].toInt();

            t.n++;
            t.p[index].x = x;
            t.p[index].y = y;
        }

        scene->clear();
        drawVertices(&t);
    }
    else
    {
        QMessageBox::information(this, "Info", "File selection cancelled.");
    }
}

void Dialog::on_calcButton_clicked()
{
    extern int solution_count;

    srand(time(NULL));

    pathLineEdit->setText("");
    resDistanceLineEdit->setText("");
    TSP_helper::initialize_solution(t.n, &s);

    random_sampling(&t, 150000, &s, this);

    repeated_hill_climbing(&t, 200, &s, this);

    repeated_annealing(&t, 10, &s, this);

    drawSolution(&t, &s);
    setResults();
}

void Dialog::drawVertices(tsp_instance* t)
{
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
}

void Dialog::setResults()
{
    QString path = "";
    for (int i = 1; i <= s.n; i++)
    {
        path += QString::number(s.p[i]);
        path += " ";
    }
    pathLineEdit->setText(path);

    double resCost = TSP_helper::solution_cost(&s,&t);
    resDistanceLineEdit->setText(QString::number(resCost));
}

void Dialog::drawSolution(tsp_instance* t, tsp_solution* s)
{
    scene->clear();

    drawVertices(t);

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
