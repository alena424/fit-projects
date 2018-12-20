#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "lut.h"

int main( int argc, char * argv[] )
{
    QGuiApplication app( argc, argv );

    /*
       Registrace třídy LUT aby byla dostupná v QML

       vysvětlení:
       šablona <...> - doplnit název C++ třídy nebo datového typu,
                       který má být dostupný v QML
       "cz.vubtbr.fit" - jmenný prostor v QML
       1 - hlavní (major) verze
       0 - "vedlejší" (minor) verze
       LUT - název třídy v QML


       Použití v QML:

       import cz.vutbr.fit 1.0

       LUT {
       }

    */
    qmlRegisterType<LUT>( "cz.vutbr.fit", 1, 0, "LUT" );

    QQmlApplicationEngine engine;
    engine.load( QUrl( QStringLiteral( "qrc:///qml/main.qml") ) );

    return app.exec();
}
