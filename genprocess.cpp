#include "genprocess.h"
#include <QDebug>
#include <sstream>

QRegExp GenProcess::processStatusReg = QRegExp("<<\\d+\\.\\d+>>");
QRegExp GenProcess::processStatusNumbReg = QRegExp("\\d+\\.\\d+");

GenProcess::GenProcess(QObject *parent) : QObject(parent)
{
    connect(processPtr.get(), &QProcess::started, this, &GenProcess::processStarted);
    connect(processPtr.get(), SIGNAL(finished(int)), this, SLOT(processFinished(int)));
    //connect(processPtr.get(), &QProcess::finished, this, &GenProcess::processFinished);
    connect(processPtr.get(), &QProcess::readyReadStandardOutput, this, &GenProcess::processRead);
    connect(processPtr.get(), &QProcess::readyReadStandardError, this, &GenProcess::processRead);
    //qDebug() << processPtr->
}

void GenProcess::runScript(const QString& pythonPath, const QString& scriptFolder)
{
    terminateProcess();
    processPtr->setProcessChannelMode(QProcess::MergedChannels);
    processPtr->setWorkingDirectory(scriptFolder);
    processPtr->start(pythonPath + "/python.exe", QStringList() << "-u" << "build_project.py");
}

void GenProcess::processStarted()
{
}

void GenProcess::processFinished(int f)
{
    QString message;
    message += "Finished with code: " + QString::number(f);
    emit logWrite(message);
}

void GenProcess::processRead()
{
    QTextStream stream(processPtr.get()->readAllStandardOutput());
    while(!stream.atEnd())
    {
        QString&& line = stream.readLine();
        if (processStatusReg.exactMatch(line))
        {
            if (processStatusNumbReg.indexIn(line) != -1)
            {
                int status = 100 * processStatusNumbReg.capturedTexts().at(0).toDouble();
                emit progressUpdate(status);
            }
        } else
        {
            emit logWrite(line);
        }
    }
}

void GenProcess::terminateProcess()
{
    if (processPtr->state() == QProcess::Running)
    {
        processPtr->terminate();
    }
}


GenProcess::~GenProcess()
{

}


