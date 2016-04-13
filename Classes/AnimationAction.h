#ifndef __ANIMATIONACTION_SCENE_H__
#define __ANIMATIONACTION_SCENE_H__


#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "cocostudio/CocoStudio.h"
#include "Final/CRunner.h"
#include "Final/C3SButton.h"
#include "Final/CEnemy.h"

class AnimationAction : public cocos2d::Layer
{
private:
	unsigned int eid_hurt,eid_jump;

	cocos2d::JumpBy *_jumpAction;
	// For Double Jump
	cocos2d::Spawn   *_myJump2;
	int _NoJumps;
	cocos2d::CallFunc *_mycallback;
	cocos2d::Point _runnerPt;
	//UI
	cocos2d::ui::Text *_hpBarValue;
	cocos2d::ui::Text *_scoreValue;
	cocos2d::ui::LoadingBar *_hpBar;
	cocos2d::Sprite *_countdown[3];
	//Enemy
	CEnemy _enemy[3];
	cocos2d::MoveTo *_enemyAction;
	float _spawntimer;
	int _enemytype;
	cocostudio::timeline::ActionTimeline *_action;
	//Runner
	CRunner _runnerRuan;
	float _changefacetimer;
	float _score;
	cocos2d::Sequence *_gethurtSequence;
	cocos2d::Sequence *_getscoreSequence;

	C3SButton _replaybtn;
	C3SButton _exitbtn;
	C3SButton _jumpbtn;
	C3SButton _startbtn;
	bool _isstart;
	int i_countdown;
	cocos2d::Sequence *_countdownSequence;

public:
	~AnimationAction();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	void doStep(float dt);
	void GameStart();

	// Action 的 CallBack 函式
	void AnimationAction::actionFinished();

	//觸碰
	cocos2d::EventListenerTouchOneByOne *_listener1;
	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //觸碰開始事件
	void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //觸碰移動事件
	void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //觸碰結束事件 

	// implement the "static create()" method manually
    CREATE_FUNC(AnimationAction);
};

#endif // __HELLOWORLD_SCENE_H__
