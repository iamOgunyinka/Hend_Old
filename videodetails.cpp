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

    m_thumbnail = new QLabel();
    m_thumbnail->setBackgroundRole( QPalette::Dark );
    m_thumbnail->setScaledContents( true );

    m_gLayout->addWidget( m_titleLabel, 0, 0 );
    m_gLayout->addWidget( m_thumbnail, 1, 0 );
    m_gLayout->addWidget( m_timestampLabel, 2, 0 );
    m_gLayout->addWidget( m_detailText, 1, 1 );

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
        m_thumbnail->setPixmap( pixmap );
    }
}
