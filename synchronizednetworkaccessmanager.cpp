#include "synchronizednetworkaccessmanager.hpp"

ProgressBar::ProgressBar(QWidget *parent):
    QDialog{ parent }, m_progressBar{ new QProgressBar }
{
    m_progressBar->setRange( 1, 100 );
    m_progressBar->setTextVisible( true );

    QVBoxLayout *layout = new QVBoxLayout( this );
    layout->addWidget( new QLabel("Loading, please wait...") );
    layout->addWidget( m_progressBar );
    setLayout( layout );
}

void ProgressBar::setProgressStatus( qint64 done )
{
    m_progressBar->setValue( done );
    if( done == 100 ){
        reset();
        accept();
    }
}

void ProgressBar::reset()
{
    m_progressBar->reset();
}

SyncNetworkAccessManager::SyncNetworkAccessManager( QObject *parent ):
    QObject( parent ), m_progressBar{ new ProgressBar }
{
    m_progressBar->setWindowFlags( Qt::Dialog | Qt::FramelessWindowHint |
                                   Qt::WindowTitleHint );
    m_progressBar->setWindowModality( Qt::ApplicationModal );
}

SyncNetworkAccessManager::~SyncNetworkAccessManager(){}

void SyncNetworkAccessManager::getRequest( QUrl const & url )
{
    QNetworkRequest request{ url };
    request.setRawHeader( "USER-AGENT", "Hend" );
    QNetworkReply *reply = m_networkManager.get( request );

    QObject::connect( reply, SIGNAL(finished()), this, SLOT(onFinished()) );
    QObject::connect( reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(onDownloadProgress(qint64, qint64)));
    QObject::connect( reply, SIGNAL( error( QNetworkReply::NetworkError )),
                      this, SLOT( onNetworkError( QNetworkReply::NetworkError )) );
    m_progressBar->show();
    m_eventLoop.exec();
}

void SyncNetworkAccessManager::onFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>( sender() );
    if( !reply || reply->error() != QNetworkReply::NoError ){
        throw FailedAttempt{ reply->errorString().toStdString().c_str() };
    }
    m_progressBar->accept();
    m_eventLoop.quit();
    result = reply->readAll();
}

void SyncNetworkAccessManager::onNetworkError( QNetworkReply::NetworkError error )
{
    m_progressBar->accept();
    m_eventLoop.quit();
    switch( error )
    {
    case QNetworkReply::NetworkError::RemoteHostClosedError:
        throw FailedAttempt( "Remote Host Connection Closed." );
    case QNetworkReply::NetworkError::HostNotFoundError:
        throw FailedAttempt( "Host not found" );
    case QNetworkReply::NetworkError::TimeoutError:
        throw FailedAttempt( "Network Timed Out" );
    case QNetworkReply::NetworkError::TemporaryNetworkFailureError:
        throw FailedAttempt( "Temporary Network Failure" );
    default:
        break;
    }
}

void SyncNetworkAccessManager::onDownloadProgress(qint64 received, qint64 total )
{
    if( total == 0 ) return;
    qint64 progressStatus = ( received * 100 )/ total;
    m_progressBar->setProgressStatus( progressStatus );
}
