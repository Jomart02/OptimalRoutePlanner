#pragma once
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QList>
#include <QPolygonF>
#include <QProcess>
#include "RouteScene.h"
#include "globalVar.h"
namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addPointClicked();
//     void on_generateObstaclesButton_clicked();
//     void on_setCorridorButton_clicked();
//     void on_runMatlabButton_clicked();
//     void on_clearButton_clicked();

protected:
//     void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::MainWindow *ui;
    RouteScene *scene = nullptr;
//     QList<QPointF> waypoints;
//     QList<QPolygonF> obstacles;
//     QPolygonF corridor;

//     void saveData();
//     void loadData();
//     bool polygonsIntersect(const QPolygonF &p1, const QPolygonF &p2);
//     QPolygonF generateRandomPolygon(const QRectF &bounds, int vertices = 5);
//     void runMatlabScript();
};