#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QIcon>
#include <QQmlContext>
#include "WarehouseManager.h"
#include "OutlinerListModel.h"
#include "presenter.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/app_icon.png"));

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/view/SimulationWindow.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    // Own variables
    WarehouseManager manager;
    Presenter presenter;
    qmlRegisterSingletonInstance<WarehouseManager>("manager",1,0,"WarehouseManager",&manager);
    qmlRegisterType<OutlinerListModel>("Outliner", 1, 0, "Outliner");
    qmlRegisterUncreatableType<Presenter>("Outliner", 1, 0, "Presenter", QStringLiteral("NO!"));

    engine.rootContext()->setContextProperty(QStringLiteral("presenter"), &presenter);

    return app.exec();
}
