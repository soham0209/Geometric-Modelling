#include "curve.h"
#include <vector>
#include <QPointF>
#include <iostream>
#include <cmath>
using namespace std;
Curve::Curve(QWidget *parent)
    : QWidget(parent){
    vector<int>elements;
    for(int j=0;j<21;j++){
        for(int i=0;i<=j;i++){
            if(i==0 || i==j)
                elements.push_back(1);
            else
                elements.push_back(this->pascal[j-1][i-1]+this->pascal[j-1][i]);
        }
        this->pascal.push_back(elements);
        elements.clear();
    }

}

float Curve::B(int i,int n,float u){
float a;
a=(float) (pascal[n][i]*pow((1-u),n-i)*pow(u,i));
return a;
}
QPointF Curve::drawBezier2(std::vector<QPointF> p,float u){
    int n=(int)p.size()-1;
    QPointF pp(0.0,0.0);
    for(int i=0;i<=n;i++){
        pp+=B(i,n,u)*p[i];
    }
    return pp;
}
QPointF Curve::drawRBezier(std::vector<QPointF> p,float u){
    QPointF z(0.0,0.0);
    float d=0.0;
    int n=(int)p.size()-1;
    float w[]={1.0,0.5,1.0};
    for(int i=0;i<p.size();i++){
        z+=B(i,n,u)*p[i]*w[i%3];
        d+=B(i,n,u)*w[i%3];
    }
    z=z/d;
    return z;

}

vector<QPointF> Curve::OneSubDivide(vector<QPointF> p,vector<QPointF>* poly1,vector<QPointF>* poly2,float u){
    size_t n=p.size();
    vector<QPointF> pprime;
    if(n==1){
        poly1->push_back(p[0]);
        poly1->insert(poly1->end(),poly2->begin(),poly2->end());
        return *poly1;
    }
    poly1->insert(poly1->end(),p[0]);
    poly2->insert(poly2->begin(),p[n-1]);
    for(int i=0;i<n-1;i++){
    pprime.push_back(p[i]+(u*(p[i+1]-p[i])));
    }

    return OneSubDivide(pprime,poly1,poly2,u);
}

vector<QPointF> Curve::SubDivide(vector<QPointF> p,int m,float u){
    vector<QPointF>* poly1=new vector<QPointF>();
    vector<QPointF>* poly2=new vector<QPointF>();
    vector<QPointF> pprime;
    if(m==1)
        return Curve::OneSubDivide(p,poly1,poly2,u);
        pprime=Curve::OneSubDivide(p,poly1,poly2,u);
        vector<QPointF> s1,s2;

        size_t mid=pprime.size()/2;
        vector<QPointF>::iterator middleIter(pprime.begin());
        advance(middleIter, mid);

        vector<QPointF> lhalf;
        lhalf.insert(lhalf.begin(),pprime.begin(), middleIter);
        lhalf.push_back(pprime[mid]);
        std::vector<QPointF> rhalf;
        rhalf.insert(rhalf.begin(),middleIter, pprime.end());

        s1=SubDivide(lhalf,m-1,u);
        s2=SubDivide(rhalf,m-1,u);
        s1.insert(s1.end(),s2.begin(),s2.end());
        return s1;
}
vector<int> Curve::genKnot(int n,int D){
    vector<int> T;
    for(int j=0;j<=(n+D);j++){
        if(j<D)
            T.push_back(0);
        else if(D<=j && j<=n)
            T.push_back(j-D+1);
        else
            T.push_back(n-D+2);
    }
    return T;
}
float Curve::N(int i,int d,float u,vector<int> T){
    float z;
    if(d==1){
        if(T[i]<=u && u<T[i+1])
            z= 1.0;
        else
            z=0.0;
    }
    else{
        if(T[i+d-1]==T[i] && T[i+d]==T[i+1])
            z= 0.0;
        else if(T[i+d-1]==T[i])
           z= (((T[i+d]-u)*N(i+1,d-1,u,T))/(T[i+d]-T[i+1]));
        else if(T[i+d]==T[i+1])
            z= (((u-T[i])*N(i,d-1,u,T))/(T[i+d-1]-T[i]));
        else
             z= (((T[i+d]-u)*N(i+1,d-1,u,T))/(T[i+d]-T[i+1]))+(((u-T[i])*N(i,d-1,u,T))/(T[i+d-1]-T[i]));

    }

    return z;
}

vector<QPointF> Curve::BSubDivide(vector<QPointF> p,int m){

    size_t n=(int) p.size();
    vector<QPointF> q;
    for(int i=0;i<n-1;i++){
    q.push_back((3*(p.at(i))+p.at(i+1))/4);
    q.push_back(((p.at(i))+3*p.at(i+1))/4);
    }

if(m==1)
    return q;

    return BSubDivide(q,m-1);
}
