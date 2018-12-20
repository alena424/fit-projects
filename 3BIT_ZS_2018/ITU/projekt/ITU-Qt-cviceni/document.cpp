#include "document.h"
#include <QTextStream>

#include <QFile>
#include <QFileInfo>
#include <QFileSelector>
#include <QQmlFile>
#include <QQmlFileSelector>
#include <QQuickTextDocument>
#include <QTextCharFormat>
#include <QTextCodec>
#include <QTextDocument>
#include <QDebug>

Document::Document( QObject * parent ) :
    QObject( parent )

  // inicializace
  , document_all(nullptr)
  , m_cursorPosition(-1)
  , m_selectionStart(0)
  , m_selectionEnd(0)
{

}

void Document::addNewPhoto(double x, double y, double width, double height, QString fileName, QString format )
{
    QTextStream out( stdout );
    out << "x -> " << x << " y -> " << y << " width -> " << width << " height -> " << height << endl;
    out << "dddd " << endl;
    Photo *photo = new Photo( this->maxIdPhoto, x, y, width, height, fileName, format );
    out << "eeeee " << endl;
    emit newImage( photo->getData() );
    out << "fffff " << endl;
    out << "baf" << endl;
    //out << "ZKOUSIM" << photo->x << endl;
    out << "ZKOUSIM" << photo->getX() << endl;
    this->allPhotos[ this->maxIdPhoto ] = photo;
    out << "alena" << this->maxIdPhoto << endl;
    //out << "X ... " << this->allPhotos[ 1 ]->x << endl;
//    this->allPhotos.insert( std::pair<int, Photo *>( this->maxIdPhoto, photo ) );
    this->maxIdPhoto++;
}

double Document::getXPhoto(int index)
{
    QTextStream out( stdout );
    out << "Index: " << index << endl;
    out << "Max: " << this->maxIdPhoto << endl;
    //return 20;
    std::map<int, Photo *>::iterator it;
    it = this->allPhotos.find( index );
    if ( it != this->allPhotos.end() ) {
        return it->second->getX();
    } else {
        return 0;
    }
}

void Document::setXPhoto( int index, double x )
{
    std::map<int, Photo *>::iterator it;
    it = this->allPhotos.find( index );
    if ( it != this->allPhotos.end() ) {
        it->second->setX( x );
    }
}

void Document::setXWidthPhoto(int index, double x, double width)
{
    std::map<int, Photo *>::iterator it;
    it = this->allPhotos.find( index );
    if ( it != this->allPhotos.end() ) {
        it->second->setX( x );
        it->second->setWidth( width );
    }
}

double Document::getYPhoto(int index)
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

void Document::setYPhoto( int index, double y )
{
    std::map<int, Photo *>::iterator it;
    it = this->allPhotos.find( index );
    if ( it != this->allPhotos.end() ) {
        it->second->setY( y );
    }
}

double Document::getWidthPhoto(int index)
{
    std::map<int, Photo *>::iterator it;
    it = this->allPhotos.find( index );
    if ( it != this->allPhotos.end() ) {
        return it->second->getWidth();
    } else {
        return 0;
    }
}

void Document::setWidthPhoto( int index, double width )
{
    std::map<int, Photo *>::iterator it;
    it = this->allPhotos.find( index );
    if ( it != this->allPhotos.end() ) {
        it->second->setWidth( width );
    }
}

double Document::getHeightPhoto(int index)
{
    std::map<int, Photo *>::iterator it;
    it = this->allPhotos.find( index );
    if ( it != this->allPhotos.end() ) {
        return it->second->getHeight();
    }
}

double Document::getOpacity(int index)
{
    std::map<int, Photo *>::iterator it;
    it = this->allPhotos.find( index );
    if ( it != this->allPhotos.end() ) {
        return it->second->getOpacity();
    }
}

int Document::getPaperPhoto(int index)
{
    std::map<int, Photo *>::iterator it;
    it = this->allPhotos.find( index );
    if ( it != this->allPhotos.end() ) {
        return it->second->getPaper();
    }
}

void Document::setHeightPhoto( int index, double height )
{
    std::map<int, Photo *>::iterator it;
    it = this->allPhotos.find( index );
    if ( it != this->allPhotos.end() ) {
        it->second->setHeight( height );
    }
}

