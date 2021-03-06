import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id: root

    property alias rowCount:    rowsSpinBox.value
    property alias columnCount: columnsSpinBox.value

    property alias titleBarHeight: titleBar.height
    property alias borderWidth:    border.width

    implicitHeight: pane.implicitHeight
    implicitWidth:  pane.implicitWidth

    Pane {
        id: pane

        anchors {
            left: parent.left; right:  border.left
            top:  parent.top;  bottom: parent.bottom
        }
        padding:        0
        bottomPadding:  10

        implicitHeight: titleBar.implicitHeight + settingsScrollView.implicitHeight

        clip:   true

        Rectangle {
            id: titleBar

            anchors {
                left: parent.left; right: parent.right
            }

            color:  Material.primary

            Label {
                id: title

                anchors {
                    left: parent.left
                    verticalCenter: parent.verticalCenter
                }
                leftPadding:    10

                text:           qsTr("Settings")
                font.pixelSize: titleBar.height * 0.5
            }
        }

        ScrollView {
            id: settingsScrollView

            anchors {
                left: parent.left;     right:  parent.right
                top:  titleBar.bottom; bottom: parent.bottom
            }
            padding:    0
            topPadding: 5

            horizontalPadding: width * 0.1

            clip: true

            ScrollBar.horizontal.policy:    ScrollBar.AlwaysOff
            ScrollBar.vertical.interactive: false

            GridLayout {
                id: gridLayout

                readonly property real settingPixelSize: editorRoot.height * 0.021

                width: settingsScrollView.width * 0.8

                columns: 2

                Label {
                    id: rowsLabel

                    padding: 0

                    text:           qsTr("Rows")
                    font.pixelSize: gridLayout.settingPixelSize
                }
                SpinBox {
                    id: rowsSpinBox

                    focusPolicy: Qt.NoFocus

                    Layout.alignment: Qt.AlignRight

                    from:  3
                    to:    100
                    value: EditorPresenter.layout.rows

                    editable: true

                    Binding {
                        target: EditorPresenter.layout
                        property: "rows"
                        value: rowsSpinBox.value
                    }

                }
                Label {
                    id: columnsLabel

                    padding: 0

                    text:           qsTr("Columns")
                    font.pixelSize: gridLayout.settingPixelSize
                }
                SpinBox {
                    id: columnsSpinBox

                    focusPolicy: Qt.NoFocus

                    Layout.alignment: Qt.AlignRight

                    from:  3
                    to:    100
                    value: EditorPresenter.layout.columns

                    editable: true

                    Binding {
                        target: EditorPresenter.layout
                        property: "columns"
                        value: columnsSpinBox.value
                    }
                }
                Button {
                    id: clearButton

                    focusPolicy: Qt.NoFocus

                    flat:                true
                    Material.background: Material.primary

                    text:                qsTr("Clear")
                    font.pixelSize:      gridLayout.settingPixelSize
                    font.capitalization: Font.MixedCase;

                    onClicked: {
                        warehouse.selectedPodList.length = 0
                        EditorPresenter.clearWarehouse()
                    }
                }
            }
        }
    }

    Rectangle {
        id: border

        anchors {
            right: parent.right
            top:   parent.top; bottom: parent.bottom
        }

        color: Material.primary
    }
}
