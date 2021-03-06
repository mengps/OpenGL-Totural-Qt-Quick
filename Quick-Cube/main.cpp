#include "openglItem.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
#ifdef Q_OS_WIN
    app.setAttribute(Qt::AA_UseDesktopOpenGL);
#else
    app.setAttribute(Qt::AA_UseOpenGLES);
#endif
    qmlRegisterType<OpenGLItem>("an.OpenGLItem", 1, 0, "OpenGLItem");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
