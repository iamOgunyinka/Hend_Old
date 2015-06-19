#include "videodetails.hpp"

VideoDetails::VideoDetails(QWidget *parent) :
    QWidget(parent)
{
    setupWindow();
}

void VideoDetails::setupWindow()
{
    m_gLayout = new QGridLayout;
    m_titleLabel = new QLabel();
    m_timestampLabel = new QLabel();
    m_videoLengthLabel = new QLabel();
    m_detailText = new QTextEdit();
    m_detailText->setReadOnly( true );

    m_thumbnail = new QLabel();
    m_thumbnail->setBackgroundRole( QPalette::Dark );
    m_thumbnail->setScaledContents( true );

    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addWidget( m_thumbnail );
    hLayout->addWidget( m_detailText );

    m_gLayout->addWidget( m_titleLabel, 0, 0 );
    m_gLayout->addLayout( hLayout, 1, 0 );
    m_gLayout->addWidget( m_timestampLabel, 2, 0 );

    setLayout( m_gLayout );
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
        if( pixmap.height() > m_detailText->height() || pixmap.width() > m_detailText->width() ){
            pixmap = pixmap.scaled( QSize( 80, 50 ), Qt::KeepAspectRatio );
        }
        m_thumbnail->setPixmap( pixmap );
    }
}
