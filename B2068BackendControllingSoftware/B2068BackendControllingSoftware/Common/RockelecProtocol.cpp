#include "stdafx.h"
#include "RockelecProtocol.h"


RockelecProtocol::RockelecProtocol()
{
	m_iSourceAddr = 0;
	m_iSourcePort = 0;
	m_iSourceResv = 0;

	m_iTargetAddr = 0;
	m_iTargetPort = 0;
	m_iTargetResv = 0;

	m_iFrameNo = 0;
	m_iCmd = 0;

	m_bDataHasFrame = false;
	m_bSendDataIs16Hex = false;
	m_bRecvDataIs16Hex = false;
	m_pFrameHandleFunc = nullptr;

	m_iSaveIndex_Start = 0;
	m_iSaveIndex_End = 0;
	m_pCheckBuf = new unsigned char[MAX_SIZE_OF_RECV_BUF];
	m_pDataUseBuf = new unsigned char[MAX_SIZE_OF_RECV_BUF];
	m_pTempBuf = new unsigned char[MAX_SIZE_OF_TEMP_BUF];
	m_pShowBuf = new char[MAX_SIZE_OF_SHOW_BUF];
}


RockelecProtocol::~RockelecProtocol()
{
	delete[] m_pCheckBuf;
	delete[] m_pDataUseBuf;
	delete[] m_pTempBuf;

	m_pCheckBuf = nullptr;
	m_pDataUseBuf = nullptr;
	m_pTempBuf = nullptr;
}

void RockelecProtocol::SetSourceAddr(int iAddr, int iPort, int iResv)
{
	m_iSourceAddr = iAddr;
	m_iSourcePort = iPort;
	m_iSourceResv = iResv;
}

void RockelecProtocol::SetTargetAddr(int iAddr, int iPort, int iResv)
{
	m_iTargetAddr = iAddr;
	m_iTargetPort = iPort;
	m_iTargetResv = iResv;
}

void RockelecProtocol::SetCommand(int iFrameNo, int iCmd)
{
	m_iFrameNo = iFrameNo;
	m_iCmd = iCmd;
}

void RockelecProtocol::SetDataHasFrame(bool bHasFrame)
{
	m_bDataHasFrame = bHasFrame;
}

void RockelecProtocol::SetSendDataIs16Hex(bool bSendIs16Hex)
{
	m_bSendDataIs16Hex = bSendIs16Hex;
}

void RockelecProtocol::SetRecvDataIs16Hex(bool bRecvIs16Hex)
{
	m_bRecvDataIs16Hex = bRecvIs16Hex;
}

void RockelecProtocol::SetFrameHandleFunc(FRAMEHANDLEFUNC pFrameHandleFunc)
{
	m_pFrameHandleFunc = pFrameHandleFunc;
}

