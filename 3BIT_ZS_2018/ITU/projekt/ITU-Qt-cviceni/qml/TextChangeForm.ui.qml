import QtQuick 2.4
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.0
import Qt.labs.calendar 1.0

Item {
    id: item1
    width: 18
    height: 24
    property alias rectangle1Color: rectangle1.color

    Rectangle {
        id: rectangle1
        color: "#27405d"
        anchors.fill: parent
    }

    Image {
        id: image
        width: 16
        height: 17
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        source: "../img/tools/text.png"
    }
}


/*##^## Designer {
    D{i:2;anchors_height:16;anchors_width:11}
}
 ##^##*/
