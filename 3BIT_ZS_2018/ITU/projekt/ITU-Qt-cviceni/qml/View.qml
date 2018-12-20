import QtQuick 2.4
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.0
import Qt.labs.calendar 1.0

Item {
    id: item1
    width: 120
    height: 116
    property alias rectangleColor: rectangle.color
    property bool isDisableCrop: false
    property string colorActive: '#2D4A6B'
    property string colorNonActive: '#27405d'
    property bool disable: false

    Rectangle {
        id: rectangle
        color: "#27405d"
        anchors.fill: parent

        Image {
            id: image
            x: 44
            y: 25
            width: 40
            height: 25
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            fillMode: Image.Stretch
            source: "../img/view.png"
        }

        Text {
            id: text1
            x: 54
            y: 73
            width: 59
            height: 22
            color: "#e1eeff"
            text: qsTr("Náhled")
            anchors.horizontalCenterOffset: 1
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 14
        }

        /*ToolTip{
            id: toolTip
             text: "Náhled dokumentu"
            // target: view
             visible: false
        }*/

        MouseArea {
            id: mouseArea1
            hoverEnabled: true
            anchors.fill: parent
            cursorShape: "PointingHandCursor"

            onExited: {
                if (! disable){
                    rectangleColor = colorNonActive

                } else {
                    cursorShape: "ForbiddenCursor"
                }
                //toolTip.visible = false;
            }
            onEntered: {
                rectangleColor = colorActive
                cursorShape: "ArrowCursor"
                //toolTip.visible = true;
            }

        }

    }
}
