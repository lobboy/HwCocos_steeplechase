#ifndef __C3SBUTTON_H__
#define __C3SBUTTON_H__

#include "cocos2d.h"
#include "cocostudio\CocoStudio.h"

#define CLICKABLE_ON 1
#define CLICKABLE_OFF 0

class C3SButton 
{
private:
	bool clickable;
	cocos2d::Node* _btnRoot;
	cocos2d::Rect _rect;
	//cocos2d::Size _size;
	cocos2d::Sprite *_btnSprite[3];

public:
	C3SButton();
	~C3SButton();
	void Init(const char *csbname, cocos2d::Layer &parent, const char *nodename, const char *name1, const char *name2, const char *name3);
	void setPosition(const cocos2d::Point &pos);
	void setPosition(const float x, const float y);//const std::string&
	void setStatus(int status);

	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	bool onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	bool onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
};

#endif