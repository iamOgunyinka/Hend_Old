#include "synchronizednetworkaccessmanager.hpp"

SyncNetworkAccessManager::SyncNetworkAccessManager( QObject *parent ): QObject( parent ) {}
SyncNetworkAccessManager::~SyncNetworkAccessManager(){}

void SyncNetworkAccessManager::getRequest( QUrl const & url )
{
    QNetworkRequest request{ url };
    request.setRawHeader( "USER-AGENT", "Hend" );
    QNetworkReply *reply = m_networkManager.get( request );

    QObject::connect( reply, SIGNAL( finished() ), this, SLOT( onFinished() ) );
    QObject::connect( reply, SIGNAL( error( QNetworkReply::NetworkError )),
                      this, SLOT( onNetworkError( QNetworkReply::NetworkError )) );

    m_eventLoop.exec();
}

void SyncNetworkAccessManager::onFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>( sender() );
    if( !reply || reply->error() != QNetworkReply::NoError ){
        throw FailedAttempt{ reply->errorString().toStdString().c_str() };
    }
    m_eventLoop.quit();
    result = reply->readAll();
}

void SyncNetworkAccessManager::onNetworkError( QNetworkReply::NetworkError error )
{
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
