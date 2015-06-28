#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QtWidgets>
#include <QMap>
#include <QHeaderView>
#include <memory>

#include "searchdialog.hpp"
#include "filterfunctions.hpp"
#include "extractor.hpp"
#include "downloadmanager.hpp"
#include "videodetails.hpp"

namespace Hend
{
    class MainWindow : public QMainWindow
    {
        Q_OBJECT
    public:
        explicit    MainWindow( QWidget *parent = nullptr );
        ~MainWindow();

        static std::map< QString, filter_function_t > filterSearchResultsByDate;
        static std::map< QString, filter_function_t > filterSearchResultsByType;
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
        void        basicSearch();
        void        advancedSearch();
        void        directDownload();
        void        findDownloadLink();
        void        closeEvent( QCloseEvent * );
        void        aboutHend();
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
        QAction     *m_aboutQt;
        QAction     *m_aboutHend;

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

        SyncNetworkAccessManager      *m_networkManager;
        CustomVideoFilterProxyModel   *m_proxyModel;
        VideoTableModel               *m_underlyingProxyTableModel;
    signals:

    public slots:

    };
}
#endif // MAINWINDOW_HPP
