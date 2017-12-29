//2014-7-24 ������������㷨

#ifndef ENCRYPT_SENDDATA_FILE
#define ENCRYPT_SENDDATA_FILE

#pragma once

#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
#include <openssl/rsa.h>
#pragma comment(lib, "libeay32.lib")
#pragma comment(lib, "ssleay32.lib")
#else
#include <openssl/rsa.h>
#endif
//����RSAͷ�ļ�


#include "Define.h"

	void Vt_RC4Encrypt(const BYTE* pSrc,BYTE* pDes,unsigned int key,int len,bool bIsPlaza=true);

	void Vt_SetPlazaRc4Key(char * szKey);
	void Vt_SetRoomRc4Key(char * szKey);

	//��մ���RC4KEY
	void ZeroPlazaKey();
	//��շ���RC4KEY
	void ZeroRoomKey();

	void bin2str(unsigned char* in,char* out,int size);


	char *base64_encode(const char* data, int data_len);
	char *base64_decode(const char *data, int data_len);

#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
	// PubKey_Decrypt		��Կ���ܺ��������ؽ��ܺ�ĳ���  
	// ����1 indata			�����ַ���ָ��
	// ����2 outdata			�������ָ��
	// ����3 PublicKey		RSA����
	// ����4 PublicKeyLen	RSA���ݳ���
	int PubKey_Decrypt(const unsigned char * indata,  unsigned char * outdata, unsigned char *PublicKey, int PublicKeyLen);



	void EncDataXor( char *src,  char dest[], int nSize);
	void DecDataXor( char *src,  char dest[], int nSize);
#else
	// PubKey_Decrypt		��Կ���ܺ��������ؽ��ܺ�ĳ���  
	// ����1 indata			�����ַ���ָ��
	// ����2 outdata			�������ָ��
	// ����3 PublicKey		RSA����
	// ����4 PublicKeyLen	RSA���ݳ���
	int PubKey_Decrypt(const unsigned char * indata, OUT unsigned char * outdata, unsigned char *PublicKey, int PublicKeyLen);


	void EncDataXor(IN char *src, OUT char dest[], int nSize);
	void DecDataXor(IN char *src, OUT char dest[], int nSize);

#endif

#endif