/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
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

#include "renderarea.h"
#include "window.h"


#include <QtWidgets>
#include <iostream>
const int IdRole = Qt::UserRole;
Window::Window()
{
    renderArea = new RenderArea;
    QGroupBox* buttonbox=new QGroupBox("Curve Selector");
    QRadioButton* b1=new QRadioButton("Bezier Curve");
    QRadioButton* b2=new QRadioButton("Sub Divide Bezier");
    QRadioButton* b3=new QRadioButton("Cubic B-Spline");
    QRadioButton* b4=new QRadioButton("Subdivide B-Spline");
    QRadioButton* b5=new QRadioButton("Rational Bezier");
    QRadioButton* b6=new QRadioButton("NURBS");
    QRadioButton* b7=new QRadioButton("Cubic Bezier Joining");
    this->b1=b1;
    b1->setChecked(true);
    this->b2=b2;
    this->b3=b3;
    this->b4=b4;
    this->b5=b5;
    this->b6=b6;
    this->b7=b7;
    QVBoxLayout *vbox=new QVBoxLayout;
    vbox->addWidget(b1);
    vbox->addWidget(b2);
    vbox->addWidget(b3);
    vbox->addWidget(b4);
    vbox->addWidget(b5);
    vbox->addWidget(b6);
    vbox->addWidget(b7);
    vbox->addStretch(7);
    buttonbox->setLayout(vbox);

    connect(b1, SIGNAL(clicked(bool)),this,SLOT(setCurve()));
    connect(b2,SIGNAL(clicked(bool)),this,SLOT(setCurve()));
    connect(b3,SIGNAL(clicked(bool)),this,SLOT(setCurve()));
    connect(b4,SIGNAL(clicked(bool)),this,SLOT(setCurve()));
    connect(b5,SIGNAL(clicked(bool)),this,SLOT(setCurve()));
    connect(b6,SIGNAL(clicked(bool)),this,SLOT(setCurve()));
    connect(b7,SIGNAL(clicked(bool)),this,SLOT(setCurve()));

    QSlider* s1=new QSlider(Qt::Horizontal);
    s1->setRange(1,5);
    s1->setValue(1);
    s1->setTickPosition(QSlider::TicksBelow);
    s1->setTickInterval(1);
    connect(s1,SIGNAL(sliderMoved(int)),this,SLOT(setLevel(int)));

    QSlider* s2=new QSlider(Qt::Horizontal);
    s2->setRange(10,1000);
    s2->setValue(100);
    s2->setTickPosition(QSlider::TicksBelow);
    s2->setTickInterval(50);
    connect(s2,SIGNAL(sliderMoved(int)),this,SLOT(setSamplingInt(int)));

    QPushButton* clr=new QPushButton("Clear");
    connect(clr,SIGNAL(clicked(bool)),this,SLOT(cls()));

    QPushButton* join=new QPushButton("Join");
    connect(join,SIGNAL(clicked(bool)),this,SLOT(joinCurve()));

    QCheckBox* editmode=new QCheckBox("Insert Duplicate Vertices Mode");
    connect(editmode,SIGNAL(toggled(bool)),this,SLOT(mode()));

    QComboBox* deg=new QComboBox;
    deg->addItem(tr("1"));
    deg->addItem(tr("2"));
    deg->addItem(tr("3"));
    deg->addItem(tr("4"));
    deg->addItem(tr("5"));
    deg->setCurrentIndex(2);

    connect(deg,SIGNAL(activated(int)),this,SLOT(setDeg(int)));

    QGridLayout *mainLayout = new QGridLayout;

    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(3, 1);
    mainLayout->addWidget(renderArea, 0, 0, 1, 4);

    mainLayout->addWidget(buttonbox);
    mainLayout->addWidget(s1);
    mainLayout->addWidget(s2);
    mainLayout->addWidget(clr);
    mainLayout->addWidget(join);
    mainLayout->addWidget(editmode);
    mainLayout->addWidget(deg);
    setLayout(mainLayout);


    setWindowTitle(tr("Basic Curves"));
}
void Window::setCurve(){
    if(b1->isChecked()){
        renderArea->setCurve(1);
    }
    else if(b2->isChecked()){
        renderArea->setCurve(2);
    }
    else if(b3->isChecked()){
        renderArea->setCurve(3);
    }
    else if(b4->isChecked()){
        renderArea->setCurve(4);
    }
    else if(b5->isChecked()){
        renderArea->setCurve(5);

    }
    else if(b6->isChecked()){
        renderArea->setCurve(6);
    }
    else if(b7->isChecked()){
        renderArea->setCurve(7);
    }
}
void Window::setLevel(int level){
   renderArea->setLevel(level);
}
void Window::cls(){
    renderArea->cls();
}
void Window::mode(){
    renderArea->setmode();
}
void Window::joinCurve(){
    renderArea->joinCurve();
}
void Window::setDeg(int deg){
renderArea->setdeg(deg+1);

}
void Window::setSamplingInt(int level){
renderArea->samplingInt(level);
}
