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
    Q_PROPERTY(QString packageName READ packageName NOTIFY packageNameChanged)
    Q_PROPERTY(QString packageVersion READ packageVersion NOTIFY packageVersionChanged)
    Q_PROPERTY(bool newVersion READ newVersion NOTIFY newVersionChanged)
    Q_PROPERTY(bool oldVersion READ oldVersion NOTIFY oldVersionChanged)
    Q_PROPERTY(QString packageCurrentVersion READ packageCurrentVersion NOTIFY packageCurrentVersionChanged)
    Q_PROPERTY(bool packageExists READ packageExists NOTIFY packageExistsChanged)
    Q_PROPERTY(QString output READ output NOTIFY outputChanged)

private:
    // for QML. File Infos.
    QString fileName() const{
        return DDpkg::debFileName;
    }
    QString fileSize() const{
        return DDpkg::debFileSizeMB;
    }
    bool packageExists(){
        return _packageExists;
    }
    QString packageName() {
        return _packageName;
    }
    QString packageVersion() {
        return _packageVersion;
    }
    QString packageCurrentVersion() const{
        return _packageCurrentVersion;
    }
    bool newVersion(){
        return _newVersion;
    }
    bool oldVersion(){
        return _oldVersion;
    }
    QString output() const{
        return _output;
    }

    bool _packageExists = false;
    QString _packageName = "";
    QString _packageVersion = "";
    QString _packageCurrentVersion = "";
    bool _newVersion = false;
    bool _oldVersion = false;
    QString _output = "";

    // If you use an another package system, just change the these two.
    QString cmd_install = "pkexec /bin/sh -c \"dpkg --install '{DEBFILE}';apt-get -y install -f\"";
    QString cmd_remove = "pkexec dpkg --purge {PACKAGE}";

    // Get the name of the package in the repository.
    QString cmd_getPackageName = "/bin/sh -c \"dpkg --info '{DEBFILE}' | grep ' Package:'\"";
    QString cmd_getPackageVersion = "/bin/sh -c \"dpkg --info '{DEBFILE}' | grep ' Version:'\"";
    QString cmd_getPackageCurrentVersion = "/bin/sh -c \"dpkg -s {PACKAGE} | grep 'Version:'\"";

    // Check if package exists command.
    QString cmd_checkIfPackageExists = "/bin/sh -c \"dpkg -l | grep 'ii  {PACKAGE}  '\"";
    QString cmd_checkIfNewVersion = "/bin/sh -c \"dpkg --compare-versions {VERSION} gt {CURRENTVERSION} && echo '1'\"";
    QString cmd_checkIfOldVersion = "/bin/sh -c \"dpkg --compare-versions {VERSION} lt {CURRENTVERSION} && echo '1'\"";

signals:
    void fileNameChanged();
    void fileSizeChanged();

    void packageExistsChanged();
    void packageNameChanged();
    void packageVersionChanged();
    void packageCurrentVersionChanged();
    void newVersionChanged();
    void oldVersionChanged();
    void outputChanged();

    void installFinished();
    void installError(int errorCode);
    void uninstallFinished();
    void uninstallError(int errorCode);

public:
    static QString debFileName;
    static QString debFilePath;
    static QString debFileSizeMB;

    DDpkg():QObject(){
        cmd_install = cmd_install.replace(QString("{DEBFILE}"), DDpkg::debFilePath);
        cmd_getPackageName = cmd_getPackageName.replace(QString("{DEBFILE}"), DDpkg::debFilePath);
        cmd_getPackageVersion = cmd_getPackageVersion.replace(QString("{DEBFILE}"), DDpkg::debFilePath);

        getPackageName();
        getPackageVersion();

        cmd_remove = cmd_remove.replace(QString("{PACKAGE}"), _packageName);
        cmd_getPackageCurrentVersion = cmd_getPackageCurrentVersion.replace(QString("{PACKAGE}"), _packageName);
        cmd_checkIfPackageExists = cmd_checkIfPackageExists.replace(QString("{PACKAGE}"), _packageName);

        checkIfPackageExists();
        getPackageCurrentVersion();

        cmd_checkIfNewVersion = cmd_checkIfNewVersion.replace(QString("{VERSION}"), _packageVersion);
        cmd_checkIfNewVersion = cmd_checkIfNewVersion.replace(QString("{CURRENTVERSION}"), _packageCurrentVersion);
        cmd_checkIfOldVersion = cmd_checkIfOldVersion.replace(QString("{VERSION}"), _packageVersion);
        cmd_checkIfOldVersion = cmd_checkIfOldVersion.replace(QString("{CURRENTVERSION}"), _packageCurrentVersion);

        checkIfNewVersion();
        checkIfOldVersion();
    }

