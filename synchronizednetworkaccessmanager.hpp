#ifndef SYNCHRONIZEDNETWORKACCESSMANAGER_HPP
#define SYNCHRONIZEDNETWORKACCESSMANAGER_HPP

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>


inline namespace ErrorMessages
{
    struct BaseError: std::runtime_error { BaseError( char const * const str ): std::runtime_error{ str } {} virtual ~BaseError() {} };
    struct FailedAttempt: public BaseError { FailedAttempt( char const * const str ): BaseError { str }{} };
    struct NotFound: public BaseError { NotFound( char const * const str ): BaseError{ str } {} };
}

class SynchronizedNetworkAccessManager : public QObject
{
    Q_OBJECT
    QNetworkAccessManager   m_networkManager;
    QNetworkRequest         m_request;
    QEventLoop              m_eventLoop;
signals:
    void networkAccessibilityChanged( QNetworkAccessManager::NetworkAccessibility );
    void networkError( QNetworkReply::NetworkError );
public:
    explicit SynchronizedNetworkAccessManager( QString const & url, QObject *parent = nullptr );
    ~SynchronizedNetworkAccessManager();
    QByteArray result {};
public slots:
    void onFinished();
    void onNetworkAccessibilityChanged( QNetworkAccessManager::NetworkAccessibility );
    void onNetworkError( QNetworkReply::NetworkError );
};

#endif // SYNCHRONIZEDNETWORKACCESSMANAGER_HPP
