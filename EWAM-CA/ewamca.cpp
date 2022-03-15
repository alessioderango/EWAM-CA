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

#include "ewamca.h"
#include "cawdn.h"

#include <QtWidgets>

Chip::Chip(string id, const QColor &color, int x, int y, vector<QGraphicsLineItem*> line, bool _visible, TIPO a)
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
    this->a = a;
}

Chip::Chip(string id, const QColor &color, int x, int y, TIPO a)
{
    this->x = x;
    this->y = y;
    this->color = color;
    this->id = id;
    setZValue((x + y) % 2);

    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
    this->a = a;
}

QRectF Chip::boundingRect() const
{
    return QRectF(0, 0, 110, 70);
}

QPainterPath Chip::shape() const
{
    QPainterPath path;
    path.addRect(14, 14, 82, 42);
    return path;
}

void Chip::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QColor fillColor = (option->state & QStyle::State_Selected) ? color.dark(150) : color;
    if (option->state & QStyle::State_MouseOver)
        fillColor = fillColor.light(125);

    QPen oldPen = painter->pen();
    QPen pen = oldPen;
    int width = 0;
    if (option->state & QStyle::State_Selected)
        width += 2;

    pen.setWidth(0);
    painter->setBrush(QBrush(this->color));
    switch (a)
    {
    case CERCHIO:
        painter->drawEllipse(13, 13, 57, 57);
        break;

    case OTTAGONO:
        QPolygon poly;

        poly << QPoint(0, 30) << QPoint(0, 60)
             << QPoint(30, 90)<< QPoint(60, 90)
             << QPoint(90, 60)<< QPoint(90, 30)
             << QPoint(60, 0) << QPoint(30, 0)
             << QPoint(0, 30);

        painter->drawPolygon(poly);
        break;

    }

    painter->drawText(13+15,13+35, QString::fromStdString(id));

}

