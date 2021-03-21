#include "WarehouseManager.h"
#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QScopedPointer>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/app_icon.png"));

    QScopedPointer<WarehouseManager> manager(new WarehouseManager());
    QQmlApplicationEngine engine;
    qmlRegisterSingletonInstance<WarehouseManager>("WarehouseManager", 1, 0, "Manager", manager.get());

    const QUrl url(QStringLiteral("qrc:/view/SimulationWindow.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    // Own variables

    return app.exec();
}
