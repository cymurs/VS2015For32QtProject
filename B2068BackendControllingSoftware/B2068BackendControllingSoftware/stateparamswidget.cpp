#include "stdafx.h"
#include "stateparamswidget.h"
#include "cradar.h"
#include "transportthread.h"


/****************************************************************************************
设备总览
*****************************************************************************************/
DeviceOverviewTab::DeviceOverviewTab(QWidget *parent)
	: QWidget(parent)
{
	

	auto runTimeLabel = new QLabel(tr("运行时间"), this);
	m_runTime = new QLabel(tr("1天10时23分02秒"), this);
	m_runTime->setObjectName(valueLabelQss);

	auto vibrationType = new QLabel(tr("本振类型"), this);
	m_vibrationType = new QLabel(tr("铷钟 锁定"), this);
	m_vibrationType->setObjectName(valueLabelQss);

	auto tamingState = new QLabel(tr("驯服状态"), this);
	m_tamingState = new QLabel(tr("锁定"), this);
	m_tamingState->setObjectName(valueLabelQss);

	auto leapTime = new QLabel(tr("闰秒时间"), this);
	m_leapTime = new QLabel(tr("未知"), this);
	m_leapTime->setObjectName(valueLabelQss);

	auto curTimezone = new QLabel(tr("当前时区"), this);
	m_curTimezone = new QLabel(tr("东八区"), this);
	m_curTimezone->setObjectName(valueLabelQss);

	auto curRefSrc = new QLabel(tr("当前参考"), this);
	m_curRefSrc = new QLabel(tr("北斗"), this);
	m_curRefSrc->setObjectName(valueLabelQss);

	m_refSrcTable = new QTableWidget(5, 3, this);
	QStringList headerList;
	headerList << tr("参考源") << tr("输入状态") << tr("通道差值");
	m_refSrcTable->setHorizontalHeaderLabels(headerList); 	
	// 行列平均并随窗体自适应
	m_refSrcTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_refSrcTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_refSrcTable->verticalHeader()->hide(); 
	// 设置表头字体特殊格式
	m_refSrcTable->horizontalHeaderItem(2)->setTextColor(Qt::lightGray);
	// 设置不可编辑
	m_refSrcTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	// 隔行变色
	m_refSrcTable->setAlternatingRowColors(true);
	
	m_refSrcTable->setItem(0, 0, new QTableWidgetItem(tr("北斗")));
	m_refSrcTable->setItem(0, 1, new QTableWidgetItem(tr("同步")));
	m_refSrcTable->setItem(0, 2, new QTableWidgetItem);
	m_refSrcTable->setItem(1, 0, new QTableWidgetItem(tr("GPS")));
	m_refSrcTable->setItem(1, 1, new QTableWidgetItem(tr("失步")));
	m_refSrcTable->setItem(1, 2, new QTableWidgetItem);
	m_refSrcTable->setItem(2, 0, new QTableWidgetItem(tr("GLONASS")));
	m_refSrcTable->setItem(2, 1, new QTableWidgetItem(tr("失步")));
	m_refSrcTable->setItem(2, 2, new QTableWidgetItem);
	m_refSrcTable->setItem(3, 0, new QTableWidgetItem(tr("BDC码")));
	m_refSrcTable->setItem(3, 1, new QTableWidgetItem(tr("失步")));
	m_refSrcTable->setItem(3, 2, new QTableWidgetItem);
	m_refSrcTable->setItem(4, 0, new QTableWidgetItem(tr("BAC码")));
	m_refSrcTable->setItem(4, 1, new QTableWidgetItem(tr("失步")));
	m_refSrcTable->setItem(4, 2, new QTableWidgetItem);
	for (int col = 0; col < 3; ++col) {
		for (int row = 0; row < 5; ++row) {
			m_refSrcTable->item(row, col)->setTextAlignment(Qt::AlignCenter);
		}

		if (1 == col) {
			for (int row = 0; row < 5; ++row) {
				m_refSrcTable->item(row, col)->setTextColor(QColor("#0306A8"));
			}
		}
	}


	auto tableLayout = new QVBoxLayout;
	tableLayout->addWidget(m_refSrcTable);

	auto baseLayout = new QGridLayout(this);
	baseLayout->addWidget(runTimeLabel, 0, 1);
	baseLayout->addWidget(m_runTime, 0, 2);
	baseLayout->addWidget(vibrationType, 1, 1);
	baseLayout->addWidget(m_vibrationType, 1, 2);
	baseLayout->addWidget(tamingState, 1, 3);
	baseLayout->addWidget(m_tamingState, 1, 4);
	baseLayout->addWidget(leapTime, 2, 1);
	baseLayout->addWidget(m_leapTime, 2, 2);
	baseLayout->addWidget(curTimezone, 3, 1);
	baseLayout->addWidget(m_curTimezone, 3, 2);
	baseLayout->addWidget(curRefSrc, 3, 3);
	baseLayout->addWidget(m_curRefSrc, 3, 4);
	baseLayout->addLayout(tableLayout, 4, 1, 6, 3);
	baseLayout->setColumnStretch(0, 1);
	baseLayout->setColumnStretch(1, 2);
	baseLayout->setColumnStretch(2, 3);
	baseLayout->setColumnStretch(3, 2);
	baseLayout->setColumnStretch(4, 3);
	baseLayout->setColumnStretch(5, 1);

	//setStyleSheet(QSS_DeviceOverview.arg(valueLabelQss));
}

