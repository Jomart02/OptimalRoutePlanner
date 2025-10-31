#include "VertexHandle.h"
#include "CustomPolygonItem.h"
#include <QBrush>
#include <QPen>
#include <QGraphicsSceneMouseEvent>

VertexHandle::VertexHandle(CustomPolygonItem *parent, int index)
    : QGraphicsEllipseItem(-8, -8, 16, 16, parent)
    , m_polygon(parent)
    , m_index(index)
{
    setBrush(QBrush(QColor(255, 200, 0)));
    setPen(QPen(QColor(200, 100, 0), 2));
    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

void VertexHandle::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    setBrush(QBrush(QColor(255, 255, 100)));
    setPen(QPen(QColor(255, 150, 0), 3));
    // setCursor(Qt::SizeAllCursor);
    QGraphicsEllipseItem::hoverEnterEvent(event);
}

void VertexHandle::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setBrush(QBrush(QColor(255, 200, 0)));
    setPen(QPen(QColor(200, 100, 0), 2));
    unsetCursor();
    QGraphicsEllipseItem::hoverLeaveEvent(event);
}

void VertexHandle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_dragging = true;
    m_dragStart = pos();
    QGraphicsEllipseItem::mousePressEvent(event);
}

void VertexHandle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_dragging) {
        QPointF newPos = parentItem()->mapFromScene(event->scenePos());
        setPos(newPos);
    }
    QGraphicsEllipseItem::mouseMoveEvent(event);
}

void VertexHandle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_dragging) {
        m_dragging = false;
        m_polygon->updatePolygonFromHandles();
    }
    QGraphicsEllipseItem::mouseReleaseEvent(event);
}