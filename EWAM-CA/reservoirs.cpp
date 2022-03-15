/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "reservoirs.h"
#include "reservoirs.h"

#include <QtWidgets>

Reservoirs::Reservoirs(string id, const QColor &color, int x, int y, vector<QGraphicsLineItem*> line, bool _visible)
{
    this->x = x;
    this->y = y;
    this->color = color;
    this->id = id;
    setZValue((x + y) % 2);

    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
    this->line = line;
    visible = _visible;
}

Reservoirs::Reservoirs(string id, const QColor &color, int x, int y)
{
    this->x = x;
    this->y = y;
    this->color = color;
    this->id = id;
    setZValue((x + y) % 2);

    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
}

QRectF Reservoirs::boundingRect() const
{
    return QRectF(0, 0, 110, 70);
}

QPainterPath Reservoirs::shape() const
{
    QPainterPath path;
    path.addRect(14, 14, 82, 42);
    return path;
}

void Reservoirs::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QColor fillColor = (option->state & QStyle::State_Selected) ? color.dark(150) : color;
    if (option->state & QStyle::State_MouseOver)
        fillColor = fillColor.light(125);

    //const qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
    //    if (lod < 0.2) {
    //        if (lod < 0.125) {
    //            painter->fillRect(QRectF(0, 0, 110, 70), fillColor);
    //            return;
    //        }

    //        QBrush b = painter->brush();
    //        //painter->setBrush(fillColor);
    //        painter->drawEllipse(13, 13, 57, 57);
    //        //painter->drawRect(13, 13, 97, 57);
    //        painter->setBrush(b);
    //        return;
    //    }

    QPen oldPen = painter->pen();
    QPen pen = oldPen;
    int width = 0;
    if (option->state & QStyle::State_Selected)
        width += 2;

    pen.setWidth(width);
    QBrush b = painter->brush();
    QColor red(Qt::red);
    b.setColor(red);

    painter->setBrush(QBrush(this->color));
    //painter->setBrush(QBrush(fillColor.dark(option->state & QStyle::State_Sunken ? 120 : 100)));


    QPolygon poly;

    poly << QPoint(0, 30) << QPoint(0, 60)
         << QPoint(30, 90) << QPoint(60, 90)
         << QPoint(90, 60) << QPoint(90, 30)
         << QPoint(60, 0) << QPoint(30, 0)
         << QPoint(0, 30);

    //painter->drawEllipse(13, 13, 57, 57);
    painter->drawPolygon(poly);
    painter->drawText(13+15,13+35, QString::fromStdString(id));

//    for (int i = 0; i < line.size();i++) {
//        // Move the required point of the line to the center of the elipse
//        QPointF p1 = line[i]->line().p1();
//        //cout << p1.x() << " "<< p1.y() << endl;
//        QPointF p2 = line[i]->line().p2();
//        QPointF arrowP1 = line[i]->line().p1() + QPointF(sin(60 + M_PI / 3) * 10,
//                                        cos(60 + M_PI / 3) * 10);
//        QPointF arrowP2 = line[i]->line().p1() + QPointF(sin(60 + M_PI - M_PI / 3) * 10,
//                                        cos(60 + M_PI - M_PI / 3) * 10);
//        //cout << p2.x() << " "<< p2.y() << endl;
//        //line[i]->setPen(QPen(QColor(0,0,255)));
//        line[i]->setLine(QLineF(p1, p2));
//    }
}

void Reservoirs::changeColor(const QColor &_color)
{
    this->color = _color;
}

//void Reservoirs::setCAWDN(CAWDN *ca)
//{
//    this->ca = ca;
//}


void Reservoirs::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    update();
}

//void Reservoirs::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
//{
//    CAWDN::activeCells(this->id);
//}

void Reservoirs::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier) {
        stuff << event->pos();
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void Reservoirs::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}
