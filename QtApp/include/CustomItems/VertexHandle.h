#pragma once
#include <QGraphicsEllipseItem>

class CustomPolygonItem;

class VertexHandle : public QGraphicsEllipseItem
{
public:
    VertexHandle(CustomPolygonItem *parent, int index);
    int index() const { return m_index; }

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    CustomPolygonItem *m_polygon;
    int m_index;
    bool m_dragging = false;
    QPointF m_dragStart;
};