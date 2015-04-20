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
    inline const QString sepRegExp() { return QString("((\\\\)|(\\\\\\\\)|(\\/))"); }
    /**
     * @brief Formats QString with ellipse.
     * @tparam t Parameters.
     * @param str Format QString
     */
    template<typename... T>
    inline const QString stringFormat(const QString& str, T... t)
    {
        QString format;
        format.sprintf(str.toStdString().c_str(), t...);
        return format;
    }

    /**
     * @brief isFileExists Checks if file exists.
     * @param path Path to file.
     * @return true if exists, otherwise false.
     */
    inline bool isFileExists(const QString &path)
    {
        QFile file(path);
        return file.exists();
    }

    /**
     * @brief getNameFromPath Get name of file of folder by its full path.
     * @param path Path to object
     * @return Name
     */
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
    inline const QString getAbsPath(const QString& path)
    {
        QString newPath = QDir(path).canonicalPath();
        return QDir(newPath).absolutePath();
    }

    /**
     * @brief splitQSettingsKeys Splits into all keys of QSettings keys.
     * @param keys input keys.
     * @param keysValues Split keys.
     */
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
    /**
     * @brief browseFolderTrigger Folder browser dialog implementation.
     * @param path Path to folder.
     * @param parent Parent object.
     * @return true if folder specified otherwise false.
     */
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