void RockelecProtocol::ParseFrames(unsigned char *pszDataBuf, int &iDataLength)
{
	// ���յ������Ƿ���16���ƣ��ǵĻ��򽫽��յ���16��������ת��ASCII����
	bool bIs16Data = m_bRecvDataIs16Hex;

	// ����ǲ���Ҫ֡ͷ֡β����ֱ����ʾ��������
	if (false == m_bDataHasFrame)
	{
		if (false == bIs16Data)
		{
			memcpy(m_pShowBuf, pszDataBuf, iDataLength);
			m_pShowBuf[iDataLength] = '\0';
		}
		else
		{
			m_pShowBuf[0] = '\0';
			int idx = 0;
			int iValue = 0;
			vector<string> hexstrVec = SplitHexString((const char *)pszDataBuf, iDataLength, " ");
			for (auto &hex : hexstrVec) {				
				HexStringToChar((const unsigned char *)hex.c_str(), iValue);				
				m_pShowBuf[idx] = iValue;
				++idx;
			}
		}

		//memset(pszDataBuf, 0, MAX_SIZE_OF_SAVE_BUF);
		memset(pszDataBuf, 0, iDataLength);
		iDataLength = 0;
		return;
	}

	do {
		m_iSaveIndex_Start = -1;
		m_iSaveIndex_End = -1;

		unsigned char chOne = 0;
		bool bFound10 = false;
		if (-1 == m_iSaveIndex_Start)        // ��֡ͷ
		{
			bFound10 = false;
			for (int i = 0; i < iDataLength; ++i)
			{
				chOne = pszDataBuf[i];
				if (false == bFound10)
				{
					if (0x10 == chOne)
					{
						bFound10 = true;
					}
				}
				else
				{
					if (0x02 == chOne)
					{
						m_iSaveIndex_Start = i - 1;     // -1
						break;
					}
					bFound10 = false;
				}
			}
		}

		if (-1 != m_iSaveIndex_Start)        // ��֡β
		{
			bFound10 = false;
			for (int i = m_iSaveIndex_Start + 2; i < iDataLength; ++i)
			{
				chOne = pszDataBuf[i];
				if (false == bFound10)
				{
					if (0x10 == chOne)
					{
						bFound10 = true;
					}
				}
				else
				{
					if (0x03 == chOne)
					{
						m_iSaveIndex_End = i;
						break;
					}
					bFound10 = false;
				}
			}

			if ((-1 != m_iSaveIndex_End) && (m_iSaveIndex_End > m_iSaveIndex_Start)) // һ֡
			{
				// ���µ�
				int iLeaveLength = iDataLength - (m_iSaveIndex_End + 1);
				if (iLeaveLength > 0)
				{
					memcpy(m_pTempBuf, pszDataBuf + (m_iSaveIndex_End + 1), iLeaveLength);
				}

				// У��һ֡����ȷ��
				{
					int iFrameLength = m_iSaveIndex_End - m_iSaveIndex_Start + 1;
					unsigned char chOne = 0;
					unsigned char* pchData0 = pszDataBuf + m_iSaveIndex_Start;

					int iSendLength = 0;
					int iIndexUse = 2;
					chOne = *(pchData0 + (iIndexUse++));
					if (0x10 == chOne)iIndexUse++;
					iSendLength = chOne * 256;
					CharToHexString(chOne, m_chOne, sizeof(m_chOne));
					chOne = *(pchData0 + (iIndexUse++));
					iSendLength += chOne;

					if (abs(iFrameLength - iSendLength) > 141/*12*/)      // ���Ȳ��ԣ���������Ϊ��˫��10�������⣬���Ȳ���ʹ�õ��ڵıȽϷ�ʽ��
					{
						// �������µ�   // ����������߼�
						memset(pszDataBuf, 0, MAX_SIZE_OF_SAVE_BUF);
						iDataLength = 0;
						if (iLeaveLength > 0)
						{
							memcpy(pszDataBuf, m_pTempBuf, iLeaveLength);
							iDataLength = iLeaveLength;
							continue;   // ���ӵ����
						}
						else
						{
							iDataLength = 0;
						}

						break;
					}
				}

				unsigned char* pchData = pszDataBuf + m_iSaveIndex_Start;
				int iUseDataLength = m_iSaveIndex_End - m_iSaveIndex_Start + 1;
				m_pShowBuf[0] = '\0';
				unsigned char chOne = 0;
				m_chOne[0] = '\0';

				// ������CRCУ��
				int iCheckIndex = 0;
				memset(m_pCheckBuf, 0, MAX_SIZE_OF_RECV_BUF);

				// ����ȥ��֡����
				st_FrameData stFrameData;

				// ע�⣺����֡ͷ��֡β��0x10���ʾΪ0x10 0x10���������������ֽڲ��㳤�ȣ�ͬʱҲ������CRCУ��

				// ASCII���ݵĳ��� = �ܳ��� - ǰASCII�ĳ��� - ��ASCII�ĳ���
				int iPreASCIILength = 0;        // ǰASCII�ĳ���
				int iAftASCIILength = 0;        // ��ASCII�ĳ���
				int iASCIILength = 0;           // ASCII���ݵĳ���

				// ���CRCУ����ȷ���򳤶�Ӧ�þ��� N + 10
				int iRecvLength = 0;

				// ֡ͷ(2)
				int iIndexUse = 0;
				strcpy_s(m_pShowBuf, MAX_SIZE_OF_SHOW_BUF, " ");
				chOne = *(pchData + (iIndexUse++));
				CharToHexString(chOne, m_chOne, sizeof(m_chOne));
				chOne = *(pchData + (iIndexUse++));
				CharToHexString(chOne, m_chOne, sizeof(m_chOne));

				// ����(2) = N + 10
				int iSendLength = 0;
				chOne = *(pchData + (iIndexUse++));
				if (0x10 == chOne)iIndexUse++;
				iSendLength = chOne * 256;
				CharToHexString(chOne, m_chOne, sizeof(m_chOne));
				chOne = *(pchData + (iIndexUse++));
				if (0x10 == chOne)iIndexUse++;
				iSendLength += chOne;
				CharToHexString(chOne, m_chOne, sizeof(m_chOne));

				int iHighLength = (iSendLength) / 256;
				int iLowLength = (iSendLength) % 256;
				m_pCheckBuf[iCheckIndex++] = iHighLength;
				m_pCheckBuf[iCheckIndex++] = iLowLength;
				
				// Դ��ַ(1)
				chOne = *(pchData + (iIndexUse++));
				m_pCheckBuf[iCheckIndex++] = chOne;
				if (0x10 == chOne) iIndexUse++;
				CharToHexString(chOne, m_chOne, sizeof(m_chOne));

				stFrameData.m_chSourceAddr = chOne;

				int iSendBackPackCount = 0;
				if (chOne == m_iSourceAddr) ++iSendBackPackCount;

				// Դ�˿�(1)
				chOne = *(pchData + (iIndexUse++));
				m_pCheckBuf[iCheckIndex++] = chOne;
				if (0x10 == chOne) iIndexUse++;
				CharToHexString(chOne, m_chOne, sizeof(m_chOne));

				stFrameData.m_chSourcePort = chOne;

				if (chOne == m_iSourcePort) ++iSendBackPackCount;

				// ��������
				char* ptrData = NULL;
				int iASCCalcCount = 0;
				int iASCIIMore = 0;
				bool bPreIs0x10 = false;
				bool bHadLost0x10 = false;				
				int iSendCRC = 0;
				int iAddCRCLength = 0;
				unsigned short usCRC = 0;
				static int stiRecvAllCount = 0;

				// ����ǻط���(Դ��һ��)����ֱ���˳�
				bool bShouldHandled = true;
				if ((iSendBackPackCount >= 2))
				{
					bShouldHandled = false;
				}

				// ���� 1(1)
				chOne = *(pchData + (iIndexUse++));
				m_pCheckBuf[iCheckIndex++] = chOne;
				if (0x10 == chOne) iIndexUse++;
				CharToHexString(chOne, m_chOne, sizeof(m_chOne));

				stFrameData.m_chSourceUse = chOne;

				// Ŀ�ĵ�ַ(1)
				chOne = *(pchData + (iIndexUse++));
				m_pCheckBuf[iCheckIndex++] = chOne;
				if (0x10 == chOne) iIndexUse++;
				CharToHexString(chOne, m_chOne, sizeof(m_chOne));

				stFrameData.m_chTargetAddr = chOne;

				iSendBackPackCount = 0;
				if (chOne == m_iSourceAddr) ++iSendBackPackCount;

				// Ŀ�Ķ˿�(1)
				chOne = *(pchData + (iIndexUse++));
				m_pCheckBuf[iCheckIndex++] = chOne;
				if (0x10 == chOne) iIndexUse++;
				CharToHexString(chOne, m_chOne, sizeof(m_chOne));

				stFrameData.m_chTargetPort = chOne;

				if (chOne == m_iSourcePort) ++iSendBackPackCount;

				// �����Ŀ�ĵ�ַ�����Լ�����ֱ���˳�
				if ((iSendBackPackCount < 2))
				{
					bShouldHandled = false;
				}

				// ���� 2(1)
				chOne = *(pchData + (iIndexUse++));
				m_pCheckBuf[iCheckIndex++] = chOne;
				if (0x10 == chOne) iIndexUse++;
				CharToHexString(chOne, m_chOne, sizeof(m_chOne));

				stFrameData.m_chTargetUse = chOne;

				// ֡��(1)
				chOne = *(pchData + (iIndexUse++));
				m_pCheckBuf[iCheckIndex++] = chOne;
				if (0x10 == chOne) iIndexUse++;
				CharToHexString(chOne, m_chOne, sizeof(m_chOne));

				stFrameData.m_chFrameNo = chOne;

				// ������(1)
				chOne = *(pchData + (iIndexUse++));
				m_pCheckBuf[iCheckIndex++] = chOne;
				if (0x10 == chOne) iIndexUse++;
				CharToHexString(chOne, m_chOne, sizeof(m_chOne));

				stFrameData.m_chFrameCmd = chOne;

				iPreASCIILength = iIndexUse;
				iASCIILength = iSendLength - 10;        // ���ǲ�����0x10��

				// ������ȫ������16���ƣ�0x10�ظ�����һ��
				ptrData = (char*)(pchData + iPreASCIILength);
				memset(m_pDataUseBuf, 0, MAX_SIZE_OF_RECV_BUF);

				for (int i = 0; ; ++i)
				{
					if (iASCIILength <= 0) break;

					bHadLost0x10 = false;
					chOne = ptrData[i];
					if (true == bPreIs0x10)
					{
						if (0x10 == chOne)
						{
							++iASCIIMore;       // ��������
							bHadLost0x10 = true;
						}
						else
						{
							m_pDataUseBuf[iASCCalcCount] = chOne;
							++iASCCalcCount;
							if (iASCCalcCount >= iASCIILength)
							{
								// ������һ������0x10��������Ȼ����һ��0x10(Ԥ��)
								if (0x10 == chOne) ++iASCIIMore;   // ��������
								break;
							}
						}
					}
					else
					{
						m_pDataUseBuf[iASCCalcCount] = chOne;
						++iASCCalcCount;
						if (iASCCalcCount >= iASCIILength)
						{
							// ������һ������0x10��������Ȼ����һ��0x10(Ԥ��)
							if (0x10 == chOne) ++iASCIIMore;   // ��������
							break;
						}
					}
					bPreIs0x10 = false;
					if (false == bHadLost0x10)
					{
						if (0x10 == chOne) bPreIs0x10 = true;
					}
					bHadLost0x10 = false;
				}				

				memcpy(m_pCheckBuf + iCheckIndex, m_pDataUseBuf, iASCCalcCount);
				iCheckIndex += iASCCalcCount;

				stFrameData.m_iFrameDataLength = iASCCalcCount;
				stFrameData.m_pFrameDataBuf = (char*)m_pDataUseBuf;
				stFrameData.m_bDataIsOK = true;
				stFrameData.m_bDataIs16Hex = bIs16Data;

				// CRC(2)
				iIndexUse += iASCIILength + iASCIIMore;
				chOne = *(pchData + (iIndexUse++));
				if (0x10 == chOne)
				{
					iIndexUse++;
					++iAddCRCLength;
				}
				iSendCRC = chOne * 256;
				CharToHexString(chOne, m_chOne, sizeof(m_chOne));
				chOne = *(pchData + (iIndexUse++));
				if (0x10 == chOne)
				{
					iIndexUse++;
					++iAddCRCLength;
				}
				iSendCRC += chOne;
				CharToHexString(chOne, m_chOne, sizeof(m_chOne));

				// ֡β(2)
				chOne = *(pchData + (iIndexUse++));
				CharToHexString(chOne, m_chOne, sizeof(m_chOne));
				chOne = *(pchData + (iIndexUse++));
				CharToHexString(chOne, m_chOne, sizeof(m_chOne));

				iAftASCIILength = 2 + 2 + iAddCRCLength;
				iRecvLength = iASCIILength + 10;				

				// ����CRC(2)(�ӳ��ȿ�ʼ��ֹ��CRC)
				usCRC = GetCRC16(m_pCheckBuf, iCheckIndex);

				++stiRecvAllCount;				

				// �����յ����ݰ�16������ʾ����
				if (true == m_bShowDebug)
				{
					strcpy_s(m_pShowBuf, MAX_SIZE_OF_SHOW_BUF, "10 02 ");

					for (int i = 0; i < iCheckIndex; ++i)
					{
						chOne = m_pCheckBuf[i];
						CharToHexString(chOne, m_chOne, sizeof(m_chOne));
						strcat_s(m_pShowBuf, MAX_SIZE_OF_SHOW_BUF, (const char*)m_chOne);
					}
					chOne = (usCRC) / 256;
					CharToHexString(chOne, m_chOne, sizeof(m_chOne));
					strcat_s(m_pShowBuf, MAX_SIZE_OF_SHOW_BUF, (const char*)m_chOne);
					chOne = (usCRC) % 256;
					CharToHexString(chOne, m_chOne, sizeof(m_chOne));
					strcat_s(m_pShowBuf, MAX_SIZE_OF_SHOW_BUF, (const char*)m_chOne);
					strcat_s(m_pShowBuf, MAX_SIZE_OF_SHOW_BUF, "10 03\n");
					//if ((NULL != m_pShowRecvFunc) && (true == bShouldHandled)) m_pShowRecvFunc(stFrameData.m_chSourceAddr, stFrameData.m_chSourcePort, (const char*)m_pShowBuf, m_bShowColor, 0, 0, 0);
				}

				// У��
				if (usCRC != iSendCRC)
				{
					strcpy_s(m_pShowBuf, MAX_SIZE_OF_SHOW_BUF, "\n����CRC ��һ�¡�\n");
					//if ((NULL != m_pShowRecvFunc) && (true == bShouldHandled)) m_pShowRecvFunc(stFrameData.m_chSourceAddr, stFrameData.m_chSourcePort, (const char*)m_pShowBuf, false, 250, 0, 0);

					stFrameData.m_bDataIsOK = false;
				}
				if (iSendLength != iRecvLength)
				{
					strcpy_s(m_pShowBuf, MAX_SIZE_OF_SHOW_BUF, "\n���󣺳��� ��һ�¡�\n");
					//if ((NULL != m_pShowRecvFunc) && (true == bShouldHandled)) m_pShowRecvFunc(stFrameData.m_chSourceAddr, stFrameData.m_chSourcePort, (const char*)m_pShowBuf, false, 250, 0, 0);

					stFrameData.m_bDataIsOK = false;
				}

				if ((NULL != m_pFrameHandleFunc) && (true == bShouldHandled)) m_pFrameHandleFunc(&stFrameData);
				// �������Զ�������, ��Ϊ0, �����ͷ�ȫ���ڴ�, ͬʱ����������ڴ������ͷŲ���
				stFrameData.m_pFrameDataBuf = nullptr;

				// �������µ�
				memset(pszDataBuf, 0, MAX_SIZE_OF_SAVE_BUF);
				iDataLength = 0;
				if (iLeaveLength > 0)
				{
					memcpy(pszDataBuf, m_pTempBuf, iLeaveLength);
					iDataLength = iLeaveLength;
				}
				else
				{
					iDataLength = 0;
				}
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	} while (iDataLength > 0);
}

void RockelecProtocol::PackOneFrame(unsigned char chCmd, const char *pszData, int iDataLength, char *pszDataBuf, int iBufSize, int &iLength)
{
	// ���͵������Ƿ���16���ƣ��ǵĻ�������ASCII����ת��16��������
	bool bIs16Data = m_bSendDataIs16Hex;

	// ��������(�ɵ��ö� �����˿ո�)
	memset(pszDataBuf, 0, iBufSize);
	unsigned char* chSendData = (unsigned char*)pszDataBuf;

	// ����ǲ���֡ͷ֡β����ֻ������������
	if (false == m_bDataHasFrame)
	{
		// ����
		int iStrLength = 0;
		if (0 == iDataLength)
		{
			iStrLength = strlen(pszData);			
		}
		else
		{
			iStrLength = iDataLength;
		}

		// ����
		int iDataUseLength = 0;
		if (0 == iDataLength)
		{
			if (false == bIs16Data)
			{
				for (int i = 0; i < iStrLength; ++i)
				{
					chSendData[iDataUseLength + i] = pszData[i];
				}
				iDataUseLength += iStrLength;
			}
			else
			{
				int iValue;
				vector<string> hexstrVec = SplitHexString(pszData, iStrLength, " ");
				for (auto &hex : hexstrVec) {
					HexStringToChar((const unsigned char *)hex.c_str(), iValue);
					chSendData[iDataUseLength++] = iValue;
				}
			}
		}
		else
		{
			memcpy(chSendData + iDataUseLength, pszData, iDataLength);
			iDataUseLength += iDataLength;
		}

		chSendData[iDataUseLength] = '\0';

		iLength = iDataUseLength;

		return;
	}

	// ע�⣺����֡ͷ��֡β��0x10���ʾΪ0x10 0x10���������������ֽڲ��㳤�ȣ�ͬʱҲ������CRCУ��

	// ��ȡһ�鲻�����ظ�0x10�����ݣ����ڼ���CRC
	unsigned short usCRC = 0;
	{
		// ֡ͷ(2)
		int iDataUseLength = 0;
		chSendData[iDataUseLength++] = 0x10;
		chSendData[iDataUseLength++] = 0x02;

		// ����(2) = N + 10
		int iStrLength = 0;
		if (0 == iDataLength)
		{
			iStrLength = strlen(pszData);	//	�����Ƿ�ʮ������, ����Ҫ������γ���	
		}
		else
		{
			iStrLength = iDataLength;
		}

		int iHighLength = (iStrLength + 10) / 256;
		int iLowLength = (iStrLength + 10) % 256;
		chSendData[iDataUseLength++] = iHighLength;
		chSendData[iDataUseLength++] = iLowLength;

		// Դ��ַ(1)
		int iSvrAddr = m_iSourceAddr;// Դ��ַ
		chSendData[iDataUseLength++] = iSvrAddr;

		// Դ�˿�(1)
		int iSvrPort = m_iSourcePort;// Դ�˿�(1)
		chSendData[iDataUseLength++] = iSvrPort;

		// ����1(1)
		int iBackup1 = m_iSourceResv;// ����1(1)
		chSendData[iDataUseLength++] = iBackup1;

		// Ŀ�ĵ�ַ(1)
		int iDestAddr = m_iTargetAddr; // Ŀ�ĵ�ַ(1)
		chSendData[iDataUseLength++] = iDestAddr;

		// Ŀ�Ķ˿�(1)
		int iDestPort = m_iTargetPort; // Ŀ�Ķ˿�
		chSendData[iDataUseLength++] = iDestPort;

		// ����2(1)
		int iBackup2 = m_iTargetResv;// ����2
		chSendData[iDataUseLength++] = iBackup2;

		// ֡��(1)
		int iNums = m_iFrameNo;// ֡��
		chSendData[iDataUseLength++] = iNums;

		// ������(1)
		int iCommand = m_iCmd;// ������
		if (0 != chCmd) iCommand = chCmd;
		chSendData[iDataUseLength++] = iCommand;

		// ����(N)
		if (0 == iDataLength)
		{
			if (false == bIs16Data)
			{
				for (int i = 0; i < iStrLength; ++i)
				{
					chSendData[iDataUseLength + i] = pszData[i];
				}
				iDataUseLength += iStrLength;
			}
			else
			{				
				int iValue = 0;
				vector<string> hexstrVec = SplitHexString(pszData, iStrLength, " ");
				for (auto &hex : hexstrVec) {
					HexStringToChar((const unsigned char *)hex.c_str(), iValue);
					chSendData[iDataUseLength++] = iValue;
				}
			}
		}
		else
		{
			memcpy(chSendData + iDataUseLength, pszData, iDataLength);
			iDataUseLength += iDataLength;
		}

		// CRC(2)(�ӳ��ȿ�ʼ��ֹ��CRC)
		usCRC = GetCRC16((chSendData + 2), iDataUseLength - 2);

		memset(pszDataBuf, 0, iBufSize);
	}

	// ֡ͷ(2)
	int iDataUseLength = 0;
	chSendData[iDataUseLength++] = 0x10;
	chSendData[iDataUseLength++] = 0x02;

	// ����(2) = N + 10
	int iStrLength = 0;
	if (0 == iDataLength)
	{
		iStrLength = strlen(pszData);  // �����Ƿ�ʮ������, ����Ҫ��������ַ�������
	}
	else
	{
		iStrLength = iDataLength;
	}

	int iHighLength = (iStrLength + 10) / 256;
	int iLowLength = (iStrLength + 10) % 256;
	chSendData[iDataUseLength++] = iHighLength;
	if (0x10 == iHighLength) chSendData[iDataUseLength++] = iHighLength;
	chSendData[iDataUseLength++] = iLowLength;
	if (0x10 == iLowLength) chSendData[iDataUseLength++] = iLowLength;

	// Դ��ַ(1)
	int iSvrAddr = m_iSourceAddr;// Դ��ַ
	chSendData[iDataUseLength++] = iSvrAddr;
	if (0x10 == iSvrAddr) chSendData[iDataUseLength++] = iSvrAddr;

	// Դ�˿�(1)
	int iSvrPort = m_iSourcePort;// Դ�˿�(1)
	chSendData[iDataUseLength++] = iSvrPort;
	if (0x10 == iSvrPort) chSendData[iDataUseLength++] = iSvrPort;

	// ����1(1)
	int iBackup1 = m_iSourceResv;// ����1(1)
	chSendData[iDataUseLength++] = iBackup1;
	if (0x10 == iBackup1) chSendData[iDataUseLength++] = iBackup1;

	// Ŀ�ĵ�ַ(1)
	int iDestAddr = m_iTargetAddr; // Ŀ�ĵ�ַ(1)
	chSendData[iDataUseLength++] = iDestAddr;
	if (0x10 == iDestAddr) chSendData[iDataUseLength++] = iDestAddr;

	// Ŀ�Ķ˿�(1)
	int iDestPort = m_iTargetPort; // Ŀ�Ķ˿�
	chSendData[iDataUseLength++] = iDestPort;
	if (0x10 == iDestPort) chSendData[iDataUseLength++] = iDestPort;

	// ����2(1)
	int iBackup2 = m_iTargetResv;// ����2
	chSendData[iDataUseLength++] = iBackup2;
	if (0x10 == iBackup2) chSendData[iDataUseLength++] = iBackup2;

	// ֡��(1)
	int iNums = m_iFrameNo;// ֡��
	chSendData[iDataUseLength++] = iNums;
	if (0x10 == iNums) chSendData[iDataUseLength++] = iNums;

	// ������(1)
	int iCommand = m_iCmd;// ������
	if (0 != chCmd) iCommand = chCmd;
	chSendData[iDataUseLength++] = iCommand;
	if (0x10 == iCommand) chSendData[iDataUseLength++] = iCommand;

	// ����(N)
	if (0 == iDataLength)
	{
		if (false == bIs16Data)
		{
			for (int i = 0; i < iStrLength; ++i)
			{
				unsigned char chHex = pszData[i];
				chSendData[iDataUseLength++] = chHex;
				if (0x10 == chHex) chSendData[iDataUseLength++] = chHex;
			}			
		}
		else
		{			
			int iValue = 0;
			vector<string> hexstrVec = SplitHexString(pszData, iStrLength, " ");
			for (auto &hex : hexstrVec) {
				HexStringToChar((const unsigned char *)hex.c_str(), iValue);
				chSendData[iDataUseLength++] = iValue;
				if (0x10 == iValue) chSendData[iDataUseLength++] = iValue;
			}
		}
	}
	else
	{
		for (int i = 0; i < iDataLength; ++i)
		{
			unsigned char chDataH = pszData[i];
			chSendData[iDataUseLength++] = chDataH;
			if (0x10 == chDataH) chSendData[iDataUseLength++] = chDataH;
		}
	}

	// CRC(2)(�ӳ��ȿ�ʼ��ֹ��CRC)
	iHighLength = (usCRC) / 256;
	iLowLength = (usCRC) % 256;
	chSendData[iDataUseLength++] = iHighLength;
	if (0x10 == iHighLength) chSendData[iDataUseLength++] = iHighLength;
	chSendData[iDataUseLength++] = iLowLength;
	if (0x10 == iLowLength) chSendData[iDataUseLength++] = iLowLength;

	// ֡β(2)
	chSendData[iDataUseLength++] = 0x10;
	chSendData[iDataUseLength++] = 0x03;

	chSendData[iDataUseLength] = '\0';

	iLength = iDataUseLength;

}

void RockelecProtocol::CharToHexString(int iData, unsigned char *pszData, int iLength)
{
	sprintf_s((char *)pszData, iLength, "%02X ", iData);
}

void RockelecProtocol::HexStringToChar(const unsigned char *pszData, int &iData)
{
	sscanf_s((const char *)pszData, "%X", &iData);
}

vector<string> RockelecProtocol::SplitHexString(const char *pszData, int iDataLength, const char *pSplitter)
{
	std::size_t pos(0), found(0);
	std::size_t sepLen = strlen(pSplitter);
	vector<string> res;
	string strData(pszData);
	found = strData.find(pSplitter);
	while (found != string::npos) {
		res.push_back(strData.substr(pos, found - pos));
		pos = found + sepLen;
		found = strData.find(pSplitter, pos);
	}
	if (pos != string::npos)
		res.push_back(strData.substr(pos));

	return res;
}

unsigned short RockelecProtocol::GetCRC16(unsigned char *cp, unsigned int leng)
{
	unsigned short crc = 0;
	unsigned int i, j;
	if (leng <= 0) {
		return (0);
	}
	for (j = 0; j < leng; j++) {
		crc = crc ^ (int)*cp++ << 8;
		for (i = 0; i < 8; ++i) {
			if (crc & 0x8000) {
				crc = crc << 1 ^ 0x1021;
			} else {
				crc = crc << 1;
			}
		}
	}

	return (unsigned short)(crc & 0xFFFF);
}
