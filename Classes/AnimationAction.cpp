#include "AnimationAction.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;

#define DOUBLEJUMP 1

using namespace cocostudio::timeline;
using namespace ui;
using namespace CocosDenshion;

Scene* AnimationAction::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = AnimationAction::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

AnimationAction::~AnimationAction()
{
	if (_countdownSequence != nullptr) _countdownSequence->release();
	if (_jumpAction != nullptr) _jumpAction->release();
	if (_myJump2 != nullptr) _myJump2->release();
	if (_mycallback != nullptr) _mycallback->release();
	if (_hpBarValue != nullptr) _hpBarValue->release();
	if (_scoreValue != nullptr) _scoreValue->release();
	if (_hpBar != nullptr) _hpBar->release();
	if (_enemyAction != nullptr)_enemyAction->release();
	if (_action != nullptr)_action->release();
	if (_countdown[0] != nullptr)_countdown[0]->release();
	if (_countdown[1] != nullptr)_countdown[1]->release();
	if (_countdown[2] != nullptr)_countdown[2]->release();
	if (_gethurtSequence != nullptr)_gethurtSequence->release();
	if (_getscoreSequence != nullptr)_getscoreSequence->release();
	

	AnimationCache::destroyInstance();  // 釋放 AnimationCache 取得的資源
	SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
	Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

// on "init" you need to initialize your instance
bool AnimationAction::init()
{
	_countdownSequence = nullptr;
	_jumpAction = nullptr;
	_myJump2 = nullptr;
	_mycallback = nullptr;
	_hpBarValue = nullptr;
	_scoreValue = nullptr;
	_hpBar = nullptr;
	_enemyAction = nullptr;
	_action = nullptr;
	_gethurtSequence = nullptr;
	_getscoreSequence = nullptr;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("AnimationAction.csb");
    addChild(rootNode);

// STEP 1 : 讀入儲存多張圖片的 plist 檔
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("scene101.plist");
// ------------------------------------------------------------------------------------------------- 

// 音效與音樂 --------------------------------------------------------------------------------
	SimpleAudioEngine::getInstance()->playBackgroundMusic("./SR_bg.mp3", true);

//---------------------------------------------------------------------------------------------

//背景動畫--------------------------------------------------------------------------------------------------

	auto action = (ActionTimeline *)CSLoader::createTimeline("AnimationAction.csb");
	rootNode->runAction(action);
	action->gotoFrameAndPlay(0, 50, true);
//--------------------------------------------------------------------------------------------------
// UI介面
	_hpBar = (cocos2d::ui::LoadingBar *)rootNode->getChildByName("LoadingBar_1");
	_hpBar->setPercent(100);

	_hpBarValue = (cocos2d::ui::Text *)rootNode->getChildByName("Text_HP");
	_hpBarValue->setString(StringUtils::format("HP = %5.2f", 100.0f));

	_score = 0;
	_scoreValue = (cocos2d::ui::Text *)rootNode->getChildByName("Text_score"); 
	_scoreValue->setString(StringUtils::format("Score : %5.2f", 0.0f));

	_replaybtn.Init("Node2.csb", *this,"replaybtn", "replaybtn01", "replaybtn02", "replaybtn03");
	_replaybtn.setPosition(Vec2(visibleSize.width/2.0f + 346.4f, visibleSize.height/2.0f - 256.47f));
	_replaybtn.setStatus(CLICKABLE_OFF);

	_exitbtn.Init("Node2.csb", *this, "exitbtn", "exitbtn01", "exitbtn02", "exitbtn03");
	_exitbtn.setPosition(Vec2(visibleSize.width / 2.0f - 558.79f, visibleSize.height / 2.0f - 256.47f));
	_exitbtn.setStatus(CLICKABLE_ON);

	_jumpbtn.Init("Node2.csb", *this, "jumpbtn", "jumpbtn01", "jumpbtn02", "jumpbtn03");
	_jumpbtn.setPosition(Vec2(visibleSize.width / 2.0f + 520.14f, visibleSize.height / 2.0f - 256.47f));
	_jumpbtn.setStatus(CLICKABLE_OFF);

	_startbtn.Init("Node2.csb", *this, "startbtn", "startbtn01", "startbtn02", "startbtn03");
	_startbtn.setPosition(Vec2(visibleSize.width / 2.0f - 140.0f, visibleSize.height / 2.0f - 256.47f));
	_startbtn.setStatus(CLICKABLE_ON);
	
	/*
	RE
	986.40 -> 346.4f
	103.53 -> -256.47f

	EXIT
	81.21 -> -558.79f
	103.53 -> -256.47f

	JUMP
	1160.14 -> 520.14f
	103.53 -> -256.47f

	START
	500.0 -> -140.0f
	103.53 -> -256.47f
	*/
	Size size;
	_countdown[0] = Sprite::create("res/num_three.png");
	size = _countdown[0]->getContentSize();
	this->_countdown[0]->setPosition(Vec2(visibleSize.width / 2 , visibleSize.height / 2 ));
	_countdown[0]->setOpacity(0);
	this->addChild(_countdown[0]);

	_countdown[1] = Sprite::create("res/num_two.png");
	size = _countdown[1]->getContentSize();
	this->_countdown[1]->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	_countdown[1]->setOpacity(0);
	this->addChild(_countdown[1]);

	_countdown[2] = Sprite::create("res/num_one.png");
	size = _countdown[2]->getContentSize();
	this->_countdown[2]->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	_countdown[2]->setOpacity(0);
	this->addChild(_countdown[2]);

	i_countdown = 0;
//-------------------------------------------------------------------------------------------------
// Enemy設定 -------------------------------------------------------------------------------------

	_spawntimer = 0;

	//單個三角形障礙物
	_enemy[0].Init("Node1.csb", *this, "triRoot");
	_enemy[0].setPosition(Point(-200.0f, 300.0f));

	//四個三角形障礙物
	_enemy[1].Init("Node1.csb", *this, "triRoot_2");
	_enemy[1].setPosition(Point(-200.0f, 300.0f));

	//三個三角形障礙物
	_enemy[2].Init("Node1.csb", *this, "triRoot_3");
	_enemy[2].setPosition(Point(-200.0f, 300.0f));

	_action = CSLoader::createTimeline("Node1.csb");
	_action->gotoFrameAndPlay(0, 35, false);

	_action->retain();

	_enemyAction = cocos2d::MoveTo::create(1.25f, Point(1500.0f, 300.0f));
	_enemyAction->retain();
//---------------------------------------------------------------------------------------------	

//嘗試把CRunner放進來
	_runnerRuan.Init("runner01.csb", *this);
	_changefacetimer = 0;
	_runnerRuan.setFace(FACE_NORMAL);
	_runnerRuan.setPosition(visibleSize.width/ 2.0f + 400, visibleSize.height / 2.0f);
	_runnerRuan.setAnimation("cuberanim.plist");
	_runnerRuan.go();

	_jumpAction = cocos2d::JumpBy::create(0.75f, Point(0.0f, 0.0f), 150, 1);
	_jumpAction->retain();

	TintBy * tintbyA = TintBy::create(0.5f, 209, 73, 63);//red
	auto tintbyABack = tintbyA->reverse();
	TintBy * tintbyB = TintBy::create(0.5f, 175, 255, 177);//green
	auto tintbyBBack = tintbyB->reverse();
	_gethurtSequence = Sequence::create(tintbyA, tintbyABack, NULL);
	_getscoreSequence = Sequence::create(tintbyB, tintbyBBack, NULL);
	_gethurtSequence->retain();
	_getscoreSequence->retain();
//-------------------------------------------------------------------------------------------------

// Double Jump 
#ifdef DOUBLEJUMP
	_NoJumps = 0;
	_runnerPt = _runnerRuan._runnerRoot->getPosition();

	auto JumpUP = cocos2d::JumpBy::create(0.75f, Point(0.0f, 0.0f), 150, 1);
	auto Rotatecircle = RotateBy::create(0.75f, -360.0f);
	_myJump2 = Spawn::createWithTwoActions(JumpUP, Rotatecircle);
	_myJump2->retain();

	_mycallback = CallFunc::create(this, callfunc_selector(AnimationAction::actionFinished));
	//CallFunc::create(std::bind(&HelloWorld::callback0, this));
	_mycallback->retain();
#endif

//-------------------------------------------------------------------------------------------------

	_listener1 = EventListenerTouchOneByOne::create();	//創建一個一對一的事件聆聽器
	_listener1->onTouchBegan = CC_CALLBACK_2(AnimationAction::onTouchBegan, this);		//加入觸碰開始事件
	_listener1->onTouchMoved = CC_CALLBACK_2(AnimationAction::onTouchMoved, this);		//加入觸碰移動事件
	_listener1->onTouchEnded = CC_CALLBACK_2(AnimationAction::onTouchEnded, this);		//加入觸碰離開事件

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener1, this);	//加入剛創建的事件聆聽器
	this->schedule(CC_SCHEDULE_SELECTOR(AnimationAction::doStep));
    return true;
}

