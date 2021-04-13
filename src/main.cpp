#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QScopedPointer>

//Model
#include "Simulator.h"

//Presenter
#include "ActorOutlinerModel.h"
#include "TaskOutlinerModel.h"
#include "SimulationWindowPresenter.h"
#include "WarehouseManager.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/app_icon.png"));

    // Instantiate //////
    QScopedPointer<SimulationWindowPresenter> simPresenter(new SimulationWindowPresenter());
    // //////////////////

    // Register Types ///
    qmlRegisterType<ActorOutlinerModel>       ("Outliner",  1, 0, "ActorOutlinerModel");
    qmlRegisterType<TaskOutlinerModel>        ("Outliner",  1, 0, "TaskOutlinerModel");
    qmlRegisterType<SimulationWindowPresenter>("Simulator", 1, 0, "TickRate");

    // //////////////////
    QQmlApplicationEngine engine;
    // Register Instances
    engine.rootContext()->setContextProperty(QStringLiteral("SimPresenter"), simPresenter.get());
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
