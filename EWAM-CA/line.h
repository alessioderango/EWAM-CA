#ifndef LINE_H
#define LINE_H

#include <QColor>
#include <QGraphicsItem>

class Line : public QGraphicsLineItem
{
public:
    Line(const QColor &color, int x1, int y1, int x2, int y2);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    int x1;
    int y1;
    int x2;
    int y2;
    QColor color;
    QVector<QPointF> stuff;
};


#endif // LINE_H
