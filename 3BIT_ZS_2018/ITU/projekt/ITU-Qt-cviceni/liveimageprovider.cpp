#include "liveimageprovider.h"
#include <QDebug>
#include <QTextStream>
/**
* @brief Image provider that is used to handle the live image stream in the QML viewer.
 */
LiveImageProvider::LiveImageProvider() : QQuickImageProvider(QQuickImageProvider::Image)
{
    this->no_image = QImage(":/images/no_image.png");
    this->blockSignals(false);
}

/**
 * @brief Delivers image. The id is not used.
 * @param id The id is the requested image source, with the "image:" scheme and provider identifier removed.
 * For example, if the image source was "image://myprovider/icons/home", the given id would be "icons/home".
 * @param size In all cases, size must be set to the original size of the image. This is used to set the
 * width and height of the relevant Image if these values have not been set explicitly.
 * @param requestedSize The requestedSize corresponds to the Image::sourceSize requested by an Image item.
 * If requestedSize is a valid size, the image returned should be of that size.
 * @return
 */
QImage LiveImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    QImage result = this->image;

    if(result.isNull()) {
        result = this->no_image;
    }

    if(size) {
        *size = result.size();
    }

    if(requestedSize.width() > 0 && requestedSize.height() > 0) {
        result = result.scaled(requestedSize.width(), requestedSize.height(), Qt::KeepAspectRatio);
    }

    return result;
}

/**
 * @brief Update of the current image.
 * @param image The new image.
 */
void LiveImageProvider::updateImage(const QImage &image)
{

    QTextStream out( stdout );
    out << "iiiii " << endl;

    if(this->image != image) {
        this->image = image;
        emit imageChanged();
    }
}
