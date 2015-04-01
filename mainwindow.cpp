#include "mainwindow.hpp"

namespace Hend
{
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

    VideoMetaData::VideoMetaData():
        VideoMetaData( "", "", "" )
    {

    }

    VideoMetaData::VideoMetaData(QString const & num, QString const &type, QString const &ext):
        m_num( num ), m_type( type ), m_ext( ext )
    {

    }
    VideoUrl::VideoUrl():
        VideoUrl{ "", "", "" }
    {

    }
    VideoUrl::VideoUrl(const QString &ext, const QString &type, const QString &url):
        m_ext( ext ), m_type( type ), m_url( url )
    {

    }

    std::map< QString, filter_function_t > MainWindow::filterByDate {
        { "All", [](...){ return true; } },
        { "Today", filterDataByTodaysDate },
        { "Last 7 days", filterDataFromLastSevenDays },
        { "This Month", filterDataFromThisMonth },
        { "This year", filterDataFromThisYear },
        { "Last year", filterDataFromLastYear }
    };

    /*
QMap< QString, filter_function_t > filterByDuration {
    { "Short( < 4minutes )" },
    { "Long( > 20 minutes )" }
};
*/

    MainWindow::MainWindow(QWidget *parent) :
        QMainWindow( parent ),
        m_fileMenu{ menuBar()->addMenu( "&File" ) },
        m_preferenceMenu{ menuBar()->addMenu( "&Preference"  ) },

        m_toolbar{ addToolBar("Toolbar") },

        m_basicSearchAction{ new QAction( QIcon(":/images/search.png"), tr("&Search"), this ) },
        m_advancedSearchAction{ new QAction( QIcon(":/images/ad_search.png"), tr("Advanced Search"), this ) },
        m_directDownloadAction{ new QAction( QIcon(":/images/refresh.png"), tr("&Direct Download"), this ) },
        m_downloadAction{ new QAction( QIcon(":/images/download.png"), tr("&Download"),this ) },
        m_exitAction{ new QAction( QIcon(":/images/exit.png"), tr( "&Exit" ), this ) },

        m_toolbarViewMenu{ new QMenu{ "&Toolbar View" } },
        m_iconOnlyAction{ new QAction( tr( "Icon Only" ), this )},
        m_textOnlyAction{ new QAction("Text Only", this )},
        m_textBesideAction{ new QAction("Text Beside Icon", this )},
        m_textBelowAction{ new QAction("Text Below Icon", this )},
        m_hideToolbarAction{ new QAction("Hide Toolbar", this )},

        m_videoDisplayTable{ new QTableView },
        m_videoDetailsList{ new VideoDetails },
        m_filterList{ new QListWidget },

        m_filterByUploadDateButton{ new QPushButton("Upload Date") },
        m_filterByTypeButton{ new QPushButton("Type") },
        m_filterByDurationButton{ new QPushButton("Duration") },
        m_filterByFeaturesButton{ new QPushButton("Features") },
        m_filterBySortButton{ new QPushButton("Sort By") },
        m_progressBar{ new ProgressBar },

        m_networkManager{ new NetworkManager },
        m_proxyModel{ new CustomVideoFilterProxyModel },
        m_underlyingProxyTableModel{ nullptr }
    {
        setWindowTitle( tr("Hend - YouTube Download") );
        showMaximized();
        setWindowIcon( QIcon( ":/images/logo.png" ));

        setupMenubar();
        setupToolbar();
        setupActions();
        connectAllActions();

        setupWindowButtons();
        statusBar()->showMessage( "Done" );
    }

    QString MainWindow::YOUTUBE_URL = "https://www.googleapis.com/youtube/v3/search/?part=snippet";
    QString MainWindow::API_KEY = "AIzaSyBhl_zBnEEv_xiIukkMpz8ayoiwT1UdfQk";
    unsigned int MainWindow::MAX_RESULT = 20;

    void MainWindow::download( QString const & url, QString const & videoTitle, QMainWindow *parent )
    {
        qDebug() << "Url is " << url << "\n\n";
        static DownloadManagerWidget downloadManager( parent );
        QString filename = QFileDialog::getSaveFileName( parent, "Get filename", QDir::currentPath() );
        downloadManager.addDownload( url, filename, videoTitle );
        if( !downloadManager.isVisible() ){
            downloadManager.show();
        }
    }

