#include "handler.h"
#include "BezierSurface.h"
#include "BsplineSurface.h"
#include "DooSabin.h"
#include "CatmullClark.h"
#include "Loop.h"
#include <QMessageBox>
#include <fstream>
#include <string>
#include <iostream>
using std::cout;
using std::endl;

Handler::Handler(){
    cp=0;
    ncp=0;
    this->surf=1;
}
void Handler::createSurface(){
    cout << "Main program\n";

        string filepoints = "./input/extrusionpoints.off";
        string fileindoo="./input/asu.off";
        string fileoutdoo = "./output/doosabin.off";
        string fileoutspline = "./output/bspline.off";
        string fileinctml="./input/abstr.off";
        string fileoutctml = "./output/CatmullClark.off";
        string loopin = "./input/asu_tri.off";
        string loopout = "./output/Loop.off";
        string filebez = "./output/bezier.off";


        if(surf==1){
        cout << "\n< Creating Bezier Surface>\n";
        BezierSurface bez;
        Mesh m=bez.createBezierPatch(filepoints,50,50);
        float planarity=checkPlanar(m);
        cout<<"planarity percentage "<<planarity<<" %"<<endl;
        if(planarity<50.0){
            cout<<"Please use smaller value of u and v"<<endl;
        }
        outputOFF_mesh(m,filebez);
        }
        else if(surf==2){
        cout << "\n< Creating BSpline Surface >\n";
        BsplineSurface b;
        Mesh mbsps = b.createBsplineSurf(filepoints,'o');
        float planarity=checkPlanar(mbsps);
        cout<<"planarity percentage "<<planarity<<" %"<<endl;
        if(planarity<50.0){
            cout<<"Please use smaller value of u and v"<<endl;
        }
        outputOFF_mesh(mbsps, fileoutspline);
        }
        else if(surf==6){
        cout << "\n< Creating BSpline Surface With Boundary>\n";
        BsplineSurface b;
        Mesh mbsps = b.createBsplineSurf(filepoints,'c');
        float planarity=checkPlanar(mbsps);
        cout<<"planarity percentage "<<planarity<<" %"<<endl;
        outputOFF_mesh(mbsps, fileoutspline);
        }
        else if(surf==3){
        cout << "\n< Creating Doo-Sabin Surface >\n";
        DooSabin d;
        Mesh m=getfile(fileindoo);
        Mesh sd=d.createDooSabin(m,2);
        outputOFF_mesh(sd,fileoutdoo);
        }
        else if(surf==4){
        cout << "\n< Creating Catmull-Clark Surface >\n";
        Mesh m=getfile(fileinctml);
        CatmullClark cat;
        Mesh ctml=cat.createCatmull(m,2);
        outputOFF_mesh(ctml,fileoutctml);
        }
        else if(surf==5){
        cout << "\n< Creating Loop Surface >\n";
        Mesh m=getfile(loopin);
        Loop l;
        Mesh loop=l.createLoop(m,1);
        outputOFF_mesh(loop,loopout);
        }
    else
            cout<<"SORRY WRONG CHOICE"<<endl;

    QMessageBox *m=new QMessageBox;
    m->setText("Writing to file done finally!!!\nPress Ok to continue");
    m->exec();
    return;
}


void Handler::setSurf(int surf){
    this->surf=surf;
}
Mesh Handler::getfile(string filename){
    ifstream input(filename);
        string str;
        input >> str;

        if (str != "OFF") { cout << "Not an OFF file"; exit(0); }
        input >> str;

        int nVertices = stoi(str);
        input >> str;
        int nFaces = stoi(str);
        input >> str;
        int nEdges = stoi(str);
        //cout<<nVertices<<" "<<nFaces<<" "<<nEdges<<"\n";
        vector<GeomVert> coord;
        for (int i = 0; i < nVertices; i++)
        {
            input >> str;    float x = stof(str);
            input >> str;    float y = stof(str);
            input >> str;    float z = stof(str);

            GeomVert v(x, y, z);
            coord.push_back(v);
        }
        Mesh outmesh;
        for (int i = 0; i < nFaces; i++)
        {
            input >> str;     int nov = stoi(str);
            vector<GeomVert> geomfacet;
            for (int j = 0; j < nov; j++)
            {
                input >> str;
                //cout<<str<<" ";
                int index = stoi(str);
                geomfacet.push_back(coord[index]);
            }
            outmesh.AddFacet(geomfacet);

        }
        return outmesh;
}
void Handler::outputOFF_mesh(Mesh m, string filename){
        int nVertex = m.GetNumberVertices();
            int nFace = m.GetNumberFacets();
            int nEdges = m.GetNumberEdges();

            ofstream output;
            output.open(filename);
            output << "OFF" << endl;
            cout << "OFF" << endl;
            output << nVertex << " " << nFace << " " << nEdges << endl;
            cout << nVertex << " " << nFace << " " << nEdges << endl;

            for (int i = 0; i<nVertex; i++) {
                GeomVert vertex = m.GetGeomVertex(i);
                for (int coor = 0; coor<3; coor++) {
                    output << vertex.GetCo(coor) << " ";
                    //cout<<vertex.GetCo(coor)<<" ";
                }
                output << "\n";
                //cout<<"\n";
                //getchar();

            }

            for (int i = 0; i<nFace; i++) {
                TopoFacet f = m.GetFacet(i);
                int nIncVertex = f.GetNumberVertices();
                output << nIncVertex << " ";
                //cout << nIncVertex << " ";
                for (int vIndex = 0; vIndex < nIncVertex; vIndex++) {
                    output << f.GetVertexInd(vIndex) << " ";
                    //cout << f.GetVertexInd(vIndex) << " ";
                }
                output << "\n";
                //cout << "\n";
            }

 }
float Handler::checkPlanar(Mesh m)
{
    double eps=1e-11;
    int nFaces = m.GetNumberFacets();
    for (int i = 0; i<nFaces; i++)
    {
        TopoFacet face = m.GetFacet(i);
        if (face.GetNumberVertices() != 4)
        {
            cout << "Not four " << face.GetNumberVertices(); continue;
        }
        int vind1 = face.GetVertexInd(0);   int vind2 = face.GetVertexInd(1);   int vind3 = face.GetVertexInd(2);   int vind4 = face.GetVertexInd(3);
        GeomVert v1 = m.GetGeomVertex(vind1);       GeomVert v2 = m.GetGeomVertex(vind2);
        GeomVert v3 = m.GetGeomVertex(vind3);       GeomVert v4 = m.GetGeomVertex(vind4);
;
        Vec3f a(v1.GetCo(0),v1.GetCo(1),v1.GetCo(2));
        Vec3f b(v2.GetCo(0),v2.GetCo(1),v2.GetCo(2));
        Vec3f c(v3.GetCo(0),v3.GetCo(1),v3.GetCo(2));
        Vec3f d(v4.GetCo(0),v4.GetCo(1),v4.GetCo(2));

        Vec3f normdir=(c-a).crossProduct(b-a);
        double vol=abs((normdir.dotProduct(d-a))/6.0);

        cout<<"vol: "<<vol<<endl;

        if(abs(vol)<eps)
            cp++;
        else
            ncp++;



    }//end of face loop
    if(cp==0 && ncp==0)
        return 50;
    //cout<<cp<<" "<<ncp<<" "<<endl;
    float percent_cp=((float)cp/(float)(ncp+cp))*100.0f;
    //cout<<percent_cp<<" "<<endl;
    return percent_cp;
}


