import QtQuick 2.4
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.0
import Qt.labs.calendar 1.0

Item {
    id: item1
    width: 120
    height: 116
    property alias rectangle1: rectangle1
    property alias mouseArea1: mouseArea1
    property alias text1Color: text1.color
    property alias rectangleColor: rectangle2.color

    Rectangle {
        id: rectangle2
        color: "#27405d"
        anchors.fill: parent

        Image {
            id: image
            x: 49
            y: 23
            width: 38
            height: 35
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            source: "../img/vlozit/smile.png"
        }
        Text {
            id: text1
            x: 54
            y: 64
            width: 59
            height: 35
            color: "#e1eeff"
            text: qsTr("Smajlíci")
            anchors.horizontalCenterOffset: 1
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 14
        }

        Rectangle {
            id: rectangle1
            x: 0
            y: 102
            width: 120
            height: 14
            color: "#11253b"

            Image {
                id: image4
                x: 33
                y: 3
                width: 15
                height: 5
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                source: "../img/vlozit/arrow.png"
            }

            MouseArea {
                id: mouseArea1
                anchors.fill: parent
            }
        }
        //cursorShape: "PointingHandCursor"
    }
}


/*##^## Designer {
    D{i:11;anchors_height:100;anchors_width:100;anchors_x:20;anchors_y:"-38"}
}
 ##^##*/