/****************************************************************************************
北斗状态 GPS状态 GLONASS状态
*****************************************************************************************/
SatTypeStateTab::SatTypeStateTab(QWidget *parent /*= 0*/)
{
	auto workStateLabel = new QLabel(tr("工作状态"), this);
	m_workState = new QLabel(tr("正常"), this);
	m_workState->setObjectName(valueLabelQss);
	auto signStateLabel = new QLabel(tr("信号状态"), this);
	m_signState = new QLabel(tr("正常"), this);
	m_signState->setObjectName(valueLabelQss);
	auto pdopLabel = new QLabel(tr("PDOP"), this);
	m_pdop = new QLabel(tr("0.8"), this);
	m_pdop->setObjectName(valueLabelQss);
	auto visSatellitesLabel = new QLabel(tr("可见卫星"), this);
	m_visSatellites = new QLabel(tr("13"), this);
	m_visSatellites->setObjectName(valueLabelQss);
	auto avlSatellitesLabel = new QLabel(tr("可用卫星"), this);
	m_avlSatellites = new QLabel(tr("10"), this);
	m_avlSatellites->setObjectName(valueLabelQss);


	QVector<QStringList> itemLists;
	itemLists << QString("1,2,3,4,5,7,8,10,11,12").split(',')
		<< QString("51,47,64,33,23,72,71,75,20,52").split(',')
		<< QString("126,236,189,109,254,131,325,324,266,325").split(',')
		<< QString("47,46,49,44,50,49,50,49,44,51").split(',');
	 
	QStandardItemModel *model = new QStandardItemModel;
	model->setColumnCount(4);
	model->setHeaderData(0, Qt::Horizontal, tr("卫星号"));
	model->setHeaderData(1, Qt::Horizontal, tr("仰角"));
	model->setHeaderData(2, Qt::Horizontal, tr("方位角"));
	model->setHeaderData(3, Qt::Horizontal, tr("信噪比"));
	model->setRowCount(10);
	for (int col = 0; col < 4; ++col) {
		for (int row = 0; row < 10; ++row) {
			QStandardItem *item = new QStandardItem(itemLists[col][row]);
			item->setTextAlignment(Qt::AlignCenter);
			model->setItem(row, col, item);			
		}
	}	
	
	m_satTypeTable = new QTableView(this);
	// 行列平均并随窗体自适应
	m_satTypeTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_satTypeTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_satTypeTable->verticalHeader()->hide();	
	// 设置不可编辑
	m_satTypeTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	// 隔行变色
	m_satTypeTable->setAlternatingRowColors(true); 
	m_satTypeTable->setObjectName(valueTableViewQss);
	m_satTypeTable->setModel(model);

	// 在雷达图上绘制点
	m_elevAzimParams = new ElevationAzimuthParams;
	double *elev = m_elevAzimParams->elevations;
	double *azim = m_elevAzimParams->azimuths;
	m_elevAzimParams->count = itemLists[1].size();
	QStringList elevList = itemLists[1];
	QStringList azimList = itemLists[2];
	QList<QPointF> points;
	for (int idx = 0; idx < m_elevAzimParams->count; ++idx) {
		elev[idx] = elevList[idx].toDouble();
		azim[idx] = azimList[idx].toDouble();
		
		//double radius = elev[idx];
		//double angle = (azim[idx] + 90) / 360.0 * 6.28;
		//points << QPointF( -radius * cos(angle), radius * sin(angle) );
		points << angleToCoordinate(elev[idx], azim[idx]);
	}

	m_radar = new CRadar(this);
	m_radar->setPoints(points);

	auto baseLayout = new QGridLayout(this);
	baseLayout->addWidget(workStateLabel, 0, 1);
	baseLayout->addWidget(m_workState, 0, 2);
	baseLayout->addWidget(m_satTypeTable, 0, 4, 6, 1);
	baseLayout->addWidget(signStateLabel, 1, 1);
	baseLayout->addWidget(m_signState, 1, 2);
	baseLayout->addWidget(pdopLabel, 2, 1);
	baseLayout->addWidget(m_pdop, 2, 2);
	baseLayout->addWidget(visSatellitesLabel, 3, 1);
	baseLayout->addWidget(m_visSatellites, 3, 2);
	baseLayout->addWidget(avlSatellitesLabel, 4, 1);
	baseLayout->addWidget(m_avlSatellites, 4, 2);
	baseLayout->addWidget(m_radar, 5, 1, 1, 2);
	baseLayout->setColumnStretch(0, 0);
	baseLayout->setColumnStretch(1, 2);
	baseLayout->setColumnStretch(2, 2);
	baseLayout->setColumnStretch(3, 0);
	baseLayout->setColumnStretch(4, 6);
	baseLayout->setColumnStretch(5, 0);
	baseLayout->setRowStretch(0, 1);
	baseLayout->setRowStretch(1, 1);
	baseLayout->setRowStretch(2, 1);
	baseLayout->setRowStretch(3, 1);
	baseLayout->setRowStretch(4, 1);
	baseLayout->setRowStretch(5, 6);
	baseLayout->setContentsMargins(10, 5, 5, 5);
}

void SatTypeStateTab::setGnssstaInfo(const st_Gnsssta &gnsssta)
{
	/****************************************************************************************/
	// 测试专用
	if (0 == strncmp(gnsssta.satelliteType, "bds", 3)) {
		return;
	}
		
	if (0 == strncmp(gnsssta.satelliteType, "gps", 3)) {
		m_visSatellites->setText("10");
		m_avlSatellites->setText("4");
		return;
	}

	if (0 == strncmp(gnsssta.satelliteType, "glo", 3)) {
		m_visSatellites->setText("8");
		m_avlSatellites->setText("2");
		return;
	}
	/*****************************************************************************************/
	
	
	m_workState->setText(tr("异常"));
	m_signState->setText(tr("异常"));
	m_pdop->setText(QString::number(gnsssta.pdop));
	m_visSatellites->setText(QString::number(gnsssta.visSatellites));
	m_avlSatellites->setText(QString::number(gnsssta.avlSatellites));
	if (gnsssta.avlSatellites > 0 && 1 == gnsssta.d3PosFlag && 1 == gnsssta.rmcStateFlag)
		m_workState->setText(tr("正常"));
	if (gnsssta.visSatellites > 3 && 1 == gnsssta.snrFlag)
		m_signState->setText(tr("正常"));
}

void SatTypeStateTab::setGnssgsvInfo(const st_Gnssgsv &gnssgsv)
{
	/****************************************************************************************/
	// 测试专用
	if (0 == strncmp(gnssgsv.satelliteType, "bds", 3)) {
		return;
	}

	if (0 == strncmp(gnssgsv.satelliteType, "gps", 3)) {
		
		//return;
	}

	if (0 == strncmp(gnssgsv.satelliteType, "glo", 3)) {
		int row(0);
		QStandardItemModel *model = (QStandardItemModel *)m_satTypeTable->model();
		model->setData(model->index(row, 0), 9);
		model->setData(model->index(row, 1), 8);
		model->setData(model->index(row, 2), 100);
		model->setData(model->index(row, 3), 40);
		++row;
		model->setData(model->index(row, 0), 13);
		model->setData(model->index(row, 1), 44);
		model->setData(model->index(row, 2), 159);
		model->setData(model->index(row, 3), 50);
		++row;
		QList<QPointF> points;
		points << angleToCoordinate(8, 100) << angleToCoordinate(44, 159);
		m_radar->setPoints(points);


		while (row < 10) {
			model->setData(model->index(row, 0), "");
			model->setData(model->index(row, 1), "");
			model->setData(model->index(row, 2), "");
			model->setData(model->index(row, 3), "");
			++row;
		}

		return;
	}
	/*****************************************************************************************/

	int row(0);
	QList<QPointF> points;
	// 默认按一帧4个卫星号计算
	if (gnssgsv.frameTotal > 1)
		row = (gnssgsv.frameNo - 1) * 4;
	QStandardItemModel *model = (QStandardItemModel *)m_satTypeTable->model();
	for (int i = 0; i < gnssgsv.count; ++i) {
		model->setData(model->index(row, 0), gnssgsv.satelliteNo[i]);
		model->setData(model->index(row, 1), gnssgsv.elevation[i]);
		model->setData(model->index(row, 2), gnssgsv.azimuth[i]);
		model->setData(model->index(row, 3), gnssgsv.snr[i]);
		++row;

		points << angleToCoordinate(gnssgsv.elevation[i], gnssgsv.azimuth[i]);
	}
	if (gnssgsv.frameTotal > 1 && gnssgsv.frameNo > 1)
		m_radar->appendPoints(points);
	else
		m_radar->setPoints(points);

	while (row < 10) {
		model->setData(model->index(row, 0), "");
		model->setData(model->index(row, 1), "");
		model->setData(model->index(row, 2), "");
		model->setData(model->index(row, 3), "");
		++row;
	}
}

QPointF SatTypeStateTab::angleToCoordinate(double elev, double azim)
{
	double radius = elev;
	double angle = (azim + 90) / 360.0 * 6.28;
	return QPointF(-radius * cos(angle), radius * sin(angle));
}

