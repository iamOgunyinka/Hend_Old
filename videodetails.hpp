#ifndef VIDEODETAILS_HPP
#define VIDEODETAILS_HPP

#include <QtWidgets>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class VideoDetails : public QWidget
{
    Q_OBJECT
public:
    explicit VideoDetails(QWidget *parent = nullptr);

    template< typename T >
    void setNewDetails( T const & videoDetails )
    {
        QString thumbnailUrl = videoDetails.thumbnail().thumbnailWithKey( "default" ).url();
        QNetworkRequest newRequest { QUrl( thumbnailUrl ) };
        QNetworkReply *reply = m_networkManager.get( newRequest );

        m_detailText->setText( videoDetails.description() );
        m_detailText->setReadOnly( true );
        m_detailText->setToolTip( "Summary of the video descrition" );

        m_titleLabel->setText( videoDetails.title() );
        QFont titleFont = m_titleLabel->font();
        titleFont.setBold( true );
        titleFont.setItalic( true );
        titleFont.setWeight( 40 );
        m_titleLabel->setFont( titleFont );

        m_timestampLabel->setText( videoDetails.publishedDate().toString() );

        QObject::connect( reply, SIGNAL(finished()), this, SLOT(finished()) );
        QObject::connect( reply, SIGNAL(error(QNetworkReply::NetworkError)),
                          this, SLOT(error(QNetworkReply::NetworkError)) );
    }
signals:
    void networkError( QNetworkReply::NetworkError );
private slots:
    void finished();
    void error( QNetworkReply::NetworkError );
private:
    void setupWindow();
private:
    QLabel  *m_thumbnail { nullptr };
    QLabel  *m_titleLabel { nullptr };
    QLabel  *m_timestampLabel { nullptr };
    QLabel  *m_videoLengthLabel { nullptr };
    QTextEdit *m_detailText { nullptr };
    QGridLayout *m_gLayout { nullptr };
    QNetworkAccessManager m_networkManager{};
};

#endif // VIDEODETAILS_HPP
