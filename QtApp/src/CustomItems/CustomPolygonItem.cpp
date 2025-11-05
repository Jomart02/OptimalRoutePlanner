#include "CustomPolygonItem.h"
#include "VertexHandle.h"
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QObject>

CustomPolygonItem::CustomPolygonItem(const QPolygonF &polygon, QGraphicsItem *parent)
    : QGraphicsObject(parent)
{
    m_polygon = polygon;

    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    // Инициализация стилей
    m_brush = QBrush(QColor(220, 60, 60, 180));
    m_normalPen = QPen(QColor(180, 0, 0), 2);
    m_hoverPen = QPen(QColor(255, 100, 100), 3);
    m_selectedPen = QPen(QColor(255, 150, 0), 3);

    createVertexHandles();
    showVertexHandles(false);
}

CustomPolygonItem::~CustomPolygonItem()
{
    qDeleteAll(m_vertexHandles);
}

QRectF CustomPolygonItem::boundingRect() const
{
    return m_polygon.boundingRect();  // ← Bounding rect полигона
}

void CustomPolygonItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setBrush(m_brush);
    painter->setPen(isSelected() ? m_selectedPen : (hasFocus() ? m_hoverPen : m_normalPen));
    painter->drawPolygon(m_polygon);  // ← Рисуем полигон вручную!
}

void CustomPolygonItem::setPolygon(const QPolygonF &poly)
{
    prepareGeometryChange();  // ← ОБЯЗАТЕЛЬНО для обновления boundingRect!
    m_polygon = poly;
    update();  // ← Перерисовка
    updateVertexHandles();
    emit polygonChanged(poly);
}

void CustomPolygonItem::enableEditing(bool enable)
{
    m_editingEnabled = enable;
    showVertexHandles(enable && isSelected());
}

void CustomPolygonItem::createVertexHandles()
{
    for (int i = 0; i < m_polygon.size(); ++i) {
        VertexHandle *handle = new VertexHandle(this);
        handle->setPos(m_polygon.at(i));  // ← Локальные координаты
        handle->setIndex(i);
        connect(handle, &VertexHandle::moved, this, &CustomPolygonItem::onVertexMoved);
        m_vertexHandles.append(handle);
    }
}

void CustomPolygonItem::onVertexMoved(int index, const QPointF &localPos)
{
    if (index >= 0 && index < m_polygon.size()) {
        prepareGeometryChange();
        m_polygon[index] = localPos;  // ← Прямо локальные!
        update();
        updateVertexHandles();
        emit polygonChanged(m_polygon);
    }
}

void CustomPolygonItem::updateVertexHandles()
{
    for (int i = 0; i < m_vertexHandles.size(); ++i) {
        if (i < m_polygon.size()) {
            m_vertexHandles[i]->setPos(m_polygon.at(i));  // ← Локальные → локальные
            m_vertexHandles[i]->setVisible(true);
        } else {
            m_vertexHandles[i]->setVisible(false);
        }
    }
}

void CustomPolygonItem::showVertexHandles(bool show)
{
    for (VertexHandle *handle : m_vertexHandles) {
        handle->setVisible(show);
    }
    updateVertexHandles();
}

// === События ===
void CustomPolygonItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    update();  // ← Перерисовка для hover-стиля
    setCursor(Qt::OpenHandCursor);
    QGraphicsObject::hoverEnterEvent(event);
}

void CustomPolygonItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    update();
    unsetCursor();
    QGraphicsObject::hoverLeaveEvent(event);
}

void CustomPolygonItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_editingEnabled) {
        m_dragging = true;
        setCursor(Qt::ClosedHandCursor);
    }
    QGraphicsObject::mousePressEvent(event);
}

void CustomPolygonItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_dragging && m_editingEnabled) {
        QPointF delta = event->scenePos() - event->lastScenePos();
        moveBy(delta.x(), delta.y());  // ← moveBy автоматически обновит itemChange
    }
    QGraphicsObject::mouseMoveEvent(event);
}

void CustomPolygonItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_dragging) {
        m_dragging = false;
        unsetCursor();
        emit polygonChanged(m_polygon);
    }
    QGraphicsObject::mouseReleaseEvent(event);
}

QVariant CustomPolygonItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSelectedHasChanged) {
        bool sel = value.toBool();
        update();  // ← Перерисовка для selected-стиля
        showVertexHandles(m_editingEnabled && sel);
        emit selectedChanged(sel);
    } else if (change == ItemPositionHasChanged) {
        // ← moveBy() автоматически обновит позиции вершин (они дети!)
        // Нет нужды в updateVertexHandles() — Qt сам!
    }
    return QGraphicsObject::itemChange(change, value);
}