#ifndef DDPKG_H
#define DDPKG_H

#include <QObject>
#include <QFile>

class DDpkg : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString debFileName MEMBER debFileName NOTIFY debFileNameChanged)
    Q_PROPERTY(QString debFileSizeMB MEMBER debFileSizeMB NOTIFY debFileSizeMBChanged)

public:
    explicit DDpkg(QObject *parent = 0, QFile debfile);

private:
    QString debFileName;
    QString debFileSizeMB;
    QFile debfile;


signals:

public slots:
};

#endif // DDPKG_H
