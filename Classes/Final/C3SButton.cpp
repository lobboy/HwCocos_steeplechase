#include "C3SButton.h"

USING_NS_CC;

C3SButton::C3SButton()
{
}


C3SButton::~C3SButton()
{
	if (_btnRoot != nullptr)_btnRoot->release();
	if (_btnSprite[0] != nullptr) _btnSprite[0]->release();
	if (_btnSprite[1] != nullptr) _btnSprite[0]->release();
	if (_btnSprite[2] != nullptr) _btnSprite[0]->release();
}

void C3SButton::Init(const char * csbname, cocos2d::Layer & parent, const char *nodename, const char *name1, const char *name2, const char *name3)
{
	auto getNode = CSLoader::createNode(csbname);
	_btnRoot = (cocos2d::Node*)getNode->getChildByName(nodename);
	_btnRoot->setPosition(0, 0); // 預設放在 (0,0) 
	_btnSprite[0] = (cocos2d::Sprite*)_btnRoot->getChildByName(name1);
	_btnSprite[1] = (cocos2d::Sprite*)_btnRoot->getChildByName(name2);
	_btnSprite[2] = (cocos2d::Sprite*)_btnRoot->getChildByName(name3);

	setStatus(CLICKABLE_ON);

	parent.addChild(_btnRoot);
}

void C3SButton::setPosition(const Point &pos)
{
	_btnRoot->setPosition(pos);

	Size size;
	size = _btnSprite[0]->getContentSize();
	_rect = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);
}

void C3SButton::setPosition(const float x, const float y)
{
	_btnRoot->setPosition(x,y);

	Size size;
	size = _btnSprite[0]->getContentSize();
	_rect = Rect(x - size.width / 2, y - size.height / 2, size.width, size.height);
}

void C3SButton::setStatus(int status)
{
	if (status == CLICKABLE_ON)
	{
		clickable = true;
		_btnSprite[0]->setVisible(true);
		_btnSprite[1]->setVisible(false);
		_btnSprite[2]->setVisible(false);
	}
	else if(status == CLICKABLE_OFF)
	{
		clickable = false;
		_btnSprite[0]->setVisible(false);
		_btnSprite[1]->setVisible(false);
		_btnSprite[2]->setVisible(true);
	}
}

bool C3SButton::onTouchBegan(cocos2d::Touch * pTouch, cocos2d::Event * pEvent)
{
	if (clickable)
	{
		Point touchLoc = pTouch->getLocation();

		if (_rect.containsPoint(touchLoc))
		{
			//do someting when onTouchBegan
			_btnSprite[0]->setVisible(false);
			_btnSprite[1]->setVisible(true);
			return true;
		}
	}
	return false;
}

bool C3SButton::onTouchMoved(cocos2d::Touch * pTouch, cocos2d::Event * pEvent)
{
	if (clickable)
	{
		Point touchLoc = pTouch->getLocation();

		if (_rect.containsPoint(touchLoc))
		{
			//do someting when onTouchMoved
			_btnSprite[0]->setVisible(false);
			_btnSprite[1]->setVisible(true);
			return true;
		}
	}
	return false;
}

bool C3SButton::onTouchEnded(cocos2d::Touch * pTouch, cocos2d::Event * pEvent)
{
	if (clickable)
	{
		Point touchLoc = pTouch->getLocation();

		if (_rect.containsPoint(touchLoc))
		{
			//do someting when onTouchEnded
			_btnSprite[0]->setVisible(true);
			_btnSprite[1]->setVisible(false);
			return true;
		}
	}
	return false;
}
