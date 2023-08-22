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

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);
    view->setTransform(QTransform::fromScale(scaleFactor, scaleFactor));
    QBrush backgroundBrush(QColor::fromRgb(255, 255, 204));
    view->setBackgroundBrush(backgroundBrush);

    calcButton = new QPushButton("Calculate", this);
    calcButton->setStyleSheet("background-color: red; color: white;");
    calcButton->setFixedWidth(250);
    QFont font = calcButton->font();
    font.setPointSize(14);
    font.setBold(true);
    calcButton->setFont(font);
    loadFromFileButton = new QPushButton("Load from File", this);
    loadFromFileButton->setFixedWidth(150);
    zoomInButton = new QPushButton("Zoom In", this);
    zoomOutButton = new QPushButton("Zoom Out", this);

    QHBoxLayout* firstRowLayout = new QHBoxLayout();
    QLabel* repeatCountLabel = new QLabel("Repeat count: ", this);
    QFont labelFont = setStyleSheetForQLabel(repeatCountLabel, 10, false);
    repeatCountSpinBox = new QSpinBox(this);
    repeatCountSpinBox->setMinimum(1);
    repeatCountSpinBox->setValue(1);
    QLabel* coolingStepsLabel = new QLabel("Cooling steps: ", this);
    coolingStepsLabel->setAlignment(Qt::AlignRight);
    coolingStepsLabel->setFont(labelFont);
    coolingStepsSpinBox = new QSpinBox(this);
    coolingStepsSpinBox->setMinimum(1);
    coolingStepsSpinBox->setMaximum(5000);
    coolingStepsSpinBox->setValue(500);
    QLabel* coolingFractionLabel = new QLabel("Cooling fraction: ", this);
    coolingFractionLabel->setAlignment(Qt::AlignRight);
    coolingFractionLabel->setFont(labelFont);
    coolingFractionSpinBox = new QDoubleSpinBox(this);
    coolingFractionSpinBox->setMinimum(0.0);
    coolingFractionSpinBox->setValue(0.97);
    firstRowLayout->addWidget(repeatCountLabel);
    firstRowLayout->addWidget(repeatCountSpinBox);
    firstRowLayout->addWidget(coolingStepsLabel);
    firstRowLayout->addWidget(coolingStepsSpinBox);
    firstRowLayout->addWidget(coolingFractionLabel);
    firstRowLayout->addWidget(coolingFractionSpinBox);

    QHBoxLayout* secondRowLayout = new QHBoxLayout();
    QLabel* initialTemperatureLabel = new QLabel("Initial temperature: ", this);
    initialTemperatureLabel->setAlignment(Qt::AlignRight);
    initialTemperatureLabel->setFont(labelFont);
    initialTemperatureSpinBox = new QDoubleSpinBox(this);
    initialTemperatureSpinBox->setMinimum(0.0);
    initialTemperatureSpinBox->setMaximum(100.0);
    initialTemperatureSpinBox->setValue(1);
    QLabel* stepsPerTempLabel = new QLabel("Steps per temp: ", this);
    stepsPerTempLabel->setAlignment(Qt::AlignRight);
    stepsPerTempLabel->setFont(labelFont);
    stepsPerTempSpinBox = new QSpinBox(this);
    stepsPerTempSpinBox->setMinimum(1);
    stepsPerTempSpinBox->setMaximum(10000);
    stepsPerTempSpinBox->setValue(1000);
    QLabel* kLabel = new QLabel("K: ", this);
    kLabel->setAlignment(Qt::AlignRight);
    kLabel->setFont(labelFont);
    kSpinBox = new QDoubleSpinBox(this);
    kSpinBox->setMinimum(0.0);
    kSpinBox->setValue(0.01);
    secondRowLayout->addWidget(initialTemperatureLabel);
    secondRowLayout->addWidget(initialTemperatureSpinBox);
    secondRowLayout->addWidget(stepsPerTempLabel);
    secondRowLayout->addWidget(stepsPerTempSpinBox);
    secondRowLayout->addWidget(kLabel);
    secondRowLayout->addWidget(kSpinBox);

    QHBoxLayout* thirdRowLayout = new QHBoxLayout();
    QLabel* randomSamplingLabel = new QLabel("Random sampling count: ", this);
    randomSamplingLabel->setAlignment(Qt::AlignRight);
    randomSamplingLabel->setFont(labelFont);
    randomSamplingSpinBox = new QSpinBox(this);
    randomSamplingSpinBox->setMinimum(0);
    randomSamplingSpinBox->setMaximum(1500000);
    randomSamplingSpinBox->setValue(150000);
    QLabel* hillClimbingLabel = new QLabel("Hill climbing count: ", this);
    hillClimbingLabel->setAlignment(Qt::AlignRight);
    hillClimbingLabel->setFont(labelFont);
    hillClimbingSpinBox = new QSpinBox(this);
    hillClimbingSpinBox->setMinimum(0);
    hillClimbingSpinBox->setMaximum(1000);
    hillClimbingSpinBox->setValue(200);
    QLabel* annealingLabel = new QLabel("Annealing count: ", this);
    annealingLabel->setAlignment(Qt::AlignRight);
    annealingLabel->setFont(labelFont);
    annealingSpinBox = new QSpinBox(this);
    annealingSpinBox->setMinimum(0);
    annealingSpinBox->setValue(10);
    thirdRowLayout->addWidget(randomSamplingLabel);
    thirdRowLayout->addWidget(randomSamplingSpinBox);
    thirdRowLayout->addWidget(hillClimbingLabel);
    thirdRowLayout->addWidget(hillClimbingSpinBox);
    thirdRowLayout->addWidget(annealingLabel);
    thirdRowLayout->addWidget(annealingSpinBox);

    QHBoxLayout* costLayout = new QHBoxLayout();
    QLabel* optimalCostLabel = new QLabel("Optimal = ", this);
    optimalCostLabel->setAlignment(Qt::AlignRight);
    optimalCostLabel->setFont(labelFont);
    optimalCostLabel->setStyleSheet("color: red;");
    optimalCostLineEdit = new QLineEdit(this);
    optimalCostLineEdit->setStyleSheet("color: red;");
    resCostLabel = new QLabel("Current = ", this);
    resCostLabel->setAlignment(Qt::AlignRight);
    resCostLabel->setFont(labelFont);
    resCostLabel->setStyleSheet("color: green;");
    resCostLineEdit = new QLineEdit(this);
    resCostLineEdit->setStyleSheet("color: green;");
    costLayout->addWidget(optimalCostLabel);
    costLayout->addWidget(optimalCostLineEdit);
    costLayout->addWidget(resCostLabel);
    costLayout->addWidget(resCostLineEdit);

    QHBoxLayout* pathLayout = new QHBoxLayout();
    QLabel* pathLabel = new QLabel("Path: ", this);
    pathLabel->setAlignment(Qt::AlignRight);
    pathLabel->setFont(labelFont);
    pathLabel->setStyleSheet("color: green;");
    pathLineEdit = new MarqueeLineEdit(this);
    pathLineEdit->setStyleSheet("color: green;");
    pathLayout->addWidget(pathLabel);
    pathLayout->addWidget(pathLineEdit);

    QHBoxLayout* zoomLayout = new QHBoxLayout();
    zoomLayout->addWidget(zoomInButton);
    zoomLayout->addWidget(zoomOutButton);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addLayout(firstRowLayout);
    layout->addLayout(secondRowLayout);
    layout->addLayout(thirdRowLayout);
    layout->addWidget(view);
    layout->addWidget(loadFromFileButton, 0, Qt::AlignCenter);
    layout->addWidget(calcButton, 0, Qt::AlignCenter);
    layout->addLayout(zoomLayout);
    layout->addLayout(costLayout);
    layout->addLayout(pathLayout);

    setLayout(layout);

    connect(calcButton, &QPushButton::clicked, this, &Dialog::on_calcButton_clicked);
    connect(loadFromFileButton, &QPushButton::clicked, this, &Dialog::on_loadFromFileButton_clicked);
    connect(zoomInButton, &QPushButton::clicked, this, &Dialog::on_zoomIn_clicked);
    connect(zoomOutButton, &QPushButton::clicked, this, &Dialog::on_zoomOut_clicked);
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
        optimalCostLineEdit->setText(optimalSolution);

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
    QLocale locale;
    repeatCount = repeatCountSpinBox->text().toInt();
    initialTemperature = locale.toDouble(initialTemperatureSpinBox->text());
    coolingSteps = coolingStepsSpinBox->text().toInt();
    coolingFraction = locale.toDouble(coolingFractionSpinBox->text());
    stepsPerTemp = stepsPerTempSpinBox->text().toInt();
    K = locale.toDouble(kSpinBox->text());

    srand(time(NULL));

    resCostLabel->setText("Current = ");
    pathLineEdit->setText("");
    resCostLineEdit->setText("");
    TSP_helper::initialize_solution(t.n, &s);

    int randomSamplingCount = randomSamplingSpinBox->text().toInt();
    random_sampling(&t, randomSamplingCount, &s, this);

    int hillClimbingCount = hillClimbingSpinBox->text().toInt();
    repeated_hill_climbing(&t, hillClimbingCount, &s, this);

    int annealingCount = annealingSpinBox->text().toInt();
    repeated_annealing(&t, annealingCount, &s, this);

    drawSolution(&t, &s);
    setResults();
}

