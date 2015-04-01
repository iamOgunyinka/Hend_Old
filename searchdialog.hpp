#ifndef SEARCHDIALOG_HPP
#define SEARCHDIALOG_HPP

#include <QDialog>
#include <QtWidgets>
#include <QMap>

namespace Hend
{
    class SearchDialog : public QDialog
    {
        Q_OBJECT
    public:

        using video_feature_t = QMap<QString, QString>;
        explicit SearchDialog( QWidget *parent = nullptr );
        static video_feature_t videoDuration;
        static video_feature_t videoDimension;
        static video_feature_t videoDefinition;
        static video_feature_t videoType;

        QString getQuery() const;
    private:
        QLabel              *m_labelSearchString;
        QLabel              *m_labelSelection;
    
        QLineEdit           *m_searchString;
        QPushButton         *m_pushButtonOk;
        QPushButton         *m_pushButtonCancel;

        QComboBox           *m_comboVideoType;
        QComboBox           *m_comboVideoDimension;
        QComboBox           *m_comboVideoDefinition;
        QComboBox           *m_comboVideoDuration;
        QSpinBox            *m_videoMaxSize;
        QGridLayout         *m_gLayout;
    
    private:
        void setupComboBoxes();
        void connectAllButtonActions();
        void configureWindows();
    };    
}

#endif // SEARCHDIALOG_HPP
