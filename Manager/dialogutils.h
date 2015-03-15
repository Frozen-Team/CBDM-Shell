#ifndef UTILS_H
#define UTILS_H

#include <QWidget>
#include <QString>
#include <QFileDialog>

inline bool browseFolderTrigger(QString& path, QWidget *parent = 0)
{
    QFileDialog dialog(parent);

    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOptions(QFileDialog::ShowDirsOnly | QFileDialog::DontUseNativeDialog);

    if (dialog.exec() != 0)
    {
        QString temp = dialog.selectedFiles().at(0);
        if (temp != "")
        {
            path = temp;
        }

        return true;
    }

    return false;
}

#endif // UTILS_H

