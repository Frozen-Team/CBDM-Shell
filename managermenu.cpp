#include "managermenu.h"
#include <QDebug>

ManagerMenu::ManagerMenu(QWidget *parent)
    : QMenu(parent)
{
    connect(this, SIGNAL(hovered(QAction*)), this, SLOT(hovered(QAction*)));
}

ManagerMenu::~ManagerMenu()
{

}
