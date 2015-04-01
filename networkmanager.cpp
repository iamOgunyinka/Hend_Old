#include "networkmanager.hpp"

namespace Hend
{
    NetworkManager::NetworkManager( QObject *parent ):
        QObject { parent },
        m_networkManager{ new QNetworkAccessManager },
        m_networkReply{ },
        m_whatToFetch{ WhatToFetch::VideoInfo }
    {
        setCallbacks();
    }

    NetworkManager::~NetworkManager()
    {
        if( m_networkManager )
        {
            delete m_networkManager;
        }
    }

    void NetworkManager::setCallbacks()
    {
        QObject::connect( m_networkManager,
                          SIGNAL( networkAccessibleChanged( QNetworkAccessManager::NetworkAccessibility )),
                          this, SLOT( processError( QNetworkAccessManager::NetworkAccessibility )) );
    }

    void NetworkManager::downloadProgressReporter( qint64 done, qint64 total )
    {
        emit reportDownloadProgress( done, total, m_whatToFetch );
    }

    void NetworkManager::processError( QNetworkAccessManager::NetworkAccessibility net_access )
    {
        switch( net_access )
        {
        case QNetworkAccessManager::NetworkAccessibility::UnknownAccessibility:
            emit problemWithRequest( "Unable to determine network connectivity!" );
            break;
        case QNetworkAccessManager::NetworkAccessibility::NotAccessible:
            emit problemWithRequest( "Network is inaccessible!" );
            break;
        default:
            emit problemWithRequest( "Network OK" );
        }
    }
    
    void NetworkManager::processFinished( )
    {
        QNetworkReply *response = qobject_cast<QNetworkReply*>( sender() );
        if( !response ){
            emit problemWithRequest( "No response received!" );
            return;
        }
    
        if( response->error() != QNetworkReply::NoError ){
            emit problemWithRequest( response->errorString() );
            return;
        }
        QObject::disconnect( m_networkReply, SIGNAL(finished()), this, SLOT(processFinished()) );
        QObject::disconnect( m_networkReply, SIGNAL(downloadProgress(qint64,qint64)),
                          this, SLOT( downloadProgressReporter( qint64, qint64)) );
        emit responseReceived( response->readAll(), m_whatToFetch );
    }

    void NetworkManager::sendRequest(const QNetworkRequest & request , WhatToFetch wtf )
    {
        m_whatToFetch = wtf;
        m_networkReply = m_networkManager->get( request );

        QObject::connect( m_networkReply, SIGNAL(downloadProgress(qint64,qint64)),
                          this, SLOT( downloadProgressReporter( qint64, qint64)) );
        QObject::connect( m_networkReply, SIGNAL(finished()), this, SLOT(processFinished()) );
    }
}
