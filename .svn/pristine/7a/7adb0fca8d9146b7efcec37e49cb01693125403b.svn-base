/*
描述：MD5加密
*/
#pragma once
#ifndef MD5_H
#define MD5_H

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	typedef unsigned int UINT4;
#else
    typedef unsigned long UINT4;
#endif 



	class  CMd5
	{
	public:

		typedef struct {
			UINT4 i[2];                   /* number of _bits_ handled mod 2^64 */
			UINT4 buf[4];                                    /* scratch buffer */
			unsigned char in[64];                              /* input buffer */
			unsigned char digest[16];     /* actual digest after MD5Final call */
		} MD5_CTX;



		CMd5();
		const unsigned char* MakeMD5(const unsigned char* strIn,int nMD5Len);

	private:
		void Init ();
		void Update (const unsigned char *inBuf,unsigned int inLen);
		void Final ();
	private:
		void Transform (UINT4 *buf,UINT4 *in);
	private:
		unsigned char	m_cPADDING[64];
		MD5_CTX			m_mdContext;
	};

#endif