/****************************************************************************************
直流B码状态
*****************************************************************************************/
DCBStateTab::DCBStateTab(QWidget *parent /*= 0*/)
	: QWidget(parent)
	, m_lblWidth(LblWidth * 1.5)
	, m_lblHeight(LblHeight * 1.5)
{
	int w = width();
	int h = height();

	QStringList formats = {"GJB97", "GJB08", "200-04"};
	
	m_runStateLabel = new QLabel(tr("运行状态"), this);	
	m_runState = new QLabel(tr("异常"), this);
	m_runState->setObjectName(valueLabelQss);

	m_synStateLabel = new QLabel(tr("同步状态"), this);
	m_synState = new QLabel(tr("异常"), this);
	m_synState->setObjectName(valueLabelQss);

	m_inFormatLabel = new QLabel(tr("直流B码输入格式"), this);
	m_inFormat = new QComboBox(this);
	m_inFormat->addItems(formats);

	m_outFormatLabel = new QLabel(tr("直流B码输出格式"), this);
	m_outFormat = new QComboBox(this);
	m_outFormat->addItems(formats);

	m_confirm = new QPushButton(tr("确认设置"), this);
	
	m_runState->setAlignment(Qt::AlignCenter);
	m_synState->setAlignment(Qt::AlignCenter);
	m_inFormat->setProperty("letterField", true);
	m_outFormat->setProperty("letterField", true);
	setTextAlignCenter(m_inFormat);
	setTextAlignCenter(m_outFormat);

	setChildrenGeometry(w, h);
	
}

void DCBStateTab::resizeEvent(QResizeEvent *event)
{
	QSize s = event->size();
	int w = s.width();
	int h = s.height();
	if (0 == w && 0 == h) {
		QWidget::resizeEvent(event);
		return;
	}

	setChildrenGeometry(w, h);
}

void DCBStateTab::setTextAlignCenter(QComboBox *comboBox)
{
	int items = comboBox->count();
	if (0 == items) return;

	for (int i = 0; i < items; ++i) {
		static_cast<QStandardItemModel *>(comboBox->model())->item(i)->setTextAlignment(Qt::AlignCenter);
	}
}

void DCBStateTab::setChildrenGeometry(int w /*= 0*/, int h /*= 0*/)
{
	if (0 == w && 0 == h) return;

	int wdth = LblWidth;	
	int hght = LblHeight;
	m_runStateLabel->setGeometry(w / 6, (h - hght * 10) / 2, m_lblWidth, m_lblHeight);
	m_runState->setGeometry(w / 2, (h - hght * 10) / 2, wdth, m_lblHeight);
	m_synStateLabel->setGeometry(w / 6, (h - hght * 6) / 2, m_lblWidth, m_lblHeight);
	m_synState->setGeometry(w / 2, (h - hght * 6) / 2, wdth, m_lblHeight);
	m_inFormatLabel->setGeometry(w / 6, (h - hght * 2) / 2, LblWidth * 2, m_lblHeight);
	m_inFormat->setGeometry(w / 2, (h - hght * 2) / 2, wdth, m_lblHeight);
	m_outFormatLabel->setGeometry(w / 6, h / 2 + hght, LblWidth * 2, m_lblHeight);
	m_outFormat->setGeometry(w / 2, h / 2 + hght, wdth, m_lblHeight);
	m_confirm->setGeometry(w / 2 + wdth / 2, h / 2 + hght * 4, m_lblWidth, m_lblHeight);
}

/****************************************************************************************
交流B码状态
*****************************************************************************************/
ACBStateTab::ACBStateTab(QWidget *parent /*= 0*/)
	: QWidget(parent)
	, m_lblWidth(LblWidth)
	, m_lblHeight(LblHeight)
{
	int w = width();
	int h = height();

	QStringList formats = { "GJB97", "GJB08", "200-04" };
	QStringList modRatio = { "2:1", "3:1", "4:1", "5:1", "6:1" };

	m_runStateLabel = new QLabel(tr("运行状态"), this);
	m_runState = new QLabel(tr("异常"), this);
	m_runState->setObjectName(valueLabelQss);

	m_synStateLabel = new QLabel(tr("同步状态"), this);
	m_synState = new QLabel(tr("异常"), this);
	m_synState->setObjectName(valueLabelQss);

	m_inFormatLabel = new QLabel(tr("交流B码输入格式"), this);
	m_inFormat = new QComboBox(this);
	m_inFormat->addItems(formats);

	m_outFormatLabel = new QLabel(tr("交流B码输出格式"), this);
	m_outFormat = new QComboBox(this);
	m_outFormat->addItems(formats);

	m_outAmpLabel = new QLabel(tr("交流B码输出幅值"), this);
	m_outAmplitude = new QLineEdit(tr("1.5"), this);
	// 无效代码
	//QDoubleValidator *validator = new QDoubleValidator(0.5, 10.0, 1, this);
	//validator->setBottom(0.5);
	QRegExp rx("^10|\\d\\.5|[1-9]$");
	QValidator *validator = new QRegExpValidator(rx, this);
	m_outAmplitude->setValidator(validator);
	m_outAmpUnit = new QLabel(tr("V"), this);

	m_outMrLabel = new QLabel(tr("交流B码输出调制比"), this);
	m_outMr = new QComboBox(this);
	m_outMr->addItems(modRatio);

	m_confirm = new QPushButton(tr("确认设置"), this);

	m_runState->setAlignment(Qt::AlignCenter);
	m_synState->setAlignment(Qt::AlignCenter);
	m_outAmplitude->setAlignment(Qt::AlignCenter);
	m_inFormat->setProperty("letterField", true);
	m_outFormat->setProperty("letterField", true);
	m_outMr->setProperty("numField", true);
	setTextAlignCenter(m_inFormat);
	setTextAlignCenter(m_outFormat);
	setTextAlignCenter(m_outMr);
	
	setChildrenGeometry(w, h);

}

void ACBStateTab::resizeEvent(QResizeEvent *event)
{
	QSize s = event->size();
	int w = s.width();
	int h = s.height();
	if (0 == w && 0 == h) {
		QWidget::resizeEvent(event);
		return;
	}

	setChildrenGeometry(w, h);
}

void ACBStateTab::setTextAlignCenter(QComboBox *comboBox)
{
	int items = comboBox->count();
	if (0 == items) return;

	for (int i = 0; i < items; ++i) {
		static_cast<QStandardItemModel *>(comboBox->model())->item(i)->setTextAlignment(Qt::AlignCenter);
	}
}

void ACBStateTab::setChildrenGeometry(int w /*= 0*/, int h /*= 0*/)
{
	if (0 == w && 0 == h)  return;

	int wdth = LblWidth;
	int hght = LblHeight * 1.5;
	int dblWdth = LblWidth * 2;
	m_runStateLabel->setGeometry(w / 6, (h - m_lblHeight * 14) / 2, m_lblWidth, m_lblHeight);
	m_runState->setGeometry(w / 2, (h - m_lblHeight * 14) / 2, m_lblWidth, m_lblHeight);
	m_synStateLabel->setGeometry(w / 6, (h - m_lblHeight * 10) / 2, m_lblWidth, m_lblHeight);
	m_synState->setGeometry(w / 2, (h - m_lblHeight * 10) / 2, m_lblWidth, m_lblHeight);
	m_inFormatLabel->setGeometry(w / 6, (h - m_lblHeight * 6) / 2, dblWdth, hght);
	m_inFormat->setGeometry(w / 2, (h - m_lblHeight * 6) / 2, wdth, hght);
	m_outFormatLabel->setGeometry(w / 6, (h - m_lblHeight * 2) / 2, dblWdth, hght);
	m_outFormat->setGeometry(w / 2, (h - m_lblHeight * 2) / 2, wdth, hght);
	m_outAmpLabel->setGeometry(w / 6, h / 2 + m_lblHeight, dblWdth, hght);
	m_outAmplitude->setGeometry(w / 2, h / 2 + m_lblHeight, wdth * 4 / 5, hght);
	m_outAmpUnit->setGeometry(w / 2 + wdth * 5 / 6, h / 2 + m_lblHeight, wdth / 6, hght);
	m_outMrLabel->setGeometry(w / 6, h / 2 + m_lblHeight * 3, dblWdth, hght);
	m_outMr->setGeometry(w / 2, h / 2 + m_lblHeight * 3, wdth, hght);
	m_confirm->setGeometry(w / 2 + wdth / 2, h / 2 + m_lblHeight * 6, LblWidth * 1.5, hght);
}

