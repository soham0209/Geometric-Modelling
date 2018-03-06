#include "utils.h"
#include "mesh.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <QVector2D>
#include <QPointF>
#include <vector>
#include <QMessageBox>

#define PI 3.1415

using namespace std;
vector<vector<int>> pascal;

Utils::Utils(){
    vector<int>elements;

        for(int j=0;j<31;j++){
            for(int i=0;i<=j;i++){
                if(i==0 || i==j)
                    elements.push_back(1);
                else
                    elements.push_back(pascal[j-1][i-1]+pascal[j-1][i]);
            }
            pascal.push_back(elements);
            elements.clear();
        }

}
Utils::~Utils(){

}

Mesh Utils::createData(vector<QPointF> mypoints, char c, int numslices,bool* flag){
    vector<GeomVert> points;
    Mesh m;
    float v;
    vec3 u1,u2,u3,u4;
    float step=(2*PI)/numslices;
    if(c=='x'){
        v=0.0;

        for(int i=0;i<mypoints.size()-2;i++){
            for(int j=0;j<numslices;j++){
                u1.x=mypoints[i].x();
                u1.y=mypoints[i].y()*cos(v);
                u1.z=mypoints[i].y()*sin(v);
                u1.round(&u1);

                u2.y=mypoints[i].y()*cos(v+step);
                u2.x=mypoints[i].x();
                u2.z=mypoints[i].y()*sin(v+step);
                u2.round(&u2);

                u3.y=mypoints[i+1].y()*cos(v);
                u3.x=mypoints[i+1].x();
                u3.z=mypoints[i+1].y()*sin(v);
                u3.round(&u3);

                u4.y=mypoints[i+1].y()*cos(v+step);
                u4.x=mypoints[i+1].x();
                u4.z=mypoints[i+1].y()*sin(v+step);
                u4.round(&u4);

                GeomVert *p1=new GeomVert(u1.x,u1.y,u1.z);
                GeomVert *p2=new GeomVert(u2.x,u2.y,u2.z);
                GeomVert *p3=new GeomVert(u3.x,u3.y,u3.z);
                GeomVert *p4=new GeomVert(u4.x,u4.y,u4.z);

                points.push_back(*p1);
                points.push_back(*p2);
                points.push_back(*p4);
                points.push_back(*p3);
                m.AddFacet(points);
                points.clear();
                v=v+step;
            }
            v=0.0f;
            if((mypoints[i].y())*(mypoints[i+1].y())<0){
                *flag=true;
            }

        }


    }
    else if(c=='y'){
            v=0.0;
            for(int i=0;i<mypoints.size()-2;i++){
                for(int j=0;j<numslices;j++){
                    u1.x=mypoints[i].x()*cos(v);
                    u1.y=mypoints[i].y();
                    u1.z=mypoints[i].x()*sin(v);
                    u1.round(&u1);

                    u2.y=mypoints[i].y();
                    u2.x=mypoints[i].x()*cos(v+step);
                    u2.z=mypoints[i].x()*sin(v+step);
                    u2.round(&u2);

                    u3.y=mypoints[i+1].y();
                    u3.x=mypoints[i+1].x()*cos(v);
                    u3.z=mypoints[i+1].x()*sin(v);
                    u3.round(&u3);

                    u4.y=mypoints[i+1].y();
                    u4.x=mypoints[i+1].x()*cos(v+step);
                    u4.z=mypoints[i+1].x()*sin(v+step);
                    u4.round(&u4);

                    GeomVert *p1=new GeomVert(u1.x,u1.y,u1.z);
                    GeomVert *p2=new GeomVert(u2.x,u2.y,u2.z);
                    GeomVert *p3=new GeomVert(u3.x,u3.y,u3.z);
                    GeomVert *p4=new GeomVert(u4.x,u4.y,u4.z);

                    points.push_back(*p1);
                    points.push_back(*p2);
                    points.push_back(*p4);
                    points.push_back(*p3);
                    m.AddFacet(points);
                    points.clear();
                    v=v+step;
                }
                v=0.0f;
            if((mypoints[i].x())*(mypoints[i+1].x())<0){
               *flag=true;
            }
         }
    }

    return m;
}


