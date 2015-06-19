#include "videotablemodel.hpp"

namespace  Hend {

    VideoTableModel::VideoTableModel( QByteArray const & data, QObject *parent ) :
        QAbstractTableModel{ parent },
        m_videoStructure{ data }
    {
    }

    VideoStructure const & VideoTableModel::videoStructure() const { return m_videoStructure; }
    Qt::ItemFlags VideoTableModel::flags(const QModelIndex &index) const
    {
        if( !index.isValid() )
        {
            return Qt::ItemIsEnabled;
        }
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    }
    
    int VideoTableModel::columnCount( const QModelIndex & ) const
    {
        return View::e_max_limit;
    }

    int VideoTableModel::rowCount( const QModelIndex & ) const
    {
        return m_videoStructure.getSize();
    }

    QVariant VideoTableModel::data( const QModelIndex &index, int role ) const
    {
        if( index.row() < 0 || index.row() >= rowCount() ){
            return QVariant{};
        }
        if( role == Qt::DisplayRole ){
            switch( index.column() ) {
            case View::e_title:
                return m_videoStructure.getSearchResponse( index.row() ).snippet().title();
            case View::e_description:
                return m_videoStructure.getSearchResponse( index.row() ).snippet().description();
            case View::e_channelTitle:
                return m_videoStructure.getSearchResponse( index.row() ).snippet().channelTitle();
            case View::e_publishDate:
                return m_videoStructure.getSearchResponse( index.row() ).snippet().publishedDate();
            default:
                return QVariant{};
            }
        }
        return QVariant{};
    }
    
    QVariant VideoTableModel::headerData( int section, Qt::Orientation orientation, int role ) const
    {
        if( role != Qt::DisplayRole ){
            return QVariant{};
        }
        if( orientation == Qt::Horizontal ){
            switch( section ){
            case View::e_title:
                return tr( "Video Title" );
            case View::e_description:
                return tr( "Description" );
            case View::e_channelTitle:
                return tr( "Channel Title" );
            case View::e_publishDate:
                return tr( "Date Published" );
            default:
                return QVariant{};
            }
        } else if ( orientation == Qt::Vertical ) {
            return section + 1;
        }
        return QVariant{};
    }
    CustomFilter::CustomFilter():
        CustomFilter{ CustomVideoFilterProxyModel::noFilters }
    {

    }
    CustomFilter::CustomFilter(std::function<bool(int, const QModelIndex &, VideoTableModel *)> proxy ):
        m_proxy{ proxy }
    {

    }

    bool CustomFilter::operator ()( int source_row, QModelIndex const & index, VideoTableModel *model ) const
    {
        return m_proxy( source_row, index, model );
    }

    CustomVideoFilterProxyModel::CustomVideoFilterProxyModel():
        m_filter{}
    {
    }
    bool CustomVideoFilterProxyModel::noFilters( int, const QModelIndex &, VideoTableModel * )
    {
        return true;
    }
    bool CustomVideoFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
    {
        auto model = dynamic_cast< Hend::VideoTableModel *>( sourceModel() );
        return m_filter( source_row, source_parent, model );
    }

    void CustomVideoFilterProxyModel::setFilter( CustomFilter filter )
    {
        m_filter = filter;
    }
}
