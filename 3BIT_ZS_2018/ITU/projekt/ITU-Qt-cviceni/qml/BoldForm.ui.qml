import QtQuick 2.4
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.0
import Qt.labs.calendar 1.0

Item {
    id: item1
    width: 17
    height: 24
    property alias rectangle1: rectangle1
    //   property alias mouseArea: mouseArea
    property alias rectangle1Color: rectangle1.color

    Rectangle {
        id: rectangle1
        color: "#27405d"
        anchors.fill: parent
    }

    Image {
        id: image
        width: 11
        height: 16
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        source: "../img/tools/Vector-7.png"
    }


    /*MouseArea {
        id: mouseArea
        hoverEnabled: true
        anchors.fill: parent
        //cursorShape: "PointingHandCursor"
    }*/
}
