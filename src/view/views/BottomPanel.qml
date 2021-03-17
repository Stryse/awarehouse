import QtQuick                   2.15
import QtQuick.Controls          2.15
import QtQuick.Controls.Material 2.15

Item {
    id: root

    property real maxHeight

    implicitHeight:          maxHeight
    SplitView.maximumHeight: maxHeight

    Pane {
        anchors.fill: parent

        Label {
            text: "View 2"
            anchors.centerIn: parent
        }
    }
}
