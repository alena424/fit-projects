import QtQuick 2.4
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.0
import Qt.labs.calendar 1.0

Item {
    id: item1
    width: 120
    height: 116

    property alias rectangleColor: rectangle.color

    //property alias text1StyleColor: text1.styleColor
    //property alias imageSource: image.source
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
            source: "../img/Vector.png"
        }

        Text {
            id: text1
            x: 54
            y: 73
            color: "#e1eeff"
            text: qsTr("Zpět")
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 14
        }

        MouseArea {
            id: mouseArea1
            hoverEnabled: true
            anchors.fill: parent
        }
    }
}
