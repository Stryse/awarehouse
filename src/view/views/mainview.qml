import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.12

CustomTitleBarWindow {
    id: root

    property string windowTitle:    qsTr("aWarehouse Manager")
    property color  secondaryColor: "#292929"

    visible: true

    title:        windowTitle
    titleText:    windowTitle
    titleVisible: true

    x: Screen.width/2 - width/2
    y: Screen.height/2 - height/2

    width:  1280
    height: 720
    minimumWidth:  1280
    minimumHeight: 720

    SplitView {
        id: content

        anchors.fill: parent

        orientation:  Qt.Horizontal

        //Left panel
        Item {
            id: leftPanel

            property real maxWidth:   root.width * 0.3
            property bool isFullSize: leftPanel.width >= leftPanel.maxWidth * 0.7

            implicitWidth:          maxWidth
            SplitView.maximumWidth: maxWidth

            //Tabs
            TabBar {
                id: leftPanelTabBar

                anchors {
                    left: parent.left; right: parent.right
                    top:  parent.top
                }
                contentHeight: parent.height * 0.06

                Material.background: secondaryColor

                TabButton { text: leftPanel.isFullSize  ?  qsTr("Outliner")   : qsTr("Out")  }
                TabButton { text: leftPanel.isFullSize  ?  qsTr("Statistics") : qsTr("Stat") }
                TabButton { text: leftPanel.isFullSize  ?  qsTr("Orders")     : qsTr("Ord")  }
                TabButton { text: leftPanel.isFullSize  ?  qsTr("Settings")   : qsTr("Set")  }
            }

            Pane {
                id: leftPanelContentPane

                property real paddingWidth: 10

                anchors {
                    left: parent.left;            right:  parent.right
                    top:  leftPanelTabBar.bottom; bottom: parent.bottom
                }

                Material.elevation: 10

                padding:       0
                topPadding:    paddingWidth
                leftPadding:   paddingWidth
                bottomPadding: paddingWidth

                StackLayout {
                    id: leftPanelStack

                    anchors.fill: parent

                    currentIndex: leftPanelTabBar.currentIndex

                    //Outliner
                    Item {
                        id: outliner

                        //BUG: Items clip over parent
                        ScrollView {
                            anchors.fill: parent

                            padding: 0

                            ScrollBar.horizontal.policy:    ScrollBar.AlwaysOff
                            ScrollBar.vertical.interactive: false

                            ListView {
                                //PLACEHOLDER
                                model: 20
                                delegate: ItemDelegate {
                                    text: "Item " + index
                                }
                            }
                        }
                    }
                    //Statistics
                    Item {
                        id: statistics
                        //TODO
                    }
                    //Orders
                    Item {
                        id: orders
                        //TODO
                    }
                    //Settings
                    Item {
                        id: settings
                        //TODO
                    }
                }
            }

        }
        //PLACEHOLDER
        Rectangle {
            id: centerItem

            SplitView.minimumWidth: 50
            SplitView.fillWidth:    true

            color: "lightgray"

            Label {
                text: "Map"
                anchors.centerIn: parent
            }
        }
    }
}
