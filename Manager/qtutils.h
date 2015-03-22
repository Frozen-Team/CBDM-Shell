#ifndef QTUTILS
#define QTUTILS

#include <QWidget>
#include <QFileDialog>
#include <QString>
#include <QFile>
#include <QDir>
#include <QFileInfo>


namespace QtUtils
{
    template<typename... T>
    inline const QString stringFormat(const QString& str, T... t)
    {
        QString format;
        format.sprintf(str.toStdString().c_str(), t...);
        return format;
    }

    inline bool isFileExists(const QString &path)
    {
        QFile file(path);
        return file.exists();
    }

    inline const QString getNameFromPath(const QString &path)
    {
        QFileInfo target(path);
        //return QDir(path).dirName();
        if (target.isDir())
        {
            QDir dir(path + "/");
            return dir.dirName();
        }
        if (target.isFile())
        {
            return target.fileName();
        }
        return QString();
    }

    inline void splitQSettingsKeys(const QStringList& keys, QMap<QString, QStringList>& keysValues)
    {
        for (const QString &key : keys)
        {
            QStringList splitValue = key.split("/");
            if (splitValue.size() <= 1)
            {
                continue;
            }
            QString name = splitValue.at(0);
            QString param = splitValue.at(1);
            if (keysValues.keys().indexOf(name) >= 0)
            {
                keysValues[name].push_back(param);
            } else {
                QStringList firstParam = { param };
                keysValues[name] = firstParam;
            }
        }
    }

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
}
#endif // QTUTILS

