#include "extractor.hpp"

namespace Hend
{
    QString Constants::user_agent { "Hend" };
    QString Constants::uefsm { "url_encoded_fmt_stream_map" };
    QString Constants::af { "adaptive_fmts" };
    QString Constants::jsplayer  = "(;\\s*ytplayer\\.config\\s*=\\s*(\\s*{.*?}\\s*)\\s*;)";

    int Constants::lifespan = 60 * 60 * 5;
    StringMap Constants::urls {
        { "gdata", "http://gdata.youtube.com/feeds/api/videos/%1?v=2" },
        { "watch_v", "http://www.youtube.com/watch?v=%1" },
        { "vidinfo", "http://www.youtube.com/get_video_info?video_id=%1&asv=3&el=detailpage&hl=en_US" },
        { "playlist", "http://www.youtube.com/list_ajax?style=json&action_get_list=1&list=%1" },
        { "age_vidinfo", "http://www.youtube.com/get_video_info?video_id=%1&eurl="
            "https://youtube.googleapis.com/v/%2&sts=1588"
        }
    };
    QMap< QString, std::tuple<QString, QString, QString, QString>> Constants::itags {
        { "5", std::make_tuple( "320x240", "flv", "normal", "" ) },
        { "17", std::make_tuple( "176x144", "3gp", "normal", "" ) },
        { "18", std::make_tuple( "640x360", "mp4", "normal", "" ) },
        { "22", std::make_tuple( "1280x720", "mp4", "normal", "") },
        { "34", std::make_tuple( "640x360", "flv", "normal", "" )},
        { "35", std::make_tuple( "854x480", "flv", "normal", "" )},
        { "36", std::make_tuple( "320x240", "3gp", "normal", "" )},
        { "37", std::make_tuple( "1920x1080", "mp4", "normal", "" )},
        { "38", std::make_tuple( "4096x3072", "mp4", "normal", "4,3 hi-res" )} ,
        { "43", std::make_tuple( "640x360", "webm", "normal", "" )},
        { "44", std::make_tuple( "854x480", "webm", "normal", "" )},
        { "45", std::make_tuple( "1280x720", "webm", "normal", "" )},
        { "46", std::make_tuple( "1920x1080", "webm", "normal", "" )},
        { "82", std::make_tuple( "640x360-3D", "mp4", "normal", "" )},
        { "83", std::make_tuple( "640x480-3D", "mp4", "normal", "" )},
        { "84", std::make_tuple( "1280x720-3D", "mp4", "normal", "" )},
        { "100", std::make_tuple( "640x360-3D", "webm", "normal", "" )},
        { "102", std::make_tuple( "1280x720-3D", "webm", "normal", "" )},
        { "133", std::make_tuple( "426x240", "m4v", "video", "" )},
        { "134", std::make_tuple( "640x360", "m4v", "video", "" )},
        { "135", std::make_tuple( "854x480", "m4v", "video", "")},
        { "136", std::make_tuple( "1280x720", "m4v", "video", "")},
        { "137", std::make_tuple( "1920x1080", "m4v", "video", "")},
        { "138", std::make_tuple( "4096x3072", "m4v", "video", "")},
        { "139", std::make_tuple( "48k", "m4a", "audio", "")},
        { "140", std::make_tuple( "128k", "m4a", "audio", "")},
        { "141", std::make_tuple( "256k", "m4a", "audio", "")},
        { "160", std::make_tuple( "256x144", "m4v", "video", "")},
        { "167", std::make_tuple( "640x480", "webm", "video", "")},
        { "168", std::make_tuple( "854x480", "webm", "video", "")},
        { "169", std::make_tuple( "1280x720", "webm", "video", "")},
        { "170", std::make_tuple( "1920x1080", "webm", "video", "")},
        { "171", std::make_tuple( "128k", "ogg", "audio", "")},
        { "172", std::make_tuple( "192k", "ogg", "audio", "")},
        { "218", std::make_tuple( "854x480", "webm", "video", "VP8")},
        { "219", std::make_tuple( "854x480", "webm", "video", "VP8")},
        { "242", std::make_tuple( "360x240", "webm", "video", "VP9")},
        { "243", std::make_tuple( "480x360", "webm", "video", "VP9")},
        { "244", std::make_tuple( "640x480", "webm", "video", "VP9 low")},
        { "245", std::make_tuple( "640x480", "webm", "video", "VP9 med")},
        { "246", std::make_tuple( "640x480", "webm", "video", "VP9 high")},
        { "247", std::make_tuple( "720x480", "webm", "video", "VP9")},
        { "248", std::make_tuple( "1920x1080", "webm", "video", "VP9")},
        { "249", std::make_tuple( "48k", "ogg", "audio", "Opus")},
        { "250", std::make_tuple( "56k", "ogg", "audio", "Opus")},
        { "251", std::make_tuple( "128k", "ogg", "audio", "Opus")},
        { "256", std::make_tuple( "192k", "m4a", "audio", "6-channel")},
        { "258", std::make_tuple( "320k", "m4a", "audio", "6-channel")},
        { "264", std::make_tuple( "2560x1440", "m4v", "video", "")},
        { "266", std::make_tuple(  "3840x2160", "m4v", "video", "AVC")},
        { "271", std::make_tuple( "1920x1280", "webm", "video", "VP9")},
        { "272", std::make_tuple( "3414x1080", "webm", "video", "VP9")},
        { "278", std::make_tuple( "256x144", "webm", "video", "VP9")},
        { "298", std::make_tuple( "1280x720", "m4v", "video", "60fps")},
        { "299", std::make_tuple( "1920x1080", "m4v", "video", "60fps")},
        { "302", std::make_tuple( "1280x720", "webm", "video", "VP9")},
        { "303", std::make_tuple( "1920x1080", "webm", "video", "VP9")}
    };

