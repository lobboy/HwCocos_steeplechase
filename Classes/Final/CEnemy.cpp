#include "CEnemy.h"

USING_NS_CC;


CEnemy::CEnemy()
{
}

void CEnemy::Init(const char *csbname, cocos2d::Layer &parent, const char *nodename/*, const char *sprite_name*/)
{
	
	char tmp[] = "Node1.csb";
	int length = sizeof(tmp) / sizeof(tmp[0]);
	for (int i = 0; i < length - 1; i++)
	{
		_csbname[i] = tmp[i];
	}

	auto getNode = CSLoader::createNode(csbname);
	_enemyRoot = (cocos2d::Node*)getNode->getChildByName(nodename);
	_enemyRoot->setPosition(0, 0); // 預設放在 (0,0)
	_startPt = _enemyRoot->getPosition();
	//_enemySprite = (cocos2d::Sprite *)_enemyRoot->getChildByName(sprite_name);

	parent.addChild(_enemyRoot);

	//sprintf(_csbname , csbname);
	//auto action = CSLoader::createTimeline(_csbname);
	//action->gotoFrameAndPlay(0, 35, false);
	//_enemyRoot->runAction(action);

	//action->retain();

}


CEnemy::~CEnemy()
{
	if (_enemyRoot != nullptr)_enemyRoot->release();
	if (_enemySprite != nullptr)_enemySprite->release();
}

void CEnemy::go()
{
	_enemyRoot->setPosition(_startPt);

	//auto action = CSLoader::createTimeline(_csbname);
	//action->gotoFrameAndPlay(0, 35, false);
	//_enemyRoot->runAction(action);

}

void CEnemy::setPosition(const Point &pos)
{
	_enemyRoot->setPosition(pos);
	_startPt = pos;
}

void CEnemy::setPosition(const float x, const float y)
{
	_enemyRoot->setPosition(x,y);
	_startPt = Point(x, y);
}

float CEnemy::getPosiX()
{
	Point pt = _enemyRoot->getPosition();
	return pt.x;
}

float CEnemy::getPosiY()
{
	Point pt = _enemyRoot->getPosition();
	return pt.y;
}

void CEnemy::setColor(const Color3B& color)
{
	_enemySprite->setColor(color);
}