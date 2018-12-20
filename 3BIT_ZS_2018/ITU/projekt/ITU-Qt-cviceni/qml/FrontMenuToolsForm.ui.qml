import QtQuick 2.4
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.0
import QtGraphicalEffects 1.0

Item {
    id: frontMenuTools
    width: 1500
    height: 120
    property alias toolBar: toolBar

    //property alias rectangleColor: rectangle.color
    ToolBar {

        id: toolBar
        anchors.fill: parent

        ToolSeparator {
            id: toolSeparator1
            x: 735
            y: 12
            width: 13
            height: 100
        }

        ToolSeparator {
            id: toolSeparator
            x: 240
            y: 10
            width: 13
            height: 100
        }

        ToolButton {
            id: toolButton
            width: 120
            height: 160
            text: qsTr("Zpet")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            checked: false
            checkable: true

            BackNew {
                id: backNew
                anchors.fill: parent
            }
        }

        ToolButton {
            id: toolButton1
            x: 120
            width: 120
            height: 120
            text: qsTr("Tool Button")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0

            Forward {
                id: forward
                anchors.fill: parent
            }
        }

        ToolButton {
            id: rotateright
            x: 255
            y: 0
            width: 120
            text: qsTr("Tool Button")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0

            RotateRight {
                id: rotateRight
                anchors.fill: parent
            }
        }

        ToolButton {
            id: rotateleft
            x: 375
            width: 120
            text: qsTr("Tool Button")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0

            RotateLeft {
                id: rotateLeft
                anchors.fill: parent
            }
        }

        ToolButton {
            id: cropimage
            x: 495
            width: 120
            text: qsTr("Tool Button")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0

            Crop {
                id: crop
                anchors.fill: parent
            }
        }

        ToolButton {
            id: insertmap
            x: 615
            y: 0
            width: 120
            height: 120
            text: qsTr("Tool Button")

            Map {
                id: map
                anchors.fill: parent
            }
        }

        Rectangle {
            id: rectangle1
            x: 754
            width: 746
            color: "#27405d"
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0

            ComboBox {
                id: comboBox
                x: 22
                y: 14
                width: 174
                height: 29
            }

            ComboBox {
                id: comboBox1
                x: 217
                y: 14
                width: 98
                height: 29
            }

            Grid {
            }

            GridLayout {
                x: 356
                y: 19
                columnSpacing: 13
                rowSpacing: 24
                rows: 2
                columns: 3

                ToolButton {
                    id: toolButton2
                    text: qsTr("Tool Button")
                    Layout.preferredHeight: 29
                    Layout.preferredWidth: 27

                    Image {
                        id: image
                        anchors.fill: parent
                        source: "../img/tools/Vector.png"
                    }
                }

                ToolButton {
                    id: toolButton3
                    text: qsTr("Tool Button")
                    Layout.preferredHeight: 29
                    Layout.preferredWidth: 27

                    Image {
                        id: image1
                        x: -46
                        y: -6
                        anchors.fill: parent
                        source: "../img/tools/Vector.png"
                    }
                }

                ToolButton {
                    id: toolButton4
                    text: qsTr("Tool Button")
                    Layout.preferredHeight: 29
                    Layout.preferredWidth: 27

                    Image {
                        id: image2
                        x: -86
                        y: -6
                        anchors.fill: parent
                        source: "../img/tools/Vector-3.png"
                    }
                }

                ToolButton {
                    id: toolButton5
                    text: qsTr("Tool Button")
                    Layout.preferredHeight: 29
                    Layout.preferredWidth: 27

                    Image {
                        id: image3
                        x: 74
                        y: -59
                        anchors.fill: parent
                        source: "../img/tools/Vector-9.png"
                    }
                }

                ToolButton {
                    id: toolButton6
                    text: qsTr("Tool Button")
                    Layout.preferredHeight: 29
                    Layout.preferredWidth: 27

                    Image {
                        id: image5
                        x: -46
                        y: -6
                        anchors.fill: parent
                        source: "../img/tools/Vector-8.png"
                    }
                }

                ToolButton {
                    id: toolButton7
                    text: qsTr("Tool Button")
                    Layout.preferredHeight: 29
                    Layout.preferredWidth: 27

                    Image {
                        id: image4
                        x: -86
                        y: -6
                        anchors.fill: parent
                        source: "../img/tools/Vector-10.png"
                    }
                }
            }

            ToolButton {
                id: boldTool
                x: 25
                y: 72
                width: 17
                height: 24
                text: qsTr("")

                Bold {
                    id: bold
                    anchors.fill: parent
                }
            }

            ToolButton {
                id: italic_tool
                x: 73
                y: 72
                width: 17
                height: 24
                text: qsTr("Tool Button")

                Italic {
                    id: italic
                    x: 723
                    y: -78
                    anchors.fill: parent
                }
            }

            ToolButton {
                id: uderlinetool
                x: 120
                y: 72
                width: 17
                height: 24
                text: qsTr("")


                /*font.italic: true
                font.bold: true
                font.pixelSize: 8
                font.capitalization: Font.AllUppercase
                font.family: "Times New Roman"*/
                Underline {
                    id: underline
                    anchors.fill: parent
                }
            }

            ToolButton {
                id: filltool
                x: 220
                y: 64
                width: 32
                height: 32
                text: qsTr("")

                FillColor {
                    id: fillColor
                    anchors.fill: parent
                }
            }

            ToolButton {
                id: filltext
                x: 280
                y: 64
                width: 32
                height: 32
                text: qsTr("")

                TextFill {
                    id: textFill
                    anchors.fill: parent
                }
            }

            ToolButton {
                id: textChangetool
                x: 165
                y: 72
                width: 20
                height: 24
                text: qsTr("")

                TextChange {
                    id: textChange
                    anchors.fill: parent
                }
            }
        }

        ToolSeparator {
            id: toolSeparator2
            x: 1090
            y: 12
            width: 13
            height: 100
        }

        ToolSeparator {
            id: toolSeparator3
            x: 1235
            y: 12
            width: 13
            height: 100
        }
    }

    View {
        id: view
        x: 1253
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 2
        anchors.top: parent.top
        anchors.topMargin: 2
    }
}
