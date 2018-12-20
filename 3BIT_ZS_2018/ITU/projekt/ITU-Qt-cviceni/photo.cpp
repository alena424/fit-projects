#include "photo.h"

Photo::Photo( int index, double x, double y, double width, double height, QString fileName, QString format, int paper )
{
    QTextStream out( stdout );
    out << "ziju" << endl;
    this->paper = paper;
    this->index = index;
    this->file = fileName;
    out << "ziju1" << endl;
    this->setPhotoPosition( x, y, width, height );
    out << "ziju2" << endl;
    out << this->getX() << endl;
    out << "ziju2" << endl;
    this->setData( fileName, format );
}

void Photo::setPhotoPosition(double x, double y, double width, double height)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}

void Photo::setHeight( double height ) {
    this->height = height;
}

double Photo::getHeight() {
    return this->height;
}

double Photo::getOpacity()
{
    return this->opacity;
}

int Photo::getPaper()
{
    return this->paper;
}

void Photo::setWidth( double width ) {
    this->width = width;
}

double Photo::getWidth() {
    return this->width;
}

void Photo::setX( double x ) {
    this->x = x;
}

double Photo::getX() {
    QTextStream out( stdout );
    out << "WRERE" << endl;
    out << this->x << endl;
    return this->x;
}

void Photo::setY( double y ) {
    this->y = y;
}

void Photo::setPaper(int paper)
{
    this->paper = paper;
}

void Photo::setData( QString fileName, QString format)
{
    QByteArray ba = format.toLocal8Bit();
    const char *tmp = ba.data();
    this->data.save( (const QString &)fileName, tmp, -1 );
}

void Photo::setOpacity(double opacity)
{
    this->opacity = opacity;
}

QString Photo::getFile()
{
    return this->file;
}

const QImage & Photo::getData()
{
    return (const QImage &)this->data;
}

double Photo::getY() {
    return this->y;
}
