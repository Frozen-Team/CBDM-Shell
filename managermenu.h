#ifndef MANAGERMENU_H
#define MANAGERMENU_H

#include <QObject>
#include <QMenu>

class ManagerMenu : public QMenu
{
    Q_OBJECT
public:
    explicit ManagerMenu(QWidget* parent = 0);
    ~ManagerMenu();
};

#endif // MANAGERMENU_H
