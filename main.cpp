#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QFileInfo>
#include <QtMath>
#include <QIcon>
#include <QTranslator>
#include "ddpkg.h"

QString DDpkg::debFileName = "";
QString DDpkg::debFilePath = "";
QString DDpkg::debFileSizeMB = "";

bool ddpkgConfigs(QString filePath){
    QFileInfo file(filePath);
    if(!(file.exists() && file.isFile()))
        return 0; // No file.

    // Send infos to QML
    DDpkg::debFileName = file.fileName();
    DDpkg::debFilePath = file.absoluteFilePath();
    QString fileSize = "";
    if(file.size() / 1000.0 / 1000.0 < 1)
        fileSize = QString::number((double)(floor(file.size() / 1000.0 * 10.0) / 10.0)) + " KB";
    else if(file.size() / 1000.0 / 1000.0 / 1000.0 < 1)
        fileSize = QString::number((double)(floor(file.size() / 1000.0 / 1000.0 * 10.0) / 10.0)) + " MB";
    else
        fileSize = QString::number((double)(floor(file.size() / 1000.0 / 1000.0 / 1000.0 * 10.0) / 10.0)) + "GB";
    DDpkg::debFileSizeMB = fileSize;

    return 1;
}

int main(int argc, char *argv[])
{
#ifndef QT_DEBUG // if it is not Debug.
    if(argc < 2)
        return 0; // No file.
#endif
    QCoreApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/img/icon.png"));

    // Get .deb file path from argument
    QString filePath = QString::fromUtf8(argv[1]);
#ifdef QT_DEBUG
    if(argc < 2)
        filePath = "/home/eminfedar/İndirilenler/bat_0.6.1_amd64.deb"; // TEST DEB FILE
#endif

    if(!ddpkgConfigs(filePath)) return 0;

    // CHECK IF TRANSLATION AVAILABLE
    QTranslator translator;
    QString lang = QLocale::system().name().left(2);
    QString langStr = QLocale::system().languageToString(QLocale::system().language());

    if(lang != "en"){
        if(translator.load(":/translations/debins_" + lang)){
            app.installTranslator(&translator);
        }else{
            qDebug() << "There is no available translations for your language.";
            qDebug() << "You can translate this program to " << langStr << " VISIT ==> https://github.com/eminfedar/debins/";
        }
    }

    // Register DDpkg class for QML accessing.
    qmlRegisterType<DDpkg>("org.debins.ddpkg", 1, 0, "DDpkg");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();
}
