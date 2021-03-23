import QtQuick
import QtQuick.Window
import QtQuick.Controls

CustomTitleBarWindow {
    id: mainRoot

    readonly property color  secondaryColor: "#2b2b2b"

    visible: true

    title:     stackView.currentItem.windowTitle
    titleText: stackView.currentItem.windowTitle

    x: Screen.width/2  - width/2
    y: Screen.height/2 - height/2

    width:  1280
    height: 720

    minimumWidth:  1280
    minimumHeight: 720

    StackView {
        id: stackView

        anchors.fill: parent

        initialItem: simulationComponent
    }

    Component {
        id: simulationComponent

        SimulationWindow {
            borderWidth:    mainRoot.borderWidth
            secondaryColor: mainRoot.secondaryColor

            onEditorOpened: stackView.replace(editorComponent)
        }
    }

    Component {
        id: editorComponent

        WarehouseEditorWindow {
            borderWidth:    mainRoot.borderWidth
            secondaryColor: mainRoot.secondaryColor

            onEditorClosed: stackView.replace(simulationComponent)
        }
    }
}
