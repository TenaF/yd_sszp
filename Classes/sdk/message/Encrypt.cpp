#include "Encrypt.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32) && CODE_LLVM_ENCRYPT
#pragma obfuscate on
#endif


char szOutBuff[255]={0};

void bin2str(unsigned char* in,char* out,int size)
{
	// 	unsigned char* pt1 = in;
	char* pt2 = out;
	// 	do
	// 	{
	// 		pt2 += sprintf(pt2,"%02X,",*pt1++);
	// 		size--;
	// 	}while(*pt1 && size);

	char tmp[255]={0};
	for (int i=0;i<size;i++)
	{
		sprintf(tmp,"%02X,",in[i]);
		strcat(pt2, tmp);
	}
}


// PriKey_Encrypt		利用私钥加密字符串，返回加密后的长度
// 参数1 indata			明文字符串
// 参数2 outdata			密文输出
// 参数3 PrivateRsa		私钥RSA数据
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
int PriKey_Encrypt(const unsigned char * indata,  unsigned char *outdata, RSA* PrivateRsa)
#else
int PriKey_Encrypt(const unsigned char * indata, OUT unsigned char *outdata, RSA* PrivateRsa)
#endif

{
//	OutputDebugStringA("--------RSA Private Encrypt Start------\n");

	int ret = 0;
	//判断位数函数, 返回RSA模的位数
	int PublicRsaLen =  RSA_size(PrivateRsa);

	unsigned int blockLen = 0;
	unsigned int blockSize = PublicRsaLen-11;

	char *tmp=new char[PublicRsaLen+1];

	if (tmp !=NULL)
	{
		for(int i=0;;i+=blockSize)
		{
			if(i>=PublicRsaLen) break;

			memset(tmp,0,PublicRsaLen+1);
			int flen=(PublicRsaLen-i)>blockSize?blockSize:PublicRsaLen-i;
			ret = RSA_private_encrypt(flen, indata+i, (unsigned char *)tmp, PrivateRsa, RSA_PKCS1_PADDING);
			if (ret != -1)
			{
				memmove(outdata+blockLen,tmp,ret);
				blockLen += ret;
			}
		}
		delete[] tmp;
	}

	RSA_free(PrivateRsa);

	if (ret == -1)
	{
//		OutputDebugStringA("RSA Private Encrypt Error\n");
	}
	return ret;  
} 

// PubKey_Decrypt		公钥解密函数，返回解密后的长度  
// 参数1 indata			密文字符串指针
// 参数2 outdata			明文输出指针
// 参数3 PublicKey		RSA数据
// 参数4 PublicKeyLen	RSA数据长度
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
int PubKey_Decrypt(const unsigned char * indata,  unsigned char * outdata, unsigned char *PublicKey, int PublicKeyLen)
#else
int PubKey_Decrypt(const unsigned char * indata, OUT unsigned char * outdata, unsigned char *PublicKey, int PublicKeyLen)
#endif
{
//	OutputDebugStringA("--------RSA Public Decrypt Satrt ------\n");

	unsigned char *PKey = PublicKey; 
	RSA* PublicRsa = d2i_RSAPublicKey(NULL, (const unsigned char**)&PKey, PublicKeyLen);

	int ret = 0;

	//判断位数函数, 返回RSA模的位数
	int PublicRsaLen =  RSA_size(PublicRsa);

	int blockLen = 0;
	int blockSize = PublicRsaLen * 2;

	char *tmp=new char[PublicRsaLen+1];

	if (tmp !=NULL)
	{
		for(int i=0;;i+=PublicRsaLen)
		{
			if(i>=blockSize) break;

			memset(tmp,0,PublicRsaLen+1);
			int flen=(blockSize-i)>PublicRsaLen?PublicRsaLen:blockSize-i;

			ret = RSA_public_decrypt(flen, indata+i, (unsigned char *)tmp, PublicRsa, RSA_PKCS1_PADDING);
			if (ret != -1)
			{
				memmove(outdata+blockLen,tmp,ret);
				blockLen += ret;
			}
		}
		delete[] tmp;
	}

	RSA_free(PublicRsa);

	if (ret == -1)
	{
//		OutputDebugStringA("RSA Public Decrypt Error\n");
	}
//	OutputDebugStringA("\n--------RSA Public Decrypt Over ------\n");
	return ret;  
} 

