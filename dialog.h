#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QGraphicsScene>
#include <QPushButton>
#include <QLabel>
#include <marquee_line_edit.h>
#include <tsp_instance.cpp>
#include <tsp_solution.cpp>

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    QLineEdit* resCostLineEdit;

    Dialog(QWidget *parent = nullptr);
    ~Dialog();
    void drawSolution(tsp_instance* t, tsp_solution* s);

private slots:
    void on_calcButton_clicked();
    void on_loadFromFileButton_clicked();
    void on_zoomIn_clicked();
    void on_zoomOut_clicked();

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
    QSpinBox* repeatCountSpinBox;
    QDoubleSpinBox* initialTemperatureSpinBox;
    QSpinBox* coolingStepsSpinBox;
    QDoubleSpinBox* coolingFractionSpinBox;
    QSpinBox* stepsPerTempSpinBox;
    QDoubleSpinBox* kSpinBox;
    QSpinBox* randomSamplingSpinBox;
    QSpinBox* hillClimbingSpinBox;
    QSpinBox* annealingSpinBox;
    MarqueeLineEdit* pathLineEdit;
    QLineEdit* optimalCostLineEdit;
    QLabel* resCostLabel;

    qreal scaleFactor = 0.087;
    double vertextRadius = 30;
    double edgeThickness = 11;
    double pointSize = 100;
    double drawCoeff = 1.2;

    void drawVertices(tsp_instance* t);

    void setResults();

    QFont setStyleSheetForQLabel(QLabel *label, int fontSize, bool isItalic);
};
#endif // DIALOG_H
