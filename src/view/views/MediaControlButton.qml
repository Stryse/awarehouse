import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

Button {
    id: root

    flat: true
    Material.foreground: hovered ? Material.accent : "white"

    icon.height: 36
    icon.width:  36

    /*
    //Text -> Fix BUG first !!!
    display: AbstractButton.TextUnderIcon
    font.capitalization: Font.MixedCase
    */
}