/****************************************************************************************
延时补偿
*****************************************************************************************/
DelayCompensationTab::DelayCompensationTab(QWidget *parent /*= 0*/)
	: QWidget(parent)
	, m_lblWidth(LblWidth)
	, m_lblHeight(LblHeight)
{
	int w = width();
	int h = height();
	
	m_bdsLabel = new QLabel(tr("北斗"), this);
	m_bds = new QLineEdit(tr("0"), this);
	m_bds->setAlignment(Qt::AlignCenter);
	m_bdsUnit = new QLabel(tr("ns"), this);	

	m_gpsLabel = new QLabel(tr("GPS"), this);
	m_gps = new QLineEdit(tr("0"), this);
	m_gps->setAlignment(Qt::AlignCenter);
	m_gpsUnit = new QLabel(tr("ns"), this);

	m_gloLabel = new QLabel(tr("GLONASS"), this);
	m_glo = new QLineEdit(tr("0"), this);
	m_glo->setAlignment(Qt::AlignCenter);
	m_gloUnit = new QLabel(tr("ns"), this);

	m_acbLabel = new QLabel(tr("交流B码"), this);
	m_acb = new QLineEdit(tr("0"), this);
	m_acb->setAlignment(Qt::AlignCenter);
	m_acbUnit = new QLabel(tr("ns"), this);

	m_dcbLabel = new QLabel(tr("直流B码"), this);
	m_dcb = new QLineEdit(tr("0"), this);
	m_dcb->setAlignment(Qt::AlignCenter);
	m_dcbUnit = new QLabel(tr("ns"), this);

	m_confirm = new QPushButton(tr("确认设置"), this);

	setChildrenGeometry(w, h);
}

void DelayCompensationTab::resizeEvent(QResizeEvent *event)
{
	QSize s = event->size();
	int w = s.width();
	int h = s.height();
	if (0 == w && 0 == h) {
		QWidget::resizeEvent(event);
		return;
	}

	setChildrenGeometry(w, h);
}

void DelayCompensationTab::setChildrenGeometry(int w /*= 0*/, int h /*= 0*/)
{
	if (0 == w && 0 == h)  return;

	int wdth = LblWidth * 1.5;
	int hght = LblHeight * 1.5;
	int mid = w / 4 + LblWidth * 1.2;
	m_bdsLabel->setGeometry(w / 4, (h - m_lblHeight * 12) / 2, wdth, hght);
	m_bds->setGeometry(mid, (h - m_lblHeight * 12) / 2, wdth, hght);
	m_bdsUnit->setGeometry(mid + wdth * 1.1, (h - m_lblHeight * 12) / 2, wdth, hght);
	m_gpsLabel->setGeometry(w / 4, (h - m_lblHeight * 8) / 2, wdth, hght);
	m_gps->setGeometry(mid, (h - m_lblHeight * 8) / 2, wdth, hght);
	m_gpsUnit->setGeometry(mid + wdth * 1.1, (h - m_lblHeight * 8) / 2, wdth, hght);
	m_gloLabel->setGeometry(w / 4, (h - m_lblHeight * 4) / 2, wdth, hght);
	m_glo->setGeometry(mid, (h - m_lblHeight * 4) / 2, wdth, hght);
	m_gloUnit->setGeometry(mid + wdth * 1.1, (h - m_lblHeight * 4) / 2, wdth, hght);
	m_acbLabel->setGeometry(w / 4, h  / 2, wdth, hght);
	m_acb->setGeometry(mid, h / 2, wdth, hght);
	m_acbUnit->setGeometry(mid + wdth * 1.1, h / 2, wdth, hght);
	m_dcbLabel->setGeometry(w / 4, h / 2 + m_lblHeight * 2, wdth, hght);
	m_dcb->setGeometry(mid, h / 2 + m_lblHeight * 2, wdth, hght);
	m_dcbUnit->setGeometry(mid + wdth * 1.1, h / 2 + m_lblHeight * 2, wdth, hght);
	m_confirm->setGeometry(mid + wdth / 2, h / 2 + m_lblHeight * 5, wdth, hght);
}

/****************************************************************************************
脉冲配置
*****************************************************************************************/
PulseSettingsTab::PulseSettingsTab(QWidget *parent /*= 0*/)
	: QWidget(parent)
	, m_lblWidth(LblWidth * 1.5)
	, m_lblHeight(LblHeight * 1.5)
{
	QStringList unit = { "ms", "ns" };
	
	QRegExp rxmsWidth("^([1-9]\\d{2}|[1-9]\\d|[1-9])$");
	m_msWidthValidator = new QRegExpValidator(rxmsWidth, this);
	QRegExp rxnsWidth("[1-9]\\d{3}|[1-9]\\d{4}|[1-9]\\d{5}|[1-9]\\d{6}|[1-9]\\d{7}|[1-9]\\d{8}");
	m_nsWidthValidator = new QRegExpValidator(rxnsWidth, this);
	QRegExp rxmsDelay("-?([1-9]?\\d|[1-9]\\d{2})");
	m_msDelayValidator = new QRegExpValidator(rxmsDelay, this);
	QRegExp rxnsDelay("-?([1-9]?\\d|[1-9]\\d{2}|[1-9]\\d{3}|[1-9]\\d{4}|[1-9]\\d{5}|[1-9]\\d{6}|[1-9]\\d{7}|[1-9]\\d{8})");
	m_nsDelayValidator = new QRegExpValidator(rxnsDelay, this);

	m_ppsWidthLabel = new QLabel(tr("脉宽"), this);
	m_ppsWidth = new QLineEdit(tr("30"), this);
	m_ppsWidth->setAlignment(Qt::AlignCenter);	
	m_ppsWidthUnit = new QComboBox(this);
	m_ppsWidthUnit->addItems(unit);
	m_ppsWidthUnit->setCurrentIndex(1);
	m_ppsWidth->setValidator(m_nsWidthValidator);

	m_ppsDelayLabel = new QLabel(tr("延时"), this);
	m_ppsDelay = new QLineEdit(tr("0"), this);
	m_ppsDelay->setAlignment(Qt::AlignCenter);	
	m_ppsDelayUnit = new QComboBox(this);	
	m_ppsDelayUnit->addItems(unit);
	m_ppsDelayUnit->setCurrentIndex(1);
	m_ppsDelay->setValidator(m_nsDelayValidator);
	// 丑得很
	//QLineEdit *centerLineEdit = new QLineEdit;	
	//centerLineEdit->setReadOnly(true);
	//centerLineEdit->setAlignment(Qt::AlignCenter);
	//m_ppsDelayUnit->setLineEdit(centerLineEdit);

	m_confirm = new QPushButton(tr("确认设置"), this);

	int w = width();
	int h = height();
	setChildrenGeometry(w, h);
	connectSlots();
}

void PulseSettingsTab::resizeEvent(QResizeEvent *event)
{
	QSize s = event->size();
	int w = s.width();
	int h = s.height();
	if (0 == w && 0 == h) {
		QWidget::resizeEvent(event);
		return;
	}

	setChildrenGeometry(w, h);
}