//////////////////////////////////////////////////////////////////////////

// 矩阵Key
const char cszKey[32] = {22,27,13,31,25,15,17,23,12,29,3,26,10,7,18,9,2,11,2,30,8,5,16,20,1,28,14,6,24,4,19,21};

char PlazaRc4Key[32] = {0};
char RoomRc4Key[32]={0};

//清空大厅RC4KEY
void ZeroPlazaKey()
{
	//ZeroMemory(&PlazaRc4Key,32);
	memset(&PlazaRc4Key, 0, 32);
}

//清空房间RC4KEY
void ZeroRoomKey()
{
	//ZeroMemory(&RoomRc4Key,32);
	memset(&RoomRc4Key, 0, 32);
}

void Vt_SetPlazaRc4Key(char * szKey)
{
	memcpy(PlazaRc4Key,szKey,32);
	return;
}

void Vt_SetRoomRc4Key(char * szKey)
{
	memcpy(RoomRc4Key,szKey,32);
	return;
}

void Vt_RC4Encrypt(const BYTE* pSrc,BYTE* pDes,unsigned int key,int len,bool bIsPlaza)
{
	int i,j,n,m;
	UINT k;
	BYTE *a,*b,*c,*d;
	memcpy(pDes,pSrc,len);
	BYTE* pchr = pDes;
	UINT* puint = (UINT*)pchr;
	k = key;
	n = 0;
	for(i = 0;i<len/4;i++)
	{
		//puint[i] = (puint[i]>>cszKey[n]) + (puint[i]<<(32-cszKey[n]));
		//判断是大厅的KEY还是是房间的KEY
		if (bIsPlaza)
		{
			if (PlazaRc4Key[0]!='\0')
			{
				puint[i] = (puint[i]>>PlazaRc4Key[n]) + (puint[i]<<(32-PlazaRc4Key[n]));
			}
			else
			{
				puint[i] = (puint[i]>>cszKey[n]) + (puint[i]<<(32-cszKey[n]));
			}
		}
		else
		{
			if (RoomRc4Key[0]!='\0')
			{
				puint[i] = (puint[i]>>RoomRc4Key[n]) + (puint[i]<<(32-RoomRc4Key[n]));
			}
			else
			{
				puint[i] = (puint[i]>>cszKey[n]) + (puint[i]<<(32-cszKey[n]));
			}
		}
		
		puint[i] ^= k;
		m = puint[i];
		a = &pchr[i*4];
		b = &pchr[i*4+1];
		c = &pchr[i*4+2];
		d = &pchr[i*4+3];
		*a ^= *c>>(8-k%8);
		*b -= *d>>(k%8);
		*c ^= *a>>(k%8);
		*d -= *b>>(8-k%8);
		k = m;
		n = pchr[3+i*4]%32;
	}

	BYTE* pKey = (BYTE*)&key;
	n%=8;
	for(j = i*4;j<len;j++)
	{
		pchr[j] = (pchr[j]>>n) + (pchr[j]<<(8-n));
		pchr[j] ^= *pKey++;
		n = pchr[j]%8;
	}
}


