#ifndef DDPKG_H
#define DDPKG_H

#include <QObject>
#include <QList>
#include <QString>
#include <QProcess>
#include <QDebug>

class DDpkg : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString fileName READ fileName NOTIFY fileNameChanged)
    Q_PROPERTY(QString fileSize READ fileSize NOTIFY fileSizeChanged)
    Q_PROPERTY(bool packageExists READ packageExists NOTIFY packageExistsChanged)

private:
    // for QML. File Infos.
    QString fileName() const{
        return DDpkg::debFileName;
    }
    QString fileSize() const{
        return DDpkg::debFileSizeMB;
    }
    bool packageExists(){
        return checkIfPackageExists();
    }

    // If you use an another package system, just change the these two.
    QString cmd_ins_beforeFileName = "pkexec dpkg --install ";
    QString cmd_ins_afterFileName = "";
    QString cmd_rem_beforeFileName = "pkexec dpkg --purge ";
    QString cmd_rem_afterFileName = "";

    // Get the name of the package in the repository.
    QString cmd_getPackageName = "dpkg --info {DEBFILE}";

    // Check if package exists command.
    QString cmd_checkIfPackageExists = "/bin/sh -c \"dpkg -l | grep 'ii  {PACKAGE}'\"";

signals:
    void fileNameChanged();
    void fileSizeChanged();
    void packageExistsChanged();
    void installFinished();
    void installError(int errorCode);
    void uninstallFinished();
    void uninstallError(int errorCode);

public:
    static QString debFileName;
    static QString debFilePath;
    static QString debFileSizeMB;

public slots:
    void install(){
        QProcess* process = new QProcess;
        process->setProcessChannelMode(QProcess::MergedChannels);

        process->connect(process, &QProcess::readyReadStandardOutput, [=](){
            QString processStdOut(process->readAllStandardOutput());
            qDebug() << "OUT: " << processStdOut;
        });
        process->connect(process, static_cast<void (QProcess::*)(int)>(&QProcess::finished), [=](int exitCode){
            if(exitCode == 0)
                emit installFinished();
            else
                emit installError(exitCode);

            process->deleteLater();
        });

        QString command = cmd_ins_beforeFileName + DDpkg::debFilePath + cmd_ins_afterFileName;
        process->start(command);
    }

    void uninstall(){
        QProcess* process = new QProcess;
        process->setProcessChannelMode(QProcess::MergedChannels);
        process->connect(process, &QProcess::readyReadStandardOutput, [=]{
            QString processStdOut = process->readAllStandardOutput();
            qDebug() << "OUT: " << processStdOut;
        });
        process->connect(process, static_cast<void (QProcess::*)(int)>(&QProcess::finished), [=](int exitCode){
            if(exitCode == 0)
                emit uninstallFinished();
            else
                emit uninstallError(exitCode);
            process->deleteLater();
        });

        QString command = cmd_rem_beforeFileName + getPackageName() + cmd_rem_afterFileName;
        process->start(command);
    }

    QString getPackageName(){
        QProcess* process = new QProcess();
        QString packageName = "";
        process->connect(process, &QProcess::readyReadStandardError, [&]{
            QString processStdErr = (process->readAllStandardError());
            qDebug() << "ERR PKG NAME: " << processStdErr;
        });
        process->connect(process, &QProcess::readyReadStandardOutput, [&]{
            QString processStdOut = (process->readAllStandardOutput());
            QList<QString> lines = processStdOut.split("\n ");
            foreach (QString value, lines) {
                if(value.split(": ")[0] == "Package"){
                    packageName = value.split(": ")[1];
                    break;
                }
            }
        });

        QString command = cmd_getPackageName.replace(QString("{DEBFILE}"), DDpkg::debFilePath);
        process->start(command);
        process->waitForFinished();

        delete process;
        return packageName;
    }

    bool checkIfPackageExists(){
        QProcess* process = new QProcess();
        QString command = cmd_checkIfPackageExists.replace(QString("{PACKAGE}"), getPackageName());
        process->start(command);
        process->waitForFinished();

        QString processStdOut(process->readAllStandardOutput());
        if(processStdOut.length() > 0){
            return true;
        }
        return false;
    }
};

#endif // DDPKG_H
