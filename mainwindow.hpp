#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QtWidgets>
#include <QMap>
#include <QHeaderView>

#include "networkmanager.hpp"
#include "searchdialog.hpp"
#include "functions.hpp"
#include "extractor.hpp"
#include "downloadmanager.hpp"
#include "videodetails.hpp"

namespace Hend
{
    struct ProgressBar: QDialog
    {
        Q_OBJECT
        QProgressBar *m_progressBar;
    public:
        ProgressBar( QWidget *parent = nullptr );
        void reset();
    public slots:
        void setProgressStatus(qint64 done);
    };

    struct VideoMetaData
    {
        VideoMetaData( QString const & num, QString const & type, QString const & ext );
        VideoMetaData();
    private:
        QString m_num;
        QString m_type;
        QString m_ext;
    };

    struct VideoUrl
    {
        VideoUrl();
        VideoUrl( QString const & ext, QString const & type, QString const & url );
    private:
        QString m_ext;
        QString m_type;
        QString m_url;
    };

    //static QMap< QString, FilterFunctions::filter_function_t > filterByType;
    //static QMap< QString, FilterFunctions::filter_function_t > filterByDuration;
    //static QMap< QString, FilterFunctions::filter_function_t > filterByFeatures;
    //static QMap< QString, FilterFunctions::filter_function_t > filterBySorts;
    class MainWindow : public QMainWindow
    {
        Q_OBJECT
    public:
        using video_list = typename std::vector<VideoUrl>;
        explicit    MainWindow( QWidget *parent = nullptr );
        ~MainWindow();
        static std::map< QString, filter_function_t > filterByDate;

        static QString YOUTUBE_URL;
        static QString API_KEY;
        static unsigned int MAX_RESULT;
        static void download( QString const & url, QString const & title,
                              QMainWindow *parent = nullptr );
    private slots:
        void        iconOnly();
        void        textOnly();
        void        textBelowIcon();
        void        textBesideIcon();
        void        hideToolbar();
        void        handleAllErrors( QString const & );
        void        processResponseReceived(QByteArray const & response, WhatToFetch whatToFetch);
        void        downloadProgressMonitor( qint64, qint64, WhatToFetch );
        void        basicSearch();
        void        advancedSearch();
        void        directDownload();
        void        findDownloadLink();
        void        closeEvent( QCloseEvent * );
        void        viewDetails( QModelIndex const & );

        void        filterUploadHandler();
        void        filterTypeHandler();
        void        filterDurationHandler();
        void        filterFeaturesHandler();
        void        filterSortHandler();
        void        filterController(QString const &current_filter);
    private:
        void        displayVideoInfo( QByteArray const & response );
        void        universalDownload( QString const & videoID );
        void        setupMenubar();
        void        setupToolbar();
        void        setupActions();
        void        connectAllActions();
        void        setupWindowButtons();
        bool        isSafeToExit();
    private:
        QMenu       *m_fileMenu;
        QMenu       *m_preferenceMenu;

        QToolBar    *m_toolbar;

        QAction     *m_basicSearchAction;
        QAction     *m_advancedSearchAction;
        QAction     *m_directDownloadAction;
        QAction     *m_downloadAction;
        QAction     *m_exitAction;

        QMenu       *m_toolbarViewMenu;
        QAction     *m_iconOnlyAction;
        QAction     *m_textOnlyAction;
        QAction     *m_textBesideAction;
        QAction     *m_textBelowAction;
        QAction     *m_hideToolbarAction;

        QTableView  *m_videoDisplayTable;
        VideoDetails *m_videoDetailsList;

        QListWidget *m_filterList;

        QPushButton *m_filterByUploadDateButton;
        QPushButton *m_filterByTypeButton;
        QPushButton *m_filterByDurationButton;
        QPushButton *m_filterByFeaturesButton;
        QPushButton *m_filterBySortButton;

        ProgressBar                   *m_progressBar;
        NetworkManager                *m_networkManager;
        CustomVideoFilterProxyModel   *m_proxyModel;
        VideoTableModel               *m_underlyingProxyTableModel;
    signals:

    public slots:

    };
}
#endif // MAINWINDOW_HPP
