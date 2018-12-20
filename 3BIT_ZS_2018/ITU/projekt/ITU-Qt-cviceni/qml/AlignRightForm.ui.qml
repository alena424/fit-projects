import QtQuick 2.4

Item {
    width: 27
    height: 29

    Rectangle {
        id: rectangle
        color: "#27405d"
        anchors.fill: parent
    }

    Image {
        id: image
        anchors.fill: parent
        source: "../img/tools/Vector.png"
    }
}

