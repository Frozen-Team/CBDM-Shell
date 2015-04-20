#ifndef GENPROCESS_H
#define GENPROCESS_H

#include <QObject>
#include <QProcess>
#include <memory>

class GenProcess : public QObject
{
    Q_OBJECT
public:
    explicit GenProcess(QObject *parent = 0);
    GenProcess(const GenProcess& rhs) = delete;
    GenProcess& operator=(const GenProcess& rhs) = delete;
    ~GenProcess();

signals:
    void logWrite(const QString& message);
    void progressUpdate(int progress);

public slots:
    void runScript(const QString &pythonPath, const QString &scriptFolder);
    void processStarted();
    void processFinished(int);
    void processRead();
    void terminateProcess();

private:
    static QRegExp processStatusReg;// = QRegExp("<<\\d+\\.\\d+>>.+$");
    static QRegExp processStatusNumbReg;// = QRegExp("\\d+\\.\\d+");
    std::unique_ptr<QProcess> processPtr = std::make_unique<QProcess>();
};

#endif // GENPROCESS_H
