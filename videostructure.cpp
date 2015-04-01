#include "videostructure.hpp"

namespace Hend
{
    SearchResponseID::SearchResponseID():
        SearchResponseID{ "", "", "", "" }
    {

    }
    SearchResponseID::SearchResponseID( QString const &kind, QString const &videoID,
                                        QString const &channelID, QString const &playlistID ):
        m_kind{ kind },
        m_videoID{ videoID },
        m_channelID{ channelID },
        m_playlistID{ playlistID }
    {

    }
    void SearchResponseID::setKind( QString const &kind )
    {
        m_kind = kind;
    }
    void SearchResponseID::setVideoID( QString const &videoID )
    {
        m_videoID = videoID;
    }

    void SearchResponseID::setChannelID(const QString &channelID)
    {
        m_channelID = channelID;
    }

    void SearchResponseID::setPlaylistID(const QString &playlistID)
    {
        m_playlistID = playlistID;
    }
    QString const &SearchResponseID::kind() const { return m_kind; }
    QString const &SearchResponseID::videoID() const { return m_videoID; }
    QString const &SearchResponseID::channelID() const { return m_channelID; }
    QString const &SearchResponseID::playlistID() const { return m_playlistID; }

    SearchResponseSnippetThumbnailKey::SearchResponseSnippetThumbnailKey():
        SearchResponseSnippetThumbnailKey{ "", 0, 0 }
    {

    }

    SearchResponseSnippetThumbnailKey::SearchResponseSnippetThumbnailKey(
            const QString &url,
            const unsigned int &width,
            const unsigned int &height ):
        m_url{ url },
        m_width{ width },
        m_height{ height }
    {

    }
    void SearchResponseSnippetThumbnailKey::setHeight(const unsigned int &height)
    {
        m_height = height;
    }
    void SearchResponseSnippetThumbnailKey::setWidth(const unsigned int &width)
    {
        m_width = width;
    }
    void SearchResponseSnippetThumbnailKey::setUrl(const QString &url)
    {
        m_url = url;
    }
    unsigned int SearchResponseSnippetThumbnailKey::height() const { return m_height; }
    unsigned int SearchResponseSnippetThumbnailKey::width() const { return m_width; }
    const QString &SearchResponseSnippetThumbnailKey::url() const { return m_url; }

    SearchResponseSnippetThumbnail::SearchResponseSnippetThumbnail():
        m_keys{}
    {

    }

    void SearchResponseSnippetThumbnail::addKey( QString const & key, const SearchResponseSnippetThumbnailKey & value )
    {
        m_keys.insert( key, value );
    }

    SearchResponseSnippet::SearchResponseSnippet():
        m_thumbnails{},
        m_channelID{},
        m_title{},
        m_description{},
        m_publishedAt{},
        m_channelTitle{},
        m_liveBroadcastContent{}
    {

    }

    QDateTime SearchResponseSnippet::toDate( const QString &dateString, const QString &format )
    {
        QDateTime date = QDateTime::fromString( dateString, format );
        return date;
    }

    QDate SearchResponseSnippet::lastMonth( const QDate &today )
    {
        if( !today.isValid() ) return QDate{};

        QDate lastMonth;

        if( today.month() == 1 )
        {
            lastMonth.setDate( today.year() - 1, 12 , 1 );
        } else {
            lastMonth.setDate( today.year(), today.month() - 1, 1 );
        }
        return lastMonth;
    }

    void SearchResponseSnippet::setChannelID(const QString &channelID)
    {
        m_channelID = channelID;
    }
    void SearchResponseSnippet::setTitle(const QString &title)
    {
        m_title = title;
    }
    void SearchResponseSnippet::setDescription(const QString &description)
    {
        m_description = description;
    }
    void SearchResponseSnippet::setPublishedDate(const QString &publishDate)
    {
        m_publishedAt = SearchResponseSnippet::toDate( publishDate );
    }
    void SearchResponseSnippet::setChannelTitle(const QString &channelTitle)
    {
        m_channelTitle = channelTitle;
    }
    void SearchResponseSnippet::setLiveBroadcastContent(const QString &liveBroadcastContent)
    {
        m_liveBroadcastContent = liveBroadcastContent;
    }
    void SearchResponseSnippet::addThumbnail( QString const & key, const SearchResponseSnippetThumbnailKey & thumbnail )
    {
        m_thumbnails.addKey( key, thumbnail );
    }

    QString const &SearchResponseSnippet::channelTitle() const { return m_channelTitle; }
    QString const &SearchResponseSnippet::title() const { return m_title; }
    QString const &SearchResponseSnippet::description() const { return m_description; }
    const QDateTime &SearchResponseSnippet::publishedDate() const { return m_publishedAt; }

