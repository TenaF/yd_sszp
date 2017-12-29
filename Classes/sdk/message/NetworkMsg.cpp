//
//  NetworkMsg.cpp
//  hello2dx
//
//  Created by on 14-7-6.
//
//

#include "NetworkMsg.h"

NetworkMsg::NetworkMsg(int flag) :
		MsgBase(flag) {
	m_eType = MSG_TYPE::MSG_TYPE_NETWORK;
	m_nMainId = 0;
	m_nSubId = 0;
	m_nLen = 0;
	m_pBuffer = nullptr;
}

NetworkMsg::~NetworkMsg() {
	CC_SAFE_DELETE(m_pBuffer);
}

void NetworkMsg::setMainId(WORD var) {
	m_nMainId = var;
}

WORD NetworkMsg::getMainId() {
	return m_nMainId;
}

void NetworkMsg::setSubId(WORD var) {
	m_nSubId = var;
}

WORD NetworkMsg::getSubId() {
	return m_nSubId;
}

WORD NetworkMsg::getLen() const {
	return m_nLen;
}

BYTE* NetworkMsg::getBuffer() const {
	return m_pBuffer;
}

void NetworkMsg::setBuffer(BYTE *buffer ,WORD len) {
	m_nLen = len;
	m_pBuffer = (BYTE*) malloc(len + 1);
	if (m_pBuffer) {
		memset(m_pBuffer, 0x00, len + 1);
		memcpy(m_pBuffer, buffer, len);
	}
}
