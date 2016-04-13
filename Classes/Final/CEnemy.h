#ifndef __CENEMY_H__
#define __CENEMY_H__

#include "cocos2d.h"
#include "cocostudio\CocoStudio.h"

class CEnemy 
{
private:
	char _csbname[20];
	cocos2d::Point _startPt;
	cocos2d::Sprite *_enemySprite; 

public:
	cocos2d::Node   *_enemyRoot;
	CEnemy();
	~CEnemy();
	void Init(const char *csbname, cocos2d::Layer &parent, const char *nodename/*, const char *sprite_name*/);
	void setPosition(const cocos2d::Point &pos);
	void setPosition(const float x, const float y);
	float getPosiX();
	float getPosiY();
	void setColor(const cocos2d::Color3B& color);
	void setAnimation(const char *plistFile);
	void go();
};

#endif