void PulseSettingsTab::setChildrenGeometry(int w /*= 0*/, int h /*= 0*/)
{
	if (0 == w && 0 == h) return;
	
	int wdth = LblWidth;
	int unitWdth = LblWidth * 0.6;
	m_ppsWidthLabel->setGeometry(w / 4, (h - m_lblHeight * 6) / 2, wdth, m_lblHeight);
	m_ppsWidth->setGeometry(w / 4 + wdth, (h - m_lblHeight * 6) / 2, m_lblWidth, m_lblHeight);
	m_ppsWidthUnit->setGeometry(w / 4 + LblWidth*2.6, (h - m_lblHeight * 6) / 2, unitWdth, m_lblHeight);
	m_ppsDelayLabel->setGeometry(w / 4, (h - m_lblHeight * 2) / 2, wdth, m_lblHeight);
	m_ppsDelay->setGeometry(w / 4 + wdth, (h - m_lblHeight * 2) / 2, m_lblWidth, m_lblHeight);
	m_ppsDelayUnit->setGeometry(w / 4 + LblWidth*2.6, (h - m_lblHeight * 2) / 2, unitWdth, m_lblHeight);
	m_confirm->setGeometry(w / 2 + unitWdth, h / 2 + m_lblHeight * 3, m_lblWidth, m_lblHeight);
}

void PulseSettingsTab::connectSlots()
{
	connect(m_ppsWidthUnit, SIGNAL(currentIndexChanged(int)), this, SLOT(slotOnWidthUnitChanged(int)));
	connect(m_ppsDelayUnit, SIGNAL(currentIndexChanged(int)), this, SLOT(slotOnDelayUnitChanged(int)));
}

void PulseSettingsTab::slotOnWidthUnitChanged(int index)
{
	m_ppsWidth->clear();
	if (0 == index)
		m_ppsWidth->setValidator(m_msWidthValidator); // (new QIntValidator(1, 999, this)); ---体验差
	else
		m_ppsWidth->setValidator(m_nsWidthValidator);
}

void PulseSettingsTab::slotOnDelayUnitChanged(int index)
{
	m_ppsDelay->clear();
	if (0 == index)
		m_ppsDelay->setValidator(m_msDelayValidator);
	else
		m_ppsDelay->setValidator(m_nsDelayValidator);
}

/****************************************************************************************
时区设置
*****************************************************************************************/
TimezoneTab::TimezoneTab(QWidget *parent /*= 0*/)
	: QWidget(parent)
	, m_lblWidth(LblWidth * 1.5)
	, m_lblHeight(LblHeight * 1.5)
{
	m_timezoneLabel = new QLabel(tr("时区"), this);
	m_timezone = new QLineEdit(tr("8"), this);
	m_timezone->setAlignment(Qt::AlignCenter);
	m_confirm = new QPushButton(tr("确认设置"), this);
	
	int w = width();
	int h = height();
	setChildrenGeometry(w, h);
}

void TimezoneTab::resizeEvent(QResizeEvent *event)
{
	QSize s = event->size();
	int w = s.width();
	int h = s.height();
	if (0 == w && 0 == h) {
		QWidget::resizeEvent(event);
		return;
	}

	setChildrenGeometry(w, h);
}

void TimezoneTab::setChildrenGeometry(int w /*= 0*/, int h /*= 0*/)
{
	if (0 == w && 0 == h) return;

	m_timezoneLabel->setGeometry(w / 5, h / 3, LblWidth, m_lblHeight);
	m_timezone->setGeometry(w / 5 + LblWidth, h / 3, m_lblWidth, m_lblHeight);
	m_confirm->setGeometry(w / 5 + m_lblWidth * 2, h / 3, m_lblWidth, m_lblHeight);
}

/****************************************************************************************
日志信息 告警信息
*****************************************************************************************/
LogInfoTab::LogInfoTab(QWidget *parent /*= 0*/)
	: QWidget(parent)
	, m_maxCount(2)
{
	m_log = new QTextEdit(tr("001:20180827-113045:切换到锁定状态"), this);

	auto baseLayout = new QHBoxLayout(this);
	baseLayout->addWidget(m_log);
}

void LogInfoTab::prepend(const QString &info)
{
	int cnt = m_log->document()->lineCount();
	int diff = cnt - m_maxCount;
	if (diff >= 0) {
		m_log->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
		m_log->moveCursor(QTextCursor::Up, QTextCursor::KeepAnchor);
		--diff;
		while (diff >= 0) {
			m_log->moveCursor(QTextCursor::Up, QTextCursor::KeepAnchor);
			--diff;
		}
		m_log->textCursor().removeSelectedText();
	}
	
	m_log->moveCursor(QTextCursor::Start, QTextCursor::MoveAnchor);
	m_log->insertPlainText(info);
}

void LogInfoTab::append(const QString &info)
{
	int cnt = m_log->document()->lineCount();
	int diff = cnt - m_maxCount;
	if (diff >= 0) {
		m_log->moveCursor(QTextCursor::Start, QTextCursor::MoveAnchor);
		m_log->moveCursor(QTextCursor::Down, QTextCursor::KeepAnchor);
		--diff;
		while (diff >= 0) {
			m_log->moveCursor(QTextCursor::Down, QTextCursor::KeepAnchor);
			--diff;
		}
		m_log->textCursor().removeSelectedText();
	}

	m_log->append(info);
}

/****************************************************************************************
屏幕设置
*****************************************************************************************/
ScreenSettingTab::ScreenSettingTab(QWidget *parent /*= 0*/)
	: QWidget(parent)
	, m_lblWidth(LblWidth * 1.5)
	, m_lblHeight(LblHeight * 1.5)
{
	m_buzzer = new QCheckBox(tr("蜂鸣器          "), this);	
	m_buzzer->setLayoutDirection(Qt::RightToLeft);	

	m_confirm = new QPushButton(tr("确认设置"), this);

	int w = width();
	int h = height();
	setChildrenGeometry(w, h);
}

void ScreenSettingTab::resizeEvent(QResizeEvent *event)
{
	QSize s = event->size();
	int w = s.width();
	int h = s.height();
	if (0 == w && 0 == h) {
		QWidget::resizeEvent(event);
		return;
	}

	setChildrenGeometry(w, h);
}

void ScreenSettingTab::setChildrenGeometry(int w /*= 0*/, int h /*= 0*/)
{
	if (0 == w && 0 == h) return;

	m_buzzer->setGeometry(w / 2 - LblWidth, h / 2 - m_lblHeight, m_lblWidth, m_lblHeight);
	m_confirm->setGeometry((w + m_lblWidth) / 2, h / 2 + m_lblHeight * 2, m_lblWidth, m_lblHeight);
}

/****************************************************************************************
出厂设置
*****************************************************************************************/
FactorySettingTab::FactorySettingTab(QWidget *parent /*= 0*/)
	: QWidget(parent)
	, m_lblWidth(LblWidth * 1.5)
	, m_lblHeight(LblHeight * 1.5)
{
	m_devTypeLabel = new QLabel(tr("1. 设备型号"), this);
	m_b2d = new QCheckBox(tr("B2068-2D"), this);
	m_b3 = new QCheckBox(tr("B2068-3"), this);
	m_typeConfirm = new QPushButton(tr("确认设置"), this);

	m_devIDLabel = new QLabel(tr("2. 设备编号"), this);
	m_devID = new QLineEdit(this);
	m_idConfirm = new QPushButton(tr("确认设置"), this);

	m_hwVersionLabel = new QLabel(tr("3. 硬件版本"), this);
	m_hwVersion = new QLineEdit(this);
	m_verConfirm = new QPushButton(tr("确认设置"), this);

	m_card1Label = new QLabel(tr("4. 网卡1物理地址"), this);
	m_networkCard1 = new QLineEdit(this);
	m_card1Confirm = new QPushButton(tr("确认设置"), this);

	m_card2Label = new QLabel(tr("5. 网卡2物理地址"), this);
	m_networkCard2 = new QLineEdit(this);
	m_card2Confirm = new QPushButton(tr("确认设置"), this);

	int w = width();
	int h = height();
	setChildrenGeometry(w, h);
}

