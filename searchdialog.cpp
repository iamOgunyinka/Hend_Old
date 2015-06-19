#include "searchdialog.hpp"

namespace Hend
{
    SearchDialog::SearchDialog(QWidget *parent):
        QDialog( parent ),
        m_labelSearchString{ new QLabel{ "Search string" } },
        m_labelSelection{ new QLabel{ "Upload Date" } },
        m_searchString { new QLineEdit },
        m_pushButtonOk{ new QPushButton{"Ok"} },
        m_pushButtonCancel{ new QPushButton{"Cancel"} },

        m_comboVideoType{ new QComboBox },
        m_comboVideoDimension{ new QComboBox },
        m_comboVideoDefinition{ new QComboBox },
        m_comboVideoDuration{ new QComboBox },
        m_videoMaxSize{ new QSpinBox },
        m_gLayout{ new QGridLayout }
    {
        setupComboBoxes();
        configureWindows();
        connectAllButtonActions();
    }

    SearchDialog::video_feature_t SearchDialog::videoDuration {
        { "", "" },
        { "Any", "&videoDuration=any" },
        { "Long( > 20minutes )", "&videoDuration=long" },
        { "Medium( 4 > N <= 20 minutes )", "&videoDuration=medium" },
        { "Short( < 4minutes )", "&videoDuration=short" }
    };
    SearchDialog::video_feature_t SearchDialog::videoDimension {
        { "", "" }, { "2D", "&videoDimension=2d" },
        { "3D", "&videoDimension=3d" }, { "Any", "&videoDimension=any" }
    };
    SearchDialog::video_feature_t SearchDialog::videoDefinition {
        { "", "" }, { "Any", "&videoDefintion=any" },
        { "High", "&videoDefintion=high" }, { "Standard", "&videoDefintion=standard" }
    };

    SearchDialog::video_feature_t SearchDialog::videoType {
        { "", "" }, { "Video", "&type=video" },
        { "Playlist", "&type=playlist" }, { "Channel", "&type=channel" }
    };
    void SearchDialog::setupComboBoxes()
    {
        m_videoMaxSize->setRange( 5, 50 );
        for( const auto &dimension: videoDimension )
            m_comboVideoDimension->addItem( videoDimension.key( dimension ) );
        for( const auto &def: videoDefinition )
            m_comboVideoDefinition->addItem( videoDefinition.key( def ) );
        for( const auto & type: videoType )
            m_comboVideoType->addItem( videoType.key( type ) );
        for( const auto & duration: videoDuration )
            m_comboVideoDuration->addItem( videoDuration.key( duration ) );
    }

    void SearchDialog::connectAllButtonActions()
    {
        QObject::connect( m_pushButtonOk, SIGNAL(clicked()), this, SLOT( accept() ) );
        QObject::connect( m_pushButtonCancel, SIGNAL(clicked()), this, SLOT( reject() ) );
    }

    void SearchDialog::configureWindows()
    {
        m_gLayout->addWidget( m_labelSearchString, 0, 0 );
        m_gLayout->addWidget( m_searchString, 0, 1 );

        m_gLayout->addWidget( new QLabel("Video Type"), 1, 0 );
        m_gLayout->addWidget(m_comboVideoType, 1, 1);

        m_gLayout->addWidget( new QLabel("Video Dimension"), 2, 0 );
        m_gLayout->addWidget( m_comboVideoDimension, 2, 1);

        m_gLayout->addWidget( new QLabel("Video Duration"), 3, 0 );
        m_gLayout->addWidget( m_comboVideoDuration, 3, 1 );

        m_gLayout->addWidget( new QLabel("Video Definition"), 4, 0 );
        m_gLayout->addWidget( m_comboVideoDefinition, 4, 1 );

        m_gLayout->addWidget( new QLabel("Video Maximum Results"), 5, 0 );
        m_gLayout->addWidget( m_videoMaxSize, 5, 1 );

        m_gLayout->addWidget( m_pushButtonOk, 6, 0 );
        m_gLayout->addWidget( m_pushButtonCancel, 6, 1 );

        this->setLayout( m_gLayout );
    }

    QString SearchDialog::getQuery() const
    {
        QString query{ m_searchString->text() };
        if( query.trimmed().isEmpty() ) return {};

        query = tr( QUrl::toPercentEncoding( query ) );
        query += QString{ tr( "&maxResults=%1").arg( m_videoMaxSize->value() ) };
        query += videoType[m_comboVideoType->currentText()];
        query += videoDimension[m_comboVideoDimension->currentText()];
        query += videoDefinition[m_comboVideoDefinition->currentText()];
        query += videoDuration[m_comboVideoDuration->currentText()];

        return query;
    }
}
