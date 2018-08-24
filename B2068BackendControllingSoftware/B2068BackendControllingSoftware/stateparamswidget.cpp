#include "stdafx.h"
#include "stateparamswidget.h"
#include "cradar.h"


/****************************************************************************************
�豸����
*****************************************************************************************/
DeviceOverviewTab::DeviceOverviewTab(QWidget *parent)
	: QWidget(parent)
{
	

	auto runTimeLabel = new QLabel(tr("����ʱ��"), this);
	m_runTime = new QLabel(tr("1��10ʱ23��02��"), this);
	m_runTime->setObjectName(valueLabelQss);

	auto vibrationType = new QLabel(tr("��������"), this);
	m_vibrationType = new QLabel(tr("��� ����"), this);
	m_vibrationType->setObjectName(valueLabelQss);

	auto tamingState = new QLabel(tr("ѱ��״̬"), this);
	m_tamingState = new QLabel(tr("����"), this);
	m_tamingState->setObjectName(valueLabelQss);

	auto leapTime = new QLabel(tr("����ʱ��"), this);
	m_leapTime = new QLabel(tr("δ֪"), this);
	m_leapTime->setObjectName(valueLabelQss);

	auto curTimezone = new QLabel(tr("��ǰʱ��"), this);
	m_curTimezone = new QLabel(tr("������"), this);
	m_curTimezone->setObjectName(valueLabelQss);

	auto curRefSrc = new QLabel(tr("��ǰ�ο�"), this);
	m_curRefSrc = new QLabel(tr("����"), this);
	m_curRefSrc->setObjectName(valueLabelQss);

	m_refSrcTable = new QTableWidget(5, 3, this);
	QStringList headerList;
	headerList << tr("�ο�Դ") << tr("����״̬") << tr("ͨ����ֵ");
	m_refSrcTable->setHorizontalHeaderLabels(headerList); 	
	// ����ƽ�����洰������Ӧ
	m_refSrcTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_refSrcTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_refSrcTable->verticalHeader()->hide(); 
	// ���ñ�ͷ���������ʽ
	m_refSrcTable->horizontalHeaderItem(2)->setTextColor(Qt::lightGray);
	// ���ò��ɱ༭
	m_refSrcTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	// ���б�ɫ
	m_refSrcTable->setAlternatingRowColors(true);
	
	m_refSrcTable->setItem(0, 0, new QTableWidgetItem(tr("����")));
	m_refSrcTable->setItem(0, 1, new QTableWidgetItem(tr("ͬ��")));
	m_refSrcTable->setItem(0, 2, new QTableWidgetItem);
	m_refSrcTable->setItem(1, 0, new QTableWidgetItem(tr("GPS")));
	m_refSrcTable->setItem(1, 1, new QTableWidgetItem(tr("ʧ��")));
	m_refSrcTable->setItem(1, 2, new QTableWidgetItem);
	m_refSrcTable->setItem(2, 0, new QTableWidgetItem(tr("GLONASS")));
	m_refSrcTable->setItem(2, 1, new QTableWidgetItem(tr("ʧ��")));
	m_refSrcTable->setItem(2, 2, new QTableWidgetItem);
	m_refSrcTable->setItem(3, 0, new QTableWidgetItem(tr("BDC��")));
	m_refSrcTable->setItem(3, 1, new QTableWidgetItem(tr("ʧ��")));
	m_refSrcTable->setItem(3, 2, new QTableWidgetItem);
	m_refSrcTable->setItem(4, 0, new QTableWidgetItem(tr("BAC��")));
	m_refSrcTable->setItem(4, 1, new QTableWidgetItem(tr("ʧ��")));
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
����״̬ GPS״̬ GLONASS״̬
*****************************************************************************************/
SatTypeStateTab::SatTypeStateTab(QWidget *parent /*= 0*/)
{
	auto workStateLabel = new QLabel(tr("����״̬"), this);
	m_workState = new QLabel(tr("����"), this);
	m_workState->setObjectName(valueLabelQss);
	auto signStateLabel = new QLabel(tr("�ź�״̬"), this);
	m_signState = new QLabel(tr("����"), this);
	m_signState->setObjectName(valueLabelQss);
	auto pdopLabel = new QLabel(tr("PDOP"), this);
	m_pdop = new QLabel(tr("0.8"), this);
	m_pdop->setObjectName(valueLabelQss);
	auto visSatellitesLabel = new QLabel(tr("�ɼ�����"), this);
	m_visSatellites = new QLabel(tr("13"), this);
	m_visSatellites->setObjectName(valueLabelQss);
	auto avlSatellitesLabel = new QLabel(tr("��������"), this);
	m_avlSatellites = new QLabel(tr("10"), this);
	m_avlSatellites->setObjectName(valueLabelQss);


	QVector<QStringList> itemLists;
	itemLists << QString("1,2,3,4,5,7,8,10,11,12").split(',')
		<< QString("51,47,64,33,23,72,71,75,20,52").split(',')
		<< QString("126,236,189,109,254,131,325,324,266,325").split(',')
		<< QString("47,46,49,44,50,49,50,49,44,51").split(',');
	 
	QStandardItemModel *model = new QStandardItemModel;
	model->setColumnCount(4);
	model->setHeaderData(0, Qt::Horizontal, tr("���Ǻ�"));
	model->setHeaderData(1, Qt::Horizontal, tr("����"));
	model->setHeaderData(2, Qt::Horizontal, tr("��λ��"));
	model->setHeaderData(3, Qt::Horizontal, tr("�����"));
	model->setRowCount(10);
	for (int col = 0; col < 4; ++col) {
		for (int row = 0; row < 10; ++row) {
			QStandardItem *item = new QStandardItem(itemLists[col][row]);
			item->setTextAlignment(Qt::AlignCenter);
			model->setItem(row, col, item);			
		}
	}	
	
	m_satTypeTable = new QTableView(this);
	// ����ƽ�����洰������Ӧ
	m_satTypeTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_satTypeTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_satTypeTable->verticalHeader()->hide();	
	// ���ò��ɱ༭
	m_satTypeTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	// ���б�ɫ
	m_satTypeTable->setAlternatingRowColors(true); 
	m_satTypeTable->setObjectName(valueTableViewQss);
	m_satTypeTable->setModel(model);

	// ���״�ͼ�ϻ��Ƶ�
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
	baseLayout->setColumnStretch(0, 1);
	baseLayout->setColumnStretch(1, 3);
	baseLayout->setColumnStretch(2, 3);
	baseLayout->setColumnStretch(3, 1);
	baseLayout->setColumnStretch(4, 6);
	baseLayout->setColumnStretch(5, 1);
	baseLayout->setRowStretch(0, 1);
	baseLayout->setRowStretch(1, 1);
	baseLayout->setRowStretch(2, 1);
	baseLayout->setRowStretch(3, 1);
	baseLayout->setRowStretch(4, 1);
	baseLayout->setRowStretch(5, 6);
}

void SatTypeStateTab::setGnssstaInfo(const st_Gnsssta &gnsssta)
{
	/****************************************************************************************/
	// ����ר��
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
	
	
	m_workState->setText(tr("�쳣"));
	m_signState->setText(tr("�쳣"));
	m_pdop->setText(QString::number(gnsssta.pdop));
	m_visSatellites->setText(QString::number(gnsssta.visSatellites));
	m_avlSatellites->setText(QString::number(gnsssta.avlSatellites));
	if (gnsssta.avlSatellites > 0 && 1 == gnsssta.d3PosFlag && 1 == gnsssta.rmcStateFlag)
		m_workState->setText(tr("����"));
	if (gnsssta.visSatellites > 3 && 1 == gnsssta.snrFlag)
		m_signState->setText(tr("����"));
}

void SatTypeStateTab::setGnssgsvInfo(const st_Gnssgsv &gnssgsv)
{
	/****************************************************************************************/
	// ����ר��
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
	// Ĭ�ϰ�һ֡4�����Ǻż���
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
ֱ��B��״̬
*****************************************************************************************/
DCBStateTab::DCBStateTab(QWidget *parent /*= 0*/)
	: QWidget(parent)
	, m_lblWidth(LblWidth * 1.5)
	, m_lblHeight(LblHeight * 1.5)
{
	int w = width();
	int h = height();

	QStringList formats = {"GJB97", "GJB08", "200-04"};
	
	m_runStateLabel = new QLabel(tr("����״̬"), this);	
	m_runState = new QLabel(tr("�쳣"), this);
	m_runState->setObjectName(valueLabelQss);

	m_synStateLabel = new QLabel(tr("ͬ��״̬"), this);
	m_synState = new QLabel(tr("�쳣"), this);
	m_synState->setObjectName(valueLabelQss);

	m_inFormatLabel = new QLabel(tr("ֱ��B�������ʽ"), this);
	m_inFormat = new QComboBox(this);
	m_inFormat->addItems(formats);

	m_outFormatLabel = new QLabel(tr("ֱ��B�������ʽ"), this);
	m_outFormat = new QComboBox(this);
	m_outFormat->addItems(formats);

	m_confirm = new QPushButton(tr("ȷ������"), this);
	
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

void DCBStateTab::setChildrenGeometry(int w /*= 0*/, int h /*= 0*/)
{
	if (0 == w && 0 == h) return;

	m_runStateLabel->setGeometry(w / 6, (h - m_lblHeight * 10) / 2, m_lblWidth, m_lblHeight);
	m_runState->setGeometry(w / 2, (h - m_lblHeight * 10) / 2, m_lblWidth, m_lblHeight);
	m_synStateLabel->setGeometry(w / 6, (h - m_lblHeight * 6) / 2, m_lblWidth, m_lblHeight);
	m_synState->setGeometry(w / 2, (h - m_lblHeight * 6) / 2, m_lblWidth, m_lblHeight);
	m_inFormatLabel->setGeometry(w / 6, (h - m_lblHeight * 2) / 2, LblWidth * 2, m_lblHeight);
	m_inFormat->setGeometry(w / 2, (h - m_lblHeight * 2) / 2, m_lblWidth, m_lblHeight);
	m_outFormatLabel->setGeometry(w / 6, h / 2 + m_lblHeight, LblWidth * 2, m_lblHeight);
	m_outFormat->setGeometry(w / 2, h / 2 + m_lblHeight, m_lblWidth, m_lblHeight);
	m_confirm->setGeometry(w / 2 + m_lblWidth / 2, h / 2 + m_lblHeight * 4, m_lblWidth, m_lblHeight);
}

/****************************************************************************************
����B��״̬
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

	m_runStateLabel = new QLabel(tr("����״̬"), this);
	m_runState = new QLabel(tr("�쳣"), this);
	m_runState->setObjectName(valueLabelQss);

	m_synStateLabel = new QLabel(tr("ͬ��״̬"), this);
	m_synState = new QLabel(tr("�쳣"), this);
	m_synState->setObjectName(valueLabelQss);

	m_inFormatLabel = new QLabel(tr("����B�������ʽ"), this);
	m_inFormat = new QComboBox(this);
	m_inFormat->addItems(formats);

	m_outFormatLabel = new QLabel(tr("����B�������ʽ"), this);
	m_outFormat = new QComboBox(this);
	m_outFormat->addItems(formats);

	m_outAmpLabel = new QLabel(tr("����B�������ֵ"), this);
	m_outAmplitude = new QLineEdit(tr("1.5"), this);
	// ��Ч����
	//QDoubleValidator *validator = new QDoubleValidator(0.5, 10.0, 1, this);
	//validator->setBottom(0.5);
	QRegExp rx("^10|\\d\\.5|[1-9]$");
	QValidator *validator = new QRegExpValidator(rx, this);
	m_outAmplitude->setValidator(validator);
	m_outAmpUnit = new QLabel(tr("V"), this);

	m_outMrLabel = new QLabel(tr("����B��������Ʊ�"), this);
	m_outMr = new QComboBox(this);
	m_outMr->addItems(modRatio);

	m_confirm = new QPushButton(tr("ȷ������"), this);

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

void ACBStateTab::setChildrenGeometry(int w /*= 0*/, int h /*= 0*/)
{
	if (0 == w && 0 == h)  return;

	int wdth = LblWidth * 1.5;
	int hght = LblHeight * 1.5;
	int dblWdth = LblWidth * 2;
	m_runStateLabel->setGeometry(w / 6, (h - m_lblHeight * 14) / 2, m_lblWidth, m_lblHeight);
	m_runState->setGeometry(w / 2, (h - m_lblHeight * 14) / 2, m_lblWidth, m_lblHeight);
	m_synStateLabel->setGeometry(w / 6, (h - m_lblHeight * 10) / 2, m_lblWidth, m_lblHeight);
	m_synState->setGeometry(w / 2, (h - m_lblHeight * 10) / 2, m_lblWidth, m_lblHeight);
	m_inFormatLabel->setGeometry(w / 6, (h - m_lblHeight * 6) / 2, dblWdth, m_lblHeight);
	m_inFormat->setGeometry(w / 2, (h - m_lblHeight * 6) / 2, wdth, hght);
	m_outFormatLabel->setGeometry(w / 6, (h - m_lblHeight * 2) / 2, dblWdth, m_lblHeight);
	m_outFormat->setGeometry(w / 2, (h - m_lblHeight * 2) / 2, wdth, hght);
	m_outAmpLabel->setGeometry(w / 6, h / 2 + m_lblHeight, dblWdth, m_lblHeight);
	m_outAmplitude->setGeometry(w / 2, h / 2 + m_lblHeight, wdth * 4 / 5, hght);
	m_outAmpUnit->setGeometry(w / 2 + wdth * 5 / 6, h / 2 + m_lblHeight, wdth / 6, hght);
	m_outMrLabel->setGeometry(w / 6, h / 2 + m_lblHeight * 3, dblWdth, m_lblHeight);
	m_outMr->setGeometry(w / 2, h / 2 + m_lblHeight * 3, wdth, hght);
	m_confirm->setGeometry(w / 2 + wdth / 2, h / 2 + m_lblHeight * 6, wdth, hght);
}

/****************************************************************************************
��ʱ����
*****************************************************************************************/
DelayCompensationTab::DelayCompensationTab(QWidget *parent /*= 0*/)
	: QWidget(parent)
	, m_lblWidth(LblWidth)
	, m_lblHeight(LblHeight)
{
	int w = width();
	int h = height();
	
	m_bdsLabel = new QLabel(tr("����"), this);
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

	m_acbLabel = new QLabel(tr("����B��"), this);
	m_acb = new QLineEdit(tr("0"), this);
	m_acb->setAlignment(Qt::AlignCenter);
	m_acbUnit = new QLabel(tr("ns"), this);

	m_dcbLabel = new QLabel(tr("ֱ��B��"), this);
	m_dcb = new QLineEdit(tr("0"), this);
	m_dcb->setAlignment(Qt::AlignCenter);
	m_dcbUnit = new QLabel(tr("ns"), this);

	m_confirm = new QPushButton(tr("ȷ������"), this);

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
	m_bdsLabel->setGeometry(w / 4, (h - m_lblHeight * 12) / 2, wdth, hght);
	m_bds->setGeometry(w / 2, (h - m_lblHeight * 12) / 2, wdth, hght);
	m_bdsUnit->setGeometry(w / 2 + wdth * 1.1, (h - m_lblHeight * 12) / 2, wdth, hght);
	m_gpsLabel->setGeometry(w / 4, (h - m_lblHeight * 8) / 2, wdth, hght);
	m_gps->setGeometry(w / 2, (h - m_lblHeight * 8) / 2, wdth, hght);
	m_gpsUnit->setGeometry(w / 2 + wdth * 1.1, (h - m_lblHeight * 8) / 2, wdth, hght);
	m_gloLabel->setGeometry(w / 4, (h - m_lblHeight * 4) / 2, wdth, hght);
	m_glo->setGeometry(w / 2, (h - m_lblHeight * 4) / 2, wdth, hght);
	m_gloUnit->setGeometry(w / 2 + wdth * 1.1, (h - m_lblHeight * 4) / 2, wdth, hght);
	m_acbLabel->setGeometry(w / 4, h  / 2, wdth, hght);
	m_acb->setGeometry(w / 2, h / 2, wdth, hght);
	m_acbUnit->setGeometry(w / 2 + wdth * 1.1, h / 2, wdth, hght);
	m_dcbLabel->setGeometry(w / 4, h / 2 + m_lblHeight * 2, wdth, hght);
	m_dcb->setGeometry(w / 2, h / 2 + m_lblHeight * 2, wdth, hght);
	m_dcbUnit->setGeometry(w / 2 + wdth * 1.1, h / 2 + m_lblHeight * 2, wdth, hght);
	m_confirm->setGeometry(w / 2 + wdth / 2, h / 2 + m_lblHeight * 5, wdth, hght);
}

/****************************************************************************************
��������
*****************************************************************************************/
PulseSettingsTab::PulseSettingsTab(QWidget *parent /*= 0*/)
	: QWidget(parent)
	, m_lblWidth(LblWidth * 1.5)
	, m_lblHeight(LblHeight * 1.5)
{
	QStringList unit = { "ms", "ns" };
	
	m_ppsWidthLabel = new QLabel(tr("����"), this);
	m_ppsWidth = new QLineEdit(tr("30"), this);
	m_ppsWidth->setAlignment(Qt::AlignCenter);
	QRegExp rx1("^[1-9]?\\d$");
	QValidator *validator1 = new QRegExpValidator(rx1, this);
	m_ppsWidth->setValidator(validator1);
	m_ppsWidthUnit = new QComboBox(this);
	m_ppsWidthUnit->addItems(unit);

	m_ppsDelayLabel = new QLabel(tr("��ʱ"), this);
	m_ppsDelay = new QLineEdit(tr("0"), this);
	m_ppsDelay->setAlignment(Qt::AlignCenter);
	QRegExp rx("^[1-9]?\\d$");
	QValidator *validator = new QRegExpValidator(rx, this);
	m_ppsDelay->setValidator(validator);
	m_ppsDelayUnit = new QComboBox(this);	
	m_ppsDelayUnit->addItems(unit);
	m_ppsDelayUnit->setCurrentIndex(1);
	// ��ú�
	//QLineEdit *centerLineEdit = new QLineEdit;	
	//centerLineEdit->setReadOnly(true);
	//centerLineEdit->setAlignment(Qt::AlignCenter);
	//m_ppsDelayUnit->setLineEdit(centerLineEdit);

	m_confirm = new QPushButton(tr("ȷ������"), this);

	int w = width();
	int h = height();
	setChildrenGeometry(w, h);
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
	m_ppsWidthLabel->setGeometry(w / 4, (h - m_lblHeight * 6) / 2, wdth, m_lblHeight);
	m_ppsWidth->setGeometry(w / 4 + wdth, (h - m_lblHeight * 6) / 2, m_lblWidth, m_lblHeight);
	m_ppsWidthUnit->setGeometry(w / 4 + LblWidth*2.6, (h - m_lblHeight * 6) / 2, wdth, m_lblHeight);
	m_ppsDelayLabel->setGeometry(w / 4, (h - m_lblHeight * 2) / 2, wdth, m_lblHeight);
	m_ppsDelay->setGeometry(w / 4 + wdth, (h - m_lblHeight * 2) / 2, m_lblWidth, m_lblHeight);
	m_ppsDelayUnit->setGeometry(w / 4 + LblWidth*2.6, (h - m_lblHeight * 2) / 2, wdth, m_lblHeight);
	m_confirm->setGeometry(w / 2 + wdth, h / 2 + m_lblHeight * 3, m_lblWidth, m_lblHeight);
}

/****************************************************************************************
ʱ������
*****************************************************************************************/
TimezoneTab::TimezoneTab(QWidget *parent /*= 0*/)
	: QWidget(parent)
	, m_lblWidth(LblWidth * 1.5)
	, m_lblHeight(LblHeight * 1.5)
{
	m_timezoneLabel = new QLabel(tr("ʱ��"), this);
	m_timezone = new QLineEdit(tr("8"), this);
	m_timezone->setAlignment(Qt::AlignCenter);
	m_confirm = new QPushButton(tr("ȷ������"), this);
	
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