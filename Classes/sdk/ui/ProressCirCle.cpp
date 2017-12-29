#include "ProressCirCle.h"

ProressCirCle::ProressCirCle() :
m_ProgressTimer(nullptr),
m_ParticleSystemQuad(nullptr),
m_BackGround(nullptr),
m_Action(nullptr),
m_TimeNum(0.0f),
m_function(nullptr),
m_isWearOff(false),
m_bDone(false),
m_RoateTail(nullptr)
{
}

ProressCirCle::~ProressCirCle()
{
	m_function = nullptr;
	unscheduleUpdate();
}

bool ProressCirCle::init()
{
	bool bRes = false;
	do 
	{
		CC_BREAK_IF(!Layer::init());
		bRes = true;
	} while (false);
	return bRes;
}
ProressCirCle* ProressCirCle::create(
	const std::string& _progressName,
	const std::function<void(Ref*)>& _function
	)
{
	ProressCirCle *pRet = new ProressCirCle();
	if (pRet && pRet->init(_progressName, TextureResType::LOCAL, _function))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return nullptr;
	}
}

ProressCirCle* ProressCirCle::createWithPlist(
	const std::string& _progressName,
	const std::function<void(Ref*)>& _function/* = nullptr*/
	)
{
	ProressCirCle *pRet = new ProressCirCle();
	if (pRet && pRet->init(_progressName,TextureResType::PLIST, _function))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return nullptr;
	}
}


bool ProressCirCle::init(
	const std::string& _progressName,
	TextureResType texType,
	const std::function<void(Ref*)>& _function
	)
{
	bool bRes = false;
	do
	{
		CC_BREAK_IF(!Layer::init());

		setProgressTimer(_progressName,texType);

		bRes = true;
	} while (false);

	return bRes;
}

void ProressCirCle::Start(float _timenum, const std::function<void(Ref*)>& _function/* = nullptr*/)
{
	if (m_ProgressTimer)
	{
		this->setWearOff(m_isWearOff);
		scheduleUpdate();
		m_TimeNum = _timenum;
		if (_function)
			m_function = _function;
	}
	if (m_ParticleSystemQuad && m_Action)
	{
		m_ParticleSystemQuad->runAction(m_Action);
	}
}

//设置背景精灵
void ProressCirCle::setBackGroundSprite(const std::string& _spriteName, TextureResType texType/* = TextureResType::LOCAL*/)
{
	if (_spriteName.empty())
		return;

	_BackGroundImageTexType = texType;
	if (m_BackGround)
	{
		m_BackGround->setTexture(_spriteName);
	}
	else
	{
		m_BackGround = ((texType == TextureResType::PLIST) ? Sprite::createWithSpriteFrameName(_spriteName) : Sprite::create(_spriteName));
		if (nullptr == m_BackGround){
			log("m_BackGround find is null~ check sp path !!!");
			return;
		}
		this->addChild(m_BackGround);
	}
}

//设置进度条
void ProressCirCle::setProgressTimer(const std::string& _progressName, TextureResType texType/* = TextureResType::LOCAL*/)
{
	if (_progressName.empty())
		return;
	if (m_ProgressTimer)
	{
		m_ProgressTimer->removeFromParent();
		m_ProgressTimer = nullptr;
	}

	_ProgressImageTexType = texType;
	m_ProgressTimer = ProgressTimer::create(((texType == TextureResType::PLIST) ? Sprite::createWithSpriteFrameName(_progressName) : Sprite::create(_progressName)));
	if (nullptr == m_ProgressTimer){
		log("m_ProgressTimer find is null~ check sp path !!!");
		return;
	}
		

	m_ProgressTimer->setPercentage(0);
	this->addChild(m_ProgressTimer);
}

//设置移动粒子效果
void ProressCirCle::setMoveParticle(const std::string& _particleName, const Vec2& _particlePos)
{
	if (m_ParticleSystemQuad)
	{
		m_ParticleSystemQuad->removeFromParent();
		m_ParticleSystemQuad = nullptr;
	}

	m_ParticleSystemQuad = ParticleSystemQuad::create(_particleName);
	m_ParticleSystemQuad->setPosition(_particlePos);
	this->addChild(m_ParticleSystemQuad);
}

