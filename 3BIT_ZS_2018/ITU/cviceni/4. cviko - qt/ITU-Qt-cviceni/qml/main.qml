import QtQuick 2.1
import QtQuick.Window 2.0
import "../js/Theme.js" as Theme
import cz.vutbr.fit 1.0

Window {
    visible: true
    width: 400
    height: 345
    
    title: "ITU - Qt 5 / QML kalkulačka"

    // Definování datového modelu s operátory
    // 'op' - zkratka pro operaci
    // 'tog' - zkratka pro toggled, označení, která operace je vybrána
    ListModel {
        id: operations;
        ListElement { op: "+"; tog: true; }
        ListElement { op: "-"; tog: false; }
        ListElement { op: "*"; tog: false; }
        ListElement { op: "/"; tog: false; }
        // TODO
        // Rozšiřte model o další dvě základní matematické operace 
    }

    // Prvek pro rozvržení prvků do sloupce
    // http://en.wikipedia.org/wiki/Layout_%28computing%29
    // http://qmlbook.org/ch04/index.html#positioning-elements
    Column {

        // Vstupní hodnota - první operand výpočtu
        Rectangle {
            id: vstup;
            height: 35;
            width: 400;
            border.color: "#bbb";
            border.width: 3;
            anchors.margins: 2
            color: "#777"


            TextInput {
                anchors.fill: parent;
                anchors.margins: 2
                horizontalAlignment: TextInput.AlignLeft
                verticalAlignment: TextInput.AlignVCenter
                id: textA
                font.pointSize: 22
                text: "0"
                
            }
        }

        // Prvek pro rozvržení prvků do řádku
        // Více jak prvek Column (výše)
        Row {
            // Obdoba ListView (ale více obecný) nebo funkce foreach()
            // obsahuje _model_ a _delegate_
            Repeater {
                // Definování modelu, data pro zobrazení
                model: operations;

                // Delegování vzhledu v MVC
                // Jak má vypadat jeden prvek
                delegate: MyButton {
                    btnColor: Theme.btn_colour
                    
                    text: model.op
                    toggled: model.tog;
                    
                    onClicked: {
                        for (var i = 0; i < operations.count; i++) {
                            operations.setProperty( i, "tog", (i == index) );
                        }
                    }
                }
            }

        }

        // "Vlastní" třída pro posuvník. Definice v MySlider.qml
        MySlider {
            id: slider
            color: Qt.darker(Theme.slider_color)
            rectColor: Theme.slider_color

        }

	// TODO
        // vložte nový textový prvek, který bude bude vizuálně 'zapadat'
        // do výsledné aplikace a bude zobrazoval vertikálně vycentrovaný
        // text 'LUT value: ' a hodnotu aktuálně vybrané položky z LUT
        Rectangle{
            color: "#777";
            width: 400; height: 30;
            anchors.margins: 2;
            Text {
                id: lut_text;
                text: "LUT value: " + lut.getValue(slider.value);
                font.pointSize: 15;

            }

        }


        // Vlastní klikací tlačítko. Definice v MyClickButton.qml
        MyClickButton {
            width: 400;
            btnColor: Theme.btn_colour
            
            text: qsTr( "Compute" )
            
            function getOperation() {
                for (var i = 0; i < operations.count; i++) {
                    var item = operations.get(i);
                    if (item.tog) {
                        return item.op;
                    }
                }
                return "+";
            }

            // Obsluha události při zachycení signálu clicked
            onClicked: {
                var a = parseFloat(textA.text, 10);
                // TODO
                // Zkontrolujte jestli funkce parseFloat vrátila 
                // korektní výsledek (tj. ne NaN, ale číslo). Pokud 
                // je hodnota a NaN, změňte barvu vstupního pole
                // na červenou a vypište chybu do pole pro výsledek
                var res = 0;
                if ( isNaN(a) ){
                    // nekorektni cislo
                    res = "CHYBA NaN !";
                    vstup.color = "#8F1A1A";
                } else {
                    vstup.color = "#777";
                }

                
                // TODO
                // Upravte načtení hodnotý b tak, aby získal hodnotu b
                // z LUT podle vybrané hodnoty na 'slider' 
                var b = lut.getValue( slider.value );

                // TODO
                // pokud se uživatel pokouší dělit nulou, změňte barvu
                // posuvníku na slideru na červenou a vypište chybu
                // do pole pro výsledek
                slider.rectColor = Theme.slider_color;


                var op = getOperation();
                if ( res == 0 ){
                    if ( op == '+'){
                        res = a + b;
                    }else if ( op == '-' ){
                        res = a-b;
                    } else if( op == '/' ){
                        if ( b == 0 ){
                            // nesmime delit nulou
                            res = "ZERO DIVISION";
                            slider.rectColor = "red";
                        } else {
                            res = a/b;
                        }
                    } else if( op == '*' ){
                        res = a *b;
                    }
                }
                result.text = res;


                console.log( a + " "+ op + " " + b + " = ?")
                
                // TODO
                // Výpočitejte vyslednou hodnotu danou operandy a, b
                // a operátorem op, výsledek uložte do prvku result
            }
        }

        // Prvek pro zobrazení výsledné hodnoty
        Rectangle {
            height: 45;
            width: 400;
            border.color: "#bbb";
            border.width: 3;
            anchors.margins: 2
            color: "#777"
            
            Text {
                id:  result;
                anchors.centerIn: parent
                height: 35;
                font.pointSize: 22
                color: "#0066FF"
            }
        }

    }

    // Vytvoření objektu LUT, který je definován v C++
    // K danému se přistupuje pomoci jeho id
    LUT {
        id: lut
    }

}

