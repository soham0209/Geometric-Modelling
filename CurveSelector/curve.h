#ifndef CURVE_H
#define CURVE_H
#include<vector>
#include <QPointF>
#include <QWidget>
using namespace std;
class Curve:public QWidget{
    Q_OBJECT
public:
    Curve(QWidget *parent=0);
    QPointF Curve::drawRBezier(vector<QPointF> p,float u);
    QPointF Curve::drawBezier2(std::vector<QPointF> p,float u);
    vector<QPointF> Curve::SubDivide(vector<QPointF> p,int m,float u);
    vector<QPointF> Curve::OneSubDivide(vector<QPointF> p,vector<QPointF>* poly1,vector<QPointF>* poly2,float u);
    vector<int> Curve::genKnot(int n,int D);
    float Curve::N(int i,int d,float u,vector<int> T);
    float Curve::B(int i,int n,float u);
    vector<QPointF> Curve::BSubDivide(vector<QPointF> p, int m);
    vector<vector<int>> pascal;
};
#endif // CURVE_H

