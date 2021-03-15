import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

CustomTitleBarWindow {
    title: qsTr("aWarehouse Manager")
    visible: true

    titleVisible: true
    titleText: "aWareHouse"

    x: Screen.width/2 - width/2
    y: Screen.height/2 - height/2

    width: 1280
    height: 720
    minimumWidth: 1280
    minimumHeight: 720
}
