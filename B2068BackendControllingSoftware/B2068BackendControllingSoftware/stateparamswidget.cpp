#include "stdafx.h"
#include "stateparamswidget.h"
#include "cradar.h"

/****************************************************************************************
qss�����ַ���
*****************************************************************************************/
const QString valueLabelQss("valuelabel");

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
����״̬
*****************************************************************************************/
BDSStateTab::BDSStateTab(QWidget *parent /*= 0*/)
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
	
	m_bdsTable = new QTableView(this);	
	// ����ƽ�����洰������Ӧ
	m_bdsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_bdsTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_bdsTable->verticalHeader()->hide();	
	// ���ò��ɱ༭
	m_bdsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	// ���б�ɫ
	m_bdsTable->setAlternatingRowColors(true); 
	m_bdsTable->setModel(model);

	m_radar = new CRadar(this);

	auto baseLayout = new QGridLayout(this);
	baseLayout->addWidget(workStateLabel, 0, 1);
	baseLayout->addWidget(m_workState, 0, 2);
	baseLayout->addWidget(m_bdsTable, 0, 4, 6, 1);
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
	baseLayout->setRowStretch(5, 5);
}
