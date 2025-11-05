#include "VertexHandle.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>

VertexHandle::VertexHandle(QGraphicsItem *parent)
    : QGraphicsObject(parent)
{
    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setCursor(Qt::SizeAllCursor);
}

QRectF VertexHandle::boundingRect() const
{
    return QRectF(-HANDLE_SIZE/2, -HANDLE_SIZE/2, HANDLE_SIZE, HANDLE_SIZE);
}

void VertexHandle::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setPen(QPen(QColor(200, 100, 0), 2));
    painter->setBrush(QBrush(QColor(255, 200, 0)));
    painter->drawEllipse(boundingRect());
}

void VertexHandle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF delta = event->scenePos() - event->lastScenePos();
    setPos(pos() + delta);  // ← pos() — локальные (от родителя)
    QGraphicsObject::mouseMoveEvent(event);
}

void VertexHandle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsObject::mouseReleaseEvent(event);
}

QVariant VertexHandle::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionHasChanged) {
        // ← localPos = pos() (локальные координаты родителя)
        emit moved(m_index, pos());
    }
    return QGraphicsObject::itemChange(change, value);
}