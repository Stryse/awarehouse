import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import ActorList 1.0

Item {
    id: root
    //TODO

    Label {
        id: timeLabel

        anchors.centerIn: parent

        text: "Total time spent: " + SimPresenter.time

        font.pixelSize: root.height * 0.05
    }
}
