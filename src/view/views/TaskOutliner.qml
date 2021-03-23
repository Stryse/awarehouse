import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

import Outliner 1.0

Item {
    id: root

    Pane {
        id: pane

        anchors.fill:   parent
        padding:        0
        bottomPadding:  10

        Rectangle {
            id: titleBar

            anchors {
                top:  parent.top
                left: parent.left; right: parent.right
            }

            height: parent.height * 0.12

            clip:   true

            color:  Material.primary

            Label {
                id: title

                anchors {
                    left: parent.left
                    verticalCenter: parent.verticalCenter
                }
                leftPadding:    10

                text:           qsTr("Tasks")
                font.pixelSize: titleBar.height * 0.4
            }
        }

        ScrollView {
            id: taskScrollView

            anchors {
                left: parent.left;     right:  parent.right
                top:  titleBar.bottom; bottom: parent.bottom

                leftMargin: 5
                topMargin:  5
            }

            ScrollBar.horizontal.policy:    ScrollBar.AlwaysOff
            ScrollBar.vertical.interactive: false

            ListView {
                id: taskListView

                anchors.fill: parent
                anchors {
                    leftMargin: 5;  rightMargin:  20
                    topMargin:  10; bottomMargin: 10
                }

                clip: true

                model: OrderOutlinerModel{
                    orders: simpresenter.orders
                }

                delegate: GridLayout {
                    id: taskDelegate

                    width:  taskListView.width
                    height: root.height * 0.15

                    columns:       2
                    columnSpacing: 0
                    rowSpacing:    0

                    Label {
                        id: taskName

                        Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
                        Layout.row:       0
                        Layout.column:    0

                        text:           model.ordername
                        font.pixelSize: taskDelegate.height * 0.3
                    }

                    Label {
                        id: orders

                        Layout.alignment: Qt.AlignRight | Qt.AlignBottom
                        Layout.row:       0
                        Layout.column:    1

                        text: model.ordernumber
                    }

                    Label {
                        id: position

                        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                        Layout.row:       1
                        Layout.column:    0

                        text: model.position
                    }

                    Label {
                        id: assignedActor

                        Layout.alignment: Qt.AlignRight | Qt.AlignTop
                        Layout.row:       1
                        Layout.column:    1

                        text: model.robotname
                    }
                }
            }
        }
    }
}
