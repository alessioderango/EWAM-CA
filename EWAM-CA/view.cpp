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

#include "view.h"

#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#endif
#endif
#ifndef QT_NO_OPENGL
#include <QtOpenGL>
#else
#include <QtWidgets>
#endif
#include <qmath.h>

#if QT_CONFIG(wheelevent)
void GraphicsView::wheelEvent(QWheelEvent *e)
{
    if (e->modifiers() & Qt::ControlModifier) {
        if (e->delta() > 0)
            view->zoomIn(6);
        else
            view->zoomOut(6);
        e->accept();
    } else {
        QGraphicsView::wheelEvent(e);
    }
}
#endif


ostream &operator<<(ostream &os, const Junction &j)
{
    os << j.getId() << ' ' << j.getElevation() << ' ' << j.getDemand();
    return os;
}

ostream &operator<<(ostream &os, const LinkResult &j)
{
    os << j.id << ' ' << j.flow << ' ' << j.velocityunit << ' ' << j.headloss << ' ' << j.status;
    return os;
}

ostream &operator<<(ostream &os, const NodeResult &j)
{
    os << j.id << ' ' << j.demand << ' ' << j.head << ' ' << j.pressure << ' ' << j.quality;
    return os;
}

ostream &operator<<(ostream &os, const NodeTable &j)
{
    os << j.id << ' ' << j.startNode << ' ' << j.endNode << ' ' << j.length << ' ' << j.diameter;
    return os;
}

ostream &operator<<(ostream &os, const Pipe &j)
{
    os << j.id << ' ' << j.node1 << ' ' << j.node2 << ' ' << j.length << ' ' << j.diameter << ' ' << j.roughness << ' ' << j.minorLoss << ' ' << j.status;
    return os;
}

ostream &operator<<(ostream &os, const Coordinate &j)
{
    os.precision(3);
    os << j.getId().c_str() << std::fixed <<' ' << j.getXCoord() << ' ' << j.getYCoord();
    return os;
}

int parseFileJunctions(std::map<string, Junction> &junctions, std::string file_name)
{
    ifstream inFile(file_name.c_str());

    if (!inFile)
    {
        cerr << "unable to open input file: "
             << file_name << " \n";
        return -1;
    }
    string strOneLine;
    while (inFile)
    {
        getline(inFile, strOneLine);
        if (strOneLine.find("[JUNCTIONS]") != std::string::npos)
        {
            getline(inFile, strOneLine);
            getline(inFile, strOneLine);
            while (strOneLine.length() > 1)
            {
                strOneLine.erase(std::remove(strOneLine.begin(), strOneLine.end(), ';'), strOneLine.end());
                string word;
                stringstream iss(strOneLine);
                std::vector<string> junctionsInfo;
                while (iss >> word)
                    junctionsInfo.push_back((word));

                Junction j(junctionsInfo[0], std::stod(junctionsInfo[1]), std::stod(junctionsInfo[2]));
                string key = (junctionsInfo[0]);
                junctions.insert(std::pair<string, Junction>(key, j));
                getline(inFile, strOneLine);
            }
        }
    }
    inFile.close();
    return 0;
}

int parseFileReservoirs(std::map<string, Reservoir> &reservoirs, std::string file_name)
{
    ifstream inFile(file_name.c_str());

    if (!inFile)
    {
        cerr << "unable to open input file: "
             << file_name << " \n";
        return -1;
    }
    string strOneLine;
    while (inFile)
    {
        getline(inFile, strOneLine);
        if (strOneLine.find("[RESERVOIRS]") != std::string::npos)
        {
            getline(inFile, strOneLine);
            getline(inFile, strOneLine);
            while (strOneLine.length() > 1)
            {
                strOneLine.erase(std::remove(strOneLine.begin(), strOneLine.end(), ';'), strOneLine.end());
                string word;
                stringstream iss(strOneLine);
                std::vector<string> reservoirsInfo;
                while (iss >> word)
                    reservoirsInfo.push_back((word));

                Reservoir j(reservoirsInfo[0], (reservoirsInfo[1]));
                string key = (reservoirsInfo[0]);
                reservoirs.insert(std::pair<string, Reservoir>(key, j));
                getline(inFile, strOneLine);
            }
        }
    }
    inFile.close();
    return 0;
}