const char base[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

static char find_pos(char ch)
{
	char *ptr = (char*)strrchr(base, ch);//the last position (the only) in base[]   
	return (ptr - base);
}

char *base64_encode(const char* data, int data_len)
{
	//int data_len = strlen(data); 
	int prepare = 0;
	int ret_len;
	int temp = 0;
	char *ret = NULL;
	char *f = NULL;
	int tmp = 0;
	char changed[4];
	int i = 0;
	ret_len = data_len / 3;
	temp = data_len % 3;
	if (temp > 0)
	{
		ret_len += 1;
	}
	ret_len = ret_len * 4 + 1;
	ret = (char *)malloc(ret_len);

	if (ret == NULL)
	{
		printf("No enough memory.\n");
		exit(0);
	}
	memset(ret, 0, ret_len);
	f = ret;
	while (tmp < data_len)
	{
		temp = 0;
		prepare = 0;
		memset(changed, '\0', 4);
		while (temp < 3)
		{
			//printf("tmp = %d\n", tmp); 
			if (tmp >= data_len)
			{
				break;
			}
			prepare = ((prepare << 8) | (data[tmp] & 0xFF));
			tmp++;
			temp++;
		}
		prepare = (prepare << ((3 - temp) * 8));
		//printf("before for : temp = %d, prepare = %d\n", temp, prepare); 
		for (i = 0; i < 4; i++)
		{
			if (temp < i)
			{
				changed[i] = 0x40;
			}
			else
			{
				changed[i] = (prepare >> ((3 - i) * 6)) & 0x3F;
			}
			*f = base[changed[i]];
			//printf("%.2X", changed[i]); 
			f++;
		}
	}
	*f = '\0';

	return ret;

}

/* */
char *base64_decode(const char *data, int data_len)
{
	int ret_len = (data_len / 4) * 3;
	int equal_count = 0;
	char *ret = NULL;
	char *f = NULL;
	int tmp = 0;
	int temp = 0;
	int prepare = 0;
	int i = 0;
	if (*(data + data_len - 1) == '=')
	{
		equal_count += 1;
	}
	if (*(data + data_len - 2) == '=')
	{
		equal_count += 1;
	}
	if (*(data + data_len - 3) == '=')
	{//seems impossible 
		equal_count += 1;
	}
	switch (equal_count)
	{
	case 0:
		ret_len += 4;//3 + 1 [1 for NULL] 
		break;
	case 1:
		ret_len += 4;//Ceil((6*3)/8)+1 
		break;
	case 2:
		ret_len += 3;//Ceil((6*2)/8)+1 
		break;
	case 3:
		ret_len += 2;//Ceil((6*1)/8)+1 
		break;
	}
	ret = (char *)malloc(ret_len);
	if (ret == NULL)
	{
		printf("No enough memory.\n");
		exit(0);
	}
	memset(ret, 0, ret_len);
	f = ret;
	while (tmp < (data_len - equal_count))
	{
		temp = 0;
		prepare = 0;
		while (temp < 4)
		{
			if (tmp >= (data_len - equal_count))
			{
				break;
			}
			prepare = (prepare << 6) | (find_pos(data[tmp]));
			temp++;
			tmp++;
		}
		prepare = prepare << ((4 - temp) * 6);
		for (i = 0; i < 3; i++)
		{
			if (i == temp)
			{
				break;
			}
			*f = (char)((prepare >> ((2 - i) * 8)) & 0xFF);
			f++;
		}
	}
	*f = '\0';
	return ret;
}

// 加密算法
char nKey[4] = { 0x9, static_cast<char>(0xF1), static_cast<char>(0xEF) };
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
void EncDataXor( char *src,  char dest[], int nSize)
#else
void EncDataXor(IN char *src, OUT char dest[], int nSize)
#endif

{
	char base64[1024 * 10] = { 0 };
	for (int i = 0; i < nSize; i++)
	{
		for (int j = 0; j < strlen(nKey); j++)
		{
			base64[i] = src[i] ^ nKey[j];
		}
	}
	strcpy(dest, base64_encode(base64, nSize));
	return;
}
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
void DecDataXor( char *src,  char dest[], int nSize)
#else
void DecDataXor(IN char *src, OUT char dest[], int nSize)
#endif
{
	char base64[1024 * 10] = { 0 };
	strcpy(base64, base64_decode(src, nSize));
	nSize = strlen(base64);
	for (int i = 0; i < nSize; i++)
	{
		for (int j = 0; j < strlen(nKey); j++)
		{
			dest[i] = base64[i] ^ nKey[j];
		}
	}
	return;
}

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32) && CODE_LLVM_ENCRYPT
#pragma obfuscate off
#endif