    namespace HelperFunctions
    {
        QByteArray fetchDecode( QString const & original_url )
        {
            QString url = original_url;
            while( url.contains( '%') )
            {
                url = QUrl::fromPercentEncoding( url.toUtf8() );
            }
            QNetworkAccessManager network_manager;
            QNetworkRequest request { QUrl( url ) };
            request.setRawHeader( QString("USER-AGENT").toUtf8(), QString{ Constants::user_agent }.toUtf8() );

            QEventLoop wait_reply;
            QNetworkReply *reply = network_manager.get( request );
            QObject::connect( &network_manager, SIGNAL(finished(QNetworkReply*)), &wait_reply, SLOT(quit()));
            QObject::connect( reply, SIGNAL(error(QNetworkReply::NetworkError)), &wait_reply, SLOT(quit()) );
            wait_reply.exec();

            if( !reply || reply->error() != QNetworkReply::NoError ) return QByteArray{};

            return reply->readAll();
        }

        inline StringMap parseQueryString( QByteArray const & data )
        {
            QUrlQuery urlQuery( data );
            QList<QPair<QString, QString>> list = urlQuery.queryItems();

            StringMap map{};
            if( !list.isEmpty() ){
                for( auto const & m: list ){
                    map.insert( m.first, m.second );
                }
            }
            return map;
        }

        QString extractVideoID( QString const & url, bool &err )
        {
            QString ws = "\\w-";
            QString reg_re = "(?:^|[^%1]+)([%2]{11})(?:[^%3]+|$)";
            QRegularExpression re { reg_re.arg( ws ).arg( ws ).arg( ws ) };
            QRegularExpressionMatch match = re.match( url );
            if( !match.hasMatch() ){
                err = true;
                return QString{ "Need 11 characters of the video URL ID" };
            }
            return match.captured(1);
        }

        QList<StringMap> extractSmap( QString const & key, StringMap const & data )
        {
            QList<StringMap> list_of_smaps{};
            if( data.contains( key ) )
            {
                QString smap = data.value( key );
                QStringList splitted_smap = smap.split( "," );
                if( splitted_smap.size() == 1 )
                {
                    splitted_smap = smap.split( "%2C" );
                }
                for( const QString &s: splitted_smap ){
                    list_of_smaps.push_back( parseQueryString( s.toUtf8() ));
                }
            }
            return list_of_smaps;
        }