    SearchResponse::SearchResponse():
        m_kind{},
        m_etag{},
        m_videoID{},
        m_snippet{}
    {

    }
    SearchResponse::SearchResponse( const QString &kind, const QString &etag,
                                    const SearchResponseID &videoID,
                                    const SearchResponseSnippet &videoSnippet ):
        m_kind{ kind },
        m_etag{ etag },
        m_videoID{ videoID },
        m_snippet{ videoSnippet }
    {

    }
    void SearchResponse::setKind(const QString &kind)
    {
        m_kind = kind;
    }
    void SearchResponse::setEtag(const QString &etag)
    {
        m_etag = etag;
    }
    void SearchResponse::setVideoID(const SearchResponseID videoID)
    {
        m_videoID = videoID;
    }
    void SearchResponse::setSnippet(const SearchResponseSnippet &snippet)
    {
        m_snippet = snippet;
    }
    QString const &SearchResponse::kind() const { return m_kind; }
    QString const &SearchResponse::etag() const { return m_etag; }
    SearchResponseID const &SearchResponse::id() const { return m_videoID; }
    const SearchResponseSnippet &SearchResponse::snippet() const { return m_snippet; }

    PageInfo::PageInfo():
        m_totalResult { 0 },
        m_resultPerPage{ 0 }
    {

    }
    void PageInfo::setTotalResult( int totalResult )
    {
        m_totalResult = totalResult;
    }
    void PageInfo::setResultPerPage(int resultPerPage )
    {
        m_resultPerPage = resultPerPage;
    }

    int PageInfo::totalResult() const { return m_totalResult; }
    int PageInfo::resultPerPage() const { return m_resultPerPage; }

    SearchListResponse::SearchListResponse():
        m_kind{},
        m_etag{},
        m_nextPageToken{},
        m_prevPageToken{},
        m_pageInfo{},
        m_items{}
    {

    }
    void SearchListResponse::setKind(const QString &kind)
    {
        m_kind = kind;
    }
    void SearchListResponse::setEtag(const QString &etag)
    {
        m_etag = etag;
    }
    void SearchListResponse::setNextPageToken(const QString &nextPageToken)
    {
        m_nextPageToken = nextPageToken;
    }
    void SearchListResponse::setPrevPageToken(const QString &prevPageToken)
    {
        m_prevPageToken = prevPageToken;
    }
    void SearchListResponse::setPageInfo(int totalResults, int resultsPerPage)
    {
        m_pageInfo.setResultPerPage( resultsPerPage );
        m_pageInfo.setTotalResult( totalResults );
    }

    void SearchListResponse::addItem( const SearchResponse &item )
    {
        m_items.push_back( item );
    }
    QList<SearchResponse>::size_type SearchListResponse::size() const { return m_items.size(); }
    QString const &SearchListResponse::etag() const { return m_etag; }
    SearchResponse const &SearchListResponse::item(std::size_t i) const { return m_items[i]; }
    QString const &SearchListResponse::kind() const { return m_kind; }
    QString const &SearchListResponse::nextPageToken() const { return m_nextPageToken; }
    QString const &SearchListResponse::prevPageToken() const { return m_prevPageToken; }
    const PageInfo &SearchListResponse::pageInfo() const { return m_pageInfo; }

    VideoStructure::VideoStructure(const QByteArray &json_response):
        m_videoResponse {},
        m_parseError{},
        m_jsonDocument{ },
        m_jsonObject{}
    {
        setNewJsonDocument( json_response );
    }

    void VideoStructure::setNewJsonDocument( QByteArray const & json_document )
    {
        readJsonFile( json_document );
    }

    void VideoStructure::readJsonFile(const QByteArray & json_doc )
    {
        if( !json_doc.isEmpty() ){
            m_jsonDocument = QJsonDocument::fromJson( json_doc, &m_parseError );
            if( m_parseError.error == QJsonParseError::NoError )
            {
                m_jsonObject = m_jsonDocument.object();
                extractVideoProperties();
            }
        }
    }

    VideoStructure::operator bool()
    {
        return m_parseError.error == QJsonParseError::NoError;
    }