int parseFileCoordinates(std::map<string, Coordinate> &coordinates, std::string file_name)
{
    ifstream inFile(file_name.c_str());

    if (!inFile)
    {
        cerr << "unable to open input file: "
             << file_name << " \n";
        return -1;
    }
    string strOneLine;
    while (inFile)
    {
        getline(inFile, strOneLine);
        if (strOneLine.find("[COORDINATES]") != std::string::npos)
        {
            getline(inFile, strOneLine);
            getline(inFile, strOneLine);
            while (strOneLine.length() > 1)
            {
                // strOneLine.erase(std::remove(strOneLine.begin(), strOneLine.end(), ';'), strOneLine.end());
                //strOneLine.erase(std::remove(strOneLine.begin(), strOneLine.end(), ' '), strOneLine.end());
                string word;
                stringstream iss(strOneLine);
                std::vector<string> coordinatesInfo;

                while (iss >> word)
                {
                    // cout << "" << endl;
                    // cout << word << endl;
                    //word.erase(std::remove(word.begin(), word.end(), ' '), word.end());
                    // cout << std::stod(word) << endl;
                    // cout << "" << endl;
                    coordinatesInfo.push_back(word);
                }
                // cout << "" << endl;
                // cout << coordinatesInfo[1] << "   " <<  coordinatesInfo[2] << endl;
                Coordinate j(coordinatesInfo[0], std::stod(coordinatesInfo[1]), std::stod(coordinatesInfo[2]));
                string key = coordinatesInfo[0];
                coordinates.insert(std::pair<string, Coordinate>(key, j));
                getline(inFile, strOneLine);
            }
        }
    }
    inFile.close();
    return 0;
}

int parseFilePipe(std::map<string, Pipe> &pipes, std::string file_name)
{
    ifstream inFile(file_name.c_str());

    if (!inFile)
    {
        cerr << "unable to open input file: "
             << file_name << " \n";
        return -1;
    }
    string strOneLine;
    while (inFile)
    {
        getline(inFile, strOneLine);
        if (strOneLine.find("[PIPES]") != std::string::npos)
        {
            getline(inFile, strOneLine);
            getline(inFile, strOneLine);
            while (strOneLine.length() > 1)
            {
                strOneLine.erase(std::remove(strOneLine.begin(), strOneLine.end(), ';'), strOneLine.end());
                string word;
                stringstream iss(strOneLine);
                std::vector<string> info;
                while (iss >> word)
                    info.push_back((word));

                Pipe p((info[0]), (info[1]), (info[2]), std::stod(info[3]), std::stod(info[4]), std::stoi(info[5]), std::stoi(info[6]), info[7]);
                string key = info[0];
                pipes.insert(std::pair<string, Pipe>(key, p));



                getline(inFile, strOneLine);
            }
        }
    }
    inFile.close();
    return 0;
}

int count_blanks(const std::string &s)
{
    return std::count_if(s.begin(), s.end(),
                         [](unsigned char c) { return std::isblank(c); } // correct
    );
}

