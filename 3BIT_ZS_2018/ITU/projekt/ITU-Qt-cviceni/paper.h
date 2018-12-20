#ifndef PAPER_H
#define PAPER_H

#include "photo.h"

class Paper
{
public:
    Paper( int number );

    std::map<int, Photo *> allPhotos;

    void addNewPhoto( double x, double y, double width, double height, QString fileName, QString format );
    double getXPhoto( int index );
    double getYPhoto( int index );
    double getWidthPhoto( int index );
    double getHeightPhoto( int index );
    QString getFilePhoto( int index );
    int getMaxIdPhoto();
    void setWidthPhoto( int index, double width );
    void setXPhoto( int index, double x );
    void setYPhoto( int index, double y );
    void setHeightPhoto( int index, double height );

private:
    int number;
    int maxIdPhoto = 0;

};

#endif // PAPER_H