        QList<StringMap> extractDash( QString const & dashUrl )
        {
            QByteArray dashdata = fetchDecode( dashUrl );
            if( dashdata.isEmpty() ) return {};

            QString ns = "urn:mpeg:DASH:schema:MPD:2011";
            QDomDocument xmlDocument;
            if( !xmlDocument.setContent( dashdata, true ) ) return {};
            
            QDomElement root = xmlDocument.documentElement();
            QDomNodeList tlist = root.elementsByTagNameNS( ns, "Representation" );
            QRegularExpression re ( "(?:\\?|&)mime=([\\w\\d\\/]+)" );
            QList<StringMap> dashmap{};

            for( int index = 0; index != tlist.size(); ++index )
            {
                QDomNode node = tlist.item( index );
                if( node.isElement() )
                {
                    QDomElement x = node.toElement();
                    QDomNodeList node_list = x.elementsByTagName( "BaseURL" );
                    QDomElement base_url = node_list.at( 0 ).toElement();
                    QString url = base_url.text();
                    QString size = base_url.attribute( "contentLength", "0" );
                    QString bitrate = x.attribute( "bandwidth" );
                    QString itag = x.attribute( "id" );
                    QString width = x.attribute( "width" );
                    QString height = x.attribute( "height" );
                    QString type = re.match( url ).captured(1);
                    dashmap.push_back( StringMap {
                                           { "bitrate", bitrate }, { "dash", "true" }, { "itag", itag },
                                           { "width", width }, { "height", height }, { "url", url },
                                           { "size", size }, { "type", type }
                                       });
                }
            }
            return dashmap;
        }

        StringMap getVideoInfo( QString const & videoID )
        {
            QString url = Constants::urls["vidinfo"].arg( videoID );
            QByteArray info = fetchDecode( url );
            if( info.isEmpty() ) return {};

            return parseQueryString( info );
        }
    }
    Stream::Stream( StringMap const & sm, QString const & title )
    {
        init( sm, title );
    }
    void Stream::init( StringMap const & sm, QString const & t )
    {
        m_itag = sm.value( "itag" );
        m_isDash = sm.contains( "dash" );
        m_title = t;
        if( !Constants::itags.contains( m_itag ) ) return;

        m_mediaType = std::get<2>( Constants::itags.value( m_itag ) );
        m_threed = ( sm.contains( "stereo3d" ) && sm.value( "stereo3d" ) == "1" );

        if( m_isDash )
        {
            if ( sm["width"] != QString("None") || !sm["width"].isEmpty() )
            {
                m_resolution = QString("%1x%2").arg(sm["width"]).arg(sm["height"]);
                m_quality = m_resolution;
                m_dimension = { sm["width"].toInt(), sm["height"].toInt() };
            } else {
                m_resolution = "0x0";
                m_dimension = { 0, 0 };
                m_rawBitRate = sm["bitrate"].toInt();
                m_bitRate = std::get<0>( Constants::itags[m_itag] );
                m_quality = m_bitRate;
            }
            m_fsize = sm["size"].toInt();

        } else {
            m_resolution = std::get<0>( Constants::itags[m_itag] );
            m_bitRate = m_rawBitRate = m_fsize = 0;
            //~ m_dimension = { m_resolution.split("-")[0].toInt(), m_resolution.split("x")[1].toInt() };
            m_quality = m_resolution;
        }
        m_videoFormat = sm["type"].split(";")[0];
        m_extension = std::get<1>( Constants::itags[m_itag] );
        m_encrypted = sm.contains( "s" );
        m_note = std::get<3>( Constants::itags[m_itag] );
        m_url = "";
        m_rawUrl = sm["url"];
        m_sig = ( m_encrypted ? sm["s"] : sm["sig"] );
        m_active = false;

        if( m_mediaType == "audio" && !m_isDash )
        {
            m_dimension = { 0, 0 };
            m_bitRate = m_resolution;
            m_quality = m_bitRate;
            m_resolution = "0x0";
            m_rawBitRate = sm["bitrate"].toInt();
        }
    }

