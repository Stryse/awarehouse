import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

import TaskList 1.0

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

                model: TaskListModel{
                    tasks: SimPresenter.layout.tasks
                }

                delegate: Label {
                    id: taskDelegate

                    width:  taskListView.width

                    text: "Row: " + model.podRow  + " Column: " + model.podColumn
                    font.pixelSize: root.height * 0.055
                }
            }
        }
    }
}
