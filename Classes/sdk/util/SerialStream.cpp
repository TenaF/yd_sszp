//#include "CommonData.h"
#include "SerialStream.h"

CSerialStream::CSerialStream(void* pData/* =NULL OPTIONAL*/,unsigned int nSize/* = UINT_MAX OPTIONAL*/) {
	SetStreamData(pData, nSize);
}

CSerialStream::~CSerialStream(void) {
}

bool CSerialStream::WriteData(const void* pData ,unsigned int nSize) {
	assert(m_nPosition + nSize <= m_nMaxSize);
	if (m_nPosition + nSize <= m_nMaxSize) {
		memcpy(m_pData + m_nPosition, pData, nSize);
		m_nPosition += nSize;
		return true;
	} else
		return false;
}

bool CSerialStream::ReadData(void* pData ,unsigned int nSize) {
	if (m_nPosition + nSize <= m_nMaxSize) {
		memcpy(pData, m_pData + m_nPosition, nSize);
		m_nPosition += nSize;
		return true;
	} else
		return false;
}

void* CSerialStream::GetData(unsigned int nSize) {
	if (m_nPosition + nSize <= m_nMaxSize) {
		void* pRead = m_pData + m_nPosition;
		m_nPosition += nSize;
		return pRead;
	} else
		return NULL;
}

const TCHAR* CSerialStream::ReadStringW() {
	if (m_nPosition + 2 <= m_nMaxSize) {
		const TCHAR* lpRead = reinterpret_cast<const TCHAR*>(m_pData + m_nPosition);
		unsigned int nSize = 0;
		while (lpRead[nSize++]) {
			if (m_nPosition + nSize * 2 > m_nMaxSize) {
				return NULL;
			}
		}

		m_nPosition += nSize * 2;
		return lpRead;
	} else
		return NULL;
}
const char* CSerialStream::ReadStringA() {

	if (m_nPosition + 1 <= m_nMaxSize) {
		const char* lpRead = reinterpret_cast<const char*>(m_pData + m_nPosition);
		size_t nSize = 0;
		while (lpRead[nSize++]) {
			if (m_nPosition + nSize > m_nMaxSize) {
				return NULL;
			}
		}

		m_nPosition += nSize;
		return lpRead;
	} else
		return NULL;
}

