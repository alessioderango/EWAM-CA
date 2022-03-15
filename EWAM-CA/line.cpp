#include "line.h"

#include <QtWidgets>

Line::Line(const QColor &color, int x1, int y1, int x2, int y2)
{
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
    this->color = color;
    setZValue((x1 + y1) % 2);

    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
}

QRectF Line::boundingRect() const
{
    return QRectF(0, 0, 110, 70);
}

QPainterPath Line::shape() const
{
    QPainterPath path;
    return path;
}

void Line::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
//    QVarLengthArray<QLineF, 1> lines;
//    lines.append(QLineF(x1, y1,x2, y2));
//    painter->drawLines(lines.data(), lines.size());

}

void Line::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    update();
}

void Line::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier) {
        stuff << event->pos();
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void Line::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}