int parseFileOuput(std::map<string, NodeTable> &nodetable,
                   std::map<string, NodeResult> &noderesults,
                   std::map<string, LinkResult> &linkresults,
                   std::vector<string> &timesteps,
                   std::string file_name)
{
    ifstream inFile(file_name.c_str());

    if (!inFile)
    {
        cerr << "unable to open input file: "
             << file_name << " \n";
        return -1;
    }
    string strOneLine;
    while (inFile)
    {
        getline(inFile, strOneLine);
        if (strOneLine.find("  Link - Node Table:") != std::string::npos)
        {
            getline(inFile, strOneLine);
            getline(inFile, strOneLine);
            getline(inFile, strOneLine);
            getline(inFile, strOneLine);
            getline(inFile, strOneLine);




            while (count_blanks(strOneLine) != (strOneLine.length() - 1))
            {
                string word;
                stringstream iss(strOneLine);
                std::vector<string> info;
                while (iss >> word)
                    info.push_back((word));

                if(info.size()>=6)
                {
                      info.pop_back();
                }

                NodeTable j((info[0]),
                            (info[1]),
                            (info[2]),
                            std::stod(info[3]),
                            std::stod(info[4]));
                string key = info[0];
                nodetable.insert(std::pair<string, NodeTable>(key, j));

                getline(inFile, strOneLine);
            }
        }
        else
        if (strOneLine.find("  Node Results at ") != std::string::npos)
        {
            string wordTime;
            stringstream iss2(strOneLine);
            std::vector<string> info2;
            while (iss2 >> wordTime)
                info2.push_back((wordTime));

            if(info2.size()>=6)
            {
                  info2.pop_back();
            }

            bool exists = std::find(std::begin(timesteps), std::end(timesteps), info2[info2.size()-2]) != std::end(timesteps);
            if(!exists)
            timesteps.push_back(info2[info2.size()-2]);

           //cout << info2[info2.size()-2] << endl;

            getline(inFile, strOneLine);
            getline(inFile, strOneLine);
            getline(inFile, strOneLine);
            getline(inFile, strOneLine);
            getline(inFile, strOneLine);
            while (count_blanks(strOneLine) != (strOneLine.length() - 1))
            {
                string word;
                stringstream iss(strOneLine);
                std::vector<string> info;
                while (iss >> word)
                    info.push_back((word));

                NodeResult j((info[0]),
                             stod(info[1]),
                             stod(info[2]),
                             stod(info[3]),
                             stod(info[4]),
                             info2[info2.size()-2]);
                string key = (info[0]+info2[info2.size()-2]);
                noderesults.insert(std::pair<string, NodeResult>(key, j));
                getline(inFile, strOneLine);
            }
        }
        else
            if (strOneLine.find("  Link Results at ") != std::string::npos)
        {
            string wordTime;
            stringstream iss2(strOneLine);
            std::vector<string> info2;
            while (iss2 >> wordTime)
                info2.push_back((wordTime));

            if(info2.size()>=6)
            {
                  info2.pop_back();
            }


            getline(inFile, strOneLine);
            getline(inFile, strOneLine);
            getline(inFile, strOneLine);
            getline(inFile, strOneLine);
            getline(inFile, strOneLine);
            while (count_blanks(strOneLine) != (strOneLine.length() - 1))
            {
                string word;
                stringstream iss(strOneLine);
                std::vector<string> info;
                while (iss >> word)
                    info.push_back((word));
                LinkResult j((info[0]),
                             stod(info[1]),
                             stod(info[2]),
                             stod(info[3]),
                             (info[4]),
                             info2[info2.size()-2],
                             info[0]);
                string key = (info[0]+info2[info2.size()-2]);
                linkresults.insert(std::pair<string, LinkResult>(key, j));
                getline(inFile, strOneLine);
            }
        }
    }
    inFile.close();
    return 0;
}



