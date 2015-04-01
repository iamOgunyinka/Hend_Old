#ifndef NETWORKMANAGER_HPP
#define NETWORKMANAGER_HPP

#include <QObject>
#include <QtNetwork>

namespace Hend
{
    enum class WhatToFetch
    {
        VideoInfo = 0,
        VideoThumbnail,
        VideoDownloadLink,
        VideoDownload
    };

    class NetworkManager: public QObject
    {
        Q_OBJECT
    
    public:
        explicit NetworkManager( QObject *parent = nullptr );
        ~NetworkManager();

        void sendRequest( QNetworkRequest const &, WhatToFetch wtf );
    private:
        void setCallbacks();
    signals:
        void problemWithRequest( QString const & );
        void responseReceived( QByteArray, WhatToFetch );
        void reportDownloadProgress( qint64, qint64, WhatToFetch );
    private slots:
        void processFinished();
        void processError( QNetworkAccessManager::NetworkAccessibility );
        void downloadProgressReporter(qint64 done, qint64 total);
    private:
        QNetworkAccessManager   *m_networkManager;
        QNetworkReply           *m_networkReply;
        WhatToFetch              m_whatToFetch;
    };
}
#endif // NETWORKMANAGER_HPP
