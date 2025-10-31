// QtApp/src/RouteScene.cpp
#include "RouteScene.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsTextItem>
#include <QRandomGenerator>
#include <QPainter>
#include <QLineF>
#include <QDebug>
#include <cmath>
#include <QGraphicsSceneMouseEvent>

static qreal randomInRange(qreal min, qreal max)
{
    return min + QRandomGenerator::global()->generateDouble() * (max - min);
}

RouteScene::RouteScene(QObject *parent)
    : QGraphicsScene(parent)
{
    // Сцена: 1000x1000, центр в (0,0)
    setSceneRect(-500, -500, 1000, 1000);
    setBackgroundBrush(QBrush(QColor(245, 250, 255)));

    // Зона генерации: 800x800, центр (0,0)
    m_generationArea = QRectF(-400, -400, 800, 800);

    redrawAll();
}

RouteScene::~RouteScene()
{
    clearObstacleItems();
}

void RouteScene::setMode(SceneMode mode)
{
    if (m_mode == mode) return;
    m_mode = mode;

    switch (mode) {
    case SceneMode::AddWaypoint:
        emit statusMessage("Режим: добавление точек (клик по сцене)");
        break;
    default:
        emit statusMessage("Режим: просмотр");
        break;
    }
}

void RouteScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        QGraphicsScene::mousePressEvent(event);
        return;
    }

    QPointF pt = event->scenePos();

    if (m_mode == SceneMode::AddWaypoint) {
        if (pointNearExisting(pt)) {
            emit statusMessage("Точка слишком близко!");
            return;
        }

        m_waypoints.append(pt);
        emit waypointAdded(pt);
        emit dataChanged();
        redrawAll();

        qDebug() << (QString("Точка %1: (%2, %3)")
                           .arg(m_waypoints.size()).arg(pt.x()).arg(pt.y()));
    }

    QGraphicsScene::mousePressEvent(event);
}

void RouteScene::generateObstacles(int count)
{
    qDebug() << "=== ГЕНЕРАЦИЯ КАСТОМНЫХ ПРЕПЯТСТВИЙ ===";

    // Очистка
    for (auto *item : m_obstacleItems) {
        removeItem(item);
        delete item;
    }
    m_obstacleItems.clear();
    m_obstacles.clear();

    int generated = 0;
    int attempts = 0;
    const int maxAttempts = 50;

    while (generated < count && attempts < maxAttempts) {
        int vertices = QRandomGenerator::global()->bounded(4, 9);
        QPolygonF poly = generateRandomPolygon(m_generationArea, vertices);

        bool valid = true;
        for (const auto &obs : m_obstacles) {
            if (polygonsIntersect(poly, obs)) {
                valid = false;
                break;
            }
        }

        if (valid) {
            CustomPolygonItem *item = new CustomPolygonItem(poly);
            item->enableEditing(true);  // ← Включаем редактирование
            item->setZValue(2);

            connect(item, &CustomPolygonItem::polygonChanged, this, [this, item](const QPolygonF &p) {
                int idx = m_obstacleItems.indexOf(item);
                if (idx != -1) m_obstacles[idx] = p;
                emit dataChanged();
            });

            m_obstacleItems.append(item);
            m_obstacles.append(poly);
            addItem(item);
            ++generated;

            qDebug() << "Препятствие" << generated << "создано и редактируемо";
        }
        ++attempts;
    }

    emit dataChanged();
}

void RouteScene::setCorridor(const QPolygonF &corridor)
{
    m_corridor = corridor;
    redrawAll();
    emit dataChanged();
}

void RouteScene::clearAll()
{
    m_waypoints.clear();
    m_obstacles.clear();
    m_corridor.clear();

    for (auto *item : m_obstacleItems) {
        removeItem(item);
        delete item;
    }
    m_obstacleItems.clear();

    clear();
    redrawAll();
    emit dataChanged();
}

void RouteScene::clearObstacleItems()
{
    for (auto *item : m_obstacleItems) {
        removeItem(item);
        delete item;
    }
    m_obstacleItems.clear();
}