void FactorySettingTab::resizeEvent(QResizeEvent *event)
{
	QSize s = event->size();
	int w = s.width();
	int h = s.height();
	if (0 == w && 0 == h) {
		QWidget::resizeEvent(event);
		return;
	}

	setChildrenGeometry(w, h);
}

void FactorySettingTab::setChildrenGeometry(int w /*= 0*/, int h /*= 0*/)
{
	if (0 == w && 0 == h) return;

	int left = LblHeight * 0.5;
	int top = LblHeight * 6 + m_lblHeight;
	int lineHght = LblHeight * 1.2;
	int lineWdth = LblWidth * 2.6;
	m_devTypeLabel->setGeometry(left, LblHeight, m_lblWidth, m_lblHeight);
	m_b2d->setGeometry(left * 1.5, LblHeight * 3, m_lblWidth, m_lblHeight);
	m_b3->setGeometry(left * 1.5, LblHeight * 5, m_lblWidth, m_lblHeight);
	m_typeConfirm->setGeometry(left * 1.5 + m_lblWidth * 1.5, LblHeight * 4, m_lblWidth, m_lblHeight);
	m_devIDLabel->setGeometry(left, top, m_lblWidth, m_lblHeight);
	m_devID->setGeometry(left + m_lblWidth * 1.1, top, lineWdth, m_lblHeight);
	m_idConfirm->setGeometry(left + m_lblWidth * 2.9, top, m_lblWidth, m_lblHeight);
	m_hwVersionLabel->setGeometry(left, top + lineHght * 2, m_lblWidth, m_lblHeight);
	m_hwVersion->setGeometry(left + m_lblWidth * 1.1, top + lineHght * 2, lineWdth, m_lblHeight);
	m_verConfirm->setGeometry(left + m_lblWidth * 2.9, top + lineHght * 2, m_lblWidth, m_lblHeight);
	m_card1Label->setGeometry(left, top + lineHght * 4, m_lblWidth * 1.5, m_lblHeight);
	m_networkCard1->setGeometry(left + m_lblWidth * 1.1, top + lineHght * 4, lineWdth, m_lblHeight);
	m_card1Confirm->setGeometry(left + m_lblWidth * 2.9, top + lineHght * 4, m_lblWidth, m_lblHeight);
	m_card2Label->setGeometry(left, top + lineHght * 6, m_lblWidth * 1.5, m_lblHeight);
	m_networkCard2->setGeometry(left + m_lblWidth * 1.1, top + lineHght * 6, lineWdth, m_lblHeight);
	m_card2Confirm->setGeometry(left + m_lblWidth * 2.9, top + lineHght * 6, m_lblWidth, m_lblHeight);
}

/****************************************************************************************
密码修改
*****************************************************************************************/
PasswordChangeTab::PasswordChangeTab(QWidget *parent /*= 0*/)
	: QWidget(parent)
	, m_lblWidth(LblWidth * 1.5)
	, m_lblHeight(LblHeight * 1.5)
{
	QRegExp rx("^\\d{6}$");
	QValidator *validator = new QRegExpValidator(rx, this);
	
	m_oldPwdLabel = new QLabel(tr("旧密码"), this); 	
	m_oldPwd = new QLineEdit(this);
	m_oldPwd->setAlignment(Qt::AlignCenter);
	m_oldPwd->setValidator(validator);
	m_oldPwd->setEchoMode(QLineEdit::Password);
	m_newPwdLabel = new QLabel(tr("新密码"), this);
	m_newPwd = new QLineEdit(this);
	m_newPwd->setAlignment(Qt::AlignCenter);
	m_newPwd->setValidator(validator);
	m_newPwd->setEchoMode(QLineEdit::Password);
	m_againPwdLabel = new QLabel(tr("确认新密码"), this);
	m_againPwd = new QLineEdit(this);
	m_againPwd->setAlignment(Qt::AlignCenter);
	m_againPwd->setValidator(validator);
	m_againPwd->setEchoMode(QLineEdit::Password);
	m_confirm = new QPushButton(tr("确认设置"), this);

	int w = width();
	int h = height();
	setChildrenGeometry(w, h);

	connectSlots();
}

void PasswordChangeTab::resizeEvent(QResizeEvent *event)
{
	QSize s = event->size();
	int w = s.width();
	int h = s.height();
	if (0 == w && 0 == h) {
		QWidget::resizeEvent(event);
		return;
	}

	setChildrenGeometry(w, h);
}

void PasswordChangeTab::setChildrenGeometry(int w /*= 0*/, int h /*= 0*/)
{
	if (0 == w && 0 == h) return;

	int left = w / 4;
	int top = (h - m_lblHeight * 8) / 2;
	m_oldPwdLabel->setGeometry(left, top, m_lblWidth, m_lblHeight);
	m_oldPwd->setGeometry(left + m_lblWidth, top, m_lblWidth, m_lblHeight);
	m_newPwdLabel->setGeometry(left, top + m_lblHeight * 2, m_lblWidth, m_lblHeight);
	m_newPwd->setGeometry(left + m_lblWidth, top + m_lblHeight * 2, m_lblWidth, m_lblHeight);
	m_againPwdLabel->setGeometry(left, top + m_lblHeight * 4, m_lblWidth, m_lblHeight);
	m_againPwd->setGeometry(left + m_lblWidth, top + m_lblHeight * 4, m_lblWidth, m_lblHeight);
	m_confirm->setGeometry(left + m_lblWidth * 1.5, top + m_lblHeight * 7, m_lblWidth, m_lblHeight);
}

void PasswordChangeTab::connectSlots()
{
	connect(m_confirm, SIGNAL(clicked(bool)), this, SLOT(slotOnConfirmClicked()));
}

void PasswordChangeTab::slotOnConfirmClicked()
{
	QString oldPwd = m_oldPwd->text();
	QString newPwd = m_newPwd->text();
	QString againPwd = m_againPwd->text();

	QMessageBox msgBox(QMessageBox::Critical, tr(""), tr(""));
	msgBox.setStyleSheet(QSS_MsgBox + QSS_PushButton);

	if (0 != oldPwd.compare(userPwd.user2pwd[userPwd.curUser])) {
		msgBox.setIcon(QMessageBox::Critical);
		msgBox.setWindowTitle(tr("错误"));
		msgBox.setText(tr("旧密码不正确!"));
		msgBox.addButton(tr("确认"), QMessageBox::AcceptRole);
		msgBox.exec();
		//QMessageBox::critical(this, tr("错误"), tr("旧密码不正确"));
		m_oldPwd->setFocus(Qt::MouseFocusReason);
		return;
	}

	if (6 != newPwd.length()) {
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.setWindowTitle(tr("警告"));
		msgBox.setText(tr("密码必须为6位数字."));
		msgBox.addButton(tr("确认"), QMessageBox::AcceptRole);
		msgBox.exec();
		//QMessageBox::warning(this, tr("警告"), tr("密码必须为6位数字"));
		m_newPwd->setFocus(Qt::MouseFocusReason);
		return;
	}

	if (0 != newPwd.compare(againPwd)) {
		//QMessageBox::warning(this, tr("警告"), tr("两次输入密码不一致"));
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.setWindowTitle(tr("警告"));
		msgBox.setText(tr("两次输入密码不一致!"));
		msgBox.addButton(tr("确认"), QMessageBox::AcceptRole);
		msgBox.exec();
		m_newPwd->setFocus(Qt::MouseFocusReason);
		return;
	}
	
	userPwd.user2pwd[userPwd.curUser] = newPwd;
	msgBox.setIcon(QMessageBox::Information);
	msgBox.setWindowTitle(tr("提示"));
	msgBox.setText(tr("密码重设成功."));
	msgBox.addButton(tr("重新登录"), QMessageBox::AcceptRole);
	msgBox.addButton(tr("取消"), QMessageBox::RejectRole);
	if (0 == msgBox.exec()) {
	//if (0 == QMessageBox::information(this, tr("信息"), tr("密码重设成功"), tr("重新登录"), tr("取消"))) {
		emit relogin();
	}
}

