#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QRadioButton>
#include "mesh.h"
#include "handler.h"

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(QWidget *parent = 0);
    ~Window();
public slots:
    void setChoice();
    void writefile();
private:

    QRadioButton *b1,*b2,*b3,*b4,*b5,*b6;
    QPushButton *create;
    Handler *handler;
};

#endif // WINDOW_H
