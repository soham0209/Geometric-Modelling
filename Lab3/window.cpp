#include "window.h"
#include <QGroupBox>
#include <QRadioButton>
#include <QPushButton>
#include <QGridLayout>

Window::Window(QWidget *parent)
    : QWidget(parent)
{
    handler=new Handler;
    QGroupBox* buttonbox=new QGroupBox("Surface Selector");
    QRadioButton* b1=new QRadioButton("Bezier Surface");
    QRadioButton* b2=new QRadioButton("Bspline Surface");
    QRadioButton* b3=new QRadioButton("Doo-Sabin Surface");
    QRadioButton* b4=new QRadioButton("Catmull-Clark Surface");
    QRadioButton *b5=new QRadioButton("Loop Surface");
    QRadioButton *b6=new QRadioButton("Bspline Surface with Boundary");
    this->b1=b1;
    b1->setChecked(true);
    this->b2=b2;
    this->b3=b3;
    this->b4=b4;
    this->b5=b5;
    this->b6=b6;

    QVBoxLayout *vbox=new QVBoxLayout;
    vbox->addWidget(b1);
    vbox->addWidget(b2);
    vbox->addWidget(b6);
    vbox->addWidget(b3);
    vbox->addWidget(b4);
    vbox->addWidget(b5);
    vbox->addStretch(5);
    buttonbox->setLayout(vbox);

    connect(b1,SIGNAL(toggled(bool)),this,SLOT(setChoice()));
    connect(b2,SIGNAL(toggled(bool)),this,SLOT(setChoice()));
    connect(b6,SIGNAL(toggled(bool)),this,SLOT(setChoice()));
    connect(b3,SIGNAL(toggled(bool)),this,SLOT(setChoice()));
    connect(b4,SIGNAL(toggled(bool)),this,SLOT(setChoice()));
    connect(b5,SIGNAL(toggled(bool)),this,SLOT(setChoice()));

    QPushButton *create=new QPushButton("Create Surface");
    this->create=create;
    connect(create,SIGNAL(clicked(bool)),this,SLOT(writefile()));

    QGridLayout *mainLayout = new QGridLayout;

    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(3, 1);
    mainLayout->addWidget(buttonbox);
    mainLayout->addWidget(create);
    setLayout(mainLayout);
    setWindowTitle(tr("Surfaces"));

}

Window::~Window()
{

}
void Window::setChoice(){
    if(b1->isChecked())
        handler->setSurf(1);

    else if(b2->isChecked())
        handler->setSurf(2);

    else if(b3->isChecked())
        handler->setSurf(3);

    else if(b4->isChecked())
        handler->setSurf(4);

    else if(b5->isChecked())
        handler->setSurf(5);
    else if(b6->isChecked())
        handler->setSurf(6);
}
void Window::writefile(){
    handler->createSurface();
}