View::View(const QString &name, QWidget *parent)
    : QFrame(parent)
{
    setFrameStyle(Sunken | StyledPanel);
    graphicsView = new GraphicsView(this);
    graphicsView->setRenderHint(QPainter::Antialiasing, false);
    graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    int size = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
    QSize iconSize(size, size);

    QToolButton *zoomInIcon = new QToolButton;
    zoomInIcon->setAutoRepeat(true);
    zoomInIcon->setAutoRepeatInterval(33);
    zoomInIcon->setAutoRepeatDelay(0);
    zoomInIcon->setIcon(QPixmap(":/zoomin.png"));
    zoomInIcon->setIconSize(iconSize);
    QToolButton *zoomOutIcon = new QToolButton;
    zoomOutIcon->setAutoRepeat(true);
    zoomOutIcon->setAutoRepeatInterval(33);
    zoomOutIcon->setAutoRepeatDelay(0);
    zoomOutIcon->setIcon(QPixmap(":/zoomout.png"));
    zoomOutIcon->setIconSize(iconSize);
    zoomSlider = new QSlider;
    zoomSlider->setMinimum(0);
    zoomSlider->setMaximum(500);
    zoomSlider->setValue(100);
    zoomSlider->setTickPosition(QSlider::TicksRight);

    // Zoom slider layout
    QVBoxLayout *zoomSliderLayout = new QVBoxLayout;
    zoomSliderLayout->addWidget(zoomInIcon);
    zoomSliderLayout->addWidget(zoomSlider);
    zoomSliderLayout->addWidget(zoomOutIcon);

    QToolButton *rotateLeftIcon = new QToolButton;
    rotateLeftIcon->setIcon(QPixmap(":/rotateleft.png"));
    rotateLeftIcon->setIconSize(iconSize);
    QToolButton *rotateRightIcon = new QToolButton;
    rotateRightIcon->setIcon(QPixmap(":/rotateright.png"));
    rotateRightIcon->setIconSize(iconSize);
    rotateSlider = new QSlider;
    rotateSlider->setOrientation(Qt::Horizontal);
    rotateSlider->setMinimum(-360);
    rotateSlider->setMaximum(360);
    rotateSlider->setValue(0);
    rotateSlider->setTickPosition(QSlider::TicksBelow);

    // Rotate slider layout
    QHBoxLayout *rotateSliderLayout = new QHBoxLayout;
    rotateSliderLayout->addWidget(rotateLeftIcon);
    rotateSliderLayout->addWidget(rotateSlider);
    rotateSliderLayout->addWidget(rotateRightIcon);

    resetButton = new QToolButton;
    resetButton->setText(tr("0"));
    resetButton->setEnabled(false);

    // Label layout
    QHBoxLayout *labelLayout = new QHBoxLayout;
    label = new QLabel(name);
    label2 = new QLabel(tr("Pointer Mode"));
    labelStep = new QLabel("Time simulation");
    labelSteptext = new QLabel("");
//    selectModeButton = new QToolButton;
//    selectModeButton->setText(tr("Select"));
//    selectModeButton->setCheckable(true);
//    selectModeButton->setChecked(false);
    dragModeButton = new QToolButton;
    dragModeButton->setText(tr("Drag"));
    dragModeButton->setCheckable(true);
    dragModeButton->setChecked(true);
    antialiasButton = new QToolButton;
    antialiasButton->setText(tr("Antialiasing"));
    antialiasButton->setCheckable(true);
    antialiasButton->setChecked(false);
    openGlButton = new QToolButton;
    openGlButton->setText(tr("OpenGL"));
    openGlButton->setCheckable(true);
#ifndef QT_NO_OPENGL
    openGlButton->setEnabled(QGLFormat::hasOpenGL());
#else
    openGlButton->setEnabled(false);
#endif
    printButton = new QToolButton;
    printButton->setIcon(QIcon(QPixmap(":/fileprint.png")));

    QButtonGroup *pointerModeGroup = new QButtonGroup(this);
    pointerModeGroup->setExclusive(true);
//    pointerModeGroup->addButton(selectModeButton);
    pointerModeGroup->addButton(dragModeButton);

    loadModeButton = new QPushButton;
    loadModeButton->setText(tr("Load File"));
    loadModeButton->setCheckable(false);
    loadModeButton->setChecked(false);

    labelLayout->addWidget(loadModeButton);
    labelLayout->addWidget(label);
    labelLayout->addStretch();
   // labelLayout->addWidget(label2);
//    labelLayout->addWidget(selectModeButton);
    //labelLayout->addWidget(dragModeButton);
    labelLayout->addWidget(labelStep);
    labelLayout->addWidget(labelSteptext);
    labelLayout->addStretch();
    labelLayout->addWidget(antialiasButton);
    labelLayout->addWidget(openGlButton);
    labelLayout->addWidget(printButton);


    QGridLayout *topLayout = new QGridLayout;
    topLayout->addLayout(labelLayout, 0, 0);
    topLayout->addWidget(graphicsView, 1, 0);
    topLayout->addLayout(zoomSliderLayout, 1, 1);
    topLayout->addLayout(rotateSliderLayout, 2, 0);
    topLayout->addWidget(resetButton, 2, 1);
    setLayout(topLayout);

    connect(resetButton, SIGNAL(clicked()), this, SLOT(resetView()));
    connect(zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(setupMatrix()));
    connect(rotateSlider, SIGNAL(valueChanged(int)), this, SLOT(setupMatrix()));
    connect(graphicsView->verticalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(setResetButtonEnabled()));
    connect(graphicsView->horizontalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(setResetButtonEnabled()));
//    connect(selectModeButton, SIGNAL(toggled(bool)), this, SLOT(togglePointerMode()));
    connect(dragModeButton, SIGNAL(toggled(bool)), this, SLOT(togglePointerMode()));
    connect(antialiasButton, SIGNAL(toggled(bool)), this, SLOT(toggleAntialiasing()));
    connect(openGlButton, SIGNAL(toggled(bool)), this, SLOT(toggleOpenGL()));
    connect(rotateLeftIcon, SIGNAL(clicked()), this, SLOT(rotateLeft()));
    connect(rotateRightIcon, SIGNAL(clicked()), this, SLOT(rotateRight()));
    connect(zoomInIcon, SIGNAL(clicked()), this, SLOT(zoomIn()));
    connect(zoomOutIcon, SIGNAL(clicked()), this, SLOT(zoomOut()));
    connect(printButton, SIGNAL(clicked()), this, SLOT(print()));

    connect(loadModeButton, SIGNAL(clicked()), this, SLOT(loadFile()));

    setupMatrix();
}