public slots:
    void install(){
        QProcess* process = new QProcess;
        process->setProcessChannelMode(QProcess::MergedChannels);

        process->connect(process, &QProcess::readyReadStandardOutput, [=](){
            QString processStdOut(process->readAllStandardOutput());
            qDebug() << "OUTi: " << processStdOut;
            _output = processStdOut;
            emit outputChanged();
        });
        process->connect(process, static_cast<void (QProcess::*)(int)>(&QProcess::finished), [=](int exitCode){
            if(exitCode == 0)
                emit installFinished();
            else
                emit installError(exitCode);

            process->deleteLater();
        });

        process->start(cmd_install);
    }

    void uninstall(){
        QProcess* process = new QProcess;
        process->setProcessChannelMode(QProcess::MergedChannels);
        process->connect(process, &QProcess::readyReadStandardOutput, [=]{
            QString processStdOut = process->readAllStandardOutput();
            qDebug() << "OUTu: " << processStdOut;
            _output = processStdOut;
            emit outputChanged();
        });
        process->connect(process, static_cast<void (QProcess::*)(int)>(&QProcess::finished), [=](int exitCode){
            if(exitCode == 0)
                emit uninstallFinished();
            else
                emit uninstallError(exitCode);
            process->deleteLater();
        });

        process->start(cmd_remove);
    }

    QString getPackageName(){
        QProcess* process = new QProcess();
        process->setProcessChannelMode(QProcess::MergedChannels);

        process->start(cmd_getPackageName);
        process->waitForFinished();

        QString processStdOut = (process->readAllStandardOutput());
        if(processStdOut.split(": ")[0] == " Package"){
            _packageName = (QString)(processStdOut.split(": ")[1]).replace('\n',"");
        }

        process->deleteLater();
        return _packageName;
    }

    QString getPackageVersion(){
        QProcess* process = new QProcess();
        process->setProcessChannelMode(QProcess::MergedChannels);

        process->start(cmd_getPackageVersion);
        process->waitForFinished();

        QString processStdOut = (process->readAllStandardOutput());
        if(processStdOut.split(": ")[0] == " Version"){
            _packageVersion = (QString)(processStdOut.split(": ")[1]).replace('\n',"");
        }

        process->deleteLater();
        return _packageVersion;
    }

    QString getPackageCurrentVersion(){
        QProcess* process = new QProcess();
        process->setProcessChannelMode(QProcess::MergedChannels);

        process->start(cmd_getPackageCurrentVersion);
        process->waitForFinished();

        QString processStdOut = (process->readAllStandardOutput());
        if(processStdOut.split(": ")[0] == "Version"){
            _packageCurrentVersion = (QString)(processStdOut.split(": ")[1]).replace('\n',"");
        }

        process->deleteLater();
        emit packageCurrentVersionChanged();
        return _packageCurrentVersion;
    }

    bool checkIfPackageExists(){
        QProcess* process = new QProcess();
        process->start(cmd_checkIfPackageExists);
        process->waitForFinished();

        QString processStdOut(process->readAllStandardOutput());

        if(processStdOut.length() > 0){
            _packageExists = true;
            process->deleteLater();
            return true;
        }
        _packageExists = false;
        process->deleteLater();
        return false;
    }

    bool checkIfNewVersion(){
        QProcess* process = new QProcess();
        process->start(cmd_checkIfNewVersion);
        process->waitForFinished();

        QString processStdOut(process->readAllStandardOutput());

        if(processStdOut.length() > 0)
            _newVersion = true;
        else
            _newVersion = false;

        process->deleteLater();
        return _newVersion;
    }

    bool checkIfOldVersion(){
        QProcess* process = new QProcess();
        process->start(cmd_checkIfOldVersion);
        process->waitForFinished();

        QString processStdOut(process->readAllStandardOutput());

        if(processStdOut.length() > 0)
            _oldVersion = true;
        else
            _oldVersion = false;

        process->deleteLater();
        return _oldVersion;
    }

    QString getErrorMessage(int errorCode){
        switch (errorCode) {
        case 1:
            return QObject::tr("We have some issues while installing the package.\nPlease run from terminal to see the errors.\n\nCommand: debins file.deb");
        case 2:
            return QObject::tr("We couldn't find the file.");
        case 100:
            return QObject::tr("A Program using the package manager (dpkg) now. Please stop it first.");
        case 126:
            return QObject::tr("Please enter your root password to make Debins access the dpkg.");
        default:
            return QString::number(errorCode);
        }
    }
};

#endif // DDPKG_H
