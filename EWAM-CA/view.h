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

#ifndef VIEW_H
#define VIEW_H

#include <QFrame>
#include <QGraphicsView>
#include <QPushButton>
#include <iostream>
#include <map>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <ctype.h>
#include "ewamca.h"
#include "reservoirs.h"

#include "cawdn.h"
using namespace std;

QT_BEGIN_NAMESPACE
class QLabel;
class QSlider;
class QToolButton;
QT_END_NAMESPACE

class View;

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(View *v) : QGraphicsView(), view(v) { }

protected:
#if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent *) override;
#endif

private:
    View *view;
};

class ManagerSimulation;

class View : public QFrame
{
    Q_OBJECT
public:
    explicit View(const QString &name, QWidget *parent = 0);

    void populateScene(QString filepath);
    QGraphicsView *view() const;

    bool getOnestep() const;
    void setOnestep(bool value);

    void settextSimulation(QString value);

    bool getUntilend() const;
    void setUntilend(bool value);
    std::map<string, Chip*> graphicalItems;

public slots:
    void zoomIn(int level = 1);
    void zoomOut(int level = 1);

protected:
    void keyPressEvent(QKeyEvent *event) override
    {
        if(event->key() == Qt::Key_S)
        {
            onestep = true;
        }

        if(event->key() == Qt::Key_R)
        {
            untilend = true;
        }
    }

private slots:
    void resetView();
    void setResetButtonEnabled();
    void setupMatrix();
    void togglePointerMode();
    void toggleOpenGL();
    void toggleAntialiasing();
    void print();
    void loadFile();
    void rotateLeft();
    void rotateRight();

private:
    GraphicsView *graphicsView;
    QLabel *label;
    QLabel *label2;
    QLabel *labelStep;
    QLabel *labelSteptext;
//    QToolButton *selectModeButton;
    QPushButton *loadModeButton;
    QToolButton *dragModeButton;
    QToolButton *openGlButton;
    QToolButton *antialiasButton;
    QToolButton *printButton;
    QToolButton *resetButton;
    QSlider *zoomSlider;
    QSlider *rotateSlider;

    CAWDN* cawdn;
    bool onestep;
    bool untilend;


    std::map<string, Junction> junctions;
    std::map<string, Reservoir> reservoirs;
    std::map<string, Coordinate> coordinates;
    std::map<string, Pipe> pipes;
    std::map<string, NodeTable> nodeTable;
    std::map<string, NodeResult> nodeResults;
    std::map<string, LinkResult> linkResults;

    std::vector<string> timesteps;
    ManagerSimulation* managersimulation;



};
#include <QThread>
class ManagerSimulation : public QThread
{
    Q_OBJECT
public:
    ManagerSimulation(CAWDN* _cawdn,
    View* _view):cawdn(_cawdn),view(_view){}


    void run()
    {
        view->setOnestep(false);
        view->setUntilend(false);
        while(cawdn->getDelta_t_sim() <= cawdn->getLimitTime())
        {
            view->settextSimulation(QString::number(cawdn->getDelta_t_sim()));//->setText(QString::number(cawdn->getDelta_t_sim()));
            if(view->getOnestep() || view->getUntilend())
            {
                view->setOnestep(false);
                cawdn->onestepFunction();
//                for (auto it = view->graphicalItems.begin(); it != view->graphicalItems.end(); it++ )
//                {
//                     emit updateView(it->second);
//                }

            }
        }
    }

signals:
    void updateView(Chip*);


private:
    CAWDN* cawdn;
    View* view;
};

#endif // VIEW_H
