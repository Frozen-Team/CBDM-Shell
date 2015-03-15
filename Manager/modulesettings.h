#ifndef MODULESETTINGS_H
#define MODULESETTINGS_H

#include <QMainWindow>
#include <QSettings>
#include "configurableui.h"

namespace Ui {
class ModuleSettings;
}

class ModuleSettings : public QMainWindow
{
    Q_OBJECT

public:
    explicit ModuleSettings(QWidget *parent = 0);

    bool loadUi(const QString &modulePath);
    ~ModuleSettings();


private:
    Ui::ModuleSettings *ui;

private slots:
    void makeUi();
    void on_okPushButton_clicked();

private:
    QSettings* settings;
    ConfUiManager uiManager;
};

#endif // MODULESETTINGS_H
