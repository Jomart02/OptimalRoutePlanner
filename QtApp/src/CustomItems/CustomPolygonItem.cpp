#include "CustomPolygonItem.h"
#include "VertexHandle.h"
#include <QBrush>
#include <QPen>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>

CustomPolygonItem::CustomPolygonItem(const QPolygonF &polygon, QGraphicsItem *parent)
    : QGraphicsPolygonItem(polygon, parent)
{
    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    setBrush(QBrush(QColor(220, 60, 60, 180)));
    setPen(QPen(QColor(180, 0, 0), 2));

    updateVertexHandles();
}

CustomPolygonItem::~CustomPolygonItem()
{
    for (auto *v : m_vertices) {
        scene()->removeItem(v);
        delete v;
    }
}

void CustomPolygonItem::enableEditing(bool enable)
{
    m_editingEnabled = enable;
    for (auto *v : m_vertices) {
        v->setVisible(enable);
    }
}

void CustomPolygonItem::updateVertexHandles()
{
    // Удаляем старые
    for (auto *v : m_vertices) {
        scene()->removeItem(v);
        delete v;
    }
    m_vertices.clear();

    if (!m_editingEnabled) return;

    const QPolygonF &poly = polygon();
    for (int i = 0; i < poly.size(); ++i) {
        VertexHandle *handle = new VertexHandle(this, i);
        handle->setPos(poly[i]);
        handle->setVisible(true);
        m_vertices.append(handle);
    }
}

void CustomPolygonItem::updatePolygonFromHandles()
{
    QPolygonF poly;
    for (auto *v : m_vertices) {
        poly << v->pos();
    }
    setPolygon(poly);
    emit polygonChanged(poly);
}

void CustomPolygonItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    setPen(QPen(QColor(255, 100, 100), 3));
    setCursor(Qt::OpenHandCursor);
    QGraphicsPolygonItem::hoverEnterEvent(event);
}

void CustomPolygonItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if (!isSelected()) {
        setPen(QPen(QColor(180, 0, 0), 2));
    }
    unsetCursor();
    QGraphicsPolygonItem::hoverLeaveEvent(event);
}

void CustomPolygonItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_editingEnabled) {
        m_dragging = true;
        m_dragStartPos = pos();
        setCursor(Qt::ClosedHandCursor);
    }
    QGraphicsPolygonItem::mousePressEvent(event);
}

void CustomPolygonItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_dragging) {
        QPointF delta = event->scenePos() - event->lastScenePos();
        moveBy(delta.x(), delta.y());
    }
    QGraphicsPolygonItem::mouseMoveEvent(event);
}

void CustomPolygonItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_dragging) {
        m_dragging = false;
        unsetCursor();
        emit polygonChanged(polygon());
    }
    QGraphicsPolygonItem::mouseReleaseEvent(event);
}

QVariant CustomPolygonItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSelectedHasChanged) {
        bool sel = value.toBool();
        setPen(sel ? QPen(QColor(255, 150, 0), 3) : QPen(QColor(180, 0, 0), 2));
        emit selectedChanged(sel);
    }
    return QGraphicsPolygonItem::itemChange(change, value);
}