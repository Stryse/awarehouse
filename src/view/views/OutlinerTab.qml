import QtQuick          2.15
import QtQuick.Controls 2.15

Item {
    id: root

    ScrollView {
        anchors.fill: parent

        padding: 0

        ScrollBar.horizontal.policy:    ScrollBar.AlwaysOff
        ScrollBar.vertical.interactive: false

        ListView {
            clip: true

            //PLACEHOLDER
            model: 20
            delegate: ItemDelegate {
                text: "Item " + index
            }
        }
    }
}
