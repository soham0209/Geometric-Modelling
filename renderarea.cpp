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


#define PI 3.14159
#include <QPainter>
#include <QVector2D>
#include <vector>
#include <iostream>
#include <iterator>
#include <QSlider>
#include <QMessageBox>
#include <cmath>
#include "utils.h"
class Utils;
using namespace std;
bool flag;
RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent)
{
    u=new Utils();
    this->curve=1;
    this->axis='x';
    this->isediting=false;
    this->insertmode=false;
    this->isjoined=false;
    this->D=4;
    this->numslice=8;
    this->max=100;
    type='a';
    this->z=0.5f;
    flag=false;

    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);




}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(100, 100);
}

void RenderArea::setCurve(int curve){
    this->curve=curve;

    repaint();
}
void RenderArea::setAxis(char axis){
    this->axis=axis;
    repaint();
}
void RenderArea::cls(){
    mypoints.clear();
    if(isjoined)
        trajpoints.clear();
    repaint();
}

QSize RenderArea::sizeHint() const
{
    return QSize(400, 200);
}
void RenderArea::setdeg(int deg){
    this->D=deg+1;
    repaint();
}
void RenderArea::samplingInt(int max){
    this->max=max;
    repaint();
}

void RenderArea::paintEvent(QPaintEvent *)
{

    QPainter painter(this);
    QSize axes;
    QPen pen(Qt::blue, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
        painter.save();
        painter.setPen(QColor(10,10,250));
        painter.setBrush(QColor (250,10,10));
        axes=this->size();
        QPointF e(0,axes.height()/2);
        QPointF w(axes.width(),axes.height()/2);
        QPointF n(axes.width()/2,axes.height());
        QPointF s(axes.width()/2,0);
        painter.drawLine(e,w);
        painter.drawLine(n,s);

        if(!isjoined){
        for(std::vector<QPointF>::iterator it=mypoints.begin();it!=mypoints.end();it++){
            painter.drawEllipse((*it),5,5);


        }
        if(mypoints.size()>1){
            painter.save();
            painter.setPen(QColor(200,200,10));
            if(type=='a'||type=='c'){
            for(int i=0;i<mypoints.size()-1;i++){
            painter.drawLine(mypoints[i],mypoints[i+1]);
           }
            }
            else if(type=='b'||type=='d'){
                size_t n=mypoints.size();
                for(int i=0;i<n;i++){
                painter.drawLine(mypoints[i],mypoints[(i+1)%n]);
               }

            }
            painter.restore();
            if(type=='a'||type=='b'){
            curvepts=u->drawBezier(mypoints,type,this->max);
            for(int i=0;i<curvepts.size()-1;i++){
            painter.drawLine(curvepts[i],curvepts[i+1]);
           }
            }
            else if(type=='c'||type=='d'){
            curvepts=u->BSubDivide(mypoints,type,5);
            for(int i=0;i<curvepts.size()-1;i++){
            painter.drawLine(curvepts[i],curvepts[i+1]);
                  }
              }
         }
       }
        else{
            for(std::vector<QPointF>::iterator t=trajpoints.begin();t!=trajpoints.end();t++){
                painter.drawEllipse((*t),5,5);


            }
            if(trajpoints.size()>1){
                painter.save();
                painter.setPen(QColor(200,200,10));
                for(int i=0;i<trajpoints.size()-1;i++){
                painter.drawLine(trajpoints[i],trajpoints[i+1]);
               }
                painter.restore();
                if(type=='a'||type=='b')
                trajcurvepts=u->drawBezier(trajpoints,type,this->max);
                else if(type=='c'||type=='d')
                    trajcurvepts=u->BSubDivide(trajpoints,type,5);
                for(int i=0;i<trajcurvepts.size()-1;i++){
                painter.drawLine(trajcurvepts[i],trajcurvepts[i+1]);
               }

            }
        }


        painter.restore();

}
void RenderArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
        if(!isjoined){

        for(int i=0;i<mypoints.size();i++){
            if(abs((mypoints[i].x())-lastPoint.x())<10 && abs((mypoints[i].y())-lastPoint.y())<10){
                editpos=i;
                isediting=true;
                break;
            }

        }
        if(isediting && insertmode){
            vector<QPointF>::iterator it(mypoints.begin());
            advance(it,editpos);
            mypoints.insert(it,mypoints[editpos]);
            repaint();
        }
        if(!isediting){
        mypoints.push_back(lastPoint);


        modified = true;
        repaint();
        }
      }
        else{
            for(int i=0;i<trajpoints.size();i++){
                if(abs((trajpoints[i].x())-lastPoint.x())<10 && abs((trajpoints[i].y())-lastPoint.y())<10){
                    editpos=i;
                    isediting=true;
                    break;
                }

            }
            if(isediting && insertmode){
                vector<QPointF>::iterator it(trajpoints.begin());
                advance(it,editpos);
                trajpoints.insert(it,trajpoints[editpos]);
                repaint();
            }
            if(!isediting){
            trajpoints.push_back(lastPoint);


            modified = true;
            repaint();
            }
        }
    }

}
void RenderArea::mouseMoveEvent(QMouseEvent *event)
{
    if((event->buttons() & Qt::LeftButton) && isediting){
        mypoints[editpos]=event->pos();
    }
    repaint();

}
void RenderArea::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton && isediting) {
            isediting = false;
        }
    }
void RenderArea::setmode(){
    insertmode=!insertmode;
}
void RenderArea::setPlane(){
    isjoined=!isjoined;
    repaint();
}
void RenderArea::genoff(){
    int h,w;
    h=height();
    w=width();
     for(std::vector<QPointF>::iterator it=curvepts.begin();it!=curvepts.end();it++){
         QPointF sp(2*(it->x()/w)-1,1-2*(it->y()/h));
         scaledpoints.push_back(sp);
        // cout<<sp.x()<<" "<<sp.y()<<endl;
     }
     for(std::vector<QPointF>::iterator it=trajcurvepts.begin();it!=trajcurvepts.end();it++){
         QPointF sp(2*(it->x()/w)-1,1-2*(it->y()/h));
         scaledtrajpoints.push_back(sp);
         //cout<<sp.x()<<" "<<sp.y()<<endl;
     }
 Mesh m;
 if(curve==1){
    m=u->createData(scaledpoints,this->axis,this->numslice,&flag);
    if(flag){
            QMessageBox msgBox;
            msgBox.setText("Overlap within surfaces");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
    }
    flag=false;
 }
 else if(curve==2)
     m=u->createEx(scaledpoints,this->numslice,z);
 else if(curve==3)
     m=u->createSweep(scaledpoints,scaledtrajpoints,this->numslice);
u->writetofile(m);
cout<<"Done Writing...."<<endl;
QMessageBox msgBox;
msgBox.setText("Writing to file done");
msgBox.setStandardButtons(QMessageBox::Ok);
msgBox.exec();
scaledpoints.clear();
scaledtrajpoints.clear();
}
void RenderArea::setType(char c){
    this->type=c;
    repaint();
}
void RenderArea::setSlice(int x){
    this->numslice=x;
}
void RenderArea::setEx(float z){
    this->z=z;
}
