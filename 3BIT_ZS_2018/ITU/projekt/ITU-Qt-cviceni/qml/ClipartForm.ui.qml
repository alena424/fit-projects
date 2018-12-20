import QtQuick 2.4
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.0
import Qt.labs.calendar 1.0
import QtQuick.Layouts 1.0

Item {
    id: item1
    width: 120
    height: 116
    property alias mouseArea1: mouseArea1
    property alias text1Color: text1.color
    property alias rectangleColor: rectangle3.color
    property alias rectangle1: rectangle1

    Rectangle {
        id: rectangle3
        color: "#27405d"
        anchors.fill: parent

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
        Text {
            id: text1
            x: 54
            y: 70
            width: 59
            height: 29
            color: "#e1eeff"
            text: qsTr("Kliparty")
            anchors.horizontalCenterOffset: 1
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 14
        }

        GridLayout {
            x: 12
            y: 19
            anchors.horizontalCenterOffset: 1
            anchors.horizontalCenter: parent.horizontalCenter
            rows: 2
            columns: 2

            Image {
                id: image
                Layout.preferredHeight: 20
                Layout.preferredWidth: 20
                source: "../img/vlozit/image.png"
            }

            Image {
                id: image1
                Layout.preferredHeight: 20
                Layout.preferredWidth: 20
                source: "../img/vlozit/clip2.png"
            }

            Image {
                id: image2
                Layout.preferredHeight: 20
                Layout.preferredWidth: 20
                source: "../img/vlozit/clip3.png"
            }

            Image {
                id: image3
                Layout.preferredHeight: 20
                Layout.preferredWidth: 20
                source: "../img/vlozit/image.png"
            }
        }
        //cursorShape: "PointingHandCursor"
    }
}

