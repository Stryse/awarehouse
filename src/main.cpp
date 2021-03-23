#include "OutlinerModel.h"
#include "SimulationWindow_Presenter.h"
#include "WarehouseManager.h"
#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QScopedPointer>
#include "Simulator.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/app_icon.png"));

    // Instantiate //////
    QScopedPointer<WarehouseManager> manager(new WarehouseManager());
    QScopedPointer<SimulationWindowPresenter> simpresenter(new SimulationWindowPresenter(*manager));
    // //////////////////

    // Register Types ///
    qmlRegisterType<OutlinerModel>("Outliner", 1, 0, "OutlinerModel");
    qmlRegisterType<SimulationWindowPresenter>("Simulator", 1, 0, "TickRate");
    // //////////////////
    QQmlApplicationEngine engine;
    // Register Instances
    //qmlRegisterSingletonInstance<WarehouseManager>("WarehouseManager", 1, 0, "Manager", manager.get());
    engine.rootContext()->setContextProperty(QStringLiteral("simpresenter"), simpresenter.get());
    // //////////////////

    const QUrl url(QStringLiteral("qrc:/view/MainView.qml"));
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