    void VideoStructure::extractVideoProperties()
    {
        QString video_kind = m_jsonObject["kind"].toString(),
                video_etag = m_jsonObject["etag"].toString(),
                video_next_page = m_jsonObject["nextPageToken"].toString(),
                video_prev_page = m_jsonObject["prevPageToken"].toString();

        m_videoResponse.setKind( video_kind );
        m_videoResponse.setEtag( video_etag );
        m_videoResponse.setNextPageToken( video_next_page );
        m_videoResponse.setPrevPageToken( video_prev_page );

        QJsonObject video_page_info = m_jsonObject["pageInfo"].toObject();
        if( !video_page_info.isEmpty() )
        {
            int page_info_total_results = video_page_info["totalResults"].toInt(),
                page_info_results_per_page = video_page_info["resultsPerPage"].toInt();
            m_videoResponse.setPageInfo( page_info_total_results, page_info_results_per_page );
        }
        extractSearchResponseItems();
    }

    void VideoStructure::extractSearchResponseItems()
    {
        QJsonArray video_items = m_jsonObject["items"].toArray();
        if( video_items.isEmpty() ) return;

        for( const auto &vi: video_items )
        {
            QJsonObject items = vi.toObject();
            QString video_items_kind = items["kind"].toString(),
                    video_items_etag = items["etag"].toString();
            SearchResponseID video_items_id = extractVideoItemsID( items["id"].toObject() );

            SearchResponseSnippet video_items_snippet = extractVideoItemSnippet( items["snippet"].toObject() );

            m_videoResponse.addItem( SearchResponse{ video_items_kind, video_items_etag ,
                                                     video_items_id, video_items_snippet });
        }
    }

    SearchResponseID VideoStructure::extractVideoItemsID( const QJsonObject & object )
    {
        SearchResponseID search_responseID{};
        if( object.isEmpty() ) return search_responseID;

        QString id_kind = object["kind"].toString(),
                id_videoID = object["videoId"].toString(),
                id_channelID = object["channelId"].toString(),
                id_playlistID = object["playlistId"].toString();

        search_responseID.setKind( id_kind );
        search_responseID.setVideoID( id_videoID );
        search_responseID.setChannelID( id_channelID );
        search_responseID.setPlaylistID( id_playlistID );

        return search_responseID;
    }

    SearchResponseSnippet VideoStructure::extractVideoItemSnippet( QJsonObject const & object )
    {
        SearchResponseSnippet response_snippet{};
        if( object.isEmpty() ) return response_snippet;

        QString snippet_publishDate = object["publishedAt"].toString(),
                snippet_channelID = object["channelId"].toString(),
                snippet_title = object["title"].toString(),
                snippet_description = object["description"].toString(),
                snippet_channelTitle = object["channelTitle"].toString(),
                snippet_liveBroadcatContent = object["liveBroadcastContent"].toString();

        response_snippet.setPublishedDate( snippet_publishDate );
        response_snippet.setChannelID( snippet_channelID );
        response_snippet.setTitle( snippet_title );
        response_snippet.setDescription( snippet_description );
        response_snippet.setChannelTitle( snippet_channelTitle );
        response_snippet.setLiveBroadcastContent( snippet_liveBroadcatContent );

        QJsonObject snippet_thumbnails = object["thumbnails"].toObject();
        if( !snippet_thumbnails.isEmpty() )
        {
            QJsonObject default_key = snippet_thumbnails["default"].toObject(),
                        medium_key = snippet_thumbnails["medium"].toObject(),
                        high_key = snippet_thumbnails["high"].toObject();
            response_snippet.addThumbnail( "default", getSnippetThumbnailKey( default_key ));
            response_snippet.addThumbnail( "medium", getSnippetThumbnailKey( medium_key ) );
            response_snippet.addThumbnail( "high", getSnippetThumbnailKey( high_key ) );
        }
        return response_snippet;
    }

    SearchResponseSnippetThumbnailKey VideoStructure::getSnippetThumbnailKey(QJsonObject const & key )
    {
        SearchResponseSnippetThumbnailKey thumbnail_key{};
        if( !key.isEmpty() )
        {
            thumbnail_key.setUrl( key["url"].toString() );
            thumbnail_key.setWidth( key["width"].toInt() );
            thumbnail_key.setHeight( key["height"].toInt() );
        }
        return thumbnail_key;
    }

    VideoStructure::size_type VideoStructure::getSize() const
    {
        return m_videoResponse.size();
    }
    QString VideoStructure::getKind()
    {
        return m_videoResponse.kind();
    }

    QString VideoStructure::getEtag()
    {
        return m_videoResponse.etag();
    }
    QString VideoStructure::getNextPageToken()
    {
        return m_videoResponse.nextPageToken();
    }
    QString VideoStructure::getPrevPageToken()
    {
        return m_videoResponse.nextPageToken();
    }
    PageInfo VideoStructure::getPageInfo()
    {
        return m_videoResponse.pageInfo();
    }
    const SearchResponse &VideoStructure::getSearchResponse( std::size_t i ) const
    {
        return m_videoResponse.item( i );
    }
}
