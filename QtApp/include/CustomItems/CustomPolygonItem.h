#pragma once

#include <QGraphicsPolygonItem>
#include <QGraphicsEllipseItem>
#include <QList>
#include <QObject>

class VertexHandle;

class CustomPolygonItem : public QObject, public QGraphicsPolygonItem
{
    Q_OBJECT
    friend VertexHandle;
public:
    explicit CustomPolygonItem(const QPolygonF &polygon, QGraphicsItem *parent = nullptr);
    ~CustomPolygonItem() override;

    void enableEditing(bool enable);
    bool isEditingEnabled() const { return m_editingEnabled; }

    QPolygonF polygon() const { return QGraphicsPolygonItem::polygon(); }

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    bool m_editingEnabled = false;
    bool m_dragging = false;
    QPointF m_dragStartPos;

    QList<VertexHandle*> m_vertices;

    void updateVertexHandles();
    void updatePolygonFromHandles();

signals:
    void polygonChanged(const QPolygonF &poly);
    void selectedChanged(bool selected);
};