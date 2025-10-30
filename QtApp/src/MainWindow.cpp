#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMouseEvent>
#include <QFile>
#include <QTextStream>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
     ui->setupUi(this);
     scene = new RouteScene(this);
     ui->graphicsView->setScene(scene);
     connect(ui->pushButton_AddPt,&QPushButton::clicked, this, &MainWindow::addPointClicked);
     connect(ui->pushButton_GenObstacles,&QPushButton::clicked, [&](){
          scene->generateObstacles(10);
     });

}
MainWindow::~MainWindow(){

}

void MainWindow::addPointClicked(){
     bool checked = ui->pushButton_AddPt->isChecked();
     scene->setMode(checked ? SceneMode::AddWaypoint : SceneMode::None);

     ui->pushButton_GenObstacles->setEnabled(!checked);
}



// void MainWindow::on_generateObstaclesButton_clicked()
// {
//     scene->clear();
//     obstacles.clear();
//     int numObstacles = ui->spinObstacles->value();
//     QRectF bounds(50, 50, 700, 500);

//     for (int i = 0; i < numObstacles; ++i) {
//         QPolygonF poly;
//         bool valid = false;
//         int attempts = 0;
//         while (!valid && attempts < 100) {
//             poly = generateRandomPolygon(bounds);
//             valid = true;
//             for (const auto &obs : obstacles) {
//                 if (polygonsIntersect(poly, obs)) {
//                     valid = false;
//                     break;
//                 }
//             }
//             ++attempts;
//         }
//         if (valid) {
//             obstacles.append(poly);
//             QGraphicsPolygonItem *item = scene->addPolygon(poly, QPen(Qt::red), QBrush(Qt::darkRed));
//             item->setOpacity(0.6);
//         }
//     }

//     // Перерисовать waypoints
//     for (int i = 0; i < waypoints.size(); ++i) {
//         QPointF pt = waypoints[i];
//         scene->addEllipse(pt.x()-5, pt.y()-5, 10, 10, QPen(Qt::blue), QBrush(Qt::blue));
//         QGraphicsTextItem *text = scene->addText(QString::number(i+1));
//         text->setDefaultTextColor(Qt::white);
//         text->setPos(pt.x() - 10, pt.y() - 10);
//     }

//     saveData();
// }

// QPolygonF MainWindow::generateRandomPolygon(const QRectF &bounds, int vertices)
// {
//     QPolygonF poly;
//     qreal cx = QRandomGenerator::global()->bounded(bounds.left(), bounds.right());
//     qreal cy = QRandomGenerator::global()->bounded(bounds.top(), bounds.bottom());
//     qreal radius = QRandomGenerator::global()->bounded(30.0, 80.0);

//     for (int i = 0; i < vertices; ++i) {
//         qreal angle = 2 * M_PI * i / vertices + QRandomGenerator::global()->bounded(-0.3, 0.3);
//         qreal r = radius * (0.7 + QRandomGenerator::global()->bounded(0.0, 0.3));
//         qreal x = cx + r * cos(angle);
//         qreal y = cy + r * sin(angle);
//         poly << QPointF(x, y);
//     }
//     return poly;
// }

// bool MainWindow::polygonsIntersect(const QPolygonF &p1, const QPolygonF &p2)
// {
//     // Простая проверка: пересечение bounding box + SAT (упрощённо)
//     if (!p1.boundingRect().intersects(p2.boundingRect())) return false;

//     // Проверка пересечения рёбер
//     auto edgesIntersect = [](QPointF a1, QPointF a2, QPointF b1, QPointF b2) {
//         auto ccw = [](QPointF a, QPointF b, QPointF c) {
//             return (c.y() - a.y()) * (b.x() - a.x()) > (b.y() - a.y()) * (c.x() - a.x());
//         };
//         return ccw(a1, b1, b2) != ccw(a2, b1, b2) && ccw(a1, a2, b1) != ccw(a1, a2, b2);
//     };

//     int n1 = p1.size(), n2 = p2.size();
//     for (int i = 0; i < n1; ++i) {
//         QPointF a1 = p1[i], a2 = p1[(i+1)%n1];
//         for (int j = 0; j < n2; ++j) {
//             QPointF b1 = p2[j], b2 = p2[(j+1)%n2];
//             if (edgesIntersect(a1, a2, b1, b2)) return true;
//         }
//     }
//     return false;
// }

// void MainWindow::on_setCorridorButton_clicked()
// {
//     // Пример: коридор как прямоугольник
//     qreal x1 = ui->corridorX1->value();
//     qreal y1 = ui->corridorY1->value();
//     qreal x2 = ui->corridorX2->value();
//     qreal y2 = ui->corridorY2->value();

//     corridor.clear();
//     corridor << QPointF(x1, y1) << QPointF(x2, y1) << QPointF(x2, y2) << QPointF(x1, y2);

//     QGraphicsPolygonItem *item = scene->addPolygon(corridor, QPen(Qt::green, 3), QBrush(Qt::transparent));
//     item->setOpacity(0.4);
//     saveData();
// }

// void MainWindow::saveData()
// {
//     // Waypoints
//     QFile wpFile("../data/waypoints.txt");
//     if (wpFile.open(QIODevice::WriteOnly)) {
//         QTextStream out(&wpFile);
//         for (const auto &p : waypoints) {
//             out << p.x() << " " << p.y() << "\n";
//         }
//     }

//     // Obstacles
//     QFile obsFile("../data/obstacles.txt");
//     if (obsFile.open(QIODevice::WriteOnly)) {
//         QTextStream out(&obsFile);
//         out << obstacles.size() << "\n";
//         for (const auto &poly : obstacles) {
//             out << poly.size() << "\n";
//             for (const auto &p : poly) {
//                 out << p.x() << " " << p.y() << " ";
//             }
//             out << "\n";
//         }
//     }

//     // Corridor
//     QFile corrFile("../data/corridor.txt");
//     if (corrFile.open(QIODevice::WriteOnly)) {
//         QTextStream out(&corrFile);
//         for (const auto &p : corridor) {
//             out << p.x() << " " << p.y() << "\n";
//         }
//     }
// }

// void MainWindow::on_runMatlabButton_clicked()
// {
//     saveData();
//     runMatlabScript();
// }

// void MainWindow::runMatlabScript()
// {
//     QProcess process;
//     QString matlabCmd = "matlab -batch \"run('../MatlabScript/run_path_planning.m')\"";
    
//     process.start(matlabCmd);
//     if (!process.waitForStarted()) {
//         QMessageBox::critical(this, "Ошибка", "Не удалось запустить MATLAB");
//         return;
//     }
//     process.waitForFinished(-1);

//     QString output = process.readAllStandardOutput();
//     QString error = process.readAllStandardError();

//     if (!error.isEmpty()) {
//         QMessageBox::warning(this, "MATLAB Output", error);
//     } else {
//         QMessageBox::information(this, "Успех", "Траектория построена!\nСм. trajectory.txt и график.");
//         // Можно перерисовать траекторию
//     }
// }