void Document::setPaperPhoto(int index, int paper)
{
    std::map<int, Photo *>::iterator it;
    it = this->allPhotos.find( index );
    if ( it != this->allPhotos.end() ) {
        it->second->setPaper( paper );
    }
}

void Document::setOpacityPhoto(int index, double opacity)
{
    std::map<int, Photo *>::iterator it;
    it = this->allPhotos.find( index );
    if ( it != this->allPhotos.end() ) {
        it->second->setOpacity( opacity );
    }
}

int Document::getMaxIdPhoto()
{
    return this->maxIdPhoto;
}

QString Document::getFilePhoto(int index)
{
    std::map<int, Photo *>::iterator it;
    it = this->allPhotos.find( index );
    if ( it != this->allPhotos.end() ) {
        return it->second->getFile();
    } else {
        return 0;
    }
}


/***************************************************/
/***************************************************/
/***************************************************/

void Document::addNewPaper( int paper ) {
    QTextStream out( stdout );
    out << "xdsdfsdfsd" << endl;

    if ( paper == 0 ) {
        Paper *paper = new Paper( this->maxIdPaper );
        this->allPapers[ this->maxIdPaper ] = paper;
    } else {
        //TODO
    }
    this->maxIdPaper++;
}

void Document::addNewPhoto(int paper, double x, double y, double width, double height, QString fileName, QString format )
{
    //Photo *photo = new Photo( this->maxIdPhoto, x, y, width, height, paper );
    std::map<int, Paper *>::iterator it;
    it = this->allPapers.find( paper );

    if ( it != this->allPapers.end() ) {
        it->second->addNewPhoto( x, y, width, height, fileName, format );
    }
}

double Document::getXPhoto(int paper, int index)
{
    std::map<int, Paper *>::iterator it;
    it = this->allPapers.find( paper );
    if ( it != this->allPapers.end() ) {
        return it->second->getXPhoto( index );
    } else {
        return 0;
    }
}

void Document::setXPhoto( int paper, int index, double x )
{
    std::map<int, Paper *>::iterator it;
    it = this->allPapers.find( paper );
    if ( it != this->allPapers.end() ) {
        it->second->setXPhoto( index, x );
    }
}

double Document::getYPhoto( int paper, int index )
{
    std::map<int, Paper *>::iterator it;
    it = this->allPapers.find( paper );
    if ( it != this->allPapers.end() ) {
        return it->second->getYPhoto( index );
    } else {
        return 0;
    }}

void Document::setYPhoto( int paper, int index, double y )
{
    std::map<int, Paper *>::iterator it;
    it = this->allPapers.find( paper );
    if ( it != this->allPapers.end() ) {
        it->second->setYPhoto( index, y );
    }
}

double Document::getWidthPhoto( int paper, int index )
{
    std::map<int, Paper *>::iterator it;
    it = this->allPapers.find( paper );
    if ( it != this->allPapers.end() ) {
        return it->second->getWidthPhoto( index );
    } else {
        return 0;
    }
}

void Document::setWidthPhoto( int paper, int index, double width )
{
    std::map<int, Paper *>::iterator it;
    it = this->allPapers.find( paper );
    if ( it != this->allPapers.end() ) {
        it->second->setWidthPhoto( index, width );
    }
}

double Document::getHeightPhoto( int paper, int index)
{
    std::map<int, Paper *>::iterator it;
    it = this->allPapers.find( paper );
    if ( it != this->allPapers.end() ) {
        return it->second->getHeightPhoto( index );
    } else {
        return 0;
    }
}

QString Document::getFilePhoto(int paper, int index)
{
    std::map<int, Paper *>::iterator it;
    it = this->allPapers.find( paper );
    if ( it != this->allPapers.end() ) {
        return it->second->getFilePhoto( index );
    } else {
        return 0;
    }
}

void Document::setHeightPhoto( int paper, int index, double height )
{
    std::map<int, Paper *>::iterator it;
    it = this->allPapers.find( paper );
    if ( it != this->allPapers.end() ) {
        it->second->setHeightPhoto( index, height );
    }
}

int Document::getMaxIdPhoto( int paper )
{
    std::map<int, Paper *>::iterator it;
    it = this->allPapers.find( paper );
    if ( it != this->allPapers.end() ) {
        return it->second->getMaxIdPhoto();
    }
}

