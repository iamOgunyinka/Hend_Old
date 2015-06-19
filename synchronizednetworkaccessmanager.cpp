#include "synchronizednetworkaccessmanager.hpp"

SynchronizedNetworkAccessManager::SynchronizedNetworkAccessManager( QString const &url, QObject *parent):
    QObject(parent),
    m_request( QUrl( url ) )
{
    m_request.setRawHeader( QString("USER-AGENT").toUtf8(), QString{ "Hend" }.toUtf8() );
    QNetworkReply *reply = m_networkManager.get( m_request );

    QObject::connect( reply, SIGNAL(finished()), this, SLOT( onFinished()) );
    QObject::connect( &m_networkManager, SIGNAL(networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)),
                      this, SLOT( onNetworkAccessibilityChanged( QNetworkAccessManager::NetworkAccessibility)) );
    QObject::connect( reply, SIGNAL( error( QNetworkReply::NetworkError )), this,
                      SLOT( onNetworkError( QNetworkReply::NetworkError )) );

    m_eventLoop.exec();
}

SynchronizedNetworkAccessManager::~SynchronizedNetworkAccessManager(){}

void SynchronizedNetworkAccessManager::onFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>( sender() );
    if( !reply || reply->error() != QNetworkReply::NoError ){
        throw FailedAttempt{ reply->errorString().toStdString().c_str() };
    }
    m_eventLoop.quit();
    result = reply->readAll();
}

void SynchronizedNetworkAccessManager::onNetworkAccessibilityChanged(QNetworkAccessManager::NetworkAccessibility net )
{
    m_eventLoop.quit();
    switch( net )
    {
    case QNetworkAccessManager::NetworkAccessibility::UnknownAccessibility:
    case QNetworkAccessManager::NetworkAccessibility::NotAccessible:
        emit networkAccessibilityChanged( net );
        break;
    default:
        return;
    }
}

void SynchronizedNetworkAccessManager::onNetworkError( QNetworkReply::NetworkError error )
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
