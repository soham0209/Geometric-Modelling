#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>
#include <QSlider>
#include "renderarea.h"
class RenderArea;
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
void genoffmain();
void setax();
void setChoice();
void setPlane();
void cls();
void setType();
void numSlice(int i);
void setSamplingInt(int u);
void setEx(double v);

private:
    RenderArea* renderArea;
    QRadioButton* b1;
    QRadioButton* b2;
    QRadioButton* b3;
    QRadioButton* x;
    QRadioButton* y;
    QRadioButton* open;
    QRadioButton* closed;
    QRadioButton* openb;
    QRadioButton* closedb;
    QSpinBox* slice;
    QSlider* step;


};

#endif // MAINWINDOW_H