int Document::getMaxIdPaper()
{
    return this->maxIdPaper;
}

int Document::getActivePaper()
{
    return this->activePaper;
}

void Document::setActivePaper( int paper )
{
    this->activePaper = paper;
}

/***********************************************************************/
// Prace s textem

/**
 * @brief Document::document - kontruktor
 * @return
 */
QQuickTextDocument *Document::document() const
{
    return document_all;
}


void Document::setDocument(QQuickTextDocument *document)
{
    if (document == document_all)
        return;

    document_all = document;
    // emitujeme signal
    emit documentChanged();
}

/**
 * @brief Document::cursorPosition
 * @return  pozice cursoru v textaree
 */
int Document::cursorPosition() const
{
    return m_cursorPosition;
}

/**
 * @brief Document::setCursorPosition Funkce nastavi pozici kurzoru
 * @param position
 */
void Document::setCursorPosition(int position)
{
    if (position == m_cursorPosition)
        return;

    m_cursorPosition = position;

    reset();
    emit cursorPositionChanged();
}

/**
 * @brief Document::selectionStart funkce vrati index na zacatku vyberu
 * @return
 */
int Document::selectionStart() const
{
    return m_selectionStart;
}

/**
 * @brief Document::setSelectionStart - nastavime pozici vyberu
 * @param position
 */
void Document::setSelectionStart(int position)
{
    if (position == m_selectionStart)
        return;

    m_selectionStart = position;
    emit selectionStartChanged();
}


/**
 * @brief Document::selectionEnd funkce vrati index konce vyberu
 * @return
 */
int Document::selectionEnd() const
{
    return m_selectionEnd;
}

void Document::setSelectionEnd(int position)
{
    if (position == m_selectionEnd)
        return;

    m_selectionEnd = position;
    emit selectionEndChanged();
}

/**
 * @brief Document::fontFamily Funkce vrati aktualni font
 * @return font
 */
QString Document::fontFamily() const
{
    QTextCursor cursor = textCursor();

    if (cursor.isNull()){
        return QString();
    }

    QTextCharFormat format = cursor.charFormat();
    return format.font().family();
}

/**
 * @brief Document::setFontFamily Funkce nastavi font vybraneho textu
 * @param family
 */
void Document::setFontFamily(const QString &family)
{
    QTextCharFormat format;
    format.setFontFamily(family);
    mergeFormatOnWordOrSelection(format);
    emit fontFamilyChanged();
}

/**
 * @brief Document::textColor Podobne jako fontFamily, zde ale vracime barvu textu
 * @return barva textu
 */
QColor Document::textColor() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return QColor(Qt::black);
    QTextCharFormat format = cursor.charFormat();
    return format.foreground().color();
}

/**
 * @brief Document::setTextColor Fukce nastavi barvu textu
 * @param color
 */
void Document::setTextColor(const QColor &color)
{
    QTextCharFormat format;
    format.setForeground(QBrush(color));
    mergeFormatOnWordOrSelection(format);
    emit textColorChanged();
}

/**
 * @brief Document::alignment Funkce vrati zarovnani textu
 * @return zarovnani textu
 */
Qt::Alignment Document::alignment() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull()){
        return Qt::AlignLeft;
    }

    return textCursor().blockFormat().alignment();
}

void Document::setAlignment(Qt::Alignment alignment)
{
    QTextBlockFormat format;
    format.setAlignment(alignment);
    QTextCursor cursor = textCursor();
    cursor.mergeBlockFormat(format);
    emit alignmentChanged();
}

/*
 *
 * FUNKCE PRO ZMENU FORMATU TEXTU (bold, italic, underline atd.)
 * */

bool Document::bold() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull()){
      return false;
    }

    return textCursor().charFormat().fontWeight() == QFont::Bold;
}

void Document::setBold(bool bold)
{
    QTextCharFormat format;
    format.setFontWeight(bold ? QFont::Bold : QFont::Normal);
    mergeFormatOnWordOrSelection(format);
    emit boldChanged();
}


bool Document::italic() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return false;
    return textCursor().charFormat().fontItalic();
}

void Document::setItalic(bool italic)
{
    QTextCharFormat format;
    format.setFontItalic(italic);
    mergeFormatOnWordOrSelection(format);
    emit italicChanged();
}

