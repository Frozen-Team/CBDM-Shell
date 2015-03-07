#include "managermenubar.h"

#include <QMessageBox>
#include <QDebug>
#include <QMouseEvent>
#include "dependencymanagermain.h"

#include <QCursor>

ManagerMenuBar::ManagerMenuBar(QWidget *parent)
    : QMenuBar(parent)
{
    //set
    moveWindow = false;

}

ManagerMenuBar::~ManagerMenuBar()
{

}

void ManagerMenuBar::mousePressEvent(QMouseEvent *e)
{
    moveWindow = true;
    oldMousePos = e->pos();

    QMenuBar::mousePressEvent(e);
}

void ManagerMenuBar::mouseReleaseEvent(QMouseEvent *e)
{
    moveWindow = false;
    QMenuBar::mouseReleaseEvent(e);
}

void ManagerMenuBar::mouseMoveEvent(QMouseEvent *e)
{
    if (moveWindow)
    {
        if (e->buttons() && Qt::LeftButton)
        {
            auto temp = ((QMainWindow*)parent())->pos();
            ((QMainWindow*)parent())->move(e->pos() - oldMousePos + temp);
        }
    }
    QMenuBar::mouseMoveEvent(e);

}

void ManagerMenuBar::holdCurrentWindowPos(QPoint &p)
{
    currentWindowPos = p;
}

