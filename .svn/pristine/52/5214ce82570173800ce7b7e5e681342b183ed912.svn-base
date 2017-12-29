#ifndef _MTNotification_H_
#define _MTNotification_H_

#include "cocos2d.h"

class MTData : public cocos2d::Ref
{
public:
	static MTData* create(string data0, int  data1, int  data2, int  data3, string data4)
	{
		MTData* dat = new MTData();
		if (dat && dat->init(data0, data1, data2, data3, data4))
		{
			dat->autorelease();
			return dat;
		}
		delete dat;
		return nullptr;
	}
	string data0;
	int data1;
	int data2;
	int data3;
	string data4;
	
private:
	MTData(){}
	~MTData(){}

	bool init(string _data0, int  _data1, int  _data2, int  _data3, string _data4)
	{
		data0 = _data0;
		data1 = _data1;
		data2 = _data2;
		data3 = _data3;
		data4 = _data4;
		return true;
	}
};



#define G_NOTIFICATION(name, obj)					__NotificationCenter::getInstance()->postNotification(name, obj)
#define G_NOTIFICATION_REG(name, callfuncO, obj)	__NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(callfuncO), name, obj)
#define G_NOTIFICATION_UNREG(name)					__NotificationCenter::getInstance()->removeObserver(this, name)

#endif 