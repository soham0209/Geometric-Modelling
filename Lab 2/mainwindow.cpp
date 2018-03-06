#include <QtWidgets>
#include <QRadioButton>
#include <QSpinBox>
#include <QSlider>
#include "mainwindow.h"
#include "renderarea.h"
#include "mesh.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    renderArea = new RenderArea;
    QPushButton* gen=new QPushButton("Generate OFF File");
    connect(gen,SIGNAL(clicked(bool)),this,SLOT(genoffmain()));

    QPushButton* nex=new QPushButton("Draw Sweep Wire");
    connect(nex,SIGNAL(clicked(bool)),this,SLOT(setPlane()));
    nex->setDisabled(true);

    QPushButton* cls=new QPushButton("Clear");
    connect(cls,SIGNAL(clicked(bool)),this,SLOT(cls()));




    QSpinBox* slice=new QSpinBox();
    this->slice=slice;
    slice->setRange(2,32);
    slice->setValue(8);
    slice->setSingleStep(2);
    connect(slice,SIGNAL(valueChanged(int)),this,SLOT(numSlice(int)));

    QGroupBox* slbox=new QGroupBox("No. of Slices");
    QVBoxLayout *pbox=new QVBoxLayout;
    pbox->addWidget(slice);
    slbox->setLayout(pbox);

    QSlider* sl=new QSlider(Qt::Horizontal);
    this->step=sl;
    sl->setRange(10,100);
    sl->setTickInterval(10);
    sl->setValue(50);
    sl->setTickPosition(QSlider::TicksBelow);
    connect(sl,SIGNAL(valueChanged(int)),this,SLOT(setSamplingInt(int)));


    QDoubleSpinBox* ex=new QDoubleSpinBox();
    ex->setRange(0,1.0);
    ex->setValue(0.5);
    ex->setSingleStep(0.1);
    connect(ex,SIGNAL(valueChanged(double)),this,SLOT(setEx(double)));


    QGroupBox* exbox=new QGroupBox("Height of Extrusion (Normalized)");
    QVBoxLayout *tbox=new QVBoxLayout;
    tbox->addWidget(ex);
    exbox->setLayout(tbox);


    b1=new QRadioButton("Surfaces of Rev");
    b1->setChecked(true);

    b2=new QRadioButton("Extrusion in Z");

    b3=new QRadioButton("Sweep Surfaces");


    QGroupBox* buttonbox=new QGroupBox("Options");

    QVBoxLayout *vbox=new QVBoxLayout;
    vbox->addWidget(b1);
    vbox->addWidget(b2);
    vbox->addWidget(b3);
    buttonbox->setLayout(vbox);

    QRadioButton* x=new QRadioButton("X-AXIS");
    this->x=x;
    x->setChecked(true);

    connect(x,SIGNAL(toggled(bool)),this,SLOT(setax()));
    QRadioButton* y=new QRadioButton("Y-AXIS");
    this->y=y;
    connect(y,SIGNAL(toggled(bool)),this,SLOT(setax()));

    QGroupBox* xbox=new QGroupBox("Genatrix");
    QVBoxLayout *axes=new QVBoxLayout;
    axes->addWidget(x);
    axes->addWidget(y);
    xbox->setLayout(axes);

    QRadioButton* open=new QRadioButton(tr("Open Bezier"));
    open->setChecked(true);
    this->open=open;
    connect(open,SIGNAL(toggled(bool)),this,SLOT(setType()));

    QRadioButton* closed=new QRadioButton(tr("Closed Bezier"));
    this->closed=closed;
    connect(closed,SIGNAL(toggled(bool)),this,SLOT(setType()));

    QRadioButton* openb=new QRadioButton(tr("Open B-Spline"));;
    this->openb=openb;
    connect(openb,SIGNAL(toggled(bool)),this,SLOT(setType()));

    QRadioButton* closedb=new QRadioButton(tr("Closed B-Spline"));
    this->closedb=closedb;
    connect(closedb,SIGNAL(toggled(bool)),this,SLOT(setType()));


    QGroupBox* ps=new QGroupBox("Curve Type");
    QVBoxLayout *type=new QVBoxLayout;
    type->addWidget(open);
    type->addWidget(closed);
    type->addWidget(openb);
    type->addWidget(closedb);
    ps->setLayout(type);


    connect(b1,SIGNAL(toggled(bool)),this,SLOT(setChoice()));
    connect(b2,SIGNAL(toggled(bool)),this,SLOT(setChoice()));
    connect(b3,SIGNAL(toggled(bool)),this,SLOT(setChoice()));
    connect(b3,SIGNAL(toggled(bool)),nex,SLOT(setEnabled(bool)));


    QGridLayout *mainLayout = new QGridLayout;

    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(5, 1);
    mainLayout->addWidget(renderArea, 0, 0, 1, 4);
    mainLayout->addWidget(slbox);
    mainLayout->addWidget(sl);
    mainLayout->addWidget(gen);
    mainLayout->addWidget(cls);
    mainLayout->addWidget(nex);
    mainLayout->addWidget(buttonbox);
    mainLayout->addWidget(xbox);
    mainLayout->addWidget(exbox);
    mainLayout->addWidget(ps);





    setLayout(mainLayout);
    setWindowTitle(tr("Basic Curves"));
}

MainWindow::~MainWindow()
{

}
void MainWindow::genoffmain(){
renderArea->genoff();
}
void MainWindow::setax(){
    if(x->isChecked())
        renderArea->setAxis('x');
    else if(y->isChecked())
        renderArea->setAxis('y');
}
void MainWindow::setChoice(){
    if(b1->isChecked())
        renderArea->setCurve(1);
    else if(b2->isChecked())
        renderArea->setCurve(2);
    else if(b3->isChecked())
        renderArea->setCurve(3);
}
void MainWindow::cls(){
    renderArea->cls();
}
void MainWindow::setPlane(){
    renderArea->setPlane();
}
void MainWindow::setType(){
    if(open->isChecked())
    renderArea->setType('a');
    else if(closed->isChecked())
    renderArea->setType('b');
    else if(openb->isChecked())
    renderArea->setType('c');
    else if(closedb->isChecked())
    renderArea->setType('d');
}
void MainWindow::numSlice(int i){
    renderArea->setSlice(i);
}
void MainWindow::setSamplingInt(int u){
    renderArea->samplingInt(u);
}
void MainWindow::setEx(double v){
    float u=(float)v;
    renderArea->setEx(u);
}
