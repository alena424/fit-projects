import QtQuick 2.2
//import QtQuick 2.8
import QtQuick.Window 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls 2.1

import cz.vutbr.fit 1.0
import Qt.labs.folderlistmodel 2.1
import QtQuick.Dialogs 1.2

import Qt.labs.platform 1.0
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.1

import QtQuick 2.8


ApplicationWindow {
    id: win;
    visible: true;
    width: 1300;
    height: 800;
    color: "#dddddd"

    title: "Deník z dovolené";
    property string activeColor: '#2D4A6B'
    property string activeNonColor: '#27405D'

    Repeater {
        id: textAreaRepeater
        model: ListModel {
            id: modelTextArea;
        }
        delegate: TextArea {
            property int ind: index
            id: uloziste
            text: { textP }
        }
    }


    // Pokus s ukladanim fotky do programu - nefunguje!
    Connections{
        target: liveImageProvider
        onImageChanged: img.reload()
    }

    Shortcut {
        sequence: StandardKey.Open
        onActivated: openDialog.open()
    }
    Shortcut {
        sequence: StandardKey.SaveAs
        onActivated: saveDialog.open()
    }
    Shortcut {
        sequence: StandardKey.Quit
        onActivated: Qt.quit()
    }
    Shortcut {
        sequence: StandardKey.Copy
        onActivated: textArea.copy()
    }
    Shortcut {
        sequence: StandardKey.Cut
        onActivated: textArea.cut()
    }
    Shortcut {
        sequence: StandardKey.Paste
        onActivated: textArea.paste()
    }
    Shortcut {
        sequence: StandardKey.Bold
        onActivated: document.bold = !document.bold
    }
    Shortcut {
        sequence: StandardKey.Italic
        onActivated: document.italic = !document.italic
    }
    Shortcut {
        sequence: StandardKey.Underline
        onActivated: document.underline = !document.underline
    }


     MenuBar {
        id: tabMenu

        Menu {
            title: qsTr("&Soubor")

            MenuItem {
                text: qsTr("&Otevřít")
                onTriggered: openDialog.open()
            }
            MenuItem {
                text: qsTr("&Uložit jako...")
                onTriggered: saveDialog.open()
            }
            MenuItem {
                text: qsTr("&Konec")
                onTriggered: Qt.quit()
            }
        }

        Menu {
            title: qsTr("&Upravit")

            MenuItem {
                text: qsTr("&Kopírovat")
                enabled: textArea.selectedText
                onTriggered: textArea.copy()
            }
            MenuItem {
                text: qsTr("Vyjmout")
                enabled: textArea.selectedText
                onTriggered: textArea.cut()
            }
            MenuItem {
                text: qsTr("&Vložit")
                enabled: textArea.canPaste
                onTriggered: textArea.paste()
            }
        }

        Menu {
            title: qsTr("Formát")

            MenuItem {
                text: qsTr("&Tučně")
                checkable: true
                checked: document.bold
                onTriggered: document.bold = !document.bold
            }
            MenuItem {
                text: qsTr("&Kurzívou")
                checkable: true
                checked: document.italic
                onTriggered: document.italic = !document.italic
            }
            MenuItem {
                text: qsTr("&Podtržení")
                checkable: true
                checked: document.underline
                onTriggered: document.underline = !document.underline
            }
        }
        Menu {
            title: qsTr("Nápověda")

            MenuItem {
                text: qsTr("Používání programu")
                checkable: true
                checked: document.bold
                onTriggered: document.bold = !document.bold
            }
        }
    }
     Menu {
         id: contextMenu

         MenuItem {
             text: qsTr("Kopírovat")
             enabled: textArea.selectedText
             onTriggered: textArea.copy()
         }
         MenuItem {
             text: qsTr("Vyjmout")
             enabled: textArea.selectedText
             onTriggered: textArea.cut()
         }
         MenuItem {
             text: qsTr("Vložit")
             enabled: textArea.canPaste
             onTriggered: textArea.paste()
         }

         MenuSeparator {}

         MenuItem {
             text: qsTr("Font...")
             onTriggered: fontDialog.open()
         }

         MenuItem {
             text: qsTr("Barva...")
             onTriggered: colorDialog.open()
         }
     }

    FileDialog {
        id: openDialog
        fileMode: FileDialog.OpenFile
        selectedNameFilter.index: 1
        nameFilters: ["Text files (*.txt)", "HTML files (*.html *.htm)", "PDF files (*.pdf)"]
        folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        onAccepted: document.load(file)
    }

    FileDialog {
        id: saveDialog
        fileMode: FileDialog.SaveFile
        defaultSuffix: document.fileType
        nameFilters: openDialog.nameFilters
        selectedNameFilter.index: document.fileType === "txt" ? 0 : 1
        folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        onAccepted: document.saveAs(file)
    }

    FontDialog {
        id: fontDialog
        onAccepted: {
            document.fontFamily = font.family;
            document.fontSize = font.pointSize;
        }
    }


    ColorDialog {
        id: colorDialog
        currentColor: "black"
    }

    MessageDialog {
        id: errorDialog
    }

     /* alca konec */


    // Dialog na vyber slozky, odkud se cerpaji fotky
    FileDialog {
        id: dialog
        title: "Vyberte složku s fotkami"
        // selectFolder: true neni podporovano
        // defaultni slozka
         nameFilters: [ "Image files (*.jpg *.png)", "All files (*)" ]


        //folder: "file:///C:/Users/jan.sorm/Documents/ITU/fotkz"
        fileMode: FileDialog.OpenFiles // vice souboru
        //fileMode: FolderDialog.
        //selectedNameFilter.index: 1
        //nameFilters: ["Image files (*.jpg, *.jpeg, *.jpe, *.jfif, *.png) | *.jpg; *.jpeg; *.jpe; *.jfif; *.png"]
        folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)


        // po potvrzeni vybrane slozky
        onAccepted: {
            // zavre se dialog
            dialog.close()
            // slozka se priradi generatoru fotek
            folderModel.folder = folder
            //folderModel.files = dialog.files

             console.log("You chose: " + dialog.fileUrls)
             console.log("You chose: " + dialog.folder)
            console.log("You chose: " + folder)
            console.log("You chose: " +dialog.files)

            // hack
            listPhoto.visible = true
        }
    }

 //   Component.onCompleted: dialog.open()
    // hlavni obdelnik, ktery vyplnuje cely prostor aplikace
    Rectangle {
        id: all
        anchors.fill: parent
        color: "#dddddd"

        // horni vyberove menu
        TabView {
            id: tabs
            width: parent.width;
            height: 150


            Tab {
                property string title: "Nástroje"
                Rectangle{
                anchors.fill: parent
                color: "#27405D"

                /*FrontMenuToolsForm{
                    id: frontMenuTools;
                    width: parent.width
                    //y: 30
                    //z: 50
                }*/
                Item {
                    id: frontMenuTools
                    width: 1230
                    height: 120
                    property alias toolBar: toolBar


                    //property alias rectangleColor: rectangle.color
                    ToolBar {

                        background: Rectangle {
                            implicitWidth: 100
                            implicitHeight: 40
                            color: '#27405D'
                            border.color: '#27405D'

                        }


                        /*style: ToolBarStyle {
                                padding {
                                    left: 8
                                    right: 8
                                    top: 3
                                    bottom: 3
                                }

                            }*/

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
                            //text: qsTr("Zpet")
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: 0
                            anchors.left: parent.left
                            anchors.leftMargin: 0
                            anchors.top: parent.top
                            anchors.topMargin: 0
                            checked: false
                            checkable: true
                            property bool disable: false


                            BackNew {
                                id: back
                                anchors.fill: parent

                                MouseArea{
                                    anchors.fill: parent
                                    cursorShape: "PointingHandCursor"
                                    onClicked: {
                                        textArea.undo();
                                        console.log("go back");
                                    }
                                    onExited: {
                                         back.rectangleColor = activeNonColor
                                    }
                                    onEntered: {
                                        back.rectangleColor = activeColor
                                        //cursorShape: "ArrowCursor"
                                    }
                                }
                            }
                        }

                        ToolButton {
                            id: toolButton1
                            x: 120
                            width: 120
                            height: 120
                            //text: qsTr("Tool Button")
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
                            //text: qsTr("Tool Button")
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: 0
                            anchors.top: parent.top
                            anchors.topMargin: 0

                            /*RotateRight {
                                id: rotateRight
                                anchors.fill: parent
                            }*/
                            RotateRight {

                            id: rotateRight
                            anchors.fill: parent

                            property string colorActive: '#2D4A6B'
                            property string colorNonActive: '#27405d'
                            property bool disable: false

                            mouseArea.cursorShape: "PointingHandCursor"

                            mouseArea.onExited: {
                                rectangleColor = colorNonActive
                                cursorShape: "ArrowCursor"

                            }

                            mouseArea.onEntered: {
                                if (disable  ){
                                    cursorShape: "ForbiddenCursor"
                                } else {
                                    rectangleColor = colorActive
                                }

                            }
                            mouseArea.onClicked: {
                                currentState="turnRight"
                                /*if ( photoRepeater.itemAt(active_index) ){
                                    //itemAt(active_index).transform: { angle:90 }
                                    console.log(photoRepeater.itemAt(active_index) );
                                    console.log(active_index);
                                    photoRepeater.itemAt(active_index).transform[0].angle = 90;
                                }*/
                            }
                           }
                        }

                        ToolButton {
                            id: rotateleft
                            x: 375
                            width: 120
                            //text: qsTr("Tool Button")
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
                            //text: qsTr("Tool Button")
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
                            //text: qsTr("Tool Button")

                            Brightness {
                                id: map
                                anchors.fill: parent
                            }
                        }

                        Rectangle {
                            id: rectangle1
                            x: 754
                            width: 476
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
                                model: [ "Roboto", "Times New Roman", "Arial", "Tahoma" ]
                            }

                            ComboBox {
                                id: comboBox1
                                x: 217
                                y: 14
                                width: 98
                                height: 29
                                 model: [ "11", "12", "13", "14", "15" ]
                            }

                            ToolButton{
                                id: bold_tool

                                /*background: Rectangle {
                                    color: '#27405D'
                                }

                                text: "\uE800"
                                font.family: "fontello"*/

                                x: 25
                                y: 72
                                z: 20
                                width: 17
                                height: 24
                                checkable: true
                                checked: document.bold

                                MouseArea{
                                    anchors.fill: parent
                                    cursorShape: "PointingHandCursor"
                                    onClicked: {
                                        document.bold = !document.bold
                                        console.log("clicked")
                                    }
                                }


                                Bold {
                                    id: bold
                                    anchors.fill: parent
                                    rectangle1Color: bold_tool.checked ? activeColor :activeNonColor
                                }

                            }

                            ToolButton {
                                id: italic_tool
                                x: 73
                                y: 72
                                width: 17
                                height: 24
                                //text: qsTr("Tool Button")
                                /*text: "\uE801" // icon-italic
                                font.family: "fontello"*/
                                focusPolicy: Qt.TabFocus
                                checkable: true
                                checked: document.italic
                                MouseArea{
                                    anchors.fill: parent
                                    cursorShape: "PointingHandCursor"
                                    onClicked: document.italic = !document.italic
                                }

                                Italic {
                                    id: italic
                                    //x: 723
                                    //y: -78
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

                                Underline {
                                    id: underline
                                    anchors.fill: parent
                                }
                                focusPolicy: Qt.TabFocus
                                checkable: true
                                checked: document.underline
                                MouseArea{
                                    anchors.fill: parent
                                    cursorShape: "PointingHandCursor"
                                    onClicked: document.underline = !document.underline
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
                                MouseArea{
                                    anchors.fill: parent
                                    cursorShape: "PointingHandCursor"
                                    onClicked: colorDialog.open()
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
                                //font.bold: document.bold
                                //font.italic: document.italic
                                //font.underline: document.underline
                                MouseArea{
                                    anchors.fill: parent
                                    cursorShape: "PointingHandCursor"
                                    onClicked: {
                                        fontDialog.currentFont.family = document.fontFamily;
                                        fontDialog.currentFont.pointSize = document.fontSize;
                                        fontDialog.open();
                                    }
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
                                MouseArea{
                                    anchors.fill: parent
                                    cursorShape: "PointingHandCursor"
                                    onClicked: colorDialog.open()
                                }
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
                                   // text: qsTr("Tool Button")
                                    Layout.preferredHeight: 29
                                    Layout.preferredWidth: 27

                                    LeftAlign {
                                        id: left
                                        anchors.fill: parent
                                    }
                                    //text: "\uE803" // icon-align-left
                                    //font.family: "fontello"
                                    focusPolicy: Qt.TabFocus
                                    checkable: true
                                    checked: document.alignment == Qt.AlignLeft

                                    MouseArea{
                                        anchors.fill: parent
                                        cursorShape: "PointingHandCursor"
                                        onClicked: document.alignment = Qt.AlignLeft
                                    }

                                }

                                ToolButton {
                                    id: toolButton3
                                    //text: qsTr("Tool Button")
                                    Layout.preferredHeight: 29
                                    Layout.preferredWidth: 27

                                    AlignRight {
                                        id: right
                                        anchors.fill: parent
                                    }
                                    checkable: true
                                    checked: document.alignment == Qt.AlignRight
                                    MouseArea{
                                        anchors.fill: parent
                                        cursorShape: "PointingHandCursor"
                                        onClicked: document.alignment = Qt.AlignRight
                                    }
                                }

                                ToolButton {
                                    id: toolButton4
                                    //text: qsTr("Tool Button")
                                    Layout.preferredHeight: 29
                                    Layout.preferredWidth: 27

                                    AlignCenter {
                                        id: center
                                        anchors.fill: parent
                                    }
                                    checkable: true
                                    checked: document.alignment == Qt.AlignCenter
                                    MouseArea{
                                        anchors.fill: parent
                                        cursorShape: "PointingHandCursor"
                                        onClicked: document.alignment = Qt.AlignCenter
                                    }
                                }

                                ToolButton {
                                    id: toolButton5
                                   // text: qsTr("Tool Button")
                                    Layout.preferredHeight: 29
                                    Layout.preferredWidth: 27
                                    background: Rectangle{
                                        color: '#27405D'
                                    }

                                    Image {
                                        id: image3
                                        x: 74
                                        y: -59
                                        anchors.fill: parent
                                        source: "../img/tools/Vector-9.png"
                                    }
                                    MouseArea{
                                        anchors.fill: parent
                                        cursorShape: "PointingHandCursor"
                                    }
                                }

                                ToolButton {
                                    id: toolButton6
                                    //text: qsTr("Tool Button")
                                    Layout.preferredHeight: 29
                                    Layout.preferredWidth: 27
                                    background: Rectangle{
                                        color: '#27405D'
                                    }

                                    Image {
                                        id: image5
                                        x: -46
                                        y: -6
                                        anchors.fill: parent
                                        source: "../img/tools/Vector-8.png"
                                    }
                                    MouseArea{
                                        anchors.fill: parent
                                        cursorShape: "PointingHandCursor"
                                    }
                                }

                                ToolButton {
                                    id: toolButton7
                                    //text: qsTr("Tool Button")
                                    Layout.preferredHeight: 29
                                    Layout.preferredWidth: 27
                                    background: Rectangle{
                                        color: '#27405D'
                                    }

                                    Image {
                                        id: image4
                                        x: -86
                                        y: -6
                                        anchors.fill: parent
                                        source: "../img/tools/Vector-10.png"
                                    }
                                    MouseArea{
                                        anchors.fill: parent
                                        cursorShape: "PointingHandCursor"
                                    }
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

                    Export {
                        id: exportdoc
                        x: 1372
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 2
                        anchors.top: parent.top
                        anchors.topMargin: 2
                    }
                  }
               }

            }

            Tab {
                property string title: "Vložení"
                anchors.fill: parent;


                Rectangle {
                    FlipartsForm{
                       id:flipartform
                       //anchors.fill: parent;

                    }


                    anchors.fill: parent
                    color: '#27405D'

                }
            }


            Tab {

                property string title: "Mapa"
                anchors.fill: parent;
                enabled: paper.selected == -1 ? 0 : 1
                //enabled: true;


                Rectangle {
                    Item {
                        width: 1500
                        height: 120

                        Rectangle {
                            id: rectangle3
                            color: "#27405d"
                            anchors.rightMargin: -5
                            anchors.fill: parent

                            ToolBar {
                                id: toolBar2
                                anchors.rightMargin: 175
                                anchors.fill: parent

                                background: Rectangle {
                                    border.color: "#27405D"
                                }

                                ToolButton {
                                    id: toolButton13
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
                                    id: toolButton12
                                    x: 120
                                    y: -4
                                    width: 120
                                    text: qsTr("")

                                    Forward {
                                        id: forward2
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
                                        id: rectangle12
                                        y: 8
                                        height: 103
                                        color: "#ffffff"
                                        anchors.right: parent.right
                                        anchors.rightMargin: 0
                                        anchors.left: parent.left
                                        anchors.leftMargin: 0


                                        Flickable {
                                            // bude chtit upravit, aby focus platil prave nad tou oblasti, kam uzivatel kliknul, diky tomu pak nebude muset posouvat mysi, ale i klavesy
                                            // scrollbar ve vertikalnim smeru

                                            flickableDirection: Flickable.HorizontalFlick

                                            //Keys.onUpPressed: hbar.decrease()
                                            //Keys.onDownPressed: hbar.increase()

                                            id: flickable
                                            x: 0
                                            y: -8
                                            anchors.fill: parent
                                            clip: true

                                            // scrollbar ve vertikalnim smeru
                                            ScrollBar.horizontal: ScrollBar {
                                                id: hbar
                                                active: hbar.active
                                                //policy: ScrollBar.AlwaysOn
                                            }

                                            RowLayout {
                                                id: rowLayout
                                                x: 7
                                                y: 8
                                                //width: 1058
                                                //height: 86


                                                function addPhoto(source){
                                                    console.log("adding");
                                                    document.addNewPhoto(
                                                                50,
                                                                50,
                                                                300,
                                                                187,
                                                               source,
                                                                'png'
                                                        );
                                                    document.setPaperPhoto( document.getMaxIdPhoto() - 1, paper.page );

                                                    modelTest.append ({ "opacityAllP" : 1, "opacityP" : 1, "focusP": false, "pageP": paper.page });


                                                    modelTest2.append( {
                                                        "paperP": paper.page,
                                                        "xP": 50,
                                                              "yP":50,
                                                              "heightP" : 187,
                                                              "widthP": 300,
                                                              "sourceP":  source,
                                                    } )
                                                     console.log( document.getMaxIdPhoto() )
                                                }

                                                Rectangle{
                                                    id:zimnirectangle
                                                    Layout.preferredHeight: 86
                                                    Layout.preferredWidth: 127
                                                    color: "red"
                                                     x: -hbar.position * width

                                                    Image {

                                                        id: image32
                                                        width: 127
                                                        height: 86
                                                        source: "../img/mapa/zimni.png"
                                                    }


                                                    MouseArea {
                                                        hoverEnabled: true
                                                        id: zimnimoje
                                                        anchors.fill: parent;
                                                        cursorShape: "PointingHandCursor"
                                                        onPressed: console.log("Ahoj!")


                                                        onClicked: {
                                                            // klikne na fotku a pridame fotku do documentu
                                                            console.log("Brou");
                                                            console.log( document.getMaxIdPhoto() )
                                                            rowLayout.addPhoto( "../img/mapa/zimni_example.png");

                                                        }
                                                    }

                                                }
                                                Image {

                                                    id: image24
                                                    Layout.preferredHeight: 86
                                                    Layout.preferredWidth: 127
                                                    source: "../img/mapa/zakladni.png"
                                                    MouseArea{
                                                        anchors.fill: parent;
                                                        cursorShape: "PointingHandCursor";
                                                        onClicked: {
                                                            rowLayout.addPhoto("../img/mapa/priklad.png")
                                                        }

                                                    }
                                                }


                                                Image {

                                                    id: image42
                                                    Layout.preferredHeight: 86
                                                    Layout.preferredWidth: 127
                                                    source: "../img/mapa/letecka.png"
                                                    MouseArea{
                                                        anchors.fill: parent;
                                                        cursorShape: "PointingHandCursor";
                                                        onClicked: {
                                                            rowLayout.addPhoto("../img/mapa/letecka_example.png")
                                                        }

                                                    }
                                                }

                                                Image {

                                                    id: image52
                                                    Layout.preferredHeight: 86
                                                    Layout.preferredWidth: 127
                                                    source: "../img/mapa/fotograf.png"
                                                    MouseArea{
                                                        anchors.fill: parent;
                                                        cursorShape: "PointingHandCursor";
                                                        onClicked: {
                                                            rowLayout.addPhoto("../img/mapa/foto_example.png")
                                                        }

                                                    }
                                                }

                                                Image {

                                                    id: image62
                                                    Layout.preferredHeight: 86
                                                    Layout.preferredWidth: 127
                                                    source: "../img/mapa/hapticka.png"
                                                    MouseArea{
                                                        anchors.fill: parent;
                                                        cursorShape: "PointingHandCursor";
                                                        onClicked: {
                                                            rowLayout.addPhoto("../img/mapa/hapticka_example.png")
                                                        }

                                                    }
                                                }

                                                Image {

                                                    id: image7
                                                    Layout.preferredHeight: 86
                                                    Layout.preferredWidth: 127
                                                    source: "../img/mapa/textova.png"
                                                    MouseArea{
                                                        anchors.fill: parent;
                                                        cursorShape: "PointingHandCursor";
                                                        onClicked: {
                                                            rowLayout.addPhoto("../img/mapa/pismo_example.png")
                                                        }

                                                    }
                                                }

                                                Image {

                                                    id: image1
                                                    Layout.preferredHeight: 86
                                                    Layout.preferredWidth: 127
                                                    source: "../img/mapa/turisticka.png"

                                                    MouseArea{
                                                        anchors.fill: parent;
                                                        cursorShape: "PointingHandCursor";
                                                        onClicked: {
                                                            rowLayout.addPhoto("../img/mapa/priklad.png")
                                                        }

                                                    }
                                                }

                                                Image {

                                                    id: image2
                                                    Layout.preferredHeight: 86
                                                    Layout.preferredWidth: 127
                                                    source: "../img/mapa/zemepisna.png"
                                                    MouseArea{
                                                        anchors.fill: parent;
                                                        cursorShape: "PointingHandCursor";
                                                        onClicked: {
                                                            rowLayout.addPhoto("../img/mapa/zemepisna_example.png")
                                                        }

                                                    }
                                                }
                                                Image {

                                                    id: image23
                                                    Layout.preferredHeight: 86
                                                    Layout.preferredWidth: 127
                                                    source: "../img/mapa/stoleti.png"
                                                }

                                                Image {

                                                    id: image25
                                                    Layout.preferredHeight: 86
                                                    Layout.preferredWidth: 127
                                                    source: "../img/mapa/zasto.png"
                                                }
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

                    anchors.fill: parent
                    color: '#27405D'

                }
            }
        }


        // levy obdelnik, ve kterem se zobrazuji fotky pro vyber
        Rectangle {
            id: photo
            width: parent.width / 4;
            height: parent.height - tabs.height
            anchors.margins: 10
            y: 150
            z: 50
            color: "#777777";

            // item kvuli roztahovani okna
            Item {
                id: positionAnchorHookPhoto
                width: 0
                height: 0
            }

            // klikatelny obdelnik pro roztahovani obdelniku doprava
            MouseArea
            {
                id: rightHandlePhoto
                width: 4
                z: 60
                opacity: 0.01
                height: parent.height
                anchors.right: parent.right
                drag.target: rightHandlePhoto
                drag.axis: Drag.XAxis
                cursorShape: Qt.SizeHorCursor
                property int clickedXInHandle: 0 // used to keep the mouse pointed at the position you clicked

                onPressed:
                {
                    // We only set the anchors for a second to put the anchor element on the right position. After that's done we release the hook to this element.
                    positionAnchorHookPhoto.anchors.left = parent.left
                    positionAnchorHookPhoto.anchors.right = parent.right
                    positionAnchorHookPhoto.anchors.top = parent.top
                    positionAnchorHookPhoto.anchors.bottom = parent.bottom
                    positionAnchorHookPhoto.anchors.left = undefined
                    positionAnchorHookPhoto.anchors.bottom = undefined
                    positionAnchorHookPhoto.anchors.right = undefined
                    positionAnchorHookPhoto.anchors.top = undefined

                    clickedXInHandle = mouseX
                    var mouseToPlaceholder = mapToItem(positionAnchorHookPhoto, mouseX, mouseY)
                }

                onReleased:
                {
                    parent.anchors.top = undefined
                    parent.anchors.bottom = undefined
                    parent.anchors.left = undefined
                    parent.anchors.right = undefined
                }

                onPositionChanged:
                {
                    var mouseToPlaceholder = mapToItem(positionAnchorHookPhoto, mouseX, mouseY)
                    var newWidth = Math.abs(mouseToPlaceholder.x) + clickedXInHandle
                    if( mouseToPlaceholder.x >= 50 && photo.x + newWidth < papers.x - 40 ) {
                        photo.width = newWidth
                    }
                }
            }

            // levy cerny obdelnik
            Rectangle {
                id: newPhotoBar
                anchors {
                    top: parent.top
                    right:  parent.right
                    left:  parent.left
                }
                height: 35;
                color: "#02122C";
                Text{
                    text: "Fotky"
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    color: "white"
                    font.pixelSize: 15

                }

                // leve plusko
                Rectangle {
                    id: plusrec
                    anchors.left: parent.left
                    anchors.leftMargin: 5
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.topMargin: 5
                    anchors.bottomMargin: 5
                    width: newPhotoBar.height - 10
                    height: newPhotoBar.height
                    Image {
                       anchors.fill: parent
                       source: "../plus.png"
                       //source: "../img/tools/Vector-7.png"
                       sourceSize.width: 32
                       sourceSize.height: 32
                    }


                    // chceme ho udelat klikatelne a aby se na nem menil kurzor
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: "PointingHandCursor"
                        onClicked: {
                            dialog.open()
                        }
                    }
                    color: parent.color
                }
            }
            Rectangle{
                id: generate
                width: newPhotoBar.width

                height: newPhotoBar.height + 25
                y: newPhotoBar.height

                anchors.left: parent.left
                //anchors.top: plusrec.bottom

                color: "#777"
                /*Button{
                    text: "Generovat deník"
                    anchors.fill: parent
                }*/
                /*Image {
                    id: rozmistit
                    source: "../img/butt.png";
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    width: newPhotoBar.width < 217 ? newPhotoBar.width : 217;
                    height: 42

                }*/
                Rectangle {
                    id: rozmistit
                    color: "#27405d"
                    radius: 3
                    border.color: "#e1eeff"
                    //anchors.fill: parent
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    width: newPhotoBar.width < 217 ? newPhotoBar.width : 217;
                    height: 42

                    Image {
                        x: 44
                        y: 12
                        width: 19
                        height: 18
                        anchors.horizontalCenterOffset: -70
                        anchors.horizontalCenter: parent.horizontalCenter
                        fillMode: Image.Stretch
                        source: "../img/rozmistit.png"

                    }

                    Text {
                        x: 54
                        y: 13
                        color: "#e1eeff"
                        text: qsTr("Automatické rozmístění")
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 14
                    }

                }
                MouseArea{
                    cursorShape: "PointingHandCursor"
                    hoverEnabled: true;
                    anchors.fill: parent;

                    onClicked: {
                        // rozsmisteni fotek
                        console.log('width' + newPhotoBar.width);

                    }
                }
            }
            Slider {
                id: photoSlider
                z: 99
                y: newPhotoBar.height + generate.height
                to: 4.0
                from: 0.5
                value: 1.0
                width: newPhotoBar.width - 10
                anchors.horizontalCenter: newPhotoBar.horizontalCenter

                onValueChanged: {
                    console.log( photoSlider.value )
                    //rect.x *= photoSlider.value
                }
            }


            // zbyla plocha pod cernym obdelnikem, chceme, aby tam fungoval scrollbar
            Flickable {
                id: flickPhoto
                // bude chtit upravit, aby focus platil prave nad tou oblasti, kam uzivatel kliknul, diky tomu pak nebude muset posouvat mysi, ale i klavesy
                //focus: true
                // nastavuje se orezavani toho, co je mimo zobrazovanou plochu
                clip: true;
                // scrollbar ve vertikalnim smeru
                flickableDirection: Flickable.VerticalFlick
                anchors {
                    top: photoSlider.bottom
                    bottom: photo.bottom
                    left: photo.left
                    right: photo.right
                }

                // souvisi s focusem, posouvani na sipku nahoru/dolu
                Keys.onUpPressed: scrollBarPhotoVert.decrease()
                Keys.onDownPressed: scrollBarPhotoVert.increase()

                contentHeight: ( listPhoto.count / 2 ) * ( ( photo.width / 2 - 15 ) * 2 / 3 + 10 )
                ScrollBar.vertical: ScrollBar {
                    id: scrollBarPhotoVert
                }

                // zobrazovac fotek ze slozky
                GridView {
                    id: listPhoto
                    z: 20
                    x: 5
                    width: parent.width - listPhoto.x; height: 600
                    cellWidth: 100 * photoSlider.value + 5; cellHeight: 70 * photoSlider.value + 5

                    // na zacatku je visible vypnute - souvisi s hackem ve filedialogu
                    visible: false
                    model: folderModel
                    delegate: fileDelegate

                    FolderListModel {
                        id: folderModel
                        nameFilters: [ "*.*" ]

                    }

                    Component {
                        id: fileDelegate
                        GridView {
                            /*  width: 250
                                                        height: 150
                                                        flow: GridView.FlowLeftToRight*/
                            //cellWidth: 100; cellHeight: 70
                            //spacing: 5
                            Rectangle {
                                id: rect
                                // vypocitavani umisteni fotky ... bude chtit upravit v souvislosti s posuvnikem
                                //x: ( ( index % ( photoSlider.to + 1.0 - photoSlider.value ) ) * ( rect.width + photo.anchors.rightMargin ) ) + 10
                                //y: ( Math.round( ( index - 1 ) / ( photoSlider.to + 1.0 - photoSlider.value ) ) * ( rect.height + 10 ) ) + 10
                                //width: ( photo.width / ( photoSlider.to + 1.0 - photoSlider.value ) ) - photo.anchors.rightMargin - photo.anchors.rightMargin / ( photoSlider.to + 1.0 - photoSlider.value )
                                //height: rect.width * 2 / 3
                                width: listPhoto.cellWidth - 5
                                height: listPhoto.cellHeight - 5


                                /*x: ( ( index % 2 ) * ( ( photo.width / 2 - 20 ) + photo.anchors.rightMargin ) ) * photoSlider.value + 10
                                                                y: ( Math.round( ( index - 1 ) / 2 ) * ( ( ( photo.width / 2 - 15 ) * 2 / 3 ) + 10 ) * photoSlider.value ) + 10
                                                                width: ( photo.width / 2 ) * photoSlider.value - 20
                                                                height: ( ( photo.width / 2 - 15 ) * 2 / 3 ) * photoSlider.value*/
                                color: "#777"
                                property point beginDrag    // promenna na ulozeni pocatecni pozice fotky, kam ji chceme vratit po pretazeni na papir
                                property bool caught: false // zda, doslo k uspesnemu umisteni fotky
                                Drag.active: mouseArea.drag.active  // nastavuje se, ze lze s timto objektem hybat

                                Image {
                                    id: rectImage
                                    anchors.fill: parent
                                    fillMode: Image.PreserveAspectFit   // nastaveni, aby byla fotka pomerove spravna
                                    smooth: true

                                    source: folderModel.folder + "/" + folderModel.get(index, "fileName" )
                                    //source: "../img/tools/Vector-8.png"
                                }

                                Item {
                                    id: positionAnchorHookRect
                                    width: 0
                                    height: 0
                                }

                                // mousearea na fotce, za kterou jde pretahovat
                                MouseArea {
                                    id: mouseArea

                                    property int oldMouseX

                                    cursorShape: "PointingHandCursor"
                                    anchors.fill: parent
                                    drag.target: parent
                                    hoverEnabled: true


                                    onPressed: {
                                        rect.beginDrag = Qt.point(rect.x, rect.y);
                                        oldMouseX = mouseX
                                        // vypiname orezavani oblasti, ktere se dela kvuli scrollbaru, nejlepsi by to ale bylo jen v horziontalnim smeru
                                        flickPhoto.clip = false;

                                    }
                                    onReleased: {
                                        if(!rect.caught) {
                                            backAnimX.from = rect.x;
                                            backAnimX.to = beginDrag.x;
                                            backAnimY.from = rect.y;
                                            backAnimY.to = beginDrag.y;
                                            backAnim.start()
                                        } else {
                                            console.log("BAFFFF");
                                            console.log("rectImage" + rectImage.sourceSize.width);
                                            console.log("rect" + rect.width);
                                            console.log("rectImage" + rectImage.sourceSize.height);
                                            console.log("rect" + rect.height);

                                            console.log( "x: " + rect.x );
                                            console.log( "x drag: " + rect.beginDrag.x );
                                            console.log( "rect.left" + index );
                                            var cols = Math.round( listPhoto.width / listPhoto.cellWidth - 0.5 )
                                            console.log( "cols " + cols );
                                            var sloupec = index % cols;
                                            console.log( "sloupec " + sloupec );
                                            var radek = Math.round( index / cols - 0.5 );
                                            console.log( "radek " + radek );
                                            // pridavani fotky do dokumentu
                                            if ( ( rectImage.sourceSize.width / rect.width ) > ( rectImage.sourceSize.height / rect.height ) ) {
                                                document.addNewPhoto(
                                                            rect.x - paper.x + sloupec * listPhoto.cellWidth + 10 - photo.width + ( scrollBarPaperHor.position * flickBackgroundPaper.contentWidth ),
                                                            rect.y - paper.y + radek * listPhoto.cellHeight + 5 + newPhotoBar.height + photoSlider.height + generate.height + ( scrollBarPaperVert.position * flickBackgroundPaper.contentHeight ) - ( scrollBarPhotoVert.position * flickPhoto.contentHeight ),
                                                            rect.width,
                                                            ( ( rectImage.sourceSize.height * rect.width ) / rectImage.sourceSize.width ),
                                                            folderModel.folder + "/" + folderModel.get(index, "fileName" ),
                                                            folderModel.get( index, "fileSuffix" )
                                                            );
                                                document.setPaperPhoto( document.getMaxIdPhoto() - 1, paper.page );

                                                modelTest.append ({ "opacityAllP" : 1, "opacityP" : 1, "focusP": false, "pageP": paper.page });
                                                var xp = ( rect.x - paper.x + sloupec * listPhoto.cellWidth + 10 - photo.width + ( scrollBarPaperHor.position * flickBackgroundPaper.contentWidth ) )
                                                if ( xp < 0 ) {
                                                    console.log( "mensi nez nula" );
                                                    xp = 0
                                                }
                                                modelTest2.append( {
                                                                      "paperP": paper.page,
                                                                      "xP": xp,
                                                                      "yP": ( rect.y - paper.y + radek * listPhoto.cellHeight + 5 + photoSlider.height + generate.height + newPhotoBar.height + ( scrollBarPaperVert.position * flickBackgroundPaper.contentHeight ) - ( scrollBarPhotoVert.position * flickPhoto.contentHeight ) ),
                                                                      "heightP": ( ( rectImage.sourceSize.height * rect.width ) / rectImage.sourceSize.width ),
                                                                      "widthP": rect.width,
                                                                      "sourceP": folderModel.folder + "/" + folderModel.get(index, "fileName" )
                                                                  } )
                                            } else {
                                                console.log("chyba")
                                                document.addNewPhoto(
                                                            rect.x - paper.x + sloupec * listPhoto.cellWidth + 10 - photo.width + ( scrollBarPaperHor.position * flickBackgroundPaper.contentWidth ),
                                                            rect.y - paper.y + radek * listPhoto.cellHeight + 5 + newPhotoBar.height + photoSlider.height + generate.height + ( scrollBarPaperVert.position * flickBackgroundPaper.contentHeight ) - ( scrollBarPhotoVert.position * flickPhoto.contentHeight ),
                                                            ( ( rectImage.sourceSize.width * rect.height ) / rectImage.sourceSize.height ),
                                                            rect.height,
                                                            folderModel.folder + "/" + folderModel.get(index, "fileName" ),
                                                            folderModel.get( index, "fileSuffix" )
                                                            );
                                                document.setPaperPhoto( document.getMaxIdPhoto() - 1, paper.page );

                                                modelTest.append( { "opacityAllP" : 1, "opacityP" : 1, "focusP": false, "pageP": paper.page } );
                                                var xp = ( rect.x - paper.x + sloupec * listPhoto.cellWidth + 10 - photo.width + ( scrollBarPaperHor.position * flickBackgroundPaper.contentWidth ) )
                                                if ( xp < 0 ) {
                                                    xp = 0
                                                }

                                                modelTest2.append( {
                                                                      "paperP": paper.page,
                                                                      "xP": xp,
                                                                      "yP": ( rect.y - paper.y + radek * listPhoto.cellHeight + 5 + photoSlider.height + generate.height + newPhotoBar.height + ( scrollBarPaperVert.position * flickBackgroundPaper.contentHeight ) - ( scrollBarPhotoVert.position * flickPhoto.contentHeight ) ),
                                                                      "heightP": rect.height,
                                                                      "widthP": ( ( rectImage.sourceSize.width * rect.height ) / rectImage.sourceSize.height ),
                                                                      "sourceP": folderModel.folder + "/" + folderModel.get(index, "fileName" )
                                                                  } )
                                            }

                                            //modelTest2.append ({ "bg" : Qt.hsla (Math.random (), 0.85, 0.45, 1.0).toString () });
                                            backAnimX.from = rect.x;
                                            backAnimX.to = rect.beginDrag.x;
                                            backAnimY.from = rect.y;
                                            backAnimY.to = rect.beginDrag.y;
                                            backAnim.start()

                                            console.log( document.getMaxIdPhoto() )
                                            //console.log( "rect.x > " +  )
                                        }

                                        flickPhoto.clip = true;
                                    }

                                    onPositionChanged: {

                                        if (pressed) {
                                            left.width = left.width + (mouseX - oldMouseX)
                                        }
                                    }

                                }
                            }
                            // krasna animace pro navraceni fotky
                            ParallelAnimation {
                                id: backAnim
                                SpringAnimation { id: backAnimX; target: rect; property: "x"; duration: 500; spring: 2; damping: 0.2 }
                                SpringAnimation { id: backAnimY; target: rect; property: "y"; duration: 500; spring: 2; damping: 0.2 }
                            }
                        }
                    }
                }

                // stary nahodne generovany obdelniky misto fotek
                /*
                                                Repeater {
                                                    model: 30
                                                    Rectangle {
                                                        id: rect
                                                        visible:false
                                                        width: photo.width / 2 - 20
                                                        height: ( photo.width / 2 - 15 ) * 2 / 3
                                                        z: mouseArea.drag.active ||  mouseArea.pressed ? 100 : 99
                                                        color: Qt.rgba(Math.random(), Math.random(), Math.random(), 1)
                                                        x: ( index % 2 ) * ( rect.width + 10 ) + 10
                                                        y: Math.round( ( index - 1 ) / 2 ) * ( rect.height + 10 ) + 10
                                                        property point beginDrag
                                                        property bool caught: false
                                                        border { width:2; color: "white" }
                                                        radius: 5
                                                        Drag.active: mouseArea.drag.active

                                                        Text {
                                                            anchors.centerIn: parent
                                                            text: index
                                                            color: "white"
                                                        }

                                                        Item {
                                                            id: positionAnchorHookRect
                                                            width: 0
                                                            height: 0
                                                        }

                                                        MouseArea {
                                                            id: mouseArea

                                                            property int oldMouseX

                                                            anchors.fill: parent
                                                            drag.target: parent
                                                            hoverEnabled: true
                                                            onPressed: {
                                                                rect.beginDrag = Qt.point(rect.x, rect.y);
                                                                oldMouseX = mouseX
                                                                flickPhoto.clip = false;
                                                            }
                                                            onReleased: {
                                                                if(!rect.caught) {
                                                                    backAnimX.from = rect.x;
                                                                    backAnimX.to = beginDrag.x;
                                                                    backAnimY.from = rect.y;
                                                                    backAnimY.to = beginDrag.y;
                                                                    backAnim.start()
                                                                } else {
                                                                    console.log("BAFFFF");
                                                                    //document.addNewPhoto( 20, 20, 20, 20 );
                                                                    document.addNewPhoto(
                                                                        rect.x - paper.x - photo.width + ( scrollBarPaperHor.position * flickBackgroundPaper.contentWidth ),
                                                                        rect.y - paper.y + newPhotoBar.height + ( scrollBarPaperVert.position * flickBackgroundPaper.contentHeight ) - ( scrollBarPhotoVert.position * flickPhoto.contentHeight ),
                                                                        rect.width,
                                                                        rect.height
                                                                    );

                                                    //photoRepeater.update()
                                                                    modelTest.append ({ "bg" : Qt.hsla (Math.random (), 0.85, 0.45, 1.0).toString () });
                                                                    modelTest2.append( {
                                                                        "xP": ( rect.x - paper.x - photo.width + ( scrollBarPaperHor.position * flickBackgroundPaper.contentWidth ) ),
                                                                        "yP": ( rect.y - paper.y + newPhotoBar.height + ( scrollBarPaperVert.position * flickBackgroundPaper.contentHeight ) - ( scrollBarPhotoVert.position * flickPhoto.contentHeight ) ),
                                                                        "heightP": rect.height,
                                                                        "widthP": rect.width
                                                                    } )
                                                                    //modelTest2.append ({ "bg" : Qt.hsla (Math.random (), 0.85, 0.45, 1.0).toString () });
                                                                    backAnimX.from = rect.x;
                                                                    backAnimX.to = beginDrag.x;
                                                                    backAnimY.from = rect.y;
                                                                    backAnimY.to = beginDrag.y;
                                                                    backAnim.start()

                                                                    console.log( document.getMaxIdPhoto() )
                                                                    //console.log( "rect.x > " +  )
                                                                }

                                                                flickPhoto.clip = true;
                                                            }

                                                            onPositionChanged: {

                                                               if (pressed) {
                                                                   left.width = left.width + (mouseX - oldMouseX)
                                                               }
                                                           }

                                                        }

                                                        ParallelAnimation {
                                                            id: backAnim
                                                            SpringAnimation { id: backAnimX; target: rect; property: "x"; duration: 500; spring: 2; damping: 0.2 }
                                                            SpringAnimation { id: backAnimY; target: rect; property: "y"; duration: 500; spring: 2; damping: 0.2 }
                                                        }
                                                    }
                                                }*/
            }
        }

        // prostredek aplikace s papirem - plati, zde podobne veci o scrollbaru jako u predchozi oblasti
        Flickable {
            id: flickBackgroundPaper
            // focus: true
            clip: true;
            z: 1
            anchors {
                top: tabs.bottom
                right:  papers.left
                left: photo.right
                bottom:  parent.bottom
            }
            Keys.onUpPressed: scrollBarPaperVert.decrease()
            Keys.onDownPressed: scrollBarPaperVert.increase()
            Keys.onRightPressed: scrollBarPaperHor.increase()
            Keys.onLeftPressed: scrollBarPaperHor.decrease()

            // 297/210 konstanta pro rozmery A4
            contentHeight: 650 * 297/210 + 2 * 50
            contentWidth: 650 + 2 * ( flickBackgroundPaper.width / 7 )
            ScrollBar.vertical: ScrollBar {
                id: scrollBarPaperVert
            }
            ScrollBar.horizontal: ScrollBar {
                id: scrollBarPaperHor
            }

            Rectangle {


                id: backgroundPaper
                anchors.fill:  parent
                z: 1
                /*
                                                anchors {
                                                    top: tabs.bottom
                                                    right:  papers.left
                                                    left: photo.right
                                                    bottom:  parent.bottom
                                                }*/
                color: "#ddd"

                Rectangle {
                    id: leftSide
                    anchors.left: parent.left
                    width: parent.width / 15
                }

                Rectangle {
                    id: rightSide
                    anchors.right: parent.right
                    width: parent.width / 15
                }

                Rectangle {
                    id: topSide
                    anchors.top: parent.top
                    width: parent.height / 15
                }

                // samotny papir
                Rectangle {
                    id: paper
                    x: ( ( all.width - papers.width - photo.width > 750 ) ? ( ( all.width - papers.width - photo.width - 650 ) / 2 ) : 50 )
                    y: 50
                    width: 650
                    height: paper.width * 297/210
                    color: "white"
                    property int selected: -1
                     property int page: 1
                    DropArea {
                        anchors.fill: parent
                        onEntered: drag.source.caught = true;
                        onExited: drag.source.caught = false;
                    }
                    transform: Scale {
                                id: scalePaper
                                xScale: 1
                                yScale: 1
                            }

                    /* alca */
                    TextArea {
                        id: textArea
                        anchors.fill: parent
                        textFormat: Qt.RichText
                        wrapMode: TextArea.Wrap
                        //       focus: true
                        selectByMouse: true
                        textMargin: 94



                        //persistentSelection: true
                        // Different styles have different padding and background
                        // decorations, but since this editor is almost taking up the
                        // entire window, we don't need them.
                        //leftPadding: 6
                        //rightPadding: 6
                        //topPadding: 0
                        //bottomPadding: 0
                        //background: null

                        MouseArea {
                            acceptedButtons: Qt.RightButton
                            anchors.fill: parent
                            onClicked: contextMenu.open()
                        }

                        onLinkActivated: Qt.openUrlExternally(link)
                    }
                    /* alca */


                    Repeater {
                        id: photoRepeater
                        model: ListModel {
                            id: modelTest;
                        }
                        delegate: Rectangle {
                            property int ind: index
                            id: wrau
                            x: document.getXPhoto( index + 1 )
                            y: document.getYPhoto( index + 1 )
                            width: document.getWidthPhoto( index + 1 )
                            height: document.getHeightPhoto( index + 1 )
                            color: "white"
                            border.color: "black";
                            border.width: 2
                            opacity: { opacityAllP }
                            z: click.drag.active ||  click.pressed ? 2 : 1
                            property point beginDrag
                            property bool caught: false
                            Drag.active: click.drag.active
                            property int page: { pageP }



                            Image {
                                id: imageWrau
                                anchors.fill: parent
                                opacity: { opacityP }
                                focus: { focusP }
                                source: document.getFilePhoto( index + 1 )

                                Keys.onDeletePressed: {
                                    wrau.width = 0
                                    wrau.height = 0
                                    document.setWidthPhoto( index, 0 );
                                    document.setHeightPhoto( index, 0 );
                                    paper.selected = -1;
                                    modelTest2.set( index, { "widthP": 0, "heightP": 0 } );
                                    modelTest.set( index, { "focusP": false } );
                                    //imageWrau.focus = false
                                }

                                Keys.onEscapePressed: {
                                    modelTest.set( index, { "focusP": false, "opacityP": 1 } );
                                    paper.selected = -1
                                }
                            }

                            Item
                            {
                                id: positionAnchorHook
                                width: 0
                                height: 0
                            }

                            MouseArea {
                                id: click

                                property int oldMouseX

                                anchors.fill: parent
                                drag.target: parent
                                hoverEnabled: true
                                onClicked: {
                                    console.log( "last: " + paper.selected );
                                    console.log( "index: " + index );
                                    //     document.setOpacityPhoto( paper.selected, 1 );
                                    //     document.setOpacityPhoto( index, 0.5 );
                                    if ( index != paper.selected ) {
                                        modelTest.set( paper.selected, { "opacityP": 1.0, "focusP": false } );
                                        //modelTest2.set( paper.selected, { "heightP": ( 100.0 ) } );
                                        //document.setWidthPhoto( index, 1000 );
                                        //modelTest.set( index + 1, { "opacityP": 0.5 } )
                                        paper.selected = index;
                                        //imageWrau.focus = true;
                                        modelTest.set( index, { "opacityP": 0.5, "focusP": true } );
                                    } else {
                                        modelTest.set( index, { "opacityP": 1, "focusP": false } );
                                        paper.selected = -1;
                                    }

                                    //imageWrau.opacity = 0.5
                                }

                                onPressed: {
                                    parent.beginDrag = Qt.point(parent.x, parent.y);
                                    oldMouseX = mouseX
                                    //imageWrau.opacity = 0.5
                                }
                                onReleased: {
                                    //imageWrau.opacity = 1
                                    if( !parent.caught ) {
                                        backAnimXPhoto.from = parent.x;
                                        backAnimXPhoto.to = parent.beginDrag.x;
                                        backAnimYPhoto.from = parent.y;
                                        backAnimYPhoto.to = parent.beginDrag.y;
                                        backAnimPhoto.start()
                                    } else {
                                        document.setXPhoto( index, wrau.x );
                                        document.setYPhoto( index, wrau.y );
                                        //document.addNewPhoto( 20, 20, 20, 20 );
                                        //document.addNewPhoto( rect.x - paper.x - photo.width, rect.y - paper.y + newPhotoBar.height, rect.width, rect.height );
                                        modelTest2.set( index, {
                                                           "xP": ( wrau.x ),
                                                           "yP": ( wrau.y )
                                                       } )
                                    }
                                }

                                onPositionChanged: {

                                    if (pressed) {
                                        left.width = left.width + (mouseX - oldMouseX)
                                    }
                                }

                            }

                            MouseArea
                            {
                                id: rightPhoto
                                width: 4
                                height: parent.height
                                opacity: 0.01
                                anchors.right: parent.right
                                drag.target: rightPhoto
                                drag.axis: Drag.XAxis
                                cursorShape: Qt.SizeHorCursor
                                property int clickedXInHandle: 0 // used to keep the mouse pointed at the position you clicked
                                property int clickedXPlace: 0

                                onPressed:
                                {
                                    // We only set the anchors for a second to put the anchor element on the right position. After that's done we release the hook to this element.
                                    positionAnchorHook.anchors.left = parent.left
                                    positionAnchorHook.anchors.bottom = parent.bottom
                                    positionAnchorHook.anchors.top = parent.top
                                    positionAnchorHook.anchors.right = parent.right
                                    positionAnchorHook.anchors.left = undefined
                                    positionAnchorHook.anchors.bottom = undefined
                                    positionAnchorHook.anchors.top = undefined
                                    positionAnchorHook.anchors.right = undefined
                                    // Now hook our object to the achor element
                                    //parent.anchors.left = positionAnchorHook.left

                                    var mouseToPlaceholder = mapToItem(positionAnchorHook, mouseX, mouseY)
                                    clickedXInHandle = mouseX
                                    clickedXPlace = Math.abs(mouseToPlaceholder.x);
                                    //modelTest.set( index, { "opacityP": 0.5 } );
                                }

                                onReleased:
                                {
                                    parent.anchors.top = undefined
                                    parent.anchors.bottom = undefined
                                    parent.anchors.left = undefined
                                    parent.anchors.right = undefined
                                    //modelTest.set( index, { "opacityP": 1 } );
                                }

                                onPositionChanged:
                                {
                                    var mouseToPlaceholder = mapToItem(positionAnchorHook, mouseX, mouseY)
                                    var newWidth = Math.abs(mouseToPlaceholder.x) + clickedXInHandle

                                    if ( mouseToPlaceholder.x >= 20 ) {
                                        console.log("b");
                                        parent.width = newWidth
                                        console.log("b");
                                        document.setWidthPhoto( index, newWidth );
                                        modelTest2.set( index, { "widthP": newWidth } )
                                    }
                                }
                            }

                            MouseArea
                            {
                                id: bottomPhoto
                                width: parent.width
                                height: 4
                                opacity: 0.01
                                anchors.bottom: parent.bottom
                                drag.target: bottomPhoto
                                drag.axis: Drag.YAxis
                                cursorShape: Qt.SizeVerCursor
                                property int clickedYInHandle: 0 // used to keep the mouse pointed at the position you clicked
                                property int clickedYPlace: 0

                                onPressed:
                                {
                                    // We only set the anchors for a second to put the anchor element on the right position. After that's done we release the hook to this element.
                                    positionAnchorHook.anchors.left = parent.left
                                    positionAnchorHook.anchors.bottom = parent.bottom
                                    positionAnchorHook.anchors.top = parent.top
                                    positionAnchorHook.anchors.right = parent.right
                                    positionAnchorHook.anchors.left = undefined
                                    positionAnchorHook.anchors.bottom = undefined
                                    positionAnchorHook.anchors.top = undefined
                                    positionAnchorHook.anchors.right = undefined
                                    // Now hook our object to the achor element
                                    //parent.anchors.top = positionAnchorHook.top

                                    var mouseToPlaceholder = mapToItem(positionAnchorHook, mouseX, mouseY)
                                    clickedYInHandle = mouseY
                                    clickedYPlace = Math.abs(mouseToPlaceholder.y);
                                    //modelTest.set( index, { "opacityP": 0.5 } );
                                }

                                onReleased:
                                {
                                    parent.anchors.top = undefined
                                    parent.anchors.bottom = undefined
                                    parent.anchors.left = undefined
                                    parent.anchors.right = undefined
                                    //modelTest.set( index, { "opacityP": 1 } );
                                }

                                onPositionChanged:
                                {
                                    var mouseToPlaceholder = mapToItem(positionAnchorHook, mouseX, mouseY)
                                    var newHeight = Math.abs(mouseToPlaceholder.y) + clickedYInHandle

                                    if ( mouseToPlaceholder.y >= 20 ) {
                                        parent.height = newHeight
                                        document.setHeightPhoto( index, newHeight );
                                        modelTest2.set( index, { "heightP": newHeight } )
                                    }
                                }
                            }

                            MouseArea
                            {
                                id: leftPhoto
                                width: 4
                                height: parent.height
                                opacity: 0.01
                                anchors.left: parent.left
                                drag.target: leftPhoto
                                drag.axis: Drag.XAxis
                                cursorShape: Qt.SizeHorCursor

                                onPressed:
                                {
                                    // We only set the anchors for a second to put the anchor element on the right position. After that's done we release the hook to this element.
                                    positionAnchorHook.anchors.left = parent.left
                                    positionAnchorHook.anchors.bottom = parent.bottom
                                    positionAnchorHook.anchors.top = parent.top
                                    positionAnchorHook.anchors.right = parent.right
                                    positionAnchorHook.anchors.left = undefined
                                    positionAnchorHook.anchors.bottom = undefined
                                    positionAnchorHook.anchors.top = undefined
                                    positionAnchorHook.anchors.right = undefined

                                    //modelTest.set( index, { "opacityP": 0.5 } );
                                }

                                onReleased:
                                {
                                    parent.anchors.top = undefined
                                    parent.anchors.bottom = undefined
                                    parent.anchors.left = undefined
                                    parent.anchors.right = undefined

                                    //modelTest.set( index, { "opacityP": 1 } );
                                }

                                onPositionChanged:
                                {
                                    var mouseToPlaceholder = mapToItem(positionAnchorHook, mouseX, mouseY)
                                    var newWidth = - mouseToPlaceholder.x + parent.width
                                    var newX = wrau.x + ( wrau.width - newWidth )
                                    document.setXPhoto( index, newX );
                                    document.setWidthPhoto( index, newWidth );
                                    modelTest2.set( index, { "widthP": newWidth, "xP": newX } )
                                    wrau.x = newX
                                    parent.width = newWidth
                                }
                            }

                            MouseArea
                            {
                                id: topPhoto
                                width: parent.width
                                height: 4
                                opacity: 0.01
                                anchors.top: parent.top
                                drag.target: topPhoto
                                drag.axis: Drag.YAxis
                                cursorShape: Qt.SizeVerCursor

                                onPressed:
                                {
                                    // We only set the anchors for a second to put the anchor element on the right position. After that's done we release the hook to this element.
                                    positionAnchorHook.anchors.left = parent.left
                                    positionAnchorHook.anchors.bottom = parent.bottom
                                    positionAnchorHook.anchors.top = parent.top
                                    positionAnchorHook.anchors.right = parent.right
                                    positionAnchorHook.anchors.left = undefined
                                    positionAnchorHook.anchors.bottom = undefined
                                    positionAnchorHook.anchors.top = undefined
                                    positionAnchorHook.anchors.right = undefined
                                    //modelTest.set( index, { "opacityP": 0.5 } );
                                }

                                onReleased:
                                {
                                    parent.anchors.top = undefined
                                    parent.anchors.bottom = undefined
                                    parent.anchors.left = undefined
                                    parent.anchors.right = undefined
                                    //modelTest.set( index, { "opacityP": 1 } );
                                }

                                onPositionChanged:
                                {
                                    var mouseToPlaceholder = mapToItem(positionAnchorHook, mouseX, mouseY)
                                    var newHeight = - mouseToPlaceholder.y + parent.height
                                    var newY = parent.y + ( parent.height - newHeight )
                                    document.setYPhoto( index, newY );
                                    document.setHeightPhoto( index, newHeight );
                                    modelTest2.set( index, { "heightP": newHeight, "yP": newY } )
                                    parent.y = newY
                                    parent.height = newHeight
                                }
                            }

                            MouseArea
                            {
                                id: rightBottomPhoto
                                width: 4
                                opacity: 0.01
                                height: 4
                                anchors.right: parent.right
                                anchors.bottom: parent.bottom
                                drag.target: rightBottomPhoto
                                drag.axis: Drag.XandYAxis
                                cursorShape: Qt.SizeFDiagCursor
                                property int clickedXInHandle: 0 // used to keep the mouse pointed at the position you clicked
                                property int clickedYInHandle: 0
                                property int clickedXPlace: 0
                                property int clickedYPlace: 0

                                onPressed:
                                {
                                    // We only set the anchors for a second to put the anchor element on the right position. After that's done we release the hook to this element.
                                    positionAnchorHook.anchors.left = parent.left
                                    positionAnchorHook.anchors.bottom = parent.bottom
                                    positionAnchorHook.anchors.top = parent.top
                                    positionAnchorHook.anchors.right = parent.right
                                    positionAnchorHook.anchors.left = undefined
                                    positionAnchorHook.anchors.bottom = undefined
                                    positionAnchorHook.anchors.top = undefined
                                    positionAnchorHook.anchors.right = undefined

                                    // Now hook our object to the achor element
                                    //                                parent.anchors.left = positionAnchorHook.left
                                    //                              parent.anchors.top = positionAnchorHook.top

                                    var mouseToPlaceholder = mapToItem(positionAnchorHook, mouseX, mouseY)
                                    clickedXInHandle = mouseX
                                    clickedYInHandle = mouseY
                                    console.log( "clickedXPlace" + Math.abs(mouseToPlaceholder.x) )
                                    clickedXPlace = Math.abs(mouseToPlaceholder.x);
                                    clickedYPlace = Math.abs(mouseToPlaceholder.y);
                                    //modelTest.set( index, { "opacityP": 0.5 } );
                                }

                                onReleased:
                                {
                                    parent.anchors.top = undefined
                                    parent.anchors.bottom = undefined
                                    parent.anchors.left = undefined
                                    parent.anchors.right = undefined
                                    //modelTest.set( index, { "opacityP": 1 } );
                                }

                                onPositionChanged:
                                {
                                    var mouseToPlaceholder = mapToItem(positionAnchorHook, mouseX, mouseY)
                                    var newWidth = Math.abs(mouseToPlaceholder.x) + clickedXInHandle
                                    var newHeight = Math.abs(mouseToPlaceholder.y) + clickedYInHandle

                                    //      console.log( "root.x - " + root.x )
                                    //      console.log( "root.width - " + root.width )
                                    console.log( "newWidth - " + newWidth )
                                    console.log( "mouseToPlaceholder.x - " + mouseToPlaceholder.x )
                                    console.log( "clickedXInHandle - " + clickedXInHandle )
                                    if ( mouseToPlaceholder.x >= 20 && mouseToPlaceholder.y >= 20 ) {
                                        parent.width = newWidth
                                        parent.height = newHeight
                                        document.setWidthPhoto( index, newWidth );
                                        document.setHeightPhoto( index, newHeight );
                                        console.log( index );
                                        modelTest2.set( index, { "widthP": newWidth, "heightP": newHeight } )
                                        //modelTest2.sync();
                                        //modelTest2.setProperty( index, "x", 50 );
                                        //modelTest2.append ({ "bg" : Qt.hsla (Math.random (), 0.85, 0.45, 1.0).toString () });
                                    }
                                }

                                Rectangle
                                {
                                    id: rightBottomHandleRect2
                                    anchors.fill: parent
                                    color: "#02122C"
                                }
                            }

                            MouseArea
                            {
                                id: topRightPhoto
                                width: 4
                                height: 4
                                opacity: 0.01
                                anchors.top: parent.top
                                anchors.right: parent.right
                                drag.target: topRightPhoto
                                drag.axis: Drag.XAndYAxis
                                cursorShape: Qt.SizeBDiagCursor
                                property int clickedXInHandle: 0 // used to keep the mouse pointed at the position you clicked

                                onPressed:
                                {
                                    // We only set the anchors for a second to put the anchor element on the right position. After that's done we release the hook to this element.
                                    positionAnchorHook.anchors.left = parent.left
                                    positionAnchorHook.anchors.bottom = parent.bottom
                                    positionAnchorHook.anchors.top = parent.top
                                    positionAnchorHook.anchors.right = parent.right
                                    positionAnchorHook.anchors.left = undefined
                                    positionAnchorHook.anchors.bottom = undefined
                                    positionAnchorHook.anchors.top = undefined
                                    positionAnchorHook.anchors.right = undefined

                                    var mouseToPlaceholder = mapToItem(positionAnchorHook, mouseX, mouseY)
                                    clickedXInHandle = mouseX
                                    //modelTest.set( index, { "opacityP": 0.5 } );
                                }

                                onReleased:
                                {
                                    parent.anchors.top = undefined
                                    parent.anchors.bottom = undefined
                                    parent.anchors.left = undefined
                                    parent.anchors.right = undefined
                                    //modelTest.set( index, { "opacityP": 1 } );
                                }

                                onPositionChanged:
                                {
                                    var mouseToPlaceholder = mapToItem(positionAnchorHook, mouseX, mouseY)
                                    var newHeight = - mouseToPlaceholder.y + parent.height
                                    //var newWidth = - mouseToPlaceholder.x + parent.width
                                    var newWidth = Math.abs(mouseToPlaceholder.x) + clickedXInHandle
                                    var newY = parent.y + ( parent.height - newHeight )
                                    document.setYPhoto( index, newY );
                                    document.setHeightPhoto( index, newHeight );
                                    document.setWidthPhoto( index, newWidth );
                                    modelTest2.set( index, { "widthP": newWidth, "heightP": newHeight, "yP": newY } )
                                    parent.y = newY
                                    parent.height = newHeight
                                    parent.width = newWidth
                                }
                            }

                            MouseArea
                            {
                                id: bottomLeftPhoto
                                width: 4
                                height: 4
                                opacity: 0.01
                                anchors.bottom: parent.bottom
                                anchors.left: parent.left
                                drag.target: bottomLeftPhoto
                                drag.axis: Drag.XAndYAxis
                                cursorShape: Qt.SizeBDiagCursor
                                property int clickedYInHandle: 0 // used to keep the mouse pointed at the position you clicked

                                onPressed:
                                {
                                    // We only set the anchors for a second to put the anchor element on the right position. After that's done we release the hook to this element.
                                    positionAnchorHook.anchors.left = parent.left
                                    positionAnchorHook.anchors.bottom = parent.bottom
                                    positionAnchorHook.anchors.top = parent.top
                                    positionAnchorHook.anchors.right = parent.right
                                    positionAnchorHook.anchors.left = undefined
                                    positionAnchorHook.anchors.bottom = undefined
                                    positionAnchorHook.anchors.top = undefined
                                    positionAnchorHook.anchors.right = undefined

                                    var mouseToPlaceholder = mapToItem(positionAnchorHook, mouseX, mouseY)
                                    clickedYInHandle = mouseY
                                    //modelTest.set( index, { "opacityP": 0.5 } );
                                }

                                onReleased:
                                {
                                    parent.anchors.top = undefined
                                    parent.anchors.bottom = undefined
                                    parent.anchors.left = undefined
                                    parent.anchors.right = undefined
                                    //modelTest.set( index, { "opacityP": 1 } );
                                }

                                onPositionChanged:
                                {
                                    var mouseToPlaceholder = mapToItem(positionAnchorHook, mouseX, mouseY)
                                    var newWidth = - mouseToPlaceholder.x + parent.width
                                    //var newWidth = - mouseToPlaceholder.x + parent.width
                                    var newHeight = Math.abs(mouseToPlaceholder.y) + clickedYInHandle
                                    var newX = parent.x + ( parent.width - newWidth )
                                    document.setXPhoto( index, newX );
                                    document.setHeightPhoto( index, newHeight );
                                    document.setWidthPhoto( index, newWidth );
                                    modelTest2.set( index, { "widthP": newWidth, "heightP": newHeight, "xP": newX } )
                                    parent.x = newX
                                    parent.height = newHeight
                                    parent.width = newWidth
                                }
                            }

                            MouseArea
                            {
                                id: topLeftPhoto
                                width: 4
                                height: 4
                                opacity: 0.01
                                anchors.top: parent.top
                                anchors.left: parent.left
                                drag.target: topLeftPhoto
                                drag.axis: Drag.XAndYAxis
                                cursorShape: Qt.SizeFDiagCursor

                                onPressed:
                                {
                                    // We only set the anchors for a second to put the anchor element on the right position. After that's done we release the hook to this element.
                                    positionAnchorHook.anchors.left = parent.left
                                    positionAnchorHook.anchors.bottom = parent.bottom
                                    positionAnchorHook.anchors.top = parent.top
                                    positionAnchorHook.anchors.right = parent.right
                                    positionAnchorHook.anchors.left = undefined
                                    positionAnchorHook.anchors.bottom = undefined
                                    positionAnchorHook.anchors.top = undefined
                                    positionAnchorHook.anchors.right = undefined
                                    //modelTest.set( index, { "opacityP": 0.5 } );
                                }

                                onReleased:
                                {
                                    parent.anchors.top = undefined
                                    parent.anchors.bottom = undefined
                                    parent.anchors.left = undefined
                                    parent.anchors.right = undefined
                                    //modelTest.set( index, { "opacityP": 1 } );
                                }

                                onPositionChanged:
                                {
                                    var mouseToPlaceholder = mapToItem(positionAnchorHook, mouseX, mouseY)
                                    var newHeight = - mouseToPlaceholder.y + parent.height
                                    var newWidth = - mouseToPlaceholder.x + parent.width
                                    var newY = parent.y + ( parent.height - newHeight )
                                    var newX = parent.x + ( parent.width - newWidth )
                                    document.setXPhoto( index, newX );
                                    document.setYPhoto( index, newY );
                                    document.setHeightPhoto( index, newHeight );
                                    document.setWidthPhoto( index, newWidth );
                                    modelTest2.set( index, { "widthP": newWidth, "heightP": newHeight, "xP": newX, "yP": newY } )
                                    parent.x = newX
                                    parent.y = newY
                                    parent.height = newHeight
                                    parent.width = newWidth
                                }
                            }

                            // krasna animace pro navraceni fotky
                            ParallelAnimation {
                                id: backAnimPhoto
                                SpringAnimation { id: backAnimXPhoto; target: wrau; property: "x"; duration: 500; spring: 2; damping: 0.2 }
                                SpringAnimation { id: backAnimYPhoto; target: wrau; property: "y"; duration: 500; spring: 2; damping: 0.2 }
                            }

                        }
                    }
                }
            }
        }

        /*GenerovatForm{
            id: zoom
            //anchors.right: rightrect.left
            //y: rightrect.y - 100
            x: win.width - rightrect.x - 93
            y: win.height - 30
            z: 100
           // x: 100

            MouseArea{
                onClicked: {
                    console.log(zoom.x +' ' + zoom.y);
                }
            }
        }*/
        Item {
            id: zoomPanel
            width: 63
            height: 30
            x: win.width - rightrect.x - 93
            y: win.height - 30
            z: 100
            property double zoomNumber: 1

            Rectangle {
                id: rectangle28
                color: "#e1eeff"
                anchors.fill: parent
            }

            Rectangle {
                id: rectangle9
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
                MouseArea{
                    hoverEnabled: true
                     cursorShape: "PointingHandCursor"
                     anchors.fill: parent;
                     onClicked: {

                         zoomPanel.zoomNumber = zoomPanel.zoomNumber - 0.1;
                         scalePaper.xScale = scalePaper.xScale * zoomPanel.zoomNumber > 0 ? zoomPanel.zoomNumber : (1/zoomPanel.zoomNumber);
                         scalePaper.yScale = scalePaper.yScale * zoomPanel.zoomNumber > 0 ? zoomPanel.zoomNumber : (1/zoomPanel.zoomNumber);

                     }

                }
            }


            Rectangle {

                id: rectangleZoom
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
                    id: image18
                    width: 48
                    anchors.fill: parent
                    source: "../img/plus.png"
                }
                MouseArea{
                    hoverEnabled: true
                     cursorShape: "PointingHandCursor"
                     anchors.fill: parent;
                     onClicked: {
                         zoomPanel.zoomNumber = zoomPanel.zoomNumber + 0.1;

                         scalePaper.xScale = scalePaper.xScale * zoomPanel.zoomNumber > 0 ? zoomPanel.zoomNumber : (1/zoomPanel.zoomNumber);
                         scalePaper.yScale = scalePaper.yScale * zoomPanel.zoomNumber > 0 ? zoomPanel.zoomNumber : (1/zoomPanel.zoomNumber);


                     }

                }
            }

        }

        Rectangle {
            id: papers
            anchors {
                top: tabs.bottom
                right:  parent.right
                bottom:  parent.bottom
            }
            width: parent.width / 5;
            y: 150
            color: "#777777";

            Item
            {
                id: positionAnchorHookPaper
                width: 0
                height: 0
            }

            MouseArea
            {
                id: leftPaper
                width: 4
                height: parent.height
                opacity: 0.01
                z: 62
                anchors.left: parent.left
                drag.target: leftPaper
                drag.axis: Drag.XAxis
                cursorShape: Qt.SizeHorCursor

                onPressed:
                {
                    console.log("aaa")
                    // We only set the anchors for a second to put the anchor element on the right position. After that's done we release the hook to this element.
                    positionAnchorHookPaper.anchors.left = parent.left
                    positionAnchorHookPaper.anchors.bottom = parent.bottom
                    positionAnchorHookPaper.anchors.top = parent.top
                    positionAnchorHookPaper.anchors.right = parent.right
                    positionAnchorHookPaper.anchors.left = undefined
                    positionAnchorHookPaper.anchors.bottom = undefined
                    positionAnchorHookPaper.anchors.top = undefined
                    positionAnchorHookPaper.anchors.right = undefined
                }

                onReleased:
                {
                    parent.anchors.top = tabs.bottom
                    parent.anchors.bottom = all.bottom
                    parent.anchors.left = undefined
                    parent.anchors.right = all.right
                }

                onPositionChanged:
                {
                    var mouseToPlaceholder = mapToItem(positionAnchorHookPaper, mouseX, mouseY)
                    var newWidth = - mouseToPlaceholder.x + parent.width
                    var newX = parent.x + ( parent.width - newWidth )
                    parent.x = newX
                    parent.width = newWidth
                }
            }

            Rectangle {
                id: newPaperBar
                anchors {
                    top: parent.top
                    right:  parent.right
                    left:  parent.left
                }
                height: 35;
                color: "#02122C";
                Text{
                    text: "Strany"
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    color: "white"
                    font.pixelSize: 15

                }

                Rectangle {
                    id: rightrect
                    anchors.right: parent.right
                    anchors.rightMargin: 5
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.topMargin: 5
                    anchors.bottomMargin: 5
                    width: newPaperBar.height - 10
                    height: newPaperBar.height
                    Image {
                        anchors.fill: parent
                        source: "../plus.png"
                        sourceSize.width: 32
                        sourceSize.height: 32
                    }
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: "PointingHandCursor"
                        onClicked: {
                            if ( modelTextArea.count == 0 ) {
                                modelTextArea.append( { "textP": textArea.text } );
                            }
                            modelView.set( document.getActivePaper() - 1, { "borderWidth": 0 } )
                            document.addNewPaper()
                            console.log( "id " + document.getActivePaper() )
                            modelTextArea.set( document.getActivePaper(), { "textP": textArea.text } );
                            document.setActivePaper( document.getActivePaper() + 1 )
                            modelView.append( { "borderWidth": 2 } );
                            paper.page = document.getActivePaper()
                            for ( var i = 0; i < document.getMaxIdPhoto() - 1; i++ ) {
                                console.log( "stranka: " + document.getPaperPhoto( i + 1 ) );
                                if ( document.getPaperPhoto( i + 1 ) == document.getActivePaper() ) {
                                    modelTest.set( i, { "opacityAllP": 1.0 } )
                                } else {
                                    modelTest.set( i, { "opacityAllP": 0.0 } )
                                }
                            }
                            //modelTextArea.set( document.getActivePaper(), { "textP": textArea.text } );
                            /*var pokus = textArea.text
                                                        textArea.text = "";
                                                        textArea.text = pokus*/
                            modelTextArea.append( { "textP": "" } );
                            textArea.text = "";

                            console.log( "maxid " + document.getMaxIdPhoto() );
                        }
                    }
                    color: parent.color
                }
            }


            Flickable {
                id: flick
                //focus: true
                clip: true;
                anchors {
                    top: newPaperBar.bottom
                    bottom: papers.bottom
                    left: papers.left
                    right: papers.right
                }

                Keys.onUpPressed: scrollBar.decrease()
                Keys.onDownPressed: scrollBar.increase()

                Repeater {
                    id: tmp
                    model: ListModel {
                        id: modelView;

                        ListElement { borderWidth: 2 }
                    }
                    delegate: Rectangle
                    {
                        x: papers.width / 5
                        y: index * ( 1.15 * paperView.height ) + 0.15 * paperView.height
                        z: ( paperView.y > 0 ) ? 2 : 0
                        width: 3 * papers.width / 5
                        height: paperView.width * 297 / 210
                        color: "white"
                        border.width: { borderWidth }
                        border.color: "black"
                        id: paperView


                        Rectangle {
                            anchors.right: parent.left
                            width: ( index > 8 ) ? ( 2 * paperView.height / 10 ) : ( paperView.height / 10 )
                            color: papers.color
                            Text {
                                text: ( index + 1 )
                                font.family: "Helvetica"
                                font.pointSize: paperView.height / 10
                                color: "black"
                            }
                        }

                        MouseArea {
                            id: rightMouseClick
                            anchors.fill: parent
                            acceptedButtons: Qt.LeftButton | Qt.RightButton
                            onClicked: {
                                if ( mouse.button == Qt.RightButton ) {
                                    //parent.color = "blue"
                                    contextMenu2.open()
                                } else {
                                    modelTextArea.set( document.getActivePaper(), { "textP": textArea.text } );
                                    modelView.set( document.getActivePaper() - 1, { "borderWidth": 0 } )
                                    document.setActivePaper( index + 1 )
                                    modelView.set( document.getActivePaper() - 1, { "borderWidth": 2 } )
                                    console.log( document.getActivePaper() )
                                    paper.page = document.getActivePaper()
                                    console.log( "maxID " + document.getMaxIdPhoto() )
                                    for ( var i = 0; i < document.getMaxIdPhoto() - 1; i++ ) {
                                        console.log( "stranka: " + document.getPaperPhoto( i + 1 ) );
                                        if ( document.getPaperPhoto( i + 1 ) == document.getActivePaper() ) {
                                            modelTest.set( i, { "opacityAllP": 1.0 } )
                                        } else {
                                            modelTest.set( i, { "opacityAllP": 0.0 } )
                                        }
                                    }
                                     textArea.text = modelTextArea.get( document.getActivePaper() ).textP
                                }
                            }

                            Menu {
                                id: contextMenu2
                                //x: tmp.x
                                //y: tmp.y
                                MenuItem {
                                    text: "Vyjmout"
                                    onTriggered: {
                                    }
                                }
                                MenuItem { text: "Kopírovat" }
                                MenuSeparator { visible: true}
                                MenuItem { text: "Přidat za" }
                                MenuItem { text: "Přidat před" }
                                MenuItem { text: "Duplikovat" }
                            }
                        }

                    }
                }

                contentHeight: tmp.count * 1.15 * ( 3 * 297 * papers.width / ( 5 * 210 ) ) + 0.15 * ( 3 * 297 * papers.width / ( 5 * 210 ) )
                ScrollBar.vertical: ScrollBar {
                    id: scrollBar
                }


                Repeater {
                    id: photoViewRepeater
                    model: ListModel {
                        id: modelTest2;
                    }
                    delegate: Rectangle {
                        property int ind: index
                        id: photoSmall
                        x: { ( xP * ( 3 * papers.width / 5 ) / paper.width ) + papers.width / 5 }
                        y: { ( yP * ( 3 * papers.width / 5 ) / paper.width ) + ( 0 * ( 1.15 * 3 * papers.width / 5 * 297 / 210 ) + 0.15 * 3 * papers.width / 5 * 297 / 210 ) + ( 1.15 * ( ( 3 * papers.width / 5 ) * 297 / 210 ) ) * ( paperP - 1 ) }
                        z: 10
                        /*x: document.getXPhoto( index + 1 )
                                                        y: document.getYPhoto( index + 1 )*/
                        width: { widthP * ( 3 * papers.width / 5 ) / paper.width }
                        height: { heightP * ( 3 * papers.width / 5 ) / paper.width }
                        color: "black"

                        Image {
                            anchors.fill: parent
                            fillMode: Image.PreserveAspectFit   // nastaveni, aby byla fotka pomerove spravna
                            source: { sourceP }
                        }
                    }
                }
            }
        }
    }


    Document {
        id: document

        document: textArea.textDocument // text area jkao richtextdocument
        cursorPosition: textArea.cursorPosition
        selectionStart: textArea.selectionStart
        selectionEnd: textArea.selectionEnd

        /* zkusime nacist html dokumnet */
        Component.onCompleted: document.load("qrc:/qml/texteditor.html")

        onLoaded: {
            textArea.text = text
        }
        // pokud prijde nejaka chyba
        onError: {
            errorDialog.text = message
            errorDialog.visible = true
        }
    }


}
