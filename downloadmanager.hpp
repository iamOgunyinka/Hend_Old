#ifndef DOWNLOADMANAGER_HPP
#define DOWNLOADMANAGER_HPP

#include <QObject>
#include <QtWidgets>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>

class Download : public QObject
{
    Q_OBJECT
public:
    explicit Download(QObject *parent = nullptr);
signals:
    void downloadComplete();
    void error( QNetworkReply::NetworkError );
    void downloadProgress( int rate );
public slots:
    void cancel();
    void pause();
    void resume();
    void download(QString const & url , const QString &filename = "" );
    void networkError( QNetworkReply::NetworkError );
    void download( QNetworkRequest const & request );
    void downloadProgress( qint64 done, qint64 total );
    void finished();
private:
    QNetworkAccessManager   *m_networkManager;
    QNetworkRequest          m_networkRequest;
    QNetworkReply           *m_networkReply;
    int                      m_downloadRate;
    QFile                   *m_file;
};

class DownloadManager: public QWidget
{
    Q_OBJECT

public:
    DownloadManager(QString const &url, QString const &filename, QString const &title, QWidget *parent = nullptr );
private slots:
    void downloadCompleted();
    void paused();
    void cancelled();
    void resumed();
private:
    void disableAllButtons();
private:
    Download     *m_downloadManager;
    QPushButton  *m_buttonCancel;
    QPushButton  *m_buttonPause;
    QPushButton  *m_buttonResume;
    QProgressBar *m_progressBar;
    QGridLayout  *m_gLayout;
};

class DownloadManagerWidget: public QWidget
{
    Q_OBJECT
public:
    DownloadManagerWidget( QWidget *parent = nullptr );
    void addDownload(QString const & url, QString const & filename, QString const &title);
private:
    QVBoxLayout     *m_windowLayout;
};

#endif // DOWNLOADMANAGER_HPP
