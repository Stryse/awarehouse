import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id: root

    property real maxHeight

    implicitHeight:          maxHeight
    SplitView.maximumHeight: maxHeight

    SplitView {
        id: horizontalSplit

        readonly property real splitMinimum:        1/3
        readonly property real titleBarHeightRatio: 0.125

        anchors.fill: parent

        orientation:  Qt.Horizontal

        ActorInfoTab {
            id: actorInfoTab

            SplitView.minimumWidth: parent.width *    horizontalSplit.splitMinimum
            SplitView.maximumWidth: parent.width * (1-horizontalSplit.splitMinimum)
            //BUG: Resets when resizing left panel
            implicitWidth: parent.width * 0.5 - 3

            titleBarHeight: maxHeight  * horizontalSplit.titleBarHeightRatio
            borderWidth:    root.width * 0.004
        }

        LogTab {
            id: logTab

            SplitView.fillWidth: true

            titleBarHeight: maxHeight  * horizontalSplit.titleBarHeightRatio
            borderWidth:    root.width * 0.004
        }
    }
}
