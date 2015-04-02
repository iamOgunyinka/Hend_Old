#include "videodetails.hpp"

VideoDetails::VideoDetails(QWidget *parent) :
    QWidget(parent)
{
    setupWindow();
}

void VideoDetails::setupWindow()
{
    m_hLayout = new QHBoxLayout( this );
    m_titleLabel = new QLabel();
    m_timestampLabel = new QLabel();
    m_videoLengthLabel = new QLabel();
    m_detailText = new QTextEdit();

    m_thumbnail = new QLabel();
    m_thumbnail->setBackgroundRole( QPalette::Dark );
    m_thumbnail->setScaledContents( true );

    QVBoxLayout *hThumbnailLayout = new QVBoxLayout;
    hThumbnailLayout->addWidget( m_thumbnail );
    hThumbnailLayout->addWidget( m_videoLengthLabel );
    hThumbnailLayout->addWidget( m_timestampLabel );

    QVBoxLayout *hDetailsLayout = new QVBoxLayout;
    hDetailsLayout->addWidget( m_titleLabel );
    hDetailsLayout->addWidget( m_detailText );

    m_hLayout->addLayout( hThumbnailLayout );
    m_hLayout->addLayout( hDetailsLayout );
}

void VideoDetails::error(QNetworkReply::NetworkError error )
{
    emit networkError( error );
}

void VideoDetails::finished()
{
    QNetworkReply *reply = qobject_cast< QNetworkReply *>( sender() );
    if( !reply ) return;
    if( reply->error() == QNetworkReply::NoError )
    {
        QPixmap pixmap{};
        pixmap.loadFromData( reply->readAll() );
        m_thumbnail->setPixmap( pixmap );
    }
}
