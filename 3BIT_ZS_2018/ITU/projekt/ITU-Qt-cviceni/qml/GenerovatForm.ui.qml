import QtQuick 2.4
import QtQuick.Controls 2.3

Item {
    id: item1
    width: 63
    height: 30

    Rectangle {
        id: rectangle2
        color: "#e1eeff"
        anchors.fill: parent
    }

    Rectangle {
        id: rectangle
        width: 30
        color: "#2d4a6b"
        anchors.left: parent.left
        anchors.leftMargin: 1
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 1
        anchors.top: parent.top
        anchors.topMargin: 1

        Image {
            id: image
            width: 26
            anchors.fill: parent
            source: "../img/minus.png"
        }
    }

    Rectangle {
        id: rectangle1
        x: 50
        y: 0
        width: 30
        color: "#2d4a6b"
        anchors.right: parent.right
        anchors.rightMargin: 1
        anchors.bottomMargin: 1
        anchors.topMargin: 1
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        Image {
            id: image1
            width: 48
            anchors.fill: parent
            source: "../img/plus.png"
        }
    }
}


/*##^## Designer {
    D{i:5;anchors_height:200;anchors_width:200;anchors_x:"-34";anchors_y:"-24"}D{i:6;anchors_height:100;anchors_width:100;anchors_x:"-20";anchors_y:"-16"}
D{i:3;anchors_height:200;anchors_width:200;anchors_x:8;anchors_y:"-7"}D{i:7;anchors_height:100;anchors_width:100;anchors_x:"-6";anchors_y:"-22"}
D{i:4;anchors_height:200;anchors_width:200;anchors_x:8;anchors_y:"-7"}
}
 ##^##*/
