import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.12

CustomTitleBarWindow {
    id: root

    property string windowTitle: qsTr("aWarehouse Manager")

    title: windowTitle
    visible: true

    titleVisible: true
    titleText: windowTitle

    x: Screen.width/2 - width/2
    y: Screen.height/2 - height/2

    width: 1280
    height: 720
    minimumWidth: 1280
    minimumHeight: 720

    SplitView {
        id: content

        anchors.fill: parent

        orientation: Qt.Horizontal

        //leftPadding: 1 //?????

        Item {
            id: leftPanel

            implicitWidth: root.width * 0.3
            SplitView.maximumWidth: root.width * 0.3

            //Tabs
            TabBar {
                id: leftPanelTabBar

                anchors {
                    left:parent.left; right: parent.right
                    top: parent.top
                }

                contentHeight: parent.height * 0.06
                contentWidth: parent.width / 4

                TabButton { text: qsTr("Outliner") }
                TabButton { text: qsTr("Statistics") }
                TabButton { text: qsTr("Orders") }
                TabButton { text: qsTr("Settings") }
            }

            StackLayout {
                id: leftPanelStack

                anchors {
                    left:parent.left; right: parent.right
                    top: leftPanelTabBar.bottom; bottom: parent.bottom
                }

                currentIndex: leftPanelTabBar.currentIndex

                Item {
                    id: outliner

                }
                Item {
                    id: statistics

                }
                Item {
                    id: orders

                }
                Item {
                    id: settings

                }
            }
        }
        Rectangle {
            id: centerItem
            SplitView.minimumWidth: 50
            SplitView.fillWidth: true
            color: "lightgray"
            Label {
                text: "View 2"
                anchors.centerIn: parent
            }
        }
    }
}
