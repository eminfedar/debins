#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QFileInfo>
#include <QtMath>
#include "ddpkg.h"

QString DDpkg::debFileName = "";
QString DDpkg::debFilePath = "";
QString DDpkg::debFileSizeMB = "";

int main(int argc, char *argv[])
{
#ifndef QT_DEBUG // if it is not Debug.
    if(argc < 2)
        return 0; // No file.
#endif
    QCoreApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
    QGuiApplication app(argc, argv);

    // Get .deb file path from argument
    QString filePath = QString::fromUtf8(argv[1]);
#ifdef QT_DEBUG
    if(argc < 2)
        filePath = "/home/eminfedar/İndirilenler/bat_0.6.1_amd64.deb"; // TEST DEB FILE
#endif

    QFileInfo file(filePath);
    if(!(file.exists() && file.isFile()))
        return 0; // No file.

    // Send infos to QML
    DDpkg::debFileName = file.fileName();
    DDpkg::debFilePath = filePath;
    DDpkg::debFileSizeMB = QString::number((double)(floor(file.size() / 1000.0 / 1000.0 * 10.0) / 10.0));

    qmlRegisterType<DDpkg>("org.debins.ddpkg", 1, 0, "DDpkg");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));



    return app.exec();
}
