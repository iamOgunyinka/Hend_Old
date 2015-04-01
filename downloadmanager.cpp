#include "downloadmanager.hpp"

Download::Download( QObject *parent ) :
    QObject(parent), m_networkManager( new QNetworkAccessManager ),
    m_networkRequest{}, m_networkReply{ nullptr }, m_downloadRate{ 0 },
    m_file{ nullptr }
{
}

void Download::cancel()
{
    if( !m_networkReply ) return;

    m_file->remove();
    QObject::disconnect( m_networkReply, SIGNAL(downloadProgress(qint64,qint64)),
                         this, SLOT(downloadProgress(qint64,qint64)) );
    QObject::disconnect( m_networkReply, SIGNAL(finished()), this, SLOT(finished()) );
    QObject::disconnect( m_networkReply, SIGNAL(error(QNetworkReply::NetworkError)),
                         this, SLOT(networkError(QNetworkReply::NetworkError)) );

    m_networkReply->abort();
    m_networkReply = nullptr;
}

void Download::pause()
{
    if( !m_networkReply ) return;

    m_file->write( m_networkReply->readAll() );
    QObject::disconnect( m_networkReply, SIGNAL(downloadProgress(qint64,qint64)),
                         this, SLOT(downloadProgress(qint64,qint64)) );
    QObject::disconnect( m_networkReply, SIGNAL(finished()), this, SLOT(finished()) );
    QObject::disconnect( m_networkReply, SIGNAL(error(QNetworkReply::NetworkError)),
                         this, SLOT(networkError(QNetworkReply::NetworkError)) );

    m_networkReply->abort();
    m_networkReply = nullptr;
}

void Download::resume()
{
    m_downloadRate = m_file->size();
    QByteArray range = "bytes=" + QByteArray::number( m_downloadRate ) + "-";
    m_networkRequest.setRawHeader( "Range", range );
    download( m_networkRequest );
}

void Download::download( QNetworkRequest const &request)
{
    m_networkReply = m_networkManager->get( request );

    QObject::connect( m_networkReply, SIGNAL( downloadProgress( qint64,qint64 ) ),
                      this, SLOT( downloadProgress( qint64,qint64 )) );
    QObject::connect( m_networkReply, SIGNAL( finished() ), this, SLOT( finished() ) );
    QObject::connect( m_networkReply, SIGNAL( error( QNetworkReply::NetworkError ) ),
                      this, SLOT( networkError( QNetworkReply::NetworkError ) ) );
}

void Download::download( QString const &url, QString const & filename )
{
    QUrl downloadUrl( url );
    QString localFilename = filename.isEmpty() ? ( downloadUrl.fileName() + QString( ".part" ) ):
                                                 filename;
    m_networkRequest = QNetworkRequest( downloadUrl );
    m_file = new QFile( localFilename );
    m_file->open( QIODevice::ReadWrite );

    download( m_networkRequest );
}

void Download::networkError(QNetworkReply::NetworkError err )
{
    emit error( err );
}

void Download::downloadProgress(qint64 done, qint64 total)
{
    if( total == 0 ) return;

    m_file->write( m_networkReply->readAll() );
    int percentage = ( ( m_downloadRate + done ) * 100 ) / ( total + m_downloadRate );
    emit downloadProgress( percentage );
}

void Download::finished()
{
    QString newFilename = m_file->fileName();
    newFilename.replace( ".part", "" );

    m_file->close();
    m_downloadRate = 0;
    m_file->rename( m_file->fileName(), newFilename );
    emit downloadComplete();
}

DownloadManager::DownloadManager(const QString &url, QString const & filename,
                                 QString const & title, QWidget *parent):
    QWidget( parent ), m_downloadManager( new Download ),
    m_buttonCancel( new QPushButton("Cancel")), m_buttonPause( new QPushButton( "Pause") ),
    m_buttonResume( new QPushButton("Resume") ), m_progressBar( new QProgressBar ),
    m_gLayout( new QGridLayout )
{
    m_progressBar->setRange( 1, 100 );
    m_gLayout->addWidget( new QLabel(title), 0, 0 );
    m_gLayout->addWidget( m_progressBar, 1, 0 );
    m_gLayout->addWidget( m_buttonPause, 1, 1 );
    m_gLayout->addWidget( m_buttonResume, 1, 2 );
    m_gLayout->addWidget( m_buttonCancel, 1, 3 );
    m_downloadManager->download( url, filename );

    QObject::connect( m_downloadManager , SIGNAL(downloadProgress(int)),
                      m_progressBar, SLOT(setValue(int)) );
    QObject::connect( m_buttonPause, SIGNAL(clicked()), this, SLOT( paused()) );
    QObject::connect( m_buttonCancel, SIGNAL(clicked()), this, SLOT(cancelled()) );
    QObject::connect( m_buttonResume, SIGNAL(clicked()), this, SLOT(resumed()) );

    setLayout( m_gLayout );
}

void DownloadManager::paused()
{
    m_buttonPause->setEnabled( false );
    m_buttonResume->setEnabled( true );
    m_downloadManager->pause();
}

void DownloadManager::resumed()
{
    m_buttonResume->setEnabled( false );
    m_buttonPause->setEnabled( true );
    m_downloadManager->resume();
}

void DownloadManager::cancelled()
{
    switch( QMessageBox::information( this, "Cancel", "This will cancel the current download and remove it from the download queue. Continue?", QMessageBox::Yes | QMessageBox::No ) )
    {
    case QMessageBox::Yes:
        m_downloadManager->cancel();
        disableAllButtons();
        break;
    default:;
    }
}

void DownloadManager::disableAllButtons()
{
    m_buttonResume->setEnabled( false );
    m_buttonCancel->setEnabled( false );
    m_buttonPause->setEnabled( false );
}

DownloadManagerWidget::DownloadManagerWidget(QWidget *parent):
    QWidget( parent ),
    m_windowLayout( new QVBoxLayout )
{
    setLayout( m_windowLayout );
}

void DownloadManagerWidget::addDownload(const QString &url, const QString &filename,
                                        const QString & title)
{
    m_windowLayout->addWidget( new DownloadManager( url, filename, title ));
}