bool Document::underline() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return false;
    return textCursor().charFormat().fontUnderline();
}

void Document::setUnderline(bool underline)
{
    QTextCharFormat format;
    format.setFontUnderline(underline);
    mergeFormatOnWordOrSelection(format);
    emit underlineChanged();
}

int Document::fontSize() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return 0;
    QTextCharFormat format = cursor.charFormat();
    return format.font().pointSize();
}

void Document::setFontSize(int size)
{
    if (size <= 0)
        return;

    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return;

    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);

    if (cursor.charFormat().property(QTextFormat::FontPointSize).toInt() == size)
        return;

    QTextCharFormat format;
    format.setFontPointSize(size);
    mergeFormatOnWordOrSelection(format);
    emit fontSizeChanged();
}

/********/

/*
 *
 * UKLADANI A NACITANI DOKUMNETU
*/

/**
 * @brief Document::fileName Funkce vraci nazev dokumentu
 * @return nazev dokumnetu (vychozi untitled.txt)
 */
QString Document::fileName() const
{
    const QString filePath = QQmlFile::urlToLocalFileOrQrc(m_fileUrl);
    const QString fileName = QFileInfo(filePath).fileName();
    if (fileName.isEmpty())
        return QStringLiteral("untitled.txt");
    return fileName;
}

QString Document::fileType() const
{
    return QFileInfo(fileName()).suffix();
}

QUrl Document::fileUrl() const
{
    return m_fileUrl;
}

void Document::load(const QUrl &fileUrl)
{
    if (fileUrl == m_fileUrl)
        // pokud je to stejny soubor, nic nedelame
        return;

    QQmlEngine *engine = qmlEngine(this);
    if (!engine) {
        qWarning() << "load() called before DocumentHandler has QQmlEngine";
        return;
    }

    const QUrl path = QQmlFileSelector::get(engine)->selector()->select(fileUrl);
    const QString fileName = QQmlFile::urlToLocalFileOrQrc(path);
    if (QFile::exists(fileName)) {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly)) {
            QByteArray data = file.readAll();

            QTextCodec *codec = QTextCodec::codecForHtml(data);
            if (QTextDocument *doc = textDocument())
                doc->setModified(false);

            emit loaded(codec->toUnicode(data));
            reset();
        }
    }

    m_fileUrl = fileUrl;
    emit fileUrlChanged();
}
/**
 * @brief Document::saveAs Ukladani souboru
 * @param fileUrl
 */
void Document::saveAs(const QUrl &fileUrl)
{
    QTextDocument *doc = textDocument();
    if (!doc)
        return;

    const QString filePath = fileUrl.toLocalFile();

    const bool isHtml = QFileInfo(filePath).suffix().contains(QLatin1String("htm"));
    QFile file(filePath);
    if (!file.open(QFile::WriteOnly | QFile::Truncate | (isHtml ? QFile::NotOpen : QFile::Text))) {
        emit error(tr("Cannot save: ") + file.errorString());
        return;
    }
    file.write((isHtml ? doc->toHtml() : doc->toPlainText()).toUtf8());
    file.close();

    if (fileUrl == m_fileUrl)
        return;

    m_fileUrl = fileUrl;
    emit fileUrlChanged();
}

/**
 * @brief Document::reset Funkce vse zresetuje
 */
void Document::reset()
{
    emit fontFamilyChanged();
    emit alignmentChanged();
    emit boldChanged();
    emit italicChanged();
    emit underlineChanged();
    emit fontSizeChanged();
    emit textColorChanged();
}

QTextCursor Document::textCursor() const
{
    QTextDocument *doc = textDocument();
    if (!doc)
        return QTextCursor();

    QTextCursor cursor = QTextCursor(doc);
    if (m_selectionStart != m_selectionEnd) {
        cursor.setPosition(m_selectionStart);
        cursor.setPosition(m_selectionEnd, QTextCursor::KeepAnchor);
    } else {
        cursor.setPosition(m_cursorPosition);
    }
    return cursor;
}

QTextDocument *Document::textDocument() const
{
    if (!document_all)
        return nullptr;

    return document_all->textDocument();
}


void Document::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
}

/******* konec document.cpp *********************/
