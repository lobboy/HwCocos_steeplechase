#include "CRunner.h"

USING_NS_CC;


CRunner::CRunner(/*const char *csbname, cocos2d::Layer &parent*/)
{
}

void CRunner::Init(const char * csbname, cocos2d::Layer & parent)
{
	// 讀取角色
	auto getNode = CSLoader::createNode(csbname);
	_runnerRoot = (cocos2d::Node*)getNode->getChildByName("Node_1");
	_runnerRoot->setPosition(0, 0); // 預設放在 (0,0) 
	_body = (cocos2d::Sprite *)_runnerRoot->getChildByName("cuber01_1");
	_body->setColor(Color3B(255, 255, 255));
	_normalFace = (cocos2d::Node *)_runnerRoot->getChildByName("normalFace");
	_happyFace = (cocos2d::Node *)_runnerRoot->getChildByName("happyFace");
	_depressedFace = (cocos2d::Node *)_runnerRoot->getChildByName("fuckFace");
	_shadow = (cocos2d::Sprite *)_runnerRoot->getChildByName("cubershadow_8");
	_happyFace->setVisible(false);
	_normalFace->setVisible(true);
	_depressedFace->setVisible(false);
	parent.addChild(_runnerRoot);
	_action = nullptr;
	_jumpAction = nullptr;

	_jumpAction = cocos2d::JumpTo::create(0.75f, Point(0, 0), 250, 1);
	_jumpAction->retain();
}


CRunner::~CRunner()
{
	if(_action != nullptr) _action->release();
	if (_jumpAction != nullptr) _jumpAction->release();
	AnimationCache::destroyInstance();
}


void CRunner::go()
{
	_body->runAction(_action);
}

void CRunner::jump()
{
	_runnerRoot->runAction(_jumpAction);
}

void CRunner::setAnimation(const char *plistFile)
{
	// 此處假設儲存角色所有動態圖片的 plist 檔已經讀入
	auto cache = AnimationCache::getInstance();
	cache->addAnimationsWithFile(plistFile);
	auto animation = cache->getAnimation("running");
	_action = RepeatForever::create(Animate::create(animation));
	_action->retain();
}

void CRunner::setFace(int type)
{
	_happyFace->setVisible(false);
	_normalFace->setVisible(false);
	_depressedFace->setVisible(false);
	switch (type)
	{
	case FACE_NORMAL:
		_normalFace->setVisible(true);
		break;
	case FACE_HAPPY:
		_happyFace->setVisible(true);
		break;
	case FACE_DEPRESSED:
		_depressedFace->setVisible(true);
		break;
	default :
		_normalFace->setVisible(true);
	}

}

void CRunner::setPosition(const Point &pos)
{
	_runnerRoot->setPosition(pos);
}

void CRunner::setPosition(const float x, const float y)
{
	_runnerRoot->setPosition(x,y);
}

float CRunner::getPosiX()
{
	Point pt = _runnerRoot->getPosition();
	return pt.x;
}

float CRunner::getPosiY()
{
	Point pt = _runnerRoot->getPosition();
	return pt.y;
}

void CRunner::setColor(const Color3B& color)
{
	_body->setColor(color);
}