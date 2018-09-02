#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>

int main(int argc, char *argv[])
{
#ifndef QT_DEBUG
    if(argc < 2)
        return 0; // No file.
#endif

    QCoreApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();
}
