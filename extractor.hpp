#ifndef EXTRACTOR_HPP
#define EXTRACTOR_HPP

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QEventLoop>
#include <QRegularExpression>
#include <QDomDocument>
#include <QtWidgets>

namespace Hend
{
    typedef QMap<QString, QString> StringMap;
    class Constants
    {
    public:
        static StringMap urls;
        static QString  user_agent;
        static QString  uefsm;
        static QString  af;
        static QString  jsplayer;
        static int      lifespan;
        static QMap< QString, std::tuple<QString, QString, QString, QString>> itags;
    };

    namespace HelperFunctions
    {
        QByteArray          fetchDecode( QString const & original_url );
        StringMap           parseQueryString( QByteArray const & data );
        QString             extractVideoID( QString const & url, bool &err );
        QList<StringMap>    extractSmap( QString const & key, StringMap const & data );
        QList<StringMap>    extractDash( QString const & dashUrl );
        StringMap           getVideoInfo( QString const & videoID );
    }

    class Stream
    {
    public:
        typedef QPair<int, int> IntPair;
        Stream( StringMap const & sm, QString const & title );

        bool threed() const;
        int rawBitRate() const;
        QString const & rawUrl() const;
        QString const & bitRate() const;
        QString const & itag() const;
        QString const & resolution() const;
        QString const & quality() const;
        QString const & title() const;
        QString const & extension() const;
        QString const & mediaType() const;
        QString const & filename() const;
        IntPair         dimension() const;
        QString const & url() const;
        QString         url_https() const;
        void            download();

    private:
        void init( StringMap const & sm, QString const & t );
        QString generateFilename();
        QString m_itag{},
                m_mediaType{},
                m_resolution{},
                m_quality{},
                m_rawUrl{},
                m_url{},
                m_sig{},
                m_title {},
                m_filename {},
                m_note {},
                m_extension{},
                m_videoFormat{},
                m_bitRate{};
        int     m_fsize{},
                m_rawBitRate{};
        IntPair m_dimension{};
        bool    m_isDash{},
                m_active{},
                m_threed{},
                m_encrypted{};
    };

    class UrlFinder
    {
    public:
        UrlFinder( QString const & videoUrl, bool start = false );
        UrlFinder();
        void            startUrlExtraction();
        QString const & getTitle()const;
        QList<Stream>   getVideoStreams() const;
        QList<Stream>   getAudioStreams() const;
        QList<Stream>   getAllStreams() const;
    private:
        void            init_functions();
        void            fetchBasic();
        void            fetchBasicDelegate();
        void            processStreams( QList<StringMap> const & dash);
    private:
        QString             m_videoUrl{},
                            m_title{},
                            m_dashUrl{},
                            m_author{},
                            m_videoRating{},
                            m_length{},
                            m_viewCount{},
                            m_thumb{};
        QString             m_videoID{},
                            m_keyword{},
                            m_bigThumb{},
                            m_bigThumbHD{},
                            m_cipherTag{},
                            m_watchVideoUrl{};
        QList<QStringList>  m_formats {};
        QList<StringMap>    m_sm{};
        QList<StringMap>    m_asm{};
        QList<Stream>       m_streams {};
        QList<Stream>       m_audioStreams{};
        QList<Stream>       m_videoStreams{};
        bool                m_hasBasic { false };
    };

    class FormatSpecifier : public QDialog
    {
        Q_OBJECT

        QVBoxLayout     *vLayout;
        QHBoxLayout     *hLayout;

        QPushButton     *downloadButton;
        QPushButton     *cancelButton;

        UrlFinder       *urlFinder;
        QSignalMapper   *sigMapper;

        QString         currentDownloadLink {};
        QList<Stream>   videoStreams {};
    private:
        void showVideoLinks();
    public:
        explicit FormatSpecifier( QString const & url, QWidget *parent = nullptr );
        QString  getDownloadLink() const;
        QString  title() const;
    private slots:
        void downloadLink( int index );
    };
}

#endif // EXTRACTOR_HPP
