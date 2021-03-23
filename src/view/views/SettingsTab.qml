import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

ScrollView {
    id: root

    padding: 5

    ScrollBar.horizontal.policy:    ScrollBar.AlwaysOff
    ScrollBar.vertical.interactive: false

    //?????????????????
    Rectangle {
        anchors.fill: gridLayout
        color: "transparent"
    }

    GridLayout {
        id: gridLayout

        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
        }
        width:  parent.width * 0.8

        columns: 2

        Label {
            id: batteryLabel

            padding: 0

            text: qsTr("Battery")
            font.pixelSize: simulationRoot.height * 0.021
        }

        Slider {
            id: batterySlider

            Layout.alignment: Qt.AlignRight

            from:  1
            to:    100
            value: to

            stepSize: 1
            snapMode: RangeSlider.SnapOnRelease
        }


        Label {
            id: wareHouseLabel

            padding: 0

            text: qsTr("Warehouse")
            font.pixelSize: simulationRoot.height * 0.021
        }

        ComboBox {
            id: wareHouseComboBox

            model: ["Basic", "Secondary"]

            Layout.alignment: Qt.AlignRight

            Material.background: Material.primary

            flat: true
        }

        Button {
            id: wareHouseEditorOpen

            Layout.alignment:  Qt.AlignRight
            Layout.columnSpan: 2

            flat: true
            Material.background: Material.primary
            font.capitalization: Font.MixedCase;

            text: qsTr("Open Editor")

            onClicked: simulationRoot.editorOpened()
        }
    }
}
