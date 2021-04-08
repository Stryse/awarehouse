import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

GridLayout {
    id: root

    property real cellSize

    required property ListModel model
    required property Component delegate

    function addCells() {
        for (var i = 0; i < model.count; ++i) {
            var cell = model.get(i)

            delegate.createObject(placeholderRepeater.itemAt(cell.rowIdx*columns + cell.columnIdx),
                                 { source:   cell.imgSource,
                                   rotation: cell.rotation })
        }
    }

    Repeater {
        id: placeholderRepeater

        property int completedPlaceholders: 0

        model:    root.rows * root.columns
        delegate: Rectangle{
            id: placeholderCell

            Layout.preferredHeight: root.cellSize
            Layout.preferredWidth:  root.cellSize

            Layout.row:    index / root.rows
            Layout.column: index % root.columns

            color: "transparent"

            Component.onCompleted: {
                placeholderRepeater.completedPlaceholders += 1

                if (placeholderRepeater.completedPlaceholders === root.rows * root.columns)
                    root.addCells()
            }
        }
    }
}