void AnimationAction::actionFinished()
{
	// do something on complete
	if (_isstart) 
	{
#ifdef DOUBLEJUMP
		_NoJumps--;
		if (_NoJumps == 1) {
			auto moveto = MoveTo::create(0.15f, _runnerPt);
			_runnerRuan._runnerRoot->runAction(moveto);
			_NoJumps--;
		}
#endif
	}
	else
	{
		if (i_countdown == 3)
		{
			_isstart = true;
			_replaybtn.setStatus(CLICKABLE_ON);
			_exitbtn.setStatus(CLICKABLE_OFF);
			_jumpbtn.setStatus(CLICKABLE_ON);
			_startbtn.setStatus(CLICKABLE_OFF);
			_countdown[0]->setOpacity(0);
			_countdown[1]->setOpacity(0);
			_countdown[2]->setOpacity(0);

			_hpBar->setPercent(100);
			_hpBarValue->setString(StringUtils::format("HP = %5.2f", 100.0f));

			_score = 0;
			_scoreValue->setString(StringUtils::format("Score : %5.2f", 0.0f));
		}
	}
}

void AnimationAction::doStep(float dt)
{
	if (_isstart)
	{
		float HP = _hpBar->getPercent();
		_changefacetimer += dt;
		if (_changefacetimer > 1.0f)
			_runnerRuan.setFace(FACE_NORMAL);

// Enemy設定 -------------------------------------------------------------------------------------
		_spawntimer += dt;
		if (_spawntimer > 3)//spawn a enemy per 3 secconds
		{
			_enemytype = rand() % 3;
			_enemy[_enemytype].go();
			_action->gotoFrameAndPlay(0, 35, false);
			_enemy[_enemytype]._enemyRoot->runAction(_action);
			_enemy[_enemytype]._enemyRoot->runAction(_enemyAction);

			_spawntimer = 0;
		}
//-------------------------------------------------------------------------------------

// 受傷, 分數設定 -------------------------------------------------------------------------------------

		Vec2 enemy_posi;
		Vec2 runner_posi;
		if (_enemytype != 99)
		{
			enemy_posi = _enemy[_enemytype]._enemyRoot->getPosition();
			runner_posi = _runnerRuan._runnerRoot->getPosition();

			if (abs(enemy_posi.x - runner_posi.x) < 100)
			{
				_changefacetimer = 0;

				switch (_enemytype)
				{
				case 1:
					if ((runner_posi.y - enemy_posi.y) < 250)
					{
						HP -= 20;
						_runnerRuan.setFace(FACE_DEPRESSED);
						_runnerRuan._runnerRoot->runAction(_gethurtSequence);
					}
					else
					{
						_runnerRuan.setFace(FACE_HAPPY);
						_runnerRuan._runnerRoot->runAction(_getscoreSequence);
						_score += 200;
						_scoreValue->setString(StringUtils::format("Score : %5.2f", _score));
					}
					break;
				case 0:
					if ((runner_posi.y - enemy_posi.y) < 100)
					{
						HP -= 5;
						_runnerRuan.setFace(FACE_DEPRESSED);
						_runnerRuan._runnerRoot->runAction(_gethurtSequence);
					}
					else
					{
						_runnerRuan.setFace(FACE_HAPPY);
						_runnerRuan._runnerRoot->runAction(_getscoreSequence);
						_score += 50;
						_scoreValue->setString(StringUtils::format("Score : %5.2f", _score));
					}
					break;
				case 2:
					if ((runner_posi.y - enemy_posi.y) < 100)
					{
						HP -= 10;
						_runnerRuan.setFace(FACE_DEPRESSED);
						_runnerRuan._runnerRoot->runAction(_gethurtSequence);
					}
					else
					{
						_runnerRuan.setFace(FACE_HAPPY);
						_runnerRuan._runnerRoot->runAction(_getscoreSequence);
						_score += 100;
						_scoreValue->setString(StringUtils::format("Score : %5.2f", _score));
					}
					break;
				}
				_enemytype = 99; // 不再扣血, 加分
			}
		}
//-------------------------------------------------------------------------------------

// 死亡設定 -------------------------------------------------------------------------------------
		if (HP <= 0)
		{
			HP = 0;
			_isstart = false;
			i_countdown = 0;
			_replaybtn.setStatus(CLICKABLE_OFF);
			_exitbtn.setStatus(CLICKABLE_ON);
			_jumpbtn.setStatus(CLICKABLE_OFF);
			_startbtn.setStatus(CLICKABLE_ON);
		}

		_hpBar->setPercent(HP);
		_hpBarValue->setString(StringUtils::format("HP = %5.2f", 1.0f * HP));
	}
}