void Utils::writetofile(Mesh m){
    cout<<"Began Writing"<<endl;
    ofstream myfile;
    myfile.open("parabola.off");
    if(myfile.is_open()){
        int n=m.GetNumberFacets();
        int t=m.GetNumberVertices();
        myfile<<"OFF"<<endl;
        myfile<<t<<" "<<n<<" "<<0<<endl;
        cout<<"OFF"<<endl;
        cout<<t<<" "<<n<<" "<<0<<endl;

        for(int i=0;i<t;i++){
            GeomVert g=m.GetGeomVertex(i);
            myfile<<g.GetCo(0)<<" "<<g.GetCo(1)<<" "<<g.GetCo(2)<<endl;

        }
        for(int i=0;i<n;i++){
            TopoFacet f=m.GetFacet(i);
            int j=f.GetNumberVertices();
            myfile<<j;

            for(int k=0;k<j;k++){
                myfile<<" "<<f.GetVertexInd(k);

            }
            myfile<<endl;


        }
        cout<<"Writing to file done"<<endl;
        myfile.close();

    }
    else{
        cout<<"Unable to Open File"<<endl;
    }
}
Mesh Utils::createEx(vector<QPointF> mypoints, int numslices,float z){
    vector<GeomVert> points;
    float height=z;
    cout<<"height: "<<z<<endl;
    Mesh m;
    vec3 u1,u2,u3,u4;
    float delta=(float)(height/numslices);
    for(int i=0;i<mypoints.size()-2;i++){
            for(int j=0;j<numslices;j++){
                u1.x=mypoints[i].x();
                u1.y=mypoints[i].y();
                u1.z=j*delta;
                u1.round(&u1);

                u2.y=mypoints[i].y();
                u2.x=mypoints[i].x();
                u2.z=(j+1)*delta;
                u2.round(&u2);

                u3.y=mypoints[i+1].y();
                u3.x=mypoints[i+1].x();
                u3.z=j*delta;
                u3.round(&u3);

                u4.y=mypoints[i+1].y();
                u4.x=mypoints[i+1].x();
                u4.z=(j+1)*delta;
                u4.round(&u4);

                GeomVert *p1=new GeomVert(u1.x,u1.y,u1.z);
                GeomVert *p2=new GeomVert(u2.x,u2.y,u2.z);
                GeomVert *p3=new GeomVert(u3.x,u3.y,u3.z);
                GeomVert *p4=new GeomVert(u4.x,u4.y,u4.z);

                points.push_back(*p1);
                points.push_back(*p2);
                points.push_back(*p4);
                points.push_back(*p3);
                m.AddFacet(points);
                points.clear();
            }
    }
    return m;
}
Mesh Utils::createSweep(vector<QPointF> mypoints, vector<QPointF> trajpoints,int numslices){
    vector<GeomVert> points;
    float lasty=0.0f,lastz=0.0f;
    Mesh m;
    vec3 u1,u2,u3,u4;
    int steps=trajpoints.size()/numslices;
    for(int k=1;k<numslices;k++){
        QVector2D dir(trajpoints[k*steps]-trajpoints[(k-1)*steps]);
            for(int j=0;j<mypoints.size()-2;j++){
                u1.x=mypoints[j].x();
                u1.y=mypoints[j].y()+lasty;
                u1.z=lastz;
                u1.round(&u1);

                u2.y=mypoints[j].y()+dir.x()+lasty;
                u2.x=mypoints[j].x();
                u2.z=dir.y()+lastz;
                u2.round(&u2);

                u3.y=mypoints[j+1].y()+lasty;
                u3.x=mypoints[j+1].x();
                u3.z=lastz;
                u3.round(&u3);

                u4.y=mypoints[j+1].y()+dir.x()+lasty;
                u4.x=mypoints[j+1].x();
                u4.z=dir.y()+lastz;
                u4.round(&u4);

                GeomVert *p1=new GeomVert(u1.x,u1.y,u1.z);
                GeomVert *p2=new GeomVert(u2.x,u2.y,u2.z);
                GeomVert *p3=new GeomVert(u3.x,u3.y,u3.z);
                GeomVert *p4=new GeomVert(u4.x,u4.y,u4.z);

                points.push_back(*p1);
                points.push_back(*p2);
                points.push_back(*p4);
                points.push_back(*p3);
                m.AddFacet(points);
                points.clear();
            }
    lasty=lasty+dir.x();
    lastz=lastz+dir.y();
    }
    return m;
}
float Utils::B(int i,int n,float u){
float a;
a=(float) (pascal[n][i]*pow((1-u),n-i)*pow(u,i));
return a;
}
vector<QPointF> Utils::drawBezier(vector<QPointF> mypoints, char type, int max){
    vector<QPointF> curvepts;
    float u;
    float delta=(float)(1.0f/max);
    QPointF pp(0.0,0.0);
    size_t n=mypoints.size()-1;
    for(u=0.0;u<=1.0;u=u+delta){
            pp.setX(0.0);
            pp.setY(0.0);
            if(type=='a'){
            for(int i=0;i<=n;i++){
                pp+=B(i,n,u)*(mypoints[i]);
            }
          }
            else if(type=='b'){
                for(int i=0;i<=n+1;i++){
                    pp+=B(i,n+1,u)*(mypoints[i%(n+1)]);
                }
            }
         curvepts.push_back(pp);
    }
    return curvepts;
}
vector<QPointF> Utils::BSubDivide(vector<QPointF> p,char type,int m){

    size_t n=(int) p.size()-1;
    vector<QPointF> q;
    if(type=='c'){
    for(int i=0;i<n;i++){
    q.push_back((3*(p.at(i))+p.at(i+1))/4);
    q.push_back(((p.at(i))+3*p.at(i+1))/4);
    }
   }
    else if(type=='d'){
        for(int i=0;i<=n;i++){
            q.push_back((3*(p.at(i))+p.at((i+1)%(n+1)))/4);
            q.push_back(((p.at(i))+3*p.at((i+1)%(n+1)))/4);
        }
    }

if(m==1)
    return q;

    return BSubDivide(q,type,m-1);
}
