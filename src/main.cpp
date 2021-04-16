#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QScopedPointer>

//Model
#include "Simulator.h"

//Presenter
#include "SimulationWindowPresenter.h"
#include "WarehouseEditorPresenter.h"
#include "WarehouseManager.h"
#include "ActorListModel.h"
#include "ChargingStationListModel.h"
#include "PodDockListModel.h"
#include "DeliveryStationListModel.h"
#include "TaskListModel.h"

#include "ActorList.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/app_icon.png"));

    // Instantiate //////
    QScopedPointer<SimulationWindowPresenter> simPresenter(new SimulationWindowPresenter());
    QScopedPointer<WarehouseEditorPresenter>  editorPresenter(new WarehouseEditorPresenter());
    // //////////////////

    // Register Types ///
    qmlRegisterType<ActorListModel>          ("ActorList",           1, 0, "ActorListModel");
    qmlRegisterType<ChargingStationListModel>("ChargingStationList", 1, 0, "ChargingStationListModel");
    qmlRegisterType<PodDockListModel>        ("PodDockList",         1, 0, "PodDockListModel");
    qmlRegisterType<DeliveryStationListModel>("DeliveryStationList", 1, 0, "DeliveryStationListModel");
    qmlRegisterType<TaskListModel>           ("TaskList",            1, 0, "TaskListModel");

    qmlRegisterUncreatableType<ActorList>          ("ActorList",           1, 0, "ActorList",           QStringLiteral("Unable to create ActorList!"));
    qmlRegisterUncreatableType<ChargingStationList>("ChargingStationList", 1, 0, "ChargingStationList", QStringLiteral("Unable to create ChargingStationList!"));
    qmlRegisterUncreatableType<PodDockList>        ("PodDockList",         1, 0, "PodDockList",         QStringLiteral("Unable to create PodDockList!"));
    qmlRegisterUncreatableType<DeliveryStationList>("DeliveryStationList", 1, 0, "DeliveryStationList", QStringLiteral("Unable to create DeliveryStationList!"));
    qmlRegisterUncreatableType<TaskList>           ("TaskList",            1, 0, "TaskList",            QStringLiteral("Unable to create TaskList!"));

    qmlRegisterType<SimulationWindowPresenter>("Simulator",           1, 0, "TickRate");
    qmlRegisterType<WarehouseEditorPresenter> ("Editor",              1, 0, "TileType");

    // //////////////////
    QQmlApplicationEngine engine;
    // Register Instances
    engine.rootContext()->setContextProperty(QStringLiteral("SimPresenter"),    simPresenter.get());
    engine.rootContext()->setContextProperty(QStringLiteral("EditorPresenter"), editorPresenter.get());
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
