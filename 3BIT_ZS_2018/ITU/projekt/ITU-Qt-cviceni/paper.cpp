#include "paper.h"

Paper::Paper( int number )
{
    this->number = number;
}

void Paper::addNewPhoto(double x, double y, double width, double height, QString fileName, QString format)
{
    Photo *photo = new Photo( this->maxIdPhoto, x, y, width, height, fileName, format );
    this->allPhotos[ this->maxIdPhoto ] = photo;
    this->maxIdPhoto++;
}

double Paper::getXPhoto(int index)
{
    std::map<int, Photo *>::iterator it;
    it = this->allPhotos.find( index );
    if ( it != this->allPhotos.end() ) {
        return it->second->getX();
    } else {
        return 0;
    }
}

void Paper::setXPhoto( int index, double x )
{
    std::map<int, Photo *>::iterator it;
    it = this->allPhotos.find( index );
    if ( it != this->allPhotos.end() ) {
        it->second->setX( x );
    }
}

double Paper::getYPhoto(int index)
{
    std::map<int, Photo *>::iterator it;
    it = this->allPhotos.find( index );
    if ( it != this->allPhotos.end() ) {
        return it->second->getY();
    } else {
        return 0;
    }
    //return 20;
}

void Paper::setYPhoto( int index, double y )
{
    std::map<int, Photo *>::iterator it;
    it = this->allPhotos.find( index );
    if ( it != this->allPhotos.end() ) {
        it->second->setY( y );
    }
}

double Paper::getWidthPhoto(int index)
{
    std::map<int, Photo *>::iterator it;
    it = this->allPhotos.find( index );
    if ( it != this->allPhotos.end() ) {
        return it->second->getWidth();
    } else {
        return 0;
    }
}

void Paper::setWidthPhoto( int index, double width )
{
    std::map<int, Photo *>::iterator it;
    it = this->allPhotos.find( index );
    if ( it != this->allPhotos.end() ) {
        it->second->setWidth( width );
    }
}

double Paper::getHeightPhoto(int index)
{
    std::map<int, Photo *>::iterator it;
    it = this->allPhotos.find( index );
    if ( it != this->allPhotos.end() ) {
        return it->second->getHeight();
    }
}

QString Paper::getFilePhoto(int index)
{
    std::map<int, Photo *>::iterator it;
    it = this->allPhotos.find( index );
    if ( it != this->allPhotos.end() ) {
        return it->second->getFile();
    }
}

void Paper::setHeightPhoto( int index, double height )
{
    std::map<int, Photo *>::iterator it;
    it = this->allPhotos.find( index );
    if ( it != this->allPhotos.end() ) {
        it->second->setHeight( height );
    }
}

int Paper::getMaxIdPhoto()
{
    return this->maxIdPhoto;
}
