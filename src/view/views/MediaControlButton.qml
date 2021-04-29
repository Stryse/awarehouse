import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

Button {
    id: root

    focusPolicy: Qt.NoFocus

    flat: true
    Material.foreground: hovered ? Material.accent : "white"
    Material.background: Material.primary

//    icon.height: 36
//    icon.width:  36
}