//设置移动粒子的动作
void ProressCirCle::setMoveParticleAciton(Action* _action)
{
	if (m_Action)
	{
		CC_SAFE_RELEASE_NULL(m_Action);
	}

	m_Action = _action;
}

//设置进度条100%效果
void ProressCirCle::setPercentOneHundred(const std::string& _particleFinName)
{

}

//设置进度条消失还是出现(false-表示进度条逐渐出现 ，true-表示进度条逐渐消失)
void ProressCirCle::setWearOff(bool _isWearOff)
{
	m_isWearOff = _isWearOff;
	m_bDone = false;
	if (m_ProgressTimer){
		if (m_isWearOff)
			m_ProgressTimer->setPercentage(100);
		else
			m_ProgressTimer->setPercentage(0);
	}
	if (m_RoateTail)
	{
		m_RoateTail->setVisible(true);
		m_RoateTail->setRotation(3);
	}
}

//添加一个旋转的拖尾效果IMG
void ProressCirCle::setRoateTail(ImageView * _roateTail)
{
	m_RoateTail = _roateTail;
}

bool ProressCirCle::isDone(){
	return m_bDone;
}

void ProressCirCle::update(float _dt)
{	
	if (!this->isVisible()){
		unscheduleUpdate();
		m_bDone = true;
		return;
	}
		
	if (m_RoateTail)
	{
		if (m_RoateTail->getRotation() >= 360)
		{
			m_RoateTail->setVisible(false);
		}
		m_RoateTail->setRotation(m_RoateTail->getRotation() + (_dt * 360 / m_TimeNum));
	}
	//进度条逐渐消失
	if (m_isWearOff)		
	{
		m_ProgressTimer->setPercentage(m_ProgressTimer->getPercentage() - _dt * 100 / m_TimeNum);
		if (m_ProgressTimer->getPercentage() > 0)
		{
			return;
		}
	}
	//进度条逐渐出现
	else                  
	{
		m_ProgressTimer->setPercentage(m_ProgressTimer->getPercentage() + _dt * 100 / m_TimeNum);
		if (m_ProgressTimer->getPercentage() < 100)
		{
			return;
		}
	}

	m_bDone = true;
	unscheduleUpdate();
	if (m_function)
		m_function(this);
	
	//this->removeFromParent();
}


//
//MoveCircle
//
MoveCircle* MoveCircle::create(float duration, const Vec2& center, float scale, float angle)
{
	MoveCircle *ret = new MoveCircle();
	ret->initWithDuration(duration, center, scale, angle);
	ret->autorelease();

	return ret;
}

bool MoveCircle::initWithDuration(float duration, const Vec2& center, float scaleDiff, float angle)
{
	if (ActionInterval::initWithDuration(duration))
	{
		_center = center;
		_scaleDiff = scaleDiff;
		_currScale = 1.0f;
		_angle = angle;

		/************************************************************************/
		/* 计算每次update调用时需要转动的弧度                                   */
		/************************************************************************/
		_anglePreFrame = angle / duration * Director::getInstance()->getAnimationInterval() / (180 / M_PI);
		_frameCnts = 0;
		return true;
	}

	return false;
}

MoveCircle* MoveCircle::clone() const
{
	// no copy constructor
	auto a = new MoveCircle();
	a->initWithDuration(_duration, _center, _scaleDiff, _angle);
	a->autorelease();
	return a;
}

void MoveCircle::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
	_initPos = target->getPosition();
}

MoveCircle* MoveCircle::reverse() const
{
	return MoveCircle::create(_duration, _center, _scaleDiff, -(_angle));
}


void MoveCircle::update(float t)
{
	if (_target)
	{
#if CC_ENABLE_STACKABLE_ACTIONS
		_frameCnts++;
		_currScale += _scaleDiff;

		Vec2 newPos = _initPos.rotateByAngle(_center, _frameCnts * _anglePreFrame);
		Vec2 diff = newPos - _center;

		newPos = diff * _currScale + _center;

		_target->setPosition(newPos);
#else
		DrawNode *node = DrawNode::create();
		node->drawDot(newPos, 3, ccc4f(128, 128, 128, 128));
		_target->getParent()->addChild(node);
#endif // CC_ENABLE_STACKABLE_ACTIONS
	}
}

