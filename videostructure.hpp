#ifndef VIDEOSTRUCTURE_HPP
#define VIDEOSTRUCTURE_HPP

#include <QByteArray>
#include <QMap>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>

namespace Hend
{
    struct SearchResponseID
    {
        SearchResponseID();
        SearchResponseID( QString const & kind, QString const & videoID,
                          QString const & channelID, QString const & playlistID );

        void setKind( QString const & kind );
        void setVideoID( QString const & videoID );
        void setChannelID( QString const & channelID );
        void setPlaylistID( QString const & playlistID );

        QString const &kind() const;
        QString const &videoID() const;
        QString const &channelID() const;
        QString const &playlistID() const;
    private:
        QString m_kind;
        QString m_videoID;
        QString m_channelID;
        QString m_playlistID;
    };

    struct SearchResponseSnippetThumbnailKey
    {
        SearchResponseSnippetThumbnailKey();
        SearchResponseSnippetThumbnailKey( QString const & url,
                                            unsigned int const & width,
                                            unsigned int const & height );
        void setUrl( QString const & url );
        void setWidth( unsigned int const & width );
        void setHeight( unsigned int const & height );

        QString const &url() const;
        unsigned int width() const;
        unsigned int height() const;
    private:
        QString         m_url;
        unsigned int    m_width;
        unsigned int    m_height;
    };

    struct SearchResponseSnippetThumbnail
    {
        SearchResponseSnippetThumbnail();
        void addKey( QString const & key, SearchResponseSnippetThumbnailKey const & value );
    private:
        QMap< QString, SearchResponseSnippetThumbnailKey > m_keys;
    };

    struct SearchResponseSnippet
    {
        static QDateTime toDate( QString const & dateString,
                             QString const & format = "yyyy-MM-ddTHH:mm:ss.zzz'Z'" );
        static QDate lastMonth( QDate const & today = QDate::currentDate() );

    public:
        SearchResponseSnippet();
        void setChannelID( QString const & channelID );
        void setTitle( QString const & title );
        void setDescription( QString const & description );
        void setPublishedDate( QString const & publishDate );
        void setChannelTitle( QString const & channelTitle );
        void setLiveBroadcastContent( QString const & liveBroadcastContent );
        void addThumbnail(QString const &key, const SearchResponseSnippetThumbnailKey &thumbnail );
        void getSnippetThumbnailKey(QJsonObject const&,SearchResponseSnippetThumbnailKey& );

        QString const &channelTitle() const;
        QString const &title() const;
        QDateTime   const &publishedDate() const;
        QString const &description() const;
    private:
        typedef SearchResponseSnippetThumbnail SRST;
        SRST                    m_thumbnails;
        QString                 m_channelID;
        QString                 m_title;
        QString                 m_description;
        QDateTime               m_publishedAt;
        QString                 m_channelTitle;
        QString                 m_liveBroadcastContent;
    };

    struct SearchResponse
    {
        SearchResponse();
        SearchResponse( QString const & kind, QString const & etag,
                        SearchResponseID const & videoID,
                        SearchResponseSnippet const & videoSnippet );
        void setKind( QString const & kind );
        void setEtag( QString const & etag );
        void setVideoID( SearchResponseID const videoID );
        void setSnippet( SearchResponseSnippet const & snippet );

        QString const &kind() const;
        QString const &etag() const;
        SearchResponseID const &id() const;
        SearchResponseSnippet const &snippet() const;
    private:
        QString                 m_kind;
        QString                 m_etag;
        SearchResponseID        m_videoID;
        SearchResponseSnippet   m_snippet;
    };
    struct PageInfo
    {
        PageInfo();
        void setTotalResult( int totalResult );
        void setResultPerPage( int resultPerPage );

        int totalResult() const;
        int resultPerPage() const;
    private:
        int                     m_totalResult;
        int                     m_resultPerPage;
    };

    struct SearchListResponse
    {
        SearchListResponse();
        void setKind( QString const & kind );
        void setEtag( QString const & etag );
        void setNextPageToken( QString const & nextPageToken );
        void setPrevPageToken( QString const & prevPageToken );
        void setPageInfo( int totalResults, int resultsPerPage );
        void addItem( SearchResponse const & item );

        QList<SearchResponse>::size_type size() const;
        QString const &kind() const;
        QString const &etag() const;
        QString const &nextPageToken() const;
        QString const &prevPageToken() const;
        PageInfo const &pageInfo() const;
        SearchResponse const &item( std::size_t i ) const;
    private:
        QString                 m_kind;
        QString                 m_etag;
        QString                 m_nextPageToken;
        QString                 m_prevPageToken;
        PageInfo                m_pageInfo;
        QList<SearchResponse>   m_items;
    };

    class VideoStructure
    {
    public:
        using size_type = QList<SearchResponse>::size_type;
        VideoStructure( QByteArray const & json_response );

        void setNewJsonDocument( QByteArray const & json_document );
        operator bool();

        size_type getSize() const;
        QString getKind();
        QString getEtag();
        QString getNextPageToken();
        QString getPrevPageToken();
        PageInfo getPageInfo();
        SearchResponse const &getSearchResponse( std::size_t i ) const;
    private:
        void readJsonFile( QByteArray const & );
        void extractVideoProperties();
        void extractSearchResponseItems();
        SearchResponseID extractVideoItemsID( QJsonObject const & );
        SearchResponseSnippet extractVideoItemSnippet( QJsonObject const & );
        SearchResponseSnippetThumbnailKey getSnippetThumbnailKey(QJsonObject const & object);

    private:
        SearchListResponse      m_videoResponse;
        QJsonParseError         m_parseError;
        QJsonDocument           m_jsonDocument;
        QJsonObject             m_jsonObject;
    };
}
#endif // VIDEOSTRUCTURE_HPP