QGraphicsView *View::view() const
{
    return static_cast<QGraphicsView *>(graphicsView);
}


void View::resetView()
{
    zoomSlider->setValue(250);
    rotateSlider->setValue(0);
    setupMatrix();
    graphicsView->ensureVisible(QRectF(0, 0, 0, 0));

    resetButton->setEnabled(false);
}

void View::setResetButtonEnabled()
{
    resetButton->setEnabled(true);
}

void View::setupMatrix()
{
    qreal scale = qPow(qreal(2), (zoomSlider->value() - 250) / qreal(50));

    QMatrix matrix;
    matrix.scale(scale, scale);
    matrix.rotate(rotateSlider->value());

    graphicsView->setMatrix(matrix);
    setResetButtonEnabled();
}
#include <iostream>
void View::togglePointerMode()
{
//    std::cout << selectModeButton->isChecked() << std::endl;
//    graphicsView->setDragMode(selectModeButton->isChecked()
//                              ? QGraphicsView::RubberBandDrag
//                              : QGraphicsView::ScrollHandDrag);
    graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
//    graphicsView->setInteractive(selectModeButton->isChecked());
    graphicsView->setInteractive(false);
}

void View::toggleOpenGL()
{
#ifndef QT_NO_OPENGL
    graphicsView->setViewport(openGlButton->isChecked() ? new QGLWidget(QGLFormat(QGL::SampleBuffers)) : new QWidget);
#endif
}

void View::toggleAntialiasing()
{
    graphicsView->setRenderHint(QPainter::Antialiasing, antialiasButton->isChecked());
}



