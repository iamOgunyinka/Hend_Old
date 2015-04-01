#ifndef VIDEODETAILS_HPP
#define VIDEODETAILS_HPP

#include <QWidget>
#include <QtWidgets>
class VideoDetails : public QWidget
{
    Q_OBJECT
public:
    explicit VideoDetails(QWidget *parent = nullptr);
signals:

public slots:
private:
    QPixmap *m_thumbnail { nullptr };
    QLabel  *m_titleLabel { nullptr };
    QLabel  *m_timestampLabel { nullptr };
    QLabel  *m_videoLengthLabel { nullptr };
    QLabel  *detailLabel { nullptr };
    QGridLayout *m_gLayout { nullptr };
};

#endif // VIDEODETAILS_HPP
