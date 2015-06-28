#include "mainwindow.hpp"

namespace Hend
{
    std::map< QString, filter_function_t > MainWindow::filterSearchResultsByDate {
        { "All", showAllResults },
        { "Today", filterDataByTodaysDate },
        { "Last 7 days", filterDataFromLastSevenDays },
        { "This Month", filterDataFromThisMonth },
        { "This year", filterDataFromThisYear },
        { "Last year", filterDataFromLastYear }
    };
    std::map< QString, filter_function_t > MainWindow::filterSearchResultsByType
    {
        { "All", showAllResults },
        { "Video", filterDataByVideo },
        { "Channel", filterDataByChannel },
        { "Playlist", filterDataByPlaylist },
        { "Movie", filterDataByMovie },
        { "Show", filterDataByShow }
    };

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
        m_aboutQt{ new QAction{ tr( "About Qt" ), this } },
        m_aboutHend{ new QAction{ tr( "About Hend" ), this } },

        m_toolbarViewMenu{ new QMenu{ "&Toolbar View" } },
        m_iconOnlyAction{ new QAction( tr( "Icon Only" ), this )},
        m_textOnlyAction{ new QAction("Text Only", this )},
        m_textBesideAction{ new QAction("Text Beside Icon", this )},
        m_textBelowAction{ new QAction("Text Below Icon", this )},
        m_hideToolbarAction{ new QAction("Hide Toolbar", this )},

        m_videoDisplayTable{ new QTableView },
        m_videoDetailsList{ new VideoDetails },
        m_filterList{ new QListWidget },

        m_filterByUploadDateButton{ new QPushButton("Filter by Upload Date") },
        m_filterByTypeButton{ new QPushButton("Filter by Type") },
        m_filterByDurationButton{ new QPushButton("Filter by Duration") },
        m_filterByFeaturesButton{ new QPushButton("Filter by Features") },
        m_filterBySortButton{ new QPushButton("Sort Videos") },

        m_networkManager{ new SyncNetworkAccessManager },
        m_proxyModel{ new CustomVideoFilterProxyModel },
        m_underlyingProxyTableModel{ nullptr }
    {
        setWindowTitle( tr( "Hend" ) );
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
    unsigned int MainWindow::MAX_RESULT = 40;

    void MainWindow::download( QString const & url, QString const & videoTitle, QMainWindow *parent )
    {
        static DownloadManagerWidget downloadManager( parent );
        QString filename = QFileDialog::getSaveFileName( parent, "Get filename", QDir::currentPath() );
        downloadManager.addDownload( url, filename, videoTitle );
        if( !downloadManager.isVisible() ){
            downloadManager.show();
        }
    }

    MainWindow::~MainWindow(){}

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
        m_preferenceMenu->addAction( m_aboutHend );
        m_preferenceMenu->addAction( m_aboutQt );
    }

    void MainWindow::setupWindowButtons()
    {
        m_videoDetailsList->setStyleSheet("QWidget{height: 20px;}");

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
        QObject::connect( m_aboutQt, SIGNAL( triggered() ), qApp, SLOT( aboutQt() ) );
        QObject::connect( m_aboutHend, SIGNAL(triggered()),this, SLOT( aboutHend() ) );

        QObject::connect( m_videoDisplayTable, SIGNAL(activated(QModelIndex)),
                          this, SLOT(viewDetails(QModelIndex)) );
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
    //TODO
    void MainWindow::aboutHend()
    {
        QMessageBox::information( this, tr("About Hend"), tr("Contact me via: ogunyinkajoshua@yahoo.com"), QMessageBox::Ok );
    }

    void MainWindow::handleAllErrors( const QString & str )
    {
        QMessageBox::critical( this->window(), "Network Manager", str, QMessageBox::Ok );
        setStatusTip( str );
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

    void MainWindow::viewDetails( QModelIndex const & index )
    {
        m_videoDetailsList->setNewDetails( m_underlyingProxyTableModel->videoStructure()
                                           .getSearchResponse( index.row() ).snippet() );
    }

    void MainWindow::filterUploadHandler()
    {
        m_filterList->clear();
        for( auto const &dates: filterSearchResultsByDate ){
            m_filterList->addItem( dates.first );
        }
    }

    void MainWindow::filterTypeHandler()
    {
        m_filterList->clear();
        for( auto const & types: filterSearchResultsByType )
            m_filterList->addItem( types.first );
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
        if( filterSearchResultsByDate.find( current_filter ) != filterSearchResultsByDate.end() ){
            m_proxyModel->setFilter( CustomFilter{ filterSearchResultsByDate[current_filter] } );
            m_proxyModel->invalidate();
        } else if( filterSearchResultsByType.find( current_filter ) != filterSearchResultsByType.cend() ){
            m_proxyModel->setFilter( CustomFilter{ filterSearchResultsByType[current_filter]} );
            m_proxyModel->invalidate();
        }
    }

    void MainWindow::basicSearch()
    {
        bool ok = false;
        QString query = QInputDialog::getText( this->window(), tr("Basic Search"),
                                               tr( "Enter video title" ),
                                               QLineEdit::Normal, QString{}, &ok );
        query = tr( QUrl::toPercentEncoding( query ) );
        if( ok && query.size() != 0 ){
            QString new_query = YOUTUBE_URL + "&q=" + query +
                    tr( "&maxResults=%1&key=%2" )
                    .arg( MAX_RESULT )
                    .arg( API_KEY );
            try {
                m_networkManager->getRequest( QUrl( new_query ) );
                displayVideoInfo( m_networkManager->result );
            } catch ( BaseError & err ) {
                handleAllErrors( err.what() );
                return;
            }
        }
    }

    void MainWindow::advancedSearch()
    {
        SearchDialog searchDialog( window() );
        searchDialog.setWindowIcon( windowIcon() );
        searchDialog.setWindowModality( Qt::ApplicationModal );

        if( searchDialog.exec() == QDialog::Accepted ){
            QString query = YOUTUBE_URL + "&q=" + searchDialog.getQuery() + "&key=" + API_KEY;
            try {
                m_networkManager->getRequest( QUrl{ query } );
                displayVideoInfo( m_networkManager->result );
            } catch( BaseError & err ){
                handleAllErrors( err.what() );
                return;
            }
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
        std::unique_ptr<Hend::FormatSpecifier> formatManager {};
        try {
            formatManager.reset( new Hend::FormatSpecifier( videoID, this ) );
        } catch( BaseError const & errorMessage ) {
            QMessageBox::critical( this, "Error", tr( errorMessage.what() ), QMessageBox::Ok );
            return;
        }
        if( formatManager->exec() == QDialog::Accepted )
        {
            download( formatManager->getDownloadLink(), formatManager->title() );
        }
    }
} //namespace Hend
