#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <QDateTime>
#include "videotablemodel.hpp"

namespace Hend
{
    using filter_function_t = std::function<bool( int, QModelIndex const &, VideoTableModel * )>;
    inline namespace FilterFunctionImplementations
    {
        static inline bool filterDataByTodaysDate( int current_row, QModelIndex const &, VideoTableModel * model )
        {
            return model->videoStructure().getSearchResponse( current_row ).snippet()
                    .publishedDate().date().day() == QDate::currentDate().day();
        }

        static inline bool filterDataFromLastSevenDays( int current_row, QModelIndex const &, VideoTableModel *model  )
        {
            return model->videoStructure().getSearchResponse( current_row ).snippet()
                    .publishedDate().date().addDays( -7 ) <= QDate::currentDate();
        }
        static inline bool filterDataFromThisMonth( int current_row, QModelIndex const &, VideoTableModel * model )
        {
            return model->videoStructure().getSearchResponse( current_row ).snippet().publishedDate().date()
                    .month() == QDate::currentDate().month();
        }
        static inline bool filterDataFromLastYear(int current_row, QModelIndex const &, VideoTableModel * model )
        {
            return model->videoStructure().getSearchResponse( current_row ).snippet().publishedDate().date()
                    .year() == ( QDate::currentDate().year() - 1 );
        }
        static inline bool filterDataFromThisYear(int current_row, QModelIndex const &, VideoTableModel * model )
        {
            return model->videoStructure().getSearchResponse( current_row ).snippet().publishedDate().date()
                    .year() == QDate::currentDate().year();
        }

        static inline bool filterByShortVideos( int, QModelIndex const &, VideoTableModel *)
        {
            return true;
        }
    }
}
#endif // FUNCTIONS_HPP