void AnimationAction::GameStart()
{
	if (i_countdown == 0)
	{
		ScaleBy * scaleby = ScaleBy::create(0.25f, 2.0f);
		auto scalebyBack = scaleby->reverse();

		FadeOut * fadeout = FadeOut::create(0.5f);

		_countdownSequence = Sequence::create(scaleby, scalebyBack, fadeout, _mycallback, NULL);
		_countdownSequence->retain();

		_countdown[0]->setOpacity(255);
		_countdown[0]->runAction(_countdownSequence);
	}
	else if (i_countdown == 1)
	{
		_countdown[i_countdown]->setOpacity(255);
		_countdown[i_countdown]->runAction(_countdownSequence);
	}
	else if (i_countdown == 2)
	{
		_countdown[i_countdown]->setOpacity(255);
		_countdown[i_countdown]->runAction(_countdownSequence);
	}

	++i_countdown;
}

bool AnimationAction::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)//觸碰開始事件
{
	Point touchLoc = pTouch->getLocation();

	_replaybtn.onTouchBegan(pTouch, pEvent);
	_exitbtn.onTouchBegan(pTouch, pEvent);
	bool isjump = _jumpbtn.onTouchBegan(pTouch, pEvent);
	_startbtn.onTouchBegan(pTouch, pEvent);

	if (isjump)
	{
		if (_NoJumps == 0) {  // 第一次被按下
			_runnerRuan._runnerRoot->runAction(Sequence::create(_jumpAction, _mycallback, NULL));
			_NoJumps++;
		}
		else if (_NoJumps == 1) {  // 第二次被按下
			_NoJumps++;
			_runnerRuan._runnerRoot->stopAllActions();
			_runnerRuan._runnerRoot->runAction(Sequence::create(_myJump2, _mycallback, NULL));
		}
	}
	return true;
}