    bool Stream::threed() const { return m_threed; }
    int Stream::rawBitRate() const { return m_rawBitRate; }
    QString const & Stream::rawUrl() const { return m_rawUrl; }
    QString const & Stream::bitRate() const { return m_bitRate; }
    QString const & Stream::itag() const { return m_itag; }
    QString const & Stream::resolution() const { return m_resolution; }
    QString const & Stream::quality() const { return m_quality; }
    QString const & Stream::title() const { return m_title; }
    QString const & Stream::extension() const { return m_extension; }
    QString const & Stream::mediaType() const { return m_mediaType; }
    QString const & Stream::filename() const { return m_filename; }
    Stream::IntPair Stream::dimension() const { return m_dimension; }
    QString Stream::url_https() const
    {
        QString temp = m_url;
        return temp.replace( "http", "https" );
    }

    UrlFinder::UrlFinder( QString const & videoUrl, bool start ): m_videoUrl( videoUrl )
    {
        if( start ){
            init_functions();
        }
    }
    void UrlFinder::startUrlExtraction()
    {
        if( !m_hasBasic ){
            init_functions();
        }
    }

    UrlFinder::UrlFinder() = delete;
    QString const & UrlFinder::getTitle() const { return m_title; }
    void UrlFinder::init_functions()
    {
        bool extractionFailed = false;
        m_videoID = HelperFunctions::extractVideoID( m_videoUrl, extractionFailed );
        if( extractionFailed ) return;

        m_watchVideoUrl = Constants::urls["watch_v"].arg( m_videoID );
        fetchBasic();
    }
    void UrlFinder::fetchBasic()
    {
        if( m_hasBasic )
            return;

        m_hasBasic = true;
        fetchBasicDelegate();
        auto dash = HelperFunctions::extractDash( m_dashUrl );
        if( dash.isEmpty() ) return;

        processStreams( dash );
    }

    void UrlFinder::fetchBasicDelegate()
    {
        auto elem = HelperFunctions::getVideoInfo( m_videoID );
        if( elem.isEmpty() ) return;

        auto get_lst = [&]( QString const & key, QString const & default_ = "unknown" ){
                            return elem.value( key, default_ );
                        };

        m_title = get_lst( "title" );
        m_dashUrl = get_lst( "dashmpd" );
        m_author = get_lst( "author" );
        m_videoID = get_lst( "video_id" );
        m_videoRating = get_lst( "avg_rating", "0.0" );
        m_length = get_lst( "length_seconds", "0" );
        m_viewCount = get_lst( "view_count", "0" );
        m_thumb = get_lst( "thumbnail_url" );

        QStringList splitFormatByComma = get_lst( "fmt_lst" ).split(",");
        for ( QString const & sp: splitFormatByComma ) {
            m_formats.push_back( sp.split( "/" ) );
        }
        m_keyword = get_lst( "keywords", "" );
        m_bigThumb = get_lst( "iurlsd", "" );
        m_bigThumbHD = get_lst( "iurlsdmaxres", "" );
        m_cipherTag = get_lst( "use_cipher_signature" ) == "True";
        m_sm = HelperFunctions::extractSmap( Constants::uefsm, elem );
        m_asm = HelperFunctions::extractSmap( Constants::af, elem );
    }
    void UrlFinder::processStreams( QList<StringMap> const & dash)
    {
        if( !m_hasBasic ) fetchBasic();

        for( auto const & sm: m_sm ){
            Stream stream = Stream( sm, getTitle() );
            if( Constants::itags.contains( stream.itag() ) )
                m_streams.push_back( stream );
        }
        QList<Stream> adptStreams{}, dashStreams{};
        for( int i = 0; i != m_asm.size(); ++i ){
            Stream temp = Stream( m_asm.at( i ), getTitle() );
            if( Constants::itags.contains( temp.itag() ) ){
                adptStreams.push_back( temp );
            }
        }

        for( auto const & d: dash ){
            Stream stream = Stream( d, getTitle() );
            if( Constants::itags.contains( stream.itag() ) )
                dashStreams.push_back( stream );
        }

        QList<Stream> audioStreams, videoStreams;
        std::for_each( adptStreams.begin(), adptStreams.end(), [&]( Stream const & stream )mutable->void{
            stream.bitRate().isEmpty() ? videoStreams.push_back( stream ) : audioStreams.push_back( stream );
        });
        QList<QString> dash_itags{};
        for( Stream const & stream : dashStreams ) dash_itags.push_back( stream.itag() );

        for( Stream const &stream: audioStreams )
        {
            if( !dash_itags.contains( stream.itag() ) ){
                m_audioStreams.push_back( stream );
            }
        }
        for( Stream const & stream: videoStreams )
        {
            if( !dash_itags.contains( stream.itag() )){
                m_videoStreams.push_back( stream );
            }
        }
        for( Stream const & stream: dashStreams )
        {
            if( stream.mediaType() == QString( "audio" ) ){
                m_audioStreams.push_back( stream );
            } else {
                m_videoStreams.push_back( stream );
            }
        }
        auto sortByBitrate = []( Stream const &a, Stream const &b ){ return a.extension() < b.extension(); };
        std::sort( m_audioStreams.begin(), m_audioStreams.end(), sortByBitrate );
        std::sort( m_videoStreams.begin(), m_videoStreams.end(), sortByBitrate );
    }
    QList<Stream> UrlFinder::getVideoStreams() const { return m_videoStreams; }
    QList<Stream> UrlFinder::getAudioStreams() const { return m_audioStreams; }
    QList<Stream> UrlFinder::getAllStreams() const { return m_streams; }