void RouteScene::drawGenerationZone()
{
    // Рисуем зону генерации
    QGraphicsRectItem *zone = addRect(m_generationArea, QPen(QColor(0, 150, 255), 3, Qt::DashLine), QBrush(Qt::NoBrush));
    zone->setZValue(1);
}

void RouteScene::redrawAll()
{
    clear();
    drawGenerationZone();

    // Препятствия — уже добавлены в generateObstaclesInFixedArea()
    // (если нужно — можно перерисовать)

    // Коридор
    if (!m_corridor.isEmpty()) {
        addPolygon(m_corridor, QPen(Qt::green, 3), QBrush(QColor(0, 255, 0, 40)))->setZValue(3);
    }

    // Точки
    for (int i = 0; i < m_waypoints.size(); ++i) {
        const QPointF &pt = m_waypoints[i];
        addEllipse(pt.x()-7, pt.y()-7, 14, 14, QPen(Qt::blue, 2.5), QBrush(Qt::blue))->setZValue(10);
        addText(QString::number(i+1))->setDefaultTextColor(Qt::white);
    }
}

QPolygonF RouteScene::generateRandomPolygon(const QRectF &bounds, int vertices)
{
    QPolygonF poly;
    qreal cx = randomInRange(bounds.left() + 50, bounds.right() - 50);
    qreal cy = randomInRange(bounds.top() + 50, bounds.bottom() - 50);
    qreal baseRadius = randomInRange(40.0, 90.0);

    for (int i = 0; i < vertices; ++i) {
        qreal angle = 2 * M_PI * i / vertices + randomInRange(-0.7, 0.7);
        qreal r = baseRadius * (0.6 + randomInRange(0.0, 0.4));
        qreal x = cx + r * std::cos(angle);
        qreal y = cy + r * std::sin(angle);
        poly << QPointF(x, y);
    }
    return poly;
}

bool RouteScene::polygonsIntersect(const QPolygonF &p1, const QPolygonF &p2) const
{
    if (!p1.boundingRect().intersects(p2.boundingRect())) return false;

    auto edgesIntersect = [](const QPointF &a1, const QPointF &a2,
                             const QPointF &b1, const QPointF &b2) -> bool {
        auto ccw = [](const QPointF &a, const QPointF &b, const QPointF &c) -> bool {
            return (c.y() - a.y()) * (b.x() - a.x()) > (b.y() - a.y()) * (c.x() - a.x());
        };
        return ccw(a1, b1, b2) != ccw(a2, b1, b2) && ccw(a1, a2, b1) != ccw(a1, a2, b2);
    };

    for (int i = 0; i < p1.size(); ++i) {
        QPointF a1 = p1[i], a2 = p1[(i+1)%p1.size()];
        for (int j = 0; j < p2.size(); ++j) {
            QPointF b1 = p2[j], b2 = p2[(j+1)%p2.size()];
            if (edgesIntersect(a1, a2, b1, b2)) return true;
        }
    }
    return false;
}

bool RouteScene::pointNearExisting(const QPointF &pt, qreal minDist) const
{
    for (const auto &p : m_waypoints) {
        if (QLineF(pt, p).length() < minDist) return true;
    }
    return false;
}

void RouteScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);
    painter->fillRect(sceneRect(), QBrush(QColor(245, 250, 255)));

    // Сетка
    painter->setPen(QPen(QColor(220, 230, 240), 1, Qt::DotLine));
    for (qreal x = -500; x <= 500; x += 50) {
        painter->drawLine(QLineF(x, -500, x, 500));
    }
    for (qreal y = -500; y <= 500; y += 50) {
        painter->drawLine(QLineF(-500, y, 500, y));
    }

    // Оси
    painter->setPen(QPen(Qt::black, 2));
    painter->drawLine(QLineF(-500, 0, 500, 0));
    painter->drawLine(QLineF(0, -500, 0, 500));
}