    MainWindow::~MainWindow()
    {
        delete m_progressBar;
        delete m_networkManager;
        delete m_proxyModel;
    }

    void MainWindow::setupMenubar()
    {
        m_fileMenu->addAction( m_downloadAction );
        m_fileMenu->addAction( m_basicSearchAction );
        m_fileMenu->addAction( m_advancedSearchAction );
        m_fileMenu->addAction( m_exitAction );

        m_toolbarViewMenu->addAction( m_iconOnlyAction );
        m_toolbarViewMenu->addAction( m_textOnlyAction );
        m_toolbarViewMenu->addAction( m_textBesideAction );
        m_toolbarViewMenu->addAction( m_textBelowAction );
        m_toolbarViewMenu->addAction( m_hideToolbarAction );

        m_preferenceMenu->addMenu( m_toolbarViewMenu );
    }

    void MainWindow::setupWindowButtons()
    {
        m_progressBar->setParent( this );
        m_progressBar->setWindowTitle( windowTitle() );
        m_progressBar->setWindowIcon( windowIcon() );
        m_progressBar->setWindowFlags( Qt::Dialog | Qt::FramelessWindowHint |
                                       Qt::WindowTitleHint );
        m_progressBar->setWindowModality( Qt::ApplicationModal );

        QWidget *widget = new QWidget;
        QVBoxLayout *vLayout = new QVBoxLayout( widget );
        vLayout->addWidget( m_filterList, 1 );
        vLayout->addWidget( m_filterByUploadDateButton );
        vLayout->addWidget( m_filterByTypeButton );
        vLayout->addWidget( m_filterByDurationButton );
        vLayout->addWidget( m_filterByFeaturesButton );
        vLayout->addWidget( m_filterBySortButton );

        QSplitter *splitter = new QSplitter( Qt::Vertical );
        splitter->addWidget( m_videoDisplayTable );
        splitter->addWidget( m_videoDetailsList );
        splitter->setStretchFactor( 0, 1 );

        QSplitter *new_splitter = new QSplitter( Qt::Horizontal );
        new_splitter->addWidget( widget );
        new_splitter->addWidget( splitter );
        new_splitter->setStretchFactor( 1, 1 );

        m_filterByUploadDateButton->click();
        setCentralWidget( new_splitter );
    }

    void MainWindow::setupActions()
    {
        m_basicSearchAction->setStatusTip( "Search for new Videos" );
        m_basicSearchAction->setShortcut( tr( "Ctrl+S" ) );

        m_advancedSearchAction->setStatusTip( "Advanced and configurable search" );
        m_advancedSearchAction->setShortcut( tr("Ctrl+F"));

        m_directDownloadAction->setStatusTip( "Enter direct youtube address" );
        m_directDownloadAction->setShortcut( tr( "Ctrl+L" ) );

        m_exitAction->setStatusTip( "Exit Hend" );
        m_exitAction->setShortcut( tr( "Ctrl+Q" ) );

        m_downloadAction->setStatusTip( "Download highlighted video" );
        m_downloadAction->setShortcut( tr("Ctrl+D") );
    }

    void MainWindow::setupToolbar()
    {
        m_toolbar->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
        m_toolbar->addAction( m_basicSearchAction );
        m_toolbar->addAction( m_advancedSearchAction );

        m_toolbar->addSeparator();
        m_toolbar->addAction( m_downloadAction );
        m_toolbar->addAction( m_directDownloadAction );

        m_toolbar->addSeparator();
        m_toolbar->addAction( m_exitAction );
    }

