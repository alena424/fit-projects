#ifndef LIVEIMAGEPROVIDER_H
#define LIVEIMAGEPROVIDER_H

#include <QImage>
#include <QQuickImageProvider>

class LiveImageProvider : public QObject, public QQuickImageProvider
{
    Q_OBJECT
public:
    LiveImageProvider();

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

public slots:
    void updateImage(const QImage &image);

signals:
    void imageChanged();

private:
    QImage image;
    QImage no_image;
};

#endif // LIVEIMAGEPROVIDER_H
