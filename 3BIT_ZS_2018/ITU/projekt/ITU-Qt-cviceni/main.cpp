#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>
#include <QFontDatabase>

#include "lut.h"
#include "document.h"
#include "menuicons.h"
#include "liveimageprovider.h"

int main( int argc, char * argv[] )
{
    //QGuiApplication app( argc, argv );

#ifdef QT_WIDGETS_LIB
    QApplication app(argc, argv);
#else
    QGuiApplication app(argc, argv);
#endif

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

    QFontDatabase fontDatabase;
    if (fontDatabase.addApplicationFont(":/font/fontello.ttf") == -1){
        qWarning() << "Failed to load fontello.ttf";
    }


    qmlRegisterType<Document>( "cz.vutbr.fit", 1, 0, "Document" );
    Document document;

    //qmlRegisterType<MenuIcons>( "cz.vutbr.fit", 1, 0, "MenuIcons" );
    //MenuIcons menuIcons;

    QQmlApplicationEngine engine;
  //  engine.load( QUrl( QStringLiteral( "qrc:///qml/main.qml") ) );

    LiveImageProvider *liveImageProvider(new LiveImageProvider());
  //  QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("liveImageProvider", liveImageProvider);
    engine.addImageProvider("live", liveImageProvider);
    engine.load(QUrl(QLatin1String("qrc:/qml/main.qml")));

    QObject::connect(&document, &Document::newImage, liveImageProvider, &LiveImageProvider::updateImage);

    return app.exec();
}