    void MainWindow::connectAllActions()
    {
        QObject::connect( m_iconOnlyAction, SIGNAL(triggered()),
                          this, SLOT( iconOnly()) );
        QObject::connect( m_textOnlyAction, SIGNAL(triggered()),
                          this, SLOT( textOnly()) );
        QObject::connect( m_textBelowAction, SIGNAL(triggered()),
                          this, SLOT( textBelowIcon()) );
        QObject::connect( m_textBesideAction, SIGNAL(triggered()),
                          this, SLOT( textBesideIcon()) );
        QObject::connect( m_hideToolbarAction, SIGNAL(triggered()),
                          this, SLOT( hideToolbar() ) );
        QObject::connect( m_directDownloadAction, SIGNAL(triggered()),
                          this, SLOT(directDownload()));
        QObject::connect( m_downloadAction, SIGNAL(triggered()),
                          this, SLOT(findDownloadLink()));
        QObject::connect( m_basicSearchAction, SIGNAL(triggered()),
                          this, SLOT( basicSearch()) );
        QObject::connect( m_advancedSearchAction, SIGNAL(triggered()),
                          this, SLOT(advancedSearch()) );
        QObject::connect( m_exitAction, SIGNAL(triggered()),
                          this, SLOT( close()) );

        QObject::connect( m_filterByUploadDateButton, SIGNAL(clicked()),
                          this, SLOT(filterUploadHandler()) );
        QObject::connect( m_filterByTypeButton, SIGNAL(clicked()),
                          this, SLOT( filterTypeHandler() ) );
        QObject::connect( m_filterByDurationButton, SIGNAL(clicked()),
                          this, SLOT(filterDurationHandler()));
        QObject::connect( m_filterByFeaturesButton, SIGNAL(clicked()),
                          this, SLOT( filterFeaturesHandler()));
        QObject::connect( m_filterBySortButton, SIGNAL(clicked()),
                          this, SLOT( filterSortHandler()) );
        QObject::connect( m_filterList, SIGNAL(currentTextChanged(QString)),
                          this, SLOT(filterController(QString)) );

        QObject::connect( m_networkManager, SIGNAL(problemWithRequest(QString)),
                          this, SLOT(handleAllErrors(QString)) );
        QObject::connect( m_networkManager, SIGNAL(responseReceived( QByteArray, WhatToFetch)),
                          this, SLOT( processResponseReceived( QByteArray, WhatToFetch )) );
        QObject::connect( m_networkManager, SIGNAL(reportDownloadProgress( qint64,qint64, WhatToFetch)),
                          this, SLOT(downloadProgressMonitor( qint64,qint64, WhatToFetch)) );
    }

    void MainWindow::iconOnly()
    {
        m_toolbar->setVisible( true );
        m_toolbar->setToolButtonStyle( Qt::ToolButtonIconOnly );
    }

    void MainWindow::textOnly()
    {
        m_toolbar->setVisible( true );
        m_toolbar->setToolButtonStyle( Qt::ToolButtonTextOnly );
    }

    void MainWindow::textBesideIcon()
    {
        m_toolbar->setVisible( true );
        m_toolbar->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
    }

    void MainWindow::textBelowIcon()
    {
        m_toolbar->setVisible( true );
        m_toolbar->setToolButtonStyle(( Qt::ToolButtonTextUnderIcon ));
    }

    void MainWindow::hideToolbar( )
    {
        m_toolbar->setVisible( false );
    }

    void MainWindow::closeEvent( QCloseEvent * event )
    {
        if( isSafeToExit() ){
            event->accept();
        } else {
            event->ignore();
        }
    }
    bool MainWindow::isSafeToExit()
    {
        switch( QMessageBox::information( this, tr("Exit"), tr("Are you sure you want to exit?"),
                                          QMessageBox::Yes | QMessageBox::No ) ){
        case QMessageBox::Yes: return true;
        case QMessageBox::No: default: return false;
        }
    }

    void MainWindow::handleAllErrors(const QString & str )
    {
        QMessageBox::critical( this->window(), "Network Manager", str, QMessageBox::Ok );
        setStatusTip( str );
    }

    void MainWindow::processResponseReceived(const QByteArray & response, WhatToFetch whatToFetch )
    {
        if( response.isEmpty() ) qDebug() << "No response received.";

        switch ( whatToFetch ){
        case WhatToFetch::VideoInfo:
            displayVideoInfo( response );
            break;
        default:
            break;
        }
    }