void Dialog::drawVertices(tsp_instance* t)
{
    for (int i = 1; i <= t->n; i++)
    {
        QGraphicsEllipseItem* pointItem = new QGraphicsEllipseItem(t->p[i].x - vertextRadius, t->p[i].y - vertextRadius, 2*vertextRadius, 2*vertextRadius);
        pointItem->setBrush(Qt::red);
        scene->addItem(pointItem);

        QGraphicsTextItem* textItem = new QGraphicsTextItem(QString::number(i));
        QFont font = textItem->font();
        font.setPointSize(pointSize);
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

    resCostLabel->setText("Result = ");
    double resCost = TSP_helper::solution_cost(&s,&t);
    resCostLineEdit->setText(QString::number(resCost));
}

void Dialog::drawSolution(tsp_instance* t, tsp_solution* s)
{
    scene->clear();

    drawVertices(t);

    QPen pen;
    pen.setWidth(edgeThickness);
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

void Dialog::on_zoomIn_clicked()
{
    scaleFactor *= drawCoeff;
    vertextRadius /= drawCoeff;
    edgeThickness /= drawCoeff;
    view->setTransform(QTransform::fromScale(scaleFactor, scaleFactor));
}

void Dialog::on_zoomOut_clicked()
{
    scaleFactor /= drawCoeff;
    vertextRadius *= drawCoeff;
    edgeThickness *= drawCoeff;
    view->setTransform(QTransform::fromScale(scaleFactor, scaleFactor));
}

QFont Dialog::setStyleSheetForQLabel(QLabel *label, int fontSize, bool isItalic)
{
    label->setAlignment(Qt::AlignRight);
    QFont font = label->font();
    font.setPointSize(fontSize);
    font.setBold(true);
    font.setItalic(isItalic);
    label->setFont(font);
    return font;
}
