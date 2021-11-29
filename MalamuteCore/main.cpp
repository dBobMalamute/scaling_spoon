#include <QApplication>
#include <QQmlApplicationEngine>

#include "SurfaceAndStyleSetup.h"
#include "QMLRegisterHelper.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setOrganizationName("MalamuteComputing");
    app.setOrganizationDomain("Null.com");
    app.setApplicationName("Malamute");

    registerQML(&app);
    setupSurfaceAndStyle();

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/QML/Malamute.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl)
    {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    },
    Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
