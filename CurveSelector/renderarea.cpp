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
#include "curve.h"


#include <QPainter>
#include <vector>
#include <iostream>
#include <iterator>
#include <cmath>

using namespace std;
RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent)
{
    draw=new Curve;
    this->curve=1;
    this->level=1;
    this->isediting=false;
    this->insertmode=false;
    this->isjoined=false;
    this->D=4;
    this->max=100;

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
void RenderArea::setLevel(int level){
    this->level=level;
    repaint();
}
void RenderArea::cls(){
    mypoints.clear();
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
    QPen pen(Qt::blue, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));

        float u=0.0;
        float delta=(float)(1.0/max);
        QPointF l1,l2,r1,r2;
       vector<QPointF> curvepoints;
       vector<int> T;
        painter.save();
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor (250,10,10));

        for(std::vector<QPointF>::iterator it=mypoints.begin();it!=mypoints.end();it++){
            painter.drawEllipse((*it),5,5);

        }
        if(mypoints.size()>1){
            painter.setPen(QColor(200,200,10));
            if(curve!=7){
                for(int i=0;i<mypoints.size()-1;i++){
                painter.drawLine(mypoints[i],mypoints[i+1]);
                }
            }
            painter.setPen(pen);
            painter.setBrush(QColor(10,10,250));
             if(curve==1){
                 l1=mypoints[0];
                 l2=mypoints[0];
                 for(int i=0;i<=max;i++){
                     l1=l2;
                    l2=draw->drawBezier2(mypoints,u);
                    painter.drawLine(l1,l2);
                    u=u+delta;
                 }
             }
                else if(curve==2){
                 l1=mypoints[0];
                 l2=mypoints[0];
                 painter.setPen(pen);
                 u=0.5;
                  curvepoints=draw->SubDivide(mypoints,level,u);
                  //u=0.0;
                  for(int i=1;i<curvepoints.size();i++){
                      l1=l2;
                     l2=curvepoints[i];
                     painter.drawLine(l1,l2);
                  }
            }
             else if(curve==3){
                painter.setPen(pen);
                 QPointF p;
                   l1=mypoints[0];
                   l2=mypoints[0];
                 int n= (int) mypoints.size()-1;

                 int D=this->D;
                 T=draw->genKnot(n,D);
                 u=0.0;
                 int ul=(int) T.back()/delta;
                 for(int j=0;j<ul;j++){

                    p.setX(0.0);
                     p.setY(0.0);
                     for(int i=0;i<=n;i++){
                         float z=draw->N(i,D,u,T);
                        p.setX(((p.x())+(z)*(mypoints[i].x())));
                        p.setY(((p.y())+(z)*(mypoints[i].y())));
                     }
                     l1=l2;
                     l2=p;
                     painter.drawLine(l1,l2);
                     u=u+delta;
                 }
             }
             else if(curve==4){
              l1=mypoints[0];
              l2=mypoints[0];
              painter.setPen(QColor(10,10,250));
              curvepoints.clear();
               curvepoints=draw->BSubDivide(mypoints,level);
               for(int i=1;i<curvepoints.size();i++){
                   l1=l2;
                  l2=curvepoints[i];
                  painter.drawLine(l1,l2);
               }
               painter.drawLine(l2,mypoints.back());
         }
             else if(curve==5){
                 u=0.0;
                 l1=mypoints[0];
                 l2=mypoints[0];
                 r1=mypoints[0];
                 r2=r1;
                 for(int i=0;i<=max;i++){
                     l1=l2;
                     r1=r2;
                    l2=draw->drawBezier2(mypoints,u);
                    r2=draw->drawRBezier(mypoints,u);
                    painter.save();
                    painter.setPen(QColor(10,200,10));
                    painter.drawLine(l1,l2);
                    painter.restore();
                    painter.drawLine(r1,r2);
                    u=u+delta;
                 }
             }

             else if(curve==6){
                painter.setPen(QColor(10,10,250));
                 QPointF p(0.0,0.0);
                   l1=mypoints[0];
                   l2=mypoints[0];
                 int n= (int) mypoints.size()-1;
                 T.clear();
                 int D=4;
                 T=draw->genKnot(n,D);
                 u=0.0;
                 float d;
                 int ul=(int) T.back()/delta;
                 float w[]={1.0f,1.0f,5.0f,1.0f};
                 for(int j=0;j<ul;j++){
                        d=0.0;
                        p.setX(0.0);
                        p.setY(0.0);
                     for(int i=0;i<=n;i++){
                         float z=draw->N(i,D,u,T);
                        p+=z*mypoints[i]*w[i%4];
                        d+=z*w[i%4];
                     }
                     l1=l2;
                     l2=p/d;
                     painter.drawLine(l1,l2);
                     u=u+delta;
                 }
             }

            else if(curve==7){
                if(mypoints.size()==8){

            painter.setPen(QColor(200,200,10));
               vector<QPointF> q,p;
                vector<QPointF>::iterator pit=mypoints.begin();
                advance(pit,4);
                p.assign(mypoints.begin(),pit);
                q.assign(pit,mypoints.end());
             if(!isjoined){
                for(int i=0;i<3;i++){
                    painter.drawLine(p[i],p[i+1]);
                    painter.drawLine(q[i],q[i+1]);
                   }
                painter.setPen(QColor(10,10,200));
                l1=p[0];
                l2=p[0];
                r1=q[0];
                r2=q[0];
                for(int i=0;i<=max;i++){
                    l1=l2;
                    r1=r2;
                   l2=draw->drawBezier2(p,u);
                   r2=draw->drawBezier2(q,u);
                   painter.drawLine(l1,l2);
                   painter.drawLine(r1,r2);
                   u=u+delta;
                }
            }

             else{
                        q[0]=p[3];
                        q[1]=p[3]+(p[3]-p[2]);
                        painter.setPen(Qt::NoPen);
                        painter.setBrush(QColor(10,250,10));
                        painter.drawEllipse(q[0],5,5);
                        painter.drawEllipse(q[1],5,5);
                        painter.setBrush(QColor(3,3,3));
                        painter.drawEllipse(mypoints[4],5,5);
                        painter.drawEllipse(mypoints[5],5,5);
                        for(int i=0;i<3;i++){
                            painter.drawLine(p[i],p[i+1]);
                            painter.drawLine(q[i],q[i+1]);
                           }
                        painter.setPen(QColor(10,10,200));
                        l1=p[0];
                        l2=p[0];
                        r1=q[0];
                        r2=q[0];
                        for(int i=0;i<=max;i++){
                            l1=l2;
                            r1=r2;
                           l2=draw->drawBezier2(p,u);
                           r2=draw->drawBezier2(q,u);
                           painter.drawLine(l1,l2);
                           painter.drawLine(r1,r2);
                           u=u+delta;
                        }


        }
        }
             }
        }
        painter.restore();
}
void RenderArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();

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
void RenderArea::joinCurve(){
    isjoined=!isjoined;
    repaint();
}
