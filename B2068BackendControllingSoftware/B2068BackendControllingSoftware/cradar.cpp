#include "cradar.h"
#include <QPainter>

CRadar::CRadar(QWidget *parent) : QWidget(parent)
{
    //初始化
    m_pieRotate = 0;
    m_timerId = -1;
    m_nSpeed = 50;
    m_points<<QPoint(50, 100)<<QPoint()<<QPoint(-100, 50)<<QPoint(150, -50)<<QPoint(-40, -60);
    m_pointsAlapha<<100<<100<<100<<100<<100;

    //启动定时器
//    m_timerId = startTimer(m_nSpeed);
//    m_pointTimerId = startTimer(1200);
}

void CRadar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    //抗锯齿
    painter.setRenderHint(QPainter::Antialiasing);

    //背景
	//painter.fillRect(getMaxSquare(), QColor(15, 45, 188));
    painter.fillRect(rect(),QColor(15,45,188));

    //边长
    int len = m_drawArea.width();

    //底盘(x轴、y轴和3个圆)
    painter.setPen(QPen(Qt::white));
    painter.drawLine(m_drawArea.topLeft() + QPoint(0,len/2),m_drawArea.topRight() + QPoint(0,len/2));
    painter.drawLine(m_drawArea.topLeft() + QPoint(len/2,0),m_drawArea.bottomLeft() + QPoint(len/2,0));
    painter.drawEllipse(m_drawArea.center(),len/2,len/2);
    painter.drawEllipse(m_drawArea.center(),len*4/9,len*4/9);
    painter.drawEllipse(m_drawArea.center(),len*7/18,len*7/18);
    painter.drawEllipse(m_drawArea.center(),len/3,len/3);
    painter.drawEllipse(m_drawArea.center(),len*5/18,len*5/18);
    painter.drawEllipse(m_drawArea.center(),len*2/9,len*2/9);
	painter.drawEllipse(m_drawArea.center(), len / 6, len / 6);
	painter.drawEllipse(m_drawArea.center(), len / 9, len / 9);
	painter.drawEllipse(m_drawArea.center(), len / 18, len / 18);

    //四个刻度
	int w = width();
	int h = height();
	int min = std::min(w, h);
    int fontWidth = QFontMetrics(QFont()).width("0");
    int fontHeight = QFontMetrics(QFont()).height();	
    painter.drawText(QPoint(w/2-fontWidth/2, fontHeight), "0");    
	painter.drawText(QPoint((w + min) /2 - fontWidth * 2, h / 2 + fontHeight / 3), "90");
    painter.drawText(QPoint(w/2-fontWidth*1.5, h-fontHeight/3), "180");    
	painter.drawText(QPoint(dv, h / 2 + fontHeight / 3), "270");

    //转动部分
        //---//线
//    qreal x = m_drawArea.center().x() + (qreal)len/2 * cos(-m_pieRotate*3.14159/180);
//    qreal y = m_drawArea.center().y() + (qreal)len/2 * sin(-m_pieRotate*3.14159/180);
//    painter.setPen(QPen(Qt::white));
//    painter.drawLine(m_drawArea.center(),QPointF(x,y));

       //----//扇形
//    QConicalGradient gradient;
//    gradient.setCenter(m_drawArea.center());
//    gradient.setAngle(m_pieRotate + 180); //渐变与旋转方向恰好相反，以扇形相反的边作为渐变角度。
//    gradient.setColorAt(0.4,QColor(255,255,255,100)); //从渐变角度开始0.5 - 0.75为扇形区域，由于Int类型计算不精确，将范围扩大到0.4-0.8
//    gradient.setColorAt(0.8,QColor(255,255,255,0));
//    painter.setBrush(QBrush(gradient));
//    painter.setPen(Qt::NoPen);
//    painter.drawPie(m_drawArea,m_pieRotate*16,90*16);

    //装饰-随机点
    for(int i = 0; i < m_points.count(); ++i)
    {
 //       int colorAlaph = m_pointsAlapha.at(i);
//        painter.setPen(QPen(QColor(0,0,0,colorAlaph),5));
//        painter.drawPoint(m_points.at(i)+m_drawArea.center());
        QPointF po = m_points[i];
        po.setY((0 - po.y()) * len / 180);
		po.setX(po.x() * len / 180);
        painter.setPen(QPen(QColor(Qt::red),2));
//        painter.drawPoint(po+m_drawArea.center()); //方点
        painter.drawEllipse(po+m_drawArea.center(), 1, 1); //圆点
    }
}

void CRadar::resizeEvent(QResizeEvent *event)
{
    //以较短的边长作为绘制区域边长
	m_drawArea = getMaxSquare();
    /*if(width() > height())
    {
        m_drawArea = QRect((width() - height())/2,0,height(),height());
    }
    else
    {
        m_drawArea = QRect(0,(height() - width())/2,width(),width());
    }*/

	 /*****************************************************************************************
	 左上角和右下角的偏移, 
	 左上 正, 右下 负, 向内缩, 
	 左上 负, 右下 正, 向外扩
	 *****************************************************************************************/	
	m_drawArea.adjust(dv, dv, -dv, -dv);    
}

void CRadar::setPoints(const QList<QPointF> &points)
{
	if (points.empty()) return;

	m_points.clear();
	m_points = points;

	update();
}

void CRadar::appendPoints(const QList<QPointF> &points)
{
	if (points.empty()) return;

	m_points.append(points);

	update();
}

void CRadar::appendPoint(const QPointF &point)
{
	m_points.append(point);

	update();
}

QRect CRadar::getMaxSquare()
{
	int w = width();
	int h = height();
	int min = std::min(w, h);
	return QRect((w - min) / 2, (h - min) / 2, min, min);
}
