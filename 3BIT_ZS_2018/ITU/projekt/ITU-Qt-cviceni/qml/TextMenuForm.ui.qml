import QtQuick 2.4
import QtQuick.Controls 2.1

Item {
    width: 483
    height: 120

    Rectangle {
        id: rectangle
        color: "#27405d"
        anchors.fill: parent

        ComboBox {
            id: fontText
            x: 20
            y: 19
            width: 173
            height: 29
            displayText: ""
            enabled: true
            flat: false
            editable: true
            currentIndex: -1
            textRole: ""
        }

        ComboBox {
            id: fontSize
            x: 208
            y: 19
            width: 103
            height: 29
            editable: true
        }

        Bold {
            id: bold
            x: 20
            y: 72
        }

        Italic {
            id: italic
            x: 62
            y: 72
        }

        Underline {
            id: underline
            x: 106
            y: 72
        }

        ToolSeparator {
            id: toolSeparator
            x: 324
            y: 8
            width: 13
            height: 104
        }

        Image {
            id: image
            x: 343
            y: 19
            width: 25
            height: 25
            source: "../img/tools/Vector-8.png"
        }

        Image {
            id: image1
            x: 389
            y: 19
            width: 25
            height: 25
            source: "../img/tools/Vector-9.png"
        }

        Image {
            id: image2
            x: 435
            y: 19
            width: 25
            height: 25
            source: "../img/tools/Vector-10.png"
        }

        Image {
            id: image3
            x: 343
            y: 69
            width: 25
            height: 25
            source: "../img/tools/Vector-1.png"
        }

        Image {
            id: image4
            x: 389
            y: 69
            width: 25
            height: 25
            source: "../img/tools/Vector.png"
        }

        Image {
            id: image5
            x: 435
            y: 69
            width: 25
            height: 25
            source: "../img/tools/Vector-2.png"
        }

        FillColor {
            id: fillColor
            x: 231
            y: 65
        }

        TextFill {
            id: textFill
            x: 279
            y: 64
        }
    }
}


/*##^## Designer {
    D{i:1;anchors_height:200;anchors_width:200;anchors_x:118;anchors_y:23}
}
 ##^##*/