    void MainWindow::displayVideoInfo( QByteArray const & response )
    {
        m_proxyModel->setSourceModel( new VideoTableModel( response ) );
        m_underlyingProxyTableModel = dynamic_cast<VideoTableModel *>( m_proxyModel->sourceModel() );
        m_videoDisplayTable->setModel( m_proxyModel );
        m_videoDisplayTable->setSelectionBehavior( QAbstractItemView::SelectRows );
        m_videoDisplayTable->setSelectionMode( QAbstractItemView::SingleSelection );
        m_videoDisplayTable->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );
    }

    void MainWindow::downloadProgressMonitor(qint64 done, qint64 total , WhatToFetch )
    {
        if( total == 0 || done == 0 ){
            m_progressBar->close();
            return;
        }
        m_progressBar->setProgressStatus( ( done * 100 ) / total );
    }

    void MainWindow::filterUploadHandler()
    {
        m_filterList->clear();
        for( const auto &dates: filterByDate ){
            m_filterList->addItem( dates.first );
        }
    }

    void MainWindow::filterTypeHandler()
    {
        m_filterList->clear();
        m_filterList->addItems( QStringList { "Video", "Channel", "Playlist",
                                              "Movie", "Show"});
    }

    void MainWindow::filterDurationHandler()
    {
        m_filterList->clear();
        m_filterList->addItem ("Short( < 4minutes )" );
        m_filterList->addItem( "Long( > 20 minutes )" );
    }

    void MainWindow::filterFeaturesHandler()
    {
        m_filterList->clear();
        m_filterList->addItems( QStringList {"HD", "Creative Commons", "3D", "Live", "Purchased"
                                });
    }

    void MainWindow::filterSortHandler()
    {
        m_filterList->clear();
        m_filterList->addItems( QStringList { "Relevance", "Upload Date", "View Count", "Rating"
                                });
    }

    void MainWindow::filterController( const QString & current_filter )
    {
        if( filterByDate.find( current_filter ) != filterByDate.end() ){
            m_proxyModel->setFilter( CustomFilter{ filterByDate[current_filter] } );
            m_proxyModel->invalidate();
        }
    }

    void MainWindow::basicSearch()
    {
        bool ok = false;
        QString query = QInputDialog::getText( this->window(), tr("Basic Search"),
                                               tr( "Enter video title" ),
                                               QLineEdit::Normal, QString{}, &ok );
        if( ok && query.trimmed().size() != 0 ){
            query.replace( "+", "%2C" );
            query.replace( ' ', '+' );
            QString new_query = YOUTUBE_URL +
                    tr( "&q=%1&maxResults=%2&key=%3" )
                    .arg(query)
                    .arg( MAX_RESULT )
                    .arg( API_KEY );
            m_networkManager->sendRequest( QNetworkRequest( new_query ), WhatToFetch::VideoInfo );
            m_progressBar->exec();
        }
    }

    void MainWindow::advancedSearch()
    {
        SearchDialog searchDialog( window() );
        searchDialog.setWindowIcon( windowIcon() );
        searchDialog.setWindowModality( Qt::ApplicationModal );

        if( searchDialog.exec() == QDialog::Accepted ){

            QString query = searchDialog.getQuery().trimmed();
            query.replace( ' ', '+' );

            QString new_query = QString( "%1&q=%2&key=%3" )
                    .arg( YOUTUBE_URL )
                    .arg( query )
                    .arg( API_KEY );
            QNetworkRequest request { new_query };
            m_networkManager->sendRequest( request, WhatToFetch::VideoInfo );
            m_progressBar->exec();
        }
    }
    void MainWindow::directDownload()
    {
        QString url = QInputDialog::getText( this, windowTitle(), tr("Input Youtube URL") );
        if( url.size() <= 0 ) return;
        universalDownload( url );
    }

    void MainWindow::findDownloadLink()
    {
        if( !m_underlyingProxyTableModel ) return;
        if( !m_videoDisplayTable->currentIndex().isValid() ){
            QMessageBox::information( window(), windowTitle(), "Select a video to download", QMessageBox::Ok );
            return;
        }
        QString videoID = m_underlyingProxyTableModel->videoStructure().getSearchResponse(
                    m_videoDisplayTable->currentIndex().row() ).id().videoID();
        universalDownload( videoID );
    }

    void MainWindow::universalDownload( QString const & videoID )
    {
        m_progressBar->show();
        Hend::FormatSpecifier *formatManager = new Hend::FormatSpecifier( videoID, this );
        m_progressBar->close();

        if( formatManager->exec() == QDialog::Accepted )
        {
            download( formatManager->getDownloadLink(), formatManager->title() );
        }
    }
} //namespace Hend
