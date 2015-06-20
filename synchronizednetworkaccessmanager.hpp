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

class SyncNetworkAccessManager : public QObject
{
    Q_OBJECT
    QNetworkAccessManager   m_networkManager {};
    QEventLoop              m_eventLoop {};
signals:
public:
    explicit SyncNetworkAccessManager( QObject *parent = nullptr );
    ~SyncNetworkAccessManager();
    void getRequest( QUrl const & );
    QByteArray result {};
public slots:
    void onFinished();
    void onNetworkError( QNetworkReply::NetworkError );
};

#endif // SYNCHRONIZEDNETWORKACCESSMANAGER_HPP
