#pragma once
#include <QGraphicsObject>  
#include <QBrush>
#include <QPen>

class VertexHandle : public QGraphicsObject
{
    Q_OBJECT

public:
    explicit VertexHandle(QGraphicsItem *parent = nullptr);
    void setIndex(int index) { m_index = index; }
    int index() const { return m_index; }

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

signals:
    void moved(int index, const QPointF &localPos);  // ← Изменил: index + localPos!

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    int m_index = -1;
    const qreal HANDLE_SIZE = 12.0;
};