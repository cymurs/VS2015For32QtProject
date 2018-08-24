#ifndef CRADAR_H
#define CRADAR_H

#include <QWidget>

class CRadar : public QWidget
{
    Q_OBJECT

public:
    explicit CRadar(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

signals:

public slots:
	void setPoints(const QList<QPointF> &points);
	void appendPoints(const QList<QPointF> &points);
	void appendPoint(const QPointF &point);

private:
	QRect getMaxSquare();

private:
    QRect      m_drawArea;				//绘制区域
    int           m_pieRotate;				//扇形旋转区域
    int           m_timerId;					//定时器ID
    int           m_pointTimerId;			//变更点定时器
    int           m_nSpeed;					//速度
    QList<QPointF> m_points;			//绘制点
    QList<int>    m_pointsAlapha;	//绘制点颜色alapha值
	const int dv{ 15 };							//绘制区域偏移量			
};

#endif // CRADAR_H