void createDynamicContentInfo(InfoJunction *info, NodeAC nac)
{
    QLabel* l =info->findChild<QLabel*>("label_id_text");
    l->setText(QString::fromStdString(nac.id));
    l =info->findChild<QLabel*>("label_concentrazione_text");
    l->setText(QString::number(nac.concentrazione));
    //  l =info->findChild<QLabel*>("label_concentrazione_text");
    //  l->setText(QString::number(nac.concentrazione));

    QScrollArea *scrollArea = info->findChild<QScrollArea*>("scrollArea_link_uscenti");
    //scrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
    scrollArea->setWidgetResizable( true );
    scrollArea->setGeometry( 10, 10, 100, 100 );

    QWidget *widget = new QWidget();
    scrollArea->setWidget( widget );

    delete widget->layout();

    int numberofitmes=0;
    QGridLayout* qg = new QGridLayout(widget);//info->findChild<QGridLayout*>("gridLayout_link_uscenti");
    for (auto l = 0; l < nac.link_uscenti.size(); l++)
    {
        numberofitmes += nac.link_uscenti[l]->massa.size()+9+2;
        QString tmpS = QString::fromStdString(nac.link_uscenti[l]->id);
        QLabel * tmpLabel = new QLabel("link id: ");
        qg->addWidget(tmpLabel,l+numberofitmes,0);
        QLabel * tmp = new QLabel(tmpS);
        qg->addWidget(tmp,l+numberofitmes,1);

        tmpS = QString::number(nac.link_uscenti[l]->length);
        tmpLabel = new QLabel("length");
        qg->addWidget(tmpLabel,l+numberofitmes+1,0);
        tmp = new QLabel(tmpS);
        qg->addWidget(tmp,l+numberofitmes+1,1);

        tmpS = QString::number(nac.link_uscenti[l]->diameter);
        tmpLabel = new QLabel("diameter");
        qg->addWidget(tmpLabel,l+numberofitmes+2,0);
        tmp = new QLabel(tmpS);
        qg->addWidget(tmp,l+numberofitmes+2,1);

        tmpS = QString::number(nac.link_uscenti[l]->v);
        tmpLabel = new QLabel("v");
        qg->addWidget(tmpLabel,l+numberofitmes+3,0);
        tmp = new QLabel(tmpS);
        qg->addWidget(tmp,l+numberofitmes+3,1);

        tmpS = QString::number(nac.link_uscenti[l]->volume);
        tmpLabel = new QLabel("volume");
        qg->addWidget(tmpLabel,l+numberofitmes+4,0);
        tmp = new QLabel(tmpS);
        qg->addWidget(tmp,l+numberofitmes+4,1);

        tmpS = QString::number(nac.link_uscenti[l]->flow);
        tmpLabel = new QLabel("flow");
        qg->addWidget(tmpLabel,l+numberofitmes+5,0);
        tmp = new QLabel(tmpS);
        qg->addWidget(tmp,l+numberofitmes+5,1);

        tmpS = QString::number(nac.link_uscenti[l]->ni);
        tmpLabel = new QLabel("ni");
        qg->addWidget(tmpLabel,l+numberofitmes+6,0);
        tmp = new QLabel(tmpS);
        qg->addWidget(tmp,l+numberofitmes+6,1);

        tmpS = QString::fromStdString(nac.link_uscenti[l]->nodes[1]->id);
        tmpLabel = new QLabel("collegato al nodo: ");
        qg->addWidget(tmpLabel,l+numberofitmes+7,0);
        tmp = new QLabel(tmpS);
        qg->addWidget(tmp,l+numberofitmes+7,1);

        tmpLabel = new QLabel("finite volume : ");
        qg->addWidget(tmpLabel,l+numberofitmes+8,0);

        for (auto m = 0; m < nac.link_uscenti[l]->massa.size(); m++)
        {
            tmp = new QLabel(QString::number(m+1));
            qg->addWidget(tmp,l+numberofitmes+(m)+9,0);
            tmpS = QString::number(nac.link_uscenti[l]->massa[m]);
            tmp = new QLabel(tmpS);
            qg->addWidget(tmp,l+numberofitmes+(m)+9,1);
        }


        tmpLabel = new QLabel(" ");
        qg->addWidget(tmpLabel,l+numberofitmes+(nac.link_uscenti[l]->massa.size())+10,0);



    }
    widget->setLayout( qg );


    QScrollArea *scrollAreaEntranti = info->findChild<QScrollArea*>("scrollArea_link_entranti");
    //scrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
    scrollAreaEntranti->setWidgetResizable( true );
    scrollAreaEntranti->setGeometry( 10, 10, 100, 100 );

    QWidget *widget_entranti = new QWidget();
    scrollAreaEntranti->setWidget( widget_entranti );

    delete widget_entranti->layout();

    numberofitmes=0;
    QGridLayout* qg_entranti = new QGridLayout(widget_entranti);//info->findChild<QGridLayout*>("gridLayout_link_uscenti");
    for (auto l = 0; l < nac.link_entranti.size(); l++)
    {
        numberofitmes += nac.link_entranti[l]->massa.size()+9+2;
        QString tmpS = QString::fromStdString(nac.link_entranti[l]->id);
        QLabel * tmpLabel = new QLabel("link id: ");
        qg_entranti->addWidget(tmpLabel,l+numberofitmes,0);
        QLabel * tmp = new QLabel(tmpS);
        qg_entranti->addWidget(tmp,l+numberofitmes,1);

        tmpS = QString::number(nac.link_entranti[l]->length);
        tmpLabel = new QLabel("length");
        qg_entranti->addWidget(tmpLabel,l+numberofitmes+1,0);
        tmp = new QLabel(tmpS);
        qg_entranti->addWidget(tmp,l+numberofitmes+1,1);

        tmpS = QString::number(nac.link_entranti[l]->diameter);
        tmpLabel = new QLabel("diameter");
        qg_entranti->addWidget(tmpLabel,l+numberofitmes+2,0);
        tmp = new QLabel(tmpS);
        qg_entranti->addWidget(tmp,l+numberofitmes+2,1);

        tmpS = QString::number(nac.link_entranti[l]->v);
        tmpLabel = new QLabel("v");
        qg_entranti->addWidget(tmpLabel,l+numberofitmes+3,0);
        tmp = new QLabel(tmpS);
        qg_entranti->addWidget(tmp,l+numberofitmes+3,1);

        tmpS = QString::number(nac.link_entranti[l]->volume);
        tmpLabel = new QLabel("volume");
        qg_entranti->addWidget(tmpLabel,l+numberofitmes+4,0);
        tmp = new QLabel(tmpS);
        qg_entranti->addWidget(tmp,l+numberofitmes+4,1);

        tmpS = QString::number(nac.link_entranti[l]->flow);
        tmpLabel = new QLabel("flow");
        qg_entranti->addWidget(tmpLabel,l+numberofitmes+5,0);
        tmp = new QLabel(tmpS);
        qg_entranti->addWidget(tmp,l+numberofitmes+5,1);

        tmpS = QString::number(nac.link_entranti[l]->ni);
        tmpLabel = new QLabel("ni");
        qg_entranti->addWidget(tmpLabel,l+numberofitmes+6,0);
        tmp = new QLabel(tmpS);
        qg_entranti->addWidget(tmp,l+numberofitmes+6,1);

        tmpS = QString::fromStdString(nac.link_entranti[l]->nodes[0]->id);
        tmpLabel = new QLabel("collegato al nodo: ");
        qg_entranti->addWidget(tmpLabel,l+numberofitmes+7,0);
        tmp = new QLabel(tmpS);
        qg_entranti->addWidget(tmp,l+numberofitmes+7,1);

        tmpLabel = new QLabel("finite volume : ");
        qg_entranti->addWidget(tmpLabel,l+numberofitmes+8,0);

        for (auto m = 0; m < nac.link_entranti[l]->massa.size(); m++)
        {
            tmp = new QLabel(QString::number(m+1));
            qg_entranti->addWidget(tmp,l+numberofitmes+(m)+9,0);
            tmpS = QString::number(nac.link_entranti[l]->massa[m]);
            tmp = new QLabel(tmpS);
            qg_entranti->addWidget(tmp,l+numberofitmes+(m)+9,1);
        }


        tmpLabel = new QLabel(" ");
        qg_entranti->addWidget(tmpLabel,l+numberofitmes+(nac.link_entranti[l]->massa.size())+10,0);

    }

    widget_entranti->setLayout( qg_entranti );
}

void Chip::changeColor(const QColor &_color)
{
    this->color = _color;
    //cout << info.isVisible() << endl;
//    if(info.isVisible()==true)
//    {
//        NodeAC nac = CAWDN::getInfoNode(this->id);
//        createDynamicContentInfo(&info, nac);
////        info.show();
////        info.exec();
//    }
}

//void Chip::setCAWDN(CAWDN *ca)
//{
//    this->ca = ca;
//}


void Chip::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    update();
}



#include "view.h"
void Chip::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    NodeAC nac = CAWDN::getInfoNode(this->id);
    createDynamicContentInfo(&info, nac);
    //View::getElementstoManagerEvents(info);
    info.show();
    info.exec();
}

void Chip::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier) {
        stuff << event->pos();
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void Chip::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}

ManagerEvents *Chip::getMe() const
{
    return me;
}