/****************************************************************************************
还原设置
*****************************************************************************************/
RestoreTab::RestoreTab(QWidget *parent /*= 0*/)
	: QWidget(parent)
	, m_lblWidth(LblWidth * 1.5)
	, m_lblHeight(LblHeight * 1.5)
{
	m_random = new QLabel(this);
	m_random->setObjectName(valueLightgrayQss);
	m_pwdLabel = new QLabel(tr("密码"), this);
	m_pwd = new QLineEdit(this);
	m_pwd->setAlignment(Qt::AlignCenter);
	m_confirm = new QPushButton(tr("确认设置"), this);

	generateRandomValue();

	int w = width();
	int h = height();
	setChildrenGeometry(w, h);

	connectSlots();
}

void RestoreTab::resizeEvent(QResizeEvent *event)
{
	QSize s = event->size();
	int w = s.width();
	int h = s.height();
	if (0 == w && 0 == h) {
		QWidget::resizeEvent(event);
		return;
	}

	setChildrenGeometry(w, h);
}

void RestoreTab::setChildrenGeometry(int w /*= 0*/, int h /*= 0*/)
{
	if (0 == w && 0 == h) return;

	m_random->setGeometry(w / 2 - m_lblWidth / 6, h / 2 - m_lblHeight * 2, m_lblWidth, m_lblHeight);
	m_pwdLabel->setGeometry(w / 2 - m_lblWidth, h / 2 - m_lblHeight, m_lblWidth / 2, m_lblHeight);
	m_pwd->setGeometry(w / 2 - m_lblWidth / 2, h / 2 - m_lblHeight, m_lblWidth, m_lblHeight);
	m_confirm->setGeometry(w / 2, h / 2 + m_lblHeight * 2, m_lblWidth, m_lblHeight);
}

void RestoreTab::connectSlots()
{
	connect(m_confirm, SIGNAL(clicked(bool)), this, SLOT(slotOnConfirmClicked()));
}

void RestoreTab::generateRandomValue()
{
	qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
	int value, total(0), last(0);
	for (int i = 0; i < 4; ++i) {
		do
		{
			value = qrand() % 10;
		} while (3 == i && 0 == value);
		total += value * pow(10, i);
		last += ((value * 2) % 10) * pow(10, i);
	}
	m_random->setText(tr("%1").arg(total));
	m_value = QString::number(last);
}

void RestoreTab::slotOnConfirmClicked()
{
	QString value = m_pwd->text();
	QMessageBox msgBox;
	msgBox.setStyleSheet(QSS_MsgBox + QSS_PushButton);
	if (4 != value.length()) {
		//QMessageBox::warning(this, tr("警告"), tr("密码应为4位数字"));
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.setWindowTitle(tr("警告"));
		msgBox.setText(tr("密码应为4位数字."));
		msgBox.addButton(tr("确认"), QMessageBox::AcceptRole);
		msgBox.exec();
		return;
	}

	if (0 != value.compare(m_value)) {
		//QMessageBox::warning(this, tr("警告"), tr("密码错误"));
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.setWindowTitle(tr("警告"));
		msgBox.setText(tr("密码错误!"));
		msgBox.addButton(tr("确认"), QMessageBox::AcceptRole);
		msgBox.exec();
		return;
	}

}

/****************************************************************************************
版本信息
*****************************************************************************************/
VersionInfoTab::VersionInfoTab(QWidget *parent /*= 0*/)
	: QWidget(parent)
	, m_lblWidth(LblWidth * 1.5)
	, m_lblHeight(LblHeight * 1.5)
{
	m_softVerLabel = new QLabel(tr("软件版本"), this);
	m_softVer = new QLabel(tr("A2.5-B1.5-C1.5-D1.7-E1.8"), this);
	m_softVer->setObjectName(valueLabelQss);
	//m_softVer->setAlignment(Qt::AlignCenter);
	m_firmVerLabel = new QLabel(tr("固件版本"), this);
	m_firmVer = new QLabel(tr("180404-02"), this);
	m_firmVer->setObjectName(valueLabelQss);
	//m_firmVer->setAlignment(Qt::AlignCenter);
	m_hardVerLabel = new QLabel(tr("硬件版本"), this);
	m_hardVer = new QLabel(tr("HARD_V1.0"), this);
	m_hardVer->setObjectName(valueLabelQss);
	//m_hardVer->setAlignment(Qt::AlignCenter);
	m_devSNLabel = new QLabel(tr("设备序列号"), this);
	m_devSN = new QLabel(tr("B206832500003"), this);
	m_devSN->setObjectName(valueLabelQss);
	//m_devSN->setAlignment(Qt::AlignCenter);

	int w = width();
	int h = height();
	setChildrenGeometry(w, h);

	m_masterVer = 0.0;
	m_net1Ver = 0.0;
	m_net2Ver = 0.0;
	m_refVer = 0.0;
	m_viewVer = 0.0;

	TransportThread *pTrans = TransportThread::Get();
	connect(pTrans, SIGNAL(verSignal(const st_MasterVer)), this, SLOT(slotOnMasterVer(const st_MasterVer)));
	connect(pTrans, SIGNAL(snResultSignal(QString)), this, SLOT(slotOnSnResult(QString)));
	connect(pTrans, SIGNAL(netVerSignal(QString)), this, SLOT(slotOnNetVer(QString)));
	connect(pTrans, SIGNAL(receiverVerSignal(QString)), this, SLOT(slotOnReceiverVer(QString)));
	connect(pTrans, SIGNAL(displayVerSignal(QString)), this, SLOT(slotOnDisplayVer(QString)));
}

void VersionInfoTab::resizeEvent(QResizeEvent *event)
{
	QSize s = event->size();
	int w = s.width();
	int h = s.height();
	if (0 == w && 0 == h) {
		QWidget::resizeEvent(event);
		return;
	}

	setChildrenGeometry(w, h);
}

void VersionInfoTab::setChildrenGeometry(int w /*= 0*/, int h /*= 0*/)
{
	if (0 == w && 0 == h) return;

	int left = w / 4;
	int mid = left + m_lblWidth;
	int top = (h - m_lblHeight * 7) / 2;
	m_softVerLabel->setGeometry(left, top, m_lblWidth, m_lblHeight);
	m_softVer->setGeometry(mid, top, m_lblWidth * 2, m_lblHeight);
	m_firmVerLabel->setGeometry(left, top + m_lblHeight * 2, m_lblWidth, m_lblHeight);
	m_firmVer->setGeometry(mid, top + m_lblHeight * 2, m_lblWidth, m_lblHeight);
	m_hardVerLabel->setGeometry(left, top + m_lblHeight * 4, m_lblWidth, m_lblHeight);
	m_hardVer->setGeometry(mid, top + m_lblHeight * 4, m_lblWidth, m_lblHeight);
	m_devSNLabel->setGeometry(left, top + m_lblHeight * 6, m_lblWidth, m_lblHeight);
	m_devSN->setGeometry(mid, top + m_lblHeight * 6, m_lblWidth, m_lblHeight);
}