void View::populateScene(QString filepath)
{
    if(view()->scene())
    {
        view()->items().clear();
        view()->scene()->clear();
        delete view()->scene();
        junctions.clear();
        coordinates.clear();
        pipes.clear();
        nodeTable.clear();
        nodeResults.clear();
        linkResults.clear();
        graphicalItems.clear();
        reservoirs.clear();
        timesteps.clear();
        delete cawdn;
    }

    QDir d = QFileInfo(filepath).absoluteDir();
    QString absolute=d.absolutePath();


    int pos = filepath.lastIndexOf(QChar('/'));
    QString fileNameWithExtension = filepath.mid(pos+1);
    QString fileName = fileNameWithExtension.split(".")[0];

    QGraphicsScene * scene = new QGraphicsScene(this);
    std::string fileNameInput = filepath.toStdString();
    QString fileNameOutput = absolute+"/"+fileName+".rpt";

    int error = parseFileJunctions(junctions, fileNameInput);
    int errorReservoir = parseFileReservoirs(reservoirs,fileNameInput);
    int errorCoordinate = parseFileCoordinates(coordinates,fileNameInput);
    int errorPipe = parseFilePipe(pipes, fileNameInput);
    parseFileOuput(nodeTable, nodeResults, linkResults, timesteps, fileNameOutput.toStdString());

    int offsetx = -40;
    int offsety = -30;

    vector<QGraphicsLineItem *> lines;




    for (auto it = junctions.cbegin(); it != junctions.cend(); it++ )
    {
        Chip *item = new Chip(it->first, QColor(255,0,0), 1, 1, CERCHIO);
        item->me = new ManagerEvents();
        graphicalItems.insert(std::pair<string, Chip*>(it->first,item));
        item->setPos(QPointF(coordinates.at((it->first)).getXCoord(), -coordinates.at((it->first)).getYCoord()));
        scene->addItem(item);
    }

    for (auto it = reservoirs.cbegin(); it != reservoirs.cend(); it++ )
    {
        Chip *item = new Chip(it->first, QColor(0,0,255), 1, 1, OTTAGONO);
        item->me = new ManagerEvents();
        graphicalItems.insert(std::pair<string, Chip*>(it->first,item));
        item->setPos(QPointF(coordinates.at((it->first)).getXCoord(), -coordinates.at((it->first)).getYCoord()));
        scene->addItem(item);
    }

    view()->setScene(scene);
    cawdn = new CAWDN(junctions,
                      reservoirs,
                      coordinates,
                      pipes,
                      nodeTable,
                      nodeResults,
                      linkResults,
                      graphicalItems,
                      lines,
                      timesteps,
                      absolute);
    cawdn->init();
    for (auto link = 0; link < cawdn->getCellularLink().size(); link++)
    {
        QLine l(coordinates.at((cawdn->getCellularLink()[link].nodes[0]->id)).getXCoord()-offsetx,
                         -coordinates.at((cawdn->getCellularLink()[link].nodes[0]->id)).getYCoord()-offsety,
                         coordinates.at((cawdn->getCellularLink()[link].nodes[1]->id)).getXCoord()-offsetx,
                         -coordinates.at((cawdn->getCellularLink()[link].nodes[1]->id)).getYCoord()-offsety);
        lines.push_back(scene->addLine(l));

        QPolygonF arrowHead;
        double angle = std::atan2(l.dy(), -l.dx());
        QPointF arrowP1 = l.p2() + QPointF(sin(angle + M_PI / 3) * 10,
                                        cos(angle + M_PI / 3) * 10);
        QPointF arrowP2 = l.p2() + QPointF(sin(angle + M_PI - M_PI / 3) * 10,
                                        cos(angle + M_PI - M_PI / 3) * 10);
        arrowHead.clear();
        arrowHead << l.p2() << arrowP1 << arrowP2;
        scene->addPolygon(arrowHead);
        cout << link << "/" << cawdn->getCellularLink().size()<< endl;

    }

    managersimulation = new ManagerSimulation(cawdn,this);

    for (auto it = graphicalItems.cbegin(); it != graphicalItems.cend(); it++ )
    {
        connect(this->managersimulation, SIGNAL(updateView(Chip*)), it->second->getMe(), SLOT(updateView(Chip*)));
    }

    managersimulation->start();
    //cawdn->run();
    cout << "finish Iniatilization" << endl;
}

void View::loadFile()
{

    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Text file"), "", tr("Text Files (*.txt *.inp) "));
    if(!fileName.isEmpty())
        populateScene(fileName);


}


void View::print()
{
#if QT_CONFIG(printdialog)
    QPrinter printer;
    QPrintDialog dialog(&printer, this);
    if (dialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        graphicsView->render(&painter);
    }
#endif
}

void View::zoomIn(int level)
{
    zoomSlider->setValue(zoomSlider->value() + level);
}

void View::zoomOut(int level)
{
    zoomSlider->setValue(zoomSlider->value() - level);
}

void View::rotateLeft()
{
    rotateSlider->setValue(rotateSlider->value() - 10);
}

void View::rotateRight()
{
    rotateSlider->setValue(rotateSlider->value() + 10);
}

bool View::getUntilend() const
{
    return untilend;
}

void View::setUntilend(bool value)
{
    untilend = value;
}

bool View::getOnestep() const
{
    return onestep;
}

void View::setOnestep(bool value)
{
    onestep = value;
}

void View::settextSimulation(QString value)
{
    labelSteptext->setText(value);
}
