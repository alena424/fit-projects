import QtQuick 2.4
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.0

Item {
    width: 1500
    height: 120
    property alias mouseArea: mouseArea
    property alias image3: image3
    property alias image4: image4
    property alias rowLayout: rowLayout
    property alias image1: image1
    property alias image7: image7
    property alias flickable: flickable

    Rectangle {
        id: rectangle3
        color: "#27405d"
        anchors.rightMargin: -5
        anchors.fill: parent

        ToolBar {
            id: toolBar
            anchors.rightMargin: 175
            anchors.fill: parent

            background: Rectangle {
                border.color: "#27405D"
            }

            ToolButton {
                id: toolButton
                width: 120
                text: qsTr("")
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                anchors.top: parent.top
                anchors.topMargin: 0

                BackNew {
                    id: backNew
                    anchors.fill: parent
                }
            }

            ToolButton {
                id: toolButton1
                x: 120
                y: -4
                width: 120
                text: qsTr("")

                Forward {
                    id: forward
                    anchors.fill: parent
                }
                anchors.bottom: parent.bottom
                anchors.top: parent.top
                anchors.topMargin: 0
                anchors.bottomMargin: 0
            }

            Rectangle {
                id: rectangle
                x: 240
                y: 0
                width: 1000
                color: "#27405d"
                anchors.top: parent.top
                anchors.topMargin: 1
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0

                Rectangle {
                    id: rectangle1
                    y: 8
                    height: 103
                    color: "#ffffff"
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0

                    Flickable {
                        id: flickable
                        x: 0
                        y: -8
                        anchors.fill: parent
                        flickableDirection: Flickable.HorizontalFlick

                        contentWidth: parent.width
                        contentHeight: parent.height

                        focus: true
                        // nastavuje se orezavani toho, co je mimo zobrazovanou plochu
                        clip: true

                        // scrollbar ve vertikalnim smeru
                        ScrollBar.horizontal: ScrollBar {
                            id: hbar
                            active: hbar.active
                            policy: ScrollBar.AlwaysOn
                        }

                        RowLayout {
                            id: rowLayout
                            x: 7
                            y: 8
                            width: 1058
                            height: 86
                            MouseArea {
                                Layout.preferredHeight: 86
                                Layout.preferredWidth: 127
                                hoverEnabled: true

                                id: zimni
                                Image {
                                    id: image3
                                    width: 127
                                    height: 86
                                    source: "../img/mapa/zimni.png"
                                }
                            }

                            Image {
                                id: image4
                                Layout.preferredHeight: 86
                                Layout.preferredWidth: 127
                                source: "../img/mapa/letecka.png"
                            }

                            Image {
                                id: image5
                                Layout.preferredHeight: 86
                                Layout.preferredWidth: 127
                                source: "../img/mapa/fotograf.png"
                            }

                            Image {
                                id: image6
                                Layout.preferredHeight: 86
                                Layout.preferredWidth: 127
                                source: "../img/mapa/hapticka.png"
                            }

                            Image {
                                id: image7
                                Layout.preferredHeight: 86
                                Layout.preferredWidth: 127
                                source: "../img/mapa/textova.png"
                            }

                            Image {
                                id: image1
                                Layout.preferredHeight: 86
                                Layout.preferredWidth: 127
                                source: "../img/mapa/turisticka.png"
                            }

                            Image {
                                id: image2
                                Layout.preferredHeight: 86
                                Layout.preferredWidth: 127
                                source: "../img/mapa/zemepisna.png"
                            }
                        }

                        MouseArea {
                            id: mouseArea
                            x: 7
                            y: 8
                            width: 128
                            height: 83
                        }
                    }
                }

                Rectangle {
                    id: rectangle2
                    x: 1000
                    width: 186
                    color: "#27405d"
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 0
                    anchors.top: parent.top
                    anchors.topMargin: 0

                    CheckBox {

                        id: checkBox
                        x: 12
                        y: 18
                        font.pointSize: 1
                        checked: true
                    }

                    CheckBox {
                        id: checkBox1
                        x: 12
                        y: 64
                        font.weight: Font.Normal
                        checked: true
                    }

                    Label {
                        id: label
                        x: 64
                        y: 76
                        color: "#ffffff"
                        text: qsTr("Spojovat místa")
                    }

                    Label {
                        id: label1
                        x: 64
                        y: 30
                        color: "#ffffff"
                        text: qsTr("Značit polohu")
                    }
                }
            }
        }
    }
}


/*##^## Designer {
    D{i:4;anchors_height:103;anchors_width:718;anchors_x:0;anchors_y:8}D{i:7;anchors_x:0;anchors_y:0}
D{i:6;anchors_height:120;anchors_x:0;anchors_y:0}
}
 ##^##*/
