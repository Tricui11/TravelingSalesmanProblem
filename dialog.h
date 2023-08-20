#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
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
    void on_calcAndDrawButton_clicked();
    void zoomIn();
    void zoomOut();

private:
    Ui::Dialog *ui;
    QGraphicsView* view;
    QGraphicsScene* scene;
    QPushButton* calcAndDrawButton;
    QPushButton* zoomInButton;
    QPushButton* zoomOutButton;
    qreal scaleFactor;
};
#endif // DIALOG_H
