/**
@file SerialStream.h
@brief 数据流序列化/反序列化类定义
*********************************************************************/
#pragma once
#include "sdk/message/Define.h"
/**
* @brief 数据流序列化/反序列化 
*/
#ifndef NO_COPY_CLASS
#define NO_COPY_CLASS(cls) private:cls(cls&);\
cls& operator = (cls&)
#endif //NO_COPY_CLASS

class CSerialStream
{
	NO_COPY_CLASS(CSerialStream);
public:
	/**
	* @brief 根据数据流构造对象
	* @param [in] pData 数据流指针
	* @param [in] nSize 数据流长度，如果为UINT_MAX则不检查长度
	*/
	CSerialStream(void* pData =NULL , unsigned int nSize = 0xFFFFffff);
	~CSerialStream(void);

	/**
	* @brief 设置数据流 
	* @param [in] pData 数据流指针
	* @param [in] nSize 数据流长度，如果为UINT_MAX则不检查长度
	*/
	void SetStreamData( void* pData = NULL, unsigned int nSize = 0xFFFFffff )
	{
		m_pData = reinterpret_cast<char*>(pData);
		m_nMaxSize = nSize;
		m_nPosition = 0;
	}

	/**
	* @brief 获得数据流数据 
	*/
	void* GetStream(void)
	{
		return m_pData;
	}

	/**
	* @brief 获得数据流最大长度 
	*/
	unsigned int GetStreamLength(void)
	{
		return m_nMaxSize;
	}

	/**
	* @brief  设置读写指针
	* @param [in] nPos 新的读写指针,新的指针不能大于数据流最大长度
	*/
	void SetPosition( unsigned int nPos)
	{
		assert(nPos<m_nMaxSize);
		assert(m_pData);
		m_nPosition = nPos;
	}

	/**
	* @brief 返回读写指针  
	*/
	unsigned int GetPosition(void)const
	{
		return m_nPosition;
	}

	/**
	* @brief 写入任意数据
	* @param [in] pData 任意数据的地址
	* @param [in] nSize 任意数据的长度
	* @return 成功返回非0，数据超过流长度返回0
	*/
	bool WriteData(const void* pData,unsigned int nSize);

	/**
	* @brief  写入GB2312或者UTF-8编码的字符串
	* @param [in] lpWrite 要写入的字符串
	* @return 成功返回非0
	*/
	bool WriteStringA(const char* lpWrite)
	{
		return WriteData( lpWrite,strlen(lpWrite)+1);
	}

	/**
	* @brief 写入UTF-16LE编码的字符串。
	* @param [in] lpWrite 要写入的字符串
	* @return 成功返回非0
	*/
	bool WriteStringW(const WORD* lpWrite)
	{
		int nLen = 0;
		for(;lpWrite[nLen];++nLen)
			;
		return WriteData( lpWrite,sizeof(WORD)*(nLen+1));
	}

	/**
	* @brief  读取任意数据
	* @param [out] pData 要读取的缓冲地址
	* @param [in] nSize 要读取的数据长度，单位：字节
	* @return 成功返回非0
	*/
	bool ReadData(void* pData,unsigned int nSize);

	/**
	* @brief 读取任意数据
	* @param [in] nSize 要读取的数据长度，单位：字节
	* @return 成功返回读取到的指针，石板返回NULL
	*/
	void* GetData(unsigned int nSize);

	/**
	* @brief  读取一个UTF-16LE编码的字符串(宽字符)，
	* @return 成功返回字符串内容，失败返回NULL
	*/
	const TCHAR* ReadStringW();

	/**
	* @brief 读取一个GB2312或者UTF-8编码的字符串（多字节编码) 
	* @return 成功返回字符串内容，失败返回NULL
	*/
	const char* ReadStringA();

	/**
	* @brief 读取一个整型变量 
	*/

	int ReadInt(void)
	{
		int nRead;
		ReadData(nRead);
		return nRead;
	}
		
	/**
	* @brief 读取一个short类型 
	*/
		
	short ReadShort(void)
	{
		short sRead;
		ReadData(sRead);
		return sRead;
	}

	/**
	* @brief 读取一个char 
	*/

	char ReadChar(void)
	{
		char ch;
		ReadData(ch);
		return ch;
	}

	/**
	* @brief 读取一个float 
	*/
	float ReadFloat(void)
	{
		float fRead;
		ReadData(fRead);
		return fRead;
	}
	
	/**
	* @brief 读取一个longlong 
	*/

	long long ReadLonglong(void)
	{
		LONGLONG llRead;
		ReadData(llRead);
		return llRead;
	}
		
	///读取一个double
	double ReadDouble(void)
	{
		double dblRead;
		ReadData(dblRead);
		return dblRead;
	}


	/**
	* @brief 写入任意数据
	* @tparam 需要写入的数据类型
	* @param [in] value 要写入的值
	* @return 成功返回非0
	*/
	template<typename T>
	bool WriteData(const T& value)
	{
		return WriteData(&value,sizeof(value));
	}

	/**
	* @brief 读取任意数据
	* @tparam 需要读取的数据类型
	* @param [out] value 如果读取到数据，写入目的的值
	* @return 成功返回非0
	*/
	template<typename T>
	bool ReadData(T& value)
	{
		return ReadData(&value,sizeof(value));
	}
		
	/**
	* @brief 序列化写入
	* @tparam 需要写入的数据类型
	* @param [in] value 要写入的值
	* @return 返回对象自己引用
	*/
	template<typename T>
	CSerialStream& operator<< (const T& value)
	{
		WriteData(&value,sizeof(value));
		return *this;
	}


	/**
	* @brief 序列化读取
	* @tparam 需要读取的数据类型
	* @param [out] value 如果读取到数据，写入目的的值
	* @return 返回对象自己引用
	*/
	template<typename T>
	CSerialStream& operator>>(T& value)
	{
		ReadData(value);
		return *this;
	}

	/**
	* @brief 序列化数组（读取）
	* @param [out] v 数组名,s数组元素个数
	* @return 返回对象自己引用
	*/
	template<typename T,unsigned int s>
	CSerialStream& operator >>(T (&v)[s])
	{
		ReadData(v,sizeof(v));
		return *this;
	}

	/**
	* @brief 序列化数组（写入） 
	* @param [in] v 数组名,s数组元素个数
	* @return 返回对象自己引用
	*/
	template<typename T,unsigned int s>
	CSerialStream& operator <<(T (&v)[s])
	{
		WriteData(v,sizeof(v));
		return *this;
	}
private:
	/**
	* @brief 阻止序列化未定义指针（读取） 
	*/
	template<typename T>
	CSerialStream& operator >>(T*&);

	/**
	* @brief 阻止序列化未定义指针（写入） 
	*/
	template<typename T>
	CSerialStream& operator<<(T*&);

			/**
	* @brief 阻止序列化未定义指针（写入） 
	*/
	template<typename T>
	CSerialStream& operator<<(T* const &);
protected:
	char*		m_pData;			//!<数据流地址
	unsigned int		m_nMaxSize;			//!<数据流长度
	unsigned int		m_nPosition;		//!<当前读写指针
};

