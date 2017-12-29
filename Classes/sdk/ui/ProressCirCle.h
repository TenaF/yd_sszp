#ifndef __Proress_Circle__H__
#define __Proress_Circle__H__

#include "sdk/sdk.h"

USING_NS_CC;
using namespace ui;
using namespace std;
using namespace cocostudio;

class ProressCirCle : public Layer
{
public:
	enum class TextureResType
	{
		LOCAL = 0,
		PLIST = 1
	};
public:
	ProressCirCle();
	virtual ~ProressCirCle();

	CREATE_FUNC(ProressCirCle);
	virtual bool init();
	static ProressCirCle* create(
		const std::string& _progressName,
		const std::function<void(Ref*)>& _function = nullptr
		);

	static ProressCirCle* createWithPlist(
		const std::string& _progressName,
		const std::function<void(Ref*)>& _function = nullptr
		);

	bool init(
		const std::string& _progressName,
		TextureResType texType,
		const std::function<void(Ref*)>& _function
		);

public:
	//开始倒计时
	void Start(float _timenum, const std::function<void(Ref*)>& _function = nullptr);

	//设置背景精灵
	void setBackGroundSprite(const std::string& _spriteName, TextureResType texType = TextureResType::LOCAL);

	//设置进度条
	void setProgressTimer(const std::string& _progressName, TextureResType texType = TextureResType::LOCAL);

	//设置移动粒子效果
	void setMoveParticle(const std::string& _particleName, const Vec2& _particlePos);

	//设置移动粒子的动作
	void setMoveParticleAciton(Action* _action);

	//设置进度条100%效果
	void setPercentOneHundred(const std::string& _particleFinName);

	//设置进度条消失还是出现(false-表示进度条逐渐出现 ，true-表示进度条逐渐消失)
	void setWearOff(bool _isWearOff);

	//添加一个旋转的拖尾效果IMG
	void setRoateTail(ImageView * _roateTail);

	//设置方向
	void setReverseDirection(bool value) {
		if (m_ProgressTimer)m_ProgressTimer->setReverseDirection(value);
	}
	//设置反进度
	void setReverseProgress(bool reverse) {
		if (m_ProgressTimer)m_ProgressTimer->setReverseProgress(reverse);
	}

	//是否完成
	bool isDone();
public:
	//update
	virtual void update(float _dt) override;
public:
	std::function<void(Ref*)> m_function;

private:
	ProgressTimer* m_ProgressTimer;
	ParticleSystemQuad* m_ParticleSystemQuad;
	Sprite* m_BackGround;
	TextureResType _ProgressImageTexType;
	TextureResType _BackGroundImageTexType;
	ImageView * m_RoateTail;
	Action* m_Action;
	float m_TimeNum;
	bool m_isWearOff;
	bool m_bDone;
};


///转圈动作
class CC_DLL MoveCircle : public cocos2d::ActionInterval
{
public:
	/** creates the action */
	static MoveCircle* create(float duration, const cocos2d::Vec2& center, float scale, float angle);

	//
	// Overrides
	//
	virtual MoveCircle* clone() const override;
	virtual MoveCircle* reverse() const override;
	virtual void startWithTarget(cocos2d::Node *target);
	virtual void update(float time);

CC_CONSTRUCTOR_ACCESS:
	MoveCircle() {};
	virtual ~MoveCircle() {};

	/** initializes the action */
	bool initWithDuration(float duration, const cocos2d::Vec2& center, float scaleDiff, float angle);
protected:
	cocos2d::Vec2 _center;
	float _scaleDiff;
	float _currScale;
	float _angle;
	float _anglePreFrame;
	int _frameCnts;
	cocos2d::Vec2 _initPos;
private:
	CC_DISALLOW_COPY_AND_ASSIGN(MoveCircle);
};

#endif