    FormatSpecifier::FormatSpecifier(const QString &url, QWidget *parent): QDialog( parent ),
        vLayout{ new QVBoxLayout }, hLayout{ new QHBoxLayout },
        downloadButton{ new QPushButton("Download") }, cancelButton{ new QPushButton("Cancel") },
        urlFinder{ new UrlFinder( url ) }, sigMapper{ new QSignalMapper }
    {
        urlFinder->startUrlExtraction();
        showVideoLinks();
        QObject::connect( cancelButton, SIGNAL(clicked()), this, SLOT(reject()) );
    }

    QString FormatSpecifier::getDownloadLink() const
    {
        return currentDownloadLink;
    }
    QString FormatSpecifier::title() const
    {
        return videoStreams.size() > 0 ? videoStreams.at(0).title() : QString();
    }
    void FormatSpecifier::showVideoLinks()
    {
        videoStreams = urlFinder->getVideoStreams();
        int sizeOfVideo = videoStreams.size();
        QString text {};
        QGridLayout *gLayout = new QGridLayout;

        if( sizeOfVideo > 0 ){
            gLayout->addWidget( new QLabel(videoStreams.at(0).title() ), 0, 0 );
            for( int i = 0; i != sizeOfVideo; ++i )
            {
                text = QString( "%1 | %2" ).arg( videoStreams.at( i ).extension() )
                        .arg( videoStreams.at(i).resolution() );
                QRadioButton *newButton = new QRadioButton( text );
                gLayout->addWidget( newButton, ( i + 1 ), 0 );
                QObject::connect( newButton, SIGNAL(clicked()), sigMapper, SLOT(map()) );
                sigMapper->setMapping( newButton, i );
            }
            QVBoxLayout *vLayout = new QVBoxLayout;
            vLayout->addWidget( downloadButton );
            vLayout->addWidget( cancelButton );

            hLayout->addLayout( gLayout );
            hLayout->addLayout( vLayout );
        } else {
            gLayout->addWidget( new QLabel("Unable to find any link associated with this video"), 0, 0 );
            downloadButton->setEnabled( false );
        }
        setLayout( hLayout );
        QObject::connect( sigMapper, SIGNAL(mapped(int)), this, SLOT(downloadLink(int)) );
        QObject::connect( downloadButton, SIGNAL(clicked()), this, SLOT( accept()) );
    }

    void FormatSpecifier::downloadLink( int index )
    {
        currentDownloadLink = videoStreams.at( index ).rawUrl();
    }
}
