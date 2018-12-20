import QtQuick 2.4

import QtGraphicalEffects 1.0
import Qt.labs.calendar 1.0

Item {
    id: item1
    width: 120
    height: 116
    property alias mouseArea: mouseArea
    property alias text1Color: text1.color
    property alias rectangleColor: rectangle.color

    Rectangle {
        id: rectangle
        color: "#27405d"
        anchors.fill: parent

        Image {
            id: image
            x: 44
            y: 33
            width: 31
            height: 18
            anchors.horizontalCenter: parent.horizontalCenter
            fillMode: Image.Stretch
            source: "../img/redo.png"
        }

        Text {
            id: text1
            x: 54
            y: 73
            color: "#5b7596"
            text: qsTr("Dopředu")
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 14
        }
        MouseArea {
            id: mouseArea
            hoverEnabled: true
            anchors.fill: parent
            //cursorShape: "PointingHandCursor"
        }
    }
}
