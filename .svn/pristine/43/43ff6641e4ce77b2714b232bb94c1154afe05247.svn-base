/*
 * GlobalSchedule.cpp
 *
 *  Created on: 2014-10-9
 *      Author: win7
 */

#include "GlobalSchedule.h"
#include "hall/data/DataManager.h"

#define SCHEDULE Director::getInstance()->getScheduler()

GlobalSchedule * GlobalSchedule::m_pSchedule = NULL;

GlobalSchedule::GlobalSchedule(float fInterval  , float fDelay) {
	//CCLog(U2F(" 初始化全局定时器 ").c_str());
	i = 0;
	SCHEDULE->scheduleSelector(schedule_selector(GlobalSchedule::globalUpdate),this,fInterval,kRepeatForever , fDelay ,false);
	m_pSchedule = this;
}

GlobalSchedule::~GlobalSchedule() {
	SCHEDULE->unscheduleSelector(schedule_selector(GlobalSchedule::globalUpdate), this);
	m_pSchedule = nullptr;
}

void GlobalSchedule::start(float fInterval  , float fDelay)
{
	//CCLog(U2F(" 启动全局定时器 ").c_str());
	if(!m_pSchedule)
		new GlobalSchedule(fInterval  ,  fDelay);
}


void GlobalSchedule::globalUpdate(float t)
{
//	NYInfo(TAG," 定时器执行次数 time = %d",i);
//	if(i >= 10)
//	{
//		GlobalSchedule::stop();
//		//DataManager::getInstance()->onScheduleOutMach();
//	}
//	i++;

	//DataManager::getInstance()->onScheduleOutMach();
}

void GlobalSchedule::stop()
{
	//CCLog(U2F(" 全局定时器停止 ").c_str());
	CC_SAFE_RELEASE_NULL(m_pSchedule);
}

void GlobalSchedule::pause()
{
	SCHEDULE->pauseTarget(m_pSchedule);
}

void GlobalSchedule::resume()
{
	SCHEDULE->resumeTarget(m_pSchedule);
}
