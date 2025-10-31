#pragma once
#include <QGraphicsScene>
#include <QList>
#include <QPolygonF>
#include <QPointF>
#include <QGraphicsPolygonItem>
#include <QRectF>
#include "globalVar.h"
#include "CustomPolygonItem.h"
class RouteScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit RouteScene(QObject *parent = nullptr);
    ~RouteScene() override;

    void setMode(SceneMode mode);
    SceneMode currentMode() const { return m_mode; }

    const QList<QPointF>& waypoints() const { return m_waypoints; }
    const QList<QPolygonF>& obstacles() const { return m_obstacles; }
    const QPolygonF& corridor() const { return m_corridor; }

    // ГЕНЕРАЦИЯ В ФИКСИРОВАННОЙ ЗОНЕ
    void generateObstacles(int count);
    void setCorridor(const QPolygonF &corridor);
    void clearAll();

signals:
    void waypointAdded(const QPointF &point);
    void statusMessage(const QString &msg);
    void dataChanged();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void drawBackground(QPainter *painter, const QRectF &rect) override;

private:
    SceneMode m_mode = SceneMode::None;
    QList<QPointF> m_waypoints;
    QList<QPolygonF> m_obstacles;
    QPolygonF m_corridor;

    QList<CustomPolygonItem*> m_obstacleItems;

    // Фиксированная зона генерации: 800x800 в центре
    QRectF m_generationArea = QRectF(-400, -400, 800, 800);

    QPolygonF generateRandomPolygon(const QRectF &bounds, int vertices = 5);
    bool polygonsIntersect(const QPolygonF &p1, const QPolygonF &p2) const;
    bool pointNearExisting(const QPointF &pt, qreal minDist = 20.0) const;

    void clearObstacleItems();
    void drawGenerationZone();  // Рисуем зону
    void redrawAll();
};