void  AnimationAction::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //觸碰移動事件
{
	Point touchLoc = pTouch->getLocation();

	_replaybtn.onTouchMoved(pTouch, pEvent);
	_exitbtn.onTouchMoved(pTouch, pEvent);
	_jumpbtn.onTouchMoved(pTouch, pEvent);
	_startbtn.onTouchMoved(pTouch, pEvent);
}

void  AnimationAction::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //觸碰結束事件 
{
	Point touchLoc = pTouch->getLocation();

	bool isreplay = _replaybtn.onTouchEnded(pTouch, pEvent);
	bool isexit = _exitbtn.onTouchEnded(pTouch, pEvent);
	_jumpbtn.onTouchEnded(pTouch, pEvent);
	bool isstart = _startbtn.onTouchEnded(pTouch, pEvent);

	if (isreplay)
	{
		_replaybtn.setStatus(CLICKABLE_ON);
		_exitbtn.setStatus(CLICKABLE_OFF);
		_jumpbtn.setStatus(CLICKABLE_ON);
		_startbtn.setStatus(CLICKABLE_OFF);

		_hpBar->setPercent(100);
		_hpBarValue->setString(StringUtils::format("HP = %5.2f", 100.0f));

		_score = 0;
		_scoreValue->setString(StringUtils::format("Score : %5.2f", 0.0f));
	}

	if (isexit)
	{
		unscheduleAllCallbacks();
		Director::getInstance()->end();
	}

	if (isstart)
	{
		GameStart();
	}
}