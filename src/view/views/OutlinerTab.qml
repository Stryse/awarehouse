import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: root

    ColumnLayout {
        id: columnLayout

        anchors.fill: parent

        ActorOutliner {
            id: actorOutliner

            Layout.fillWidth:       true
            Layout.preferredHeight: parent.height * 0.5
        }

        TaskOutliner {
            id: taskOutliner

            Layout.fillWidth:  true
            Layout.fillHeight: true
        }
    }
}