void VersionInfoTab::slotOnMasterVer(const st_MasterVer&ret)
{
	const float EPSILON = 0.0001;
	m_masterVer = ret.mainv;
	if (fabs(ret.net1v - 0.0) > EPSILON)
		m_net1Ver = ret.net1v;
	if (fabs(ret.net2v - 0.0) > EPSILON)
		m_net2Ver = ret.net2v;
	if (fabs(ret.refv - 0.0) > EPSILON)
		m_refVer = ret.refv;
	if (fabs(ret.viewv - 0.0) > EPSILON)
		m_viewVer = ret.viewv;
	QString softVer = QString("A%1-B%2-C%3-D%4-E%5")
		.arg(m_masterVer).arg(m_net1Ver).arg(m_net2Ver).arg(m_refVer).arg(m_viewVer);
	m_softVer->setText(softVer);
	m_firmVer->setText(ret.firmware);
	m_hardVer->setText(ret.hardware.toUpper());
}

void VersionInfoTab::slotOnSnResult(const QString &ret)
{
	m_devSN->setText(ret.toUpper());
}

void VersionInfoTab::slotOnNetVer(const QString &ver)
{	
	QStringList lst = ver.split(SepCharComma);
	if (lst.size() < 2) return;
	if (0 == lst.at(0).compare("net1")) {
		m_net1Ver = lst.at(1).toFloat();
	}
	else {
		m_net2Ver = lst.at(1).toFloat();
	}

	QString softVer = QString("A%1-B%2-C%3-D%4-E%5")
		.arg(m_masterVer).arg(m_net1Ver).arg(m_net2Ver).arg(m_refVer).arg(m_viewVer);
	m_softVer->setText(softVer);
}

void VersionInfoTab::slotOnReceiverVer(const QString &ver)
{
	m_refVer = ver.toFloat();
	QString softVer = QString("A%1-B%2-C%3-D%4-E%5")
		.arg(m_masterVer).arg(m_net1Ver).arg(m_net2Ver).arg(m_refVer).arg(m_viewVer);
	m_softVer->setText(softVer);
}

void VersionInfoTab::slotOnDisplayVer(const QString &ver)
{
	m_viewVer = ver.toFloat();
	QString softVer = QString("A%1-B%2-C%3-D%4-E%5")
		.arg(m_masterVer).arg(m_net1Ver).arg(m_net2Ver).arg(m_refVer).arg(m_viewVer);
	m_softVer->setText(softVer);
}

/****************************************************************************************
状态参数
*****************************************************************************************/
StateMainTab::StateMainTab(QWidget *parent /*= 0*/)
{
	m_devOverview = new QPushButton(tr("设备总览"), this);	
	m_bdsStatus = new QPushButton(tr("北斗状态"), this);
	m_gpsStatus = new QPushButton(tr("GPS状态"), this);
	m_gloStatus = new QPushButton(tr("GLONASS状态"), this);
	m_acbStatus = new QPushButton(tr("交流B码状态"), this);
	m_dcbStatus = new QPushButton(tr("直流B码状态"), this);

	m_posConfiguration = new QPushButton(tr("位置配置"), this);
	m_posConfiguration->setEnabled(false);
	m_sportConfiguration = new QPushButton(tr("运动配置"), this);
	m_sportConfiguration->setEnabled(false);
	m_minElevation = new QPushButton(tr("最小仰角"), this);
	m_minElevation->setDisabled(true);
	m_minSnr = new QPushButton(tr("最小信噪比"), this);
	m_minSnr->setDisabled(true);
	m_delayCompensation = new QPushButton(tr("延时补偿"), this);
	m_pulseConfiguration = new QPushButton(tr("脉冲配置"), this);
	m_timezone = new QPushButton(tr("时区"), this);

	m_logInfo = new QPushButton(tr("日志信息"), this);
	m_alarmInfo = new QPushButton(tr("告警信息"), this);
	m_screenSettings = new QPushButton(tr("屏幕设置"), this);
	m_factorySettings = new QPushButton(tr("出厂设置"), this);
	m_pwdChanging = new QPushButton(tr("密码修改"), this);
	m_restoreSettings = new QPushButton(tr("还原设置"), this);
	m_verInfo = new QPushButton(tr("版本信息"), this);

	m_btnGroup = new QButtonGroup;
	m_btnGroup->addButton(m_devOverview, 0);
	m_btnGroup->addButton(m_bdsStatus, 1);
	m_btnGroup->addButton(m_gpsStatus, 2);
	m_btnGroup->addButton(m_gloStatus, 3);
	m_btnGroup->addButton(m_acbStatus, 4);
	m_btnGroup->addButton(m_dcbStatus, 5);
	m_btnGroup->addButton(m_posConfiguration, 6);
	m_btnGroup->addButton(m_sportConfiguration, 7);
	m_btnGroup->addButton(m_minElevation, 8);
	m_btnGroup->addButton(m_minSnr, 9);
	m_btnGroup->addButton(m_delayCompensation, 10);
	m_btnGroup->addButton(m_pulseConfiguration, 11);
	m_btnGroup->addButton(m_timezone, 12);
	m_btnGroup->addButton(m_logInfo, 13);
	m_btnGroup->addButton(m_alarmInfo, 14);
	m_btnGroup->addButton(m_screenSettings, 15);
	m_btnGroup->addButton(m_factorySettings, 16);
	m_btnGroup->addButton(m_pwdChanging, 17);
	m_btnGroup->addButton(m_restoreSettings, 18);
	m_btnGroup->addButton(m_verInfo, 19);

	auto baseLayout = new QGridLayout(this);
	baseLayout->addWidget(m_devOverview, 1, 1);
	baseLayout->addWidget(m_bdsStatus, 3, 1);
	baseLayout->addWidget(m_gpsStatus, 5, 1);
	baseLayout->addWidget(m_gloStatus, 7, 1);
	baseLayout->addWidget(m_acbStatus, 9, 1);
	baseLayout->addWidget(m_dcbStatus, 11, 1);

	baseLayout->addWidget(m_posConfiguration, 1, 3);
	baseLayout->addWidget(m_sportConfiguration, 3, 3);
	baseLayout->addWidget(m_minElevation, 5, 3);
	baseLayout->addWidget(m_minSnr, 7, 3);
	baseLayout->addWidget(m_delayCompensation, 9, 3);
	baseLayout->addWidget(m_pulseConfiguration, 11, 3);
	baseLayout->addWidget(m_timezone, 13, 3);

	baseLayout->addWidget(m_logInfo, 1, 5);
	baseLayout->addWidget(m_alarmInfo, 3, 5);
	baseLayout->addWidget(m_screenSettings, 5, 5);
	baseLayout->addWidget(m_factorySettings, 7, 5);
	baseLayout->addWidget(m_pwdChanging, 9, 5);
	baseLayout->addWidget(m_restoreSettings, 11, 5);
	baseLayout->addWidget(m_verInfo, 13, 5);

	for (int i = 0; i < 7; ++i) {
		if (i % 2) {
			baseLayout->setColumnStretch(i, 2);
			continue;
		}
		baseLayout->setColumnStretch(i, 1);
	}

	for (int i = 0; i < 15; ++i) {		
		if (0 == i || 14 == i) {
			baseLayout->setRowStretch(i, 2);
			continue;
		}
		if (i % 2) {
			baseLayout->setRowStretch(i, 4);
			continue;
		}

		baseLayout->setRowStretch(i, 1);		
	}

	setStyleSheet(QSS_StateMain.arg(LblWidth * 1.5).arg(LblHeight));

	connect(m_btnGroup, SIGNAL(buttonClicked(int)), this, SIGNAL(stateParamsButtonClicked(int)));
}
