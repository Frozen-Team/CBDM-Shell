#ifndef MANAGERMENU_H
#define MANAGERMENU_H

#include <QObject>
#include <QMenu>

class ManagerMenu : public QMenu
{
public:
    explicit ManagerMenu(QWidget* parent = 0);
    ~ManagerMenu();
};

#endif // MANAGERMENU_H
