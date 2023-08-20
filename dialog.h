#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QGraphicsScene>
#include <QPushButton>
#include "tsp_instance.cpp"
#include "tsp_solution.cpp"

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();
    void drawSolution(tsp_instance* t, tsp_solution* s);

private slots:
    void on_calcButton_clicked();
    void on_loadFromFileButton_clicked();
    void zoomIn();
    void zoomOut();

private:
    Ui::Dialog *ui;
    tsp_instance t;
    tsp_solution s;
    QGraphicsView* view;
    QGraphicsScene* scene;
    QPushButton* calcButton;
    QPushButton* loadFromFileButton;
    QPushButton* zoomInButton;
    QPushButton* zoomOutButton;
    qreal scaleFactor;
    QSpinBox* repeatCountSpinBox;
    QDoubleSpinBox* initialTemperatureSpinBox;
    QSpinBox* coolingStepsSpinBox;
    QDoubleSpinBox* coolingFractionSpinBox;
    QSpinBox* stepsPerTempSpinBox;
    QDoubleSpinBox* kSpinBox;
    QLineEdit* pathLineEdit;
    QLineEdit* optimalDistanceLineEdit;
    QLineEdit* resDistanceLineEdit;

    void drawVertices(tsp_instance* t);

    void setResults();
};
#endif // DIALOG_H
