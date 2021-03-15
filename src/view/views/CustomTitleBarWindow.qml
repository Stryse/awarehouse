import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.12

//App window
ApplicationWindow {
    id: root

    property alias titleVisible: titleLabel.visible
    property alias titleText: titleLabel.text

    property color titleBarColor: "#212121"

    property bool isDraggable: true
    property bool isFullScreen: false

    flags: Qt.FramelessWindowHint  |
           Qt.Window

    //Titlebar
    menuBar: Rectangle {
        id: titleBar

        height: Math.max(titleLabel.height, buttonLayout.buttonIconHeight) + 10
        color: titleBarColor
        clip: false

        //Title
        Label {
            id: titleLabel

            visible: false
            anchors.centerIn: parent

            font.pixelSize: 14
        }

        //Buttons
        RowLayout {
            id: buttonLayout

            property real buttonPadding: 18
            property real buttonIconHeight: 21

            anchors {
                right: parent.right
                top: parent.top; bottom: parent.bottom
            }

            spacing: 0

            //Minimize
            ToolButton {
                id: minimizeButton

                icon.height: parent.buttonIconHeight
                Layout.preferredWidth: icon.width + 2*parent.buttonPadding
                Layout.preferredHeight: parent.height

                flat: true
                icon.source: "qrc:/minimize_white.png"

                onClicked: showMinimized()
            }
            //Fullscreen
            ToolButton {
                id: fullScreenButton

                icon.height: parent.buttonIconHeight
                Layout.preferredWidth: icon.width + 2*parent.buttonPadding
                Layout.preferredHeight: parent.height

                flat: true
                icon.source: isFullScreen ? "qrc:/fullscreen_exit_white.png" : "qrc:/fullscreen_white.png"

                onClicked: {
                    if (isFullScreen) { showNormal()     }
                    else              { showFullScreen() }

                    isFullScreen = !isFullScreen
                }
            }
            //Close
            ToolButton {
                id: closeButton

                icon.height: parent.buttonIconHeight
                Layout.preferredWidth: icon.width + 2*parent.buttonPadding
                Layout.preferredHeight: parent.height

                flat: true
                icon.source: "qrc:/close_white.png"

                Rectangle{
                    id: redShadeRect
                    anchors.fill: parent
                    color: Material.color(Material.Red, Material.Shade900)
                    opacity: 0
                }
                onHoveredChanged: hovered ? redShadeRect.opacity = .5 : redShadeRect.opacity = 0;

                onClicked: Qt.quit()
            }
        }

        //Drag area
        MouseArea {
            id: titleBarDragArea

            property real lastMouseX: 0
            property real lastMouseY: 0

            anchors {
                left: parent.left; right: buttonLayout.left
                top: parent.top; bottom: parent.bottom
            }

            onPressed: { lastMouseX = mouseX
                         lastMouseY = mouseY }
            onMouseXChanged: if(isDraggable) { root.x += (mouseX - lastMouseX) }
            onMouseYChanged: if(isDraggable) { root.y += (mouseY - lastMouseY) }
        }
    }

    //Resize area
    MouseArea {
        id: resizeArea

        anchors {
            right: parent.right
            bottom: parent.bottom
        }

        width: 25
        height: 25

        cursorShape: Qt.SizeFDiagCursor

        DragHandler {
            target: null
            onActiveChanged: if (active) { root.startSystemResize(Qt.RightEdge | Qt.BottomEdge) }
        }
    }
}
