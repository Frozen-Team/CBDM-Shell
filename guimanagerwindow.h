#ifndef GUIMANAGERWINDOW_H
#define GUIMANAGERWINDOW_H

#include <QMainWindow>

class GUIManagerWindow : public QMainWindow
{
    Q_OBJECT

public:
    GUIManagerWindow(QWidget *parent = 0);
    ~GUIManagerWindow();
static void setupStyles();
private:


};

#endif // GUIMANAGERWINDOW_H
