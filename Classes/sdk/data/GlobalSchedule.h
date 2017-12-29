/*
 * GlobalSchedule.h
 *
 *  Created on: 2014-10-9
 *      Author: win7
 *
 *      全局定时器
 */

#ifndef GLOBALSCHEDULE_H_
#define GLOBALSCHEDULE_H_

#include "cocos2d.h"

USING_NS_CC;

class GlobalSchedule: public cocos2d::Ref {
public:

	static GlobalSchedule * getInstance();

	static void start(float fInterval = 1.0f, float fDelay = 1.0f);

	static void stop();

	static void pause();

	static void resume();

	void globalUpdate(float t);
private:
	GlobalSchedule(float fInterval , float fDelay);
	~GlobalSchedule();

	//SEL_CallFunc  pFunc ;

	static GlobalSchedule * m_pSchedule;
	int i;

};

#endif /* GLOBALSCHEDULE_H_ */
