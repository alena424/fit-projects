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
        source: "../img/tools/Vector-1.png"
    }
}

/*##^## Designer {
    D{i:1;anchors_height:200;anchors_width:200;anchors_x:-39;anchors_y:-65}D{i:2;anchors_height:100;anchors_width:100;anchors_x:-19;anchors_y:-27}
}
 ##^##*/
