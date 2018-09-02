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

private:
    // for QML. File Infos.
    QString fileName() const{
        return DDpkg::debFileName;
    }
    QString fileSize() const{
        return DDpkg::debFileSizeMB;
    }

    // If you use an another package system, just change the these two.
    QString cmd_ins_beforeFileName = "pkexec dpkg -i";
    QString cmd_ins_afterFileName = "";
    QString cmd_rem_beforeFileName = "pkexec dpkg -p";
    QString cmd_rem_afterFileName = "";

signals:
    void fileNameChanged();
    void fileSizeChanged();

public:
    static QString debFileName;
    static QString debFilePath;
    static QString debFileSizeMB;

public slots:
    void install() const{
        QByteArray processStdErr, processStdOut;
        QProcess* process = new QProcess();
        process->connect(process, &QProcess::readyReadStandardError, [&]{
            processStdErr = process->readAllStandardError();
            qDebug() << processStdErr;
        });
        process->connect(process, &QProcess::readyReadStandardOutput, [&]{
            processStdOut = process->readAllStandardOutput();
            qDebug() << processStdOut;
        });

        QString command = cmd_ins_beforeFileName + " " + DDpkg::debFilePath + " " + cmd_ins_afterFileName;
        process->start(command);
        //process->waitForFinished();
    }

    void uninstall() const{
        QByteArray processStdErr, processStdOut;
        QProcess* process = new QProcess();
        process->connect(process, &QProcess::readyReadStandardError, [&]{
            processStdErr = process->readAllStandardError();
            qDebug() << "ERR: " << processStdErr;
        });
        process->connect(process, &QProcess::readyReadStandardOutput, [&]{
            processStdOut = process->readAllStandardOutput();
            qDebug() << "OUT: " << processStdOut;
        });

        QString command = cmd_rem_beforeFileName + " " + DDpkg::debFilePath + " " + cmd_rem_afterFileName;
        process->start(command);
        //process->waitForFinished();
    }
};

#endif // DDPKG_H
