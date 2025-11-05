#pragma once

#include <QGraphicsPolygonItem>
#include <QGraphicsEllipseItem>
#include <QList>
#include <QObject>
#include <QPainter>
class VertexHandle;

class CustomPolygonItem : public QGraphicsObject
{
    Q_OBJECT

public:
    explicit CustomPolygonItem(const QPolygonF &polygon, QGraphicsItem *parent = nullptr);
    ~CustomPolygonItem() override;

    void enableEditing(bool enable);

    // Доступ к полигону
    QPolygonF polygon() const { return m_polygon; }
    void setPolygon(const QPolygonF &poly);

signals:
    void polygonChanged(const QPolygonF &polygon);
    void selectedChanged(bool selected);

protected:
    // QGraphicsItem interface — ОБЯЗАТЕЛЬНО!
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private slots:
    void onVertexMoved(int index, const QPointF &localPos);  // ← Локальные координаты!

private:
    void createVertexHandles();
    void updateVertexHandles();
    void showVertexHandles(bool show);

    QPolygonF m_polygon;  // ← Храним полигон здесь (нет отдельного элемента!)
    QList<VertexHandle*> m_vertexHandles;

    bool m_editingEnabled = false;
    bool m_dragging = false;
    QPointF m_dragStartPos;

    QPen m_normalPen;
    QPen m_hoverPen;
    QPen m_selectedPen;
    QBrush m_brush;
};