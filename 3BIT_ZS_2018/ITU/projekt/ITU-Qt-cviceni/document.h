#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QObject>
#include "photo.h"
#include "paper.h"
#include "string.h"

#include <QFont>
#include <QObject>
#include <QTextCursor>
#include <QUrl>

QT_BEGIN_NAMESPACE
class QTextDocument;
class QQuickTextDocument;
QT_END_NAMESPACE

class Document : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QQuickTextDocument *document READ document WRITE setDocument NOTIFY documentChanged)
    Q_PROPERTY(int cursorPosition READ cursorPosition WRITE setCursorPosition NOTIFY cursorPositionChanged)
    Q_PROPERTY(int selectionStart READ selectionStart WRITE setSelectionStart NOTIFY selectionStartChanged)
    Q_PROPERTY(int selectionEnd READ selectionEnd WRITE setSelectionEnd NOTIFY selectionEndChanged)

    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor NOTIFY textColorChanged)
    Q_PROPERTY(QString fontFamily READ fontFamily WRITE setFontFamily NOTIFY fontFamilyChanged)
    Q_PROPERTY(Qt::Alignment alignment READ alignment WRITE setAlignment NOTIFY alignmentChanged)

    Q_PROPERTY(bool bold READ bold WRITE setBold NOTIFY boldChanged)
    Q_PROPERTY(bool italic READ italic WRITE setItalic NOTIFY italicChanged)
    Q_PROPERTY(bool underline READ underline WRITE setUnderline NOTIFY underlineChanged)

    Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize NOTIFY fontSizeChanged)

    Q_PROPERTY(QString fileName READ fileName NOTIFY fileUrlChanged)
    Q_PROPERTY(QString fileType READ fileType NOTIFY fileUrlChanged)
    Q_PROPERTY(QUrl fileUrl READ fileUrl NOTIFY fileUrlChanged)

public:
    explicit Document(QObject *parent = 0);

    std::map<int, Photo *> allPhotos;
    std::map<int, Paper *> allPapers;

    Q_INVOKABLE void addNewPhoto( double x, double y, double width, double height, QString fileName, QString format );
    Q_INVOKABLE double getXPhoto( int index );
    Q_INVOKABLE double getYPhoto( int index );
    Q_INVOKABLE double getWidthPhoto( int index );
    Q_INVOKABLE double getHeightPhoto( int index );
    Q_INVOKABLE double getOpacity( int index );
    Q_INVOKABLE int getPaperPhoto( int index );
    Q_INVOKABLE int getMaxIdPhoto();
    Q_INVOKABLE QString getFilePhoto( int index );
    Q_INVOKABLE void setWidthPhoto( int index, double width );
    Q_INVOKABLE void setXPhoto( int index, double x );
    Q_INVOKABLE void setXWidthPhoto( int index, double x, double width );
    Q_INVOKABLE void setYPhoto( int index, double y );
    Q_INVOKABLE void setHeightPhoto( int index, double height );
    Q_INVOKABLE void setPaperPhoto( int index, int paper );
    Q_INVOKABLE void setOpacityPhoto( int index, double opacity );

    Q_INVOKABLE void addNewPaper( int paper = 0 );
    Q_INVOKABLE void addNewPhoto( int paper, double x, double y, double width, double height, QString fileName, QString format );
    Q_INVOKABLE double getXPhoto( int paper, int index );
    Q_INVOKABLE double getYPhoto( int paper, int index );
    Q_INVOKABLE double getWidthPhoto( int paper, int index );
    Q_INVOKABLE double getHeightPhoto( int paper, int index );
    Q_INVOKABLE QString getFilePhoto( int paper, int index );
    Q_INVOKABLE int getMaxIdPhoto( int paper );
    Q_INVOKABLE int getMaxIdPaper();
    Q_INVOKABLE void setWidthPhoto( int paper, int index, double width );
    Q_INVOKABLE void setXPhoto( int paper, int index, double x );
    Q_INVOKABLE void setYPhoto( int paper, int index, double y );
    Q_INVOKABLE void setHeightPhoto( int paper, int index, double height );

    Q_INVOKABLE int getActivePaper();
    Q_INVOKABLE void setActivePaper( int paper );

    QQuickTextDocument *document() const;

    void setDocument(QQuickTextDocument *document);


    int cursorPosition() const;
    void setCursorPosition(int position);

    int selectionStart() const;
    void setSelectionStart(int position);

    int selectionEnd() const;
    void setSelectionEnd(int position);

    QString fontFamily() const;

    void setFontFamily(const QString &family);

    QColor textColor() const;
    void setTextColor(const QColor &color);

    Qt::Alignment alignment() const;
    void setAlignment(Qt::Alignment alignment);

    bool bold() const;
    void setBold(bool bold);

    bool italic() const;
    void setItalic(bool italic);

    bool underline() const;
    void setUnderline(bool underline);

    int fontSize() const;
    void setFontSize(int size);

    QString fileName() const;
    QString fileType() const;
    QUrl fileUrl() const;

signals:
    void newImage( const QImage &);

Q_SIGNALS:

    void documentChanged();
    void cursorPositionChanged();
    void selectionStartChanged();
    void selectionEndChanged();

    void fontFamilyChanged();
    void textColorChanged();
    void alignmentChanged();

    void boldChanged();
    void italicChanged();
    void underlineChanged();

    void fontSizeChanged();

    void textChanged();
    void fileUrlChanged();

    void loaded(const QString &text);
    void error(const QString &message);

public Q_SLOTS:
    /*
     * metody, ktere reaguji na vyvolany signal
     * */
    void load(const QUrl &fileUrl);
    void saveAs(const QUrl &fileUrl);

private:
    int maxIdPhoto = 1;
    int maxIdPaper = 2;
    int activePaper = 1;

    void reset();
    QTextCursor textCursor() const;
    QTextDocument *textDocument() const;
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);

    QQuickTextDocument *document_all;

    int m_cursorPosition;
    int m_selectionStart;
    int m_selectionEnd;

    QFont m_font;
    int m_fontSize;
    QUrl m_fileUrl;

};

#endif // DOCUMENT_H
