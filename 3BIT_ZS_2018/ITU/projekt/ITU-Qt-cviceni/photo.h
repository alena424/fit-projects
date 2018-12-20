#ifndef PHOTO_H
#define PHOTO_H

#include <QObject>
#include <QTextStream>
#include <QImage>

class Photo
{
public:
    Photo( int index, double x, double y, double width, double height, QString fileName, QString format, int paper = 0 );

    void setPhotoPosition( double x, double y, double width, double height );
    Q_INVOKABLE double getX();
    double getY();
    double getWidth();
    double getHeight();
    double getOpacity();
    int getPaper();
    void setWidth( double width );
    void setHeight( double height );
    void setX( double x );
    void setY( double y );
    void setPaper( int paper );
    void setData( QString fileName, QString format );
    void setOpacity( double opacity );
    QString getFile();
    const QImage &getData();

private:
    int index, paper = 0;
    double x, y;
    double width, height, opacity = 1;
    QString file;
    QImage data;
};

#endif // PHOTO_H
