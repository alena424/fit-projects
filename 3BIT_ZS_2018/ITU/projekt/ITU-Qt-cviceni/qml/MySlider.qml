import QtQuick 2.0

// Základem každého prvku je čtverec
// Vlastně vytvořený posuvník
Rectangle {
    id: container
    width: 400
    height: 35
    color: "#aaaaaa"
    property alias rectColor: rect.color

    property double minValue: 0
    property double maxValue: 200
    property int value: rect.x/(container.width-rect.width) * (maxValue-minValue) + minValue

    Rectangle {
        id: rect
        width: 50; height: container.height
        color: "red"

        // Textový prvek pro zobrazení textu
        Text {
            anchors.centerIn: parent;
            text: container.value

        }

        // Oblast zachytávající události myši
        MouseArea {
            anchors.fill: parent
            drag.target: rect
            drag.axis: Drag.XAxis
            drag.minimumX: 0
            drag.maximumX: container.width - rect.width
        }
    }
}
