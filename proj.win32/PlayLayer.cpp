#include "PlayLayer.h"
#include "SceneManager.h"
#include "StopLayer.h"
#include "MySound.h"
#include "Basic.h"
#include <math.h>
//#pragma comment(lib, "pthreadVC2.lib")  //必须加上这句

#include "cocos-ext.h"
USING_NS_CC_EXT;

bool PlayLayer::init()
{	
	if (!CCLayer::init())  
	{  
		return false;  
	}
    s = CCDirector::sharedDirector()->getWinSize();
	//init 

	//bg
	CCSprite* bg = CCSprite::create(PIC_BG);
	bg->setPosition(ccp(s.width*0.5, s.height*0.5));
	addChild(bg);

	//stop button...
	CCMenuItemImage* stopItem = CCMenuItemImage::create(
            PIC_STOPBUTTON,
            PIC_STOPBUTTON_SELECTED,
            this,
            menu_selector(PlayLayer::menuStopCallback));
	stopItem->setPosition(ccp(s.width - 54, s.height - 52));
	// Create a menu with the "close" menu item, it's an auto release object.
	m_stopMenu = CCMenu::create(stopItem, NULL);
	m_stopMenu->setPosition(CCPointZero);
	this->addChild(m_stopMenu, LAYER_MENU);

	m_dis = 300;
	initBalls();
	m_movable = true;
	m_auto = false;
	m_autoCount = 0;
	m_touchCount = 0;

	m_circle1 = CCSprite::create(PIC_CIRCLE1);
	m_circle1->setPosition(ccp(BASE_X, BASE_Y));
	m_circle1->setVisible(false);
	addChild(m_circle1, LAYER_CIRCLE);

	return true;
}

PlayLayer::~PlayLayer()
{

}

void PlayLayer::initBalls()
{
	//m_ballBatch = CCSpriteBatchNode::create(PIC_BALL1);
	m_ballBatch = CCNode::create();
	m_ballBatch->setAnchorPoint(ccp(0,0));
	m_ballBatch->setPosition(0,0);
	this->addChild(m_ballBatch, LAYER_BALL);
	
	m_allBalls.push_back(Ball(BALL_NORMAL, BASE_X, BASE_Y));
	m_allBalls.push_back(Ball(BALL_NORMAL, BASE_X, s.height*0.5+100));
	//m_aroundBalls.insert(&m_allBalls.back());
	int count = 2;
	while(count < 100)
	{
		int dis = 100+(rand() % BallR)*2;
		float angle = 30+rand()%150;
		int dx = dis * cos(angle*PI/180);
		int dy = dis * sin(angle*PI/180);

		int newx = m_allBalls.back().x + dx;
		int newy = m_allBalls.back().y + dy;
		if(newy > s.height-BallR)
			break;
		if(newx < BallR || newx > s.width-BallR)
			continue;
		Ball_Type type = (Ball_Type)(1 + rand() % 6);
		m_allBalls.push_back(Ball(type, newx, newy));
		count++;
	}

	for(std::list<Ball>::iterator iter=m_allBalls.begin();iter!=m_allBalls.end();++iter)
	{
		(*iter).sprite = CCSprite::create(s_pics[(*iter).type].c_str());
		(*iter).sprite->setPosition(ccp((*iter).x, (*iter).y));
		(*iter).sprite->setAnchorPoint(ccp(0.5,0.5));
		m_ballBatch->addChild((*iter).sprite);
	}
}

void PlayLayer::addNewBall()
{
	while(1)
	{
		int dis = 100+(rand() % BallR)*2;
		float angle = 30+rand()%150;
		int dx = dis * cos(angle*PI/180);
		int dy = dis * sin(angle*PI/180);

		int newx = m_allBalls.back().x + dx;
		int newy = m_allBalls.back().y + dy;
		if(newy > s.height-m_dp.y-BallR)
			break;
		if(newx < -m_dp.x - BallR || newx > s.width-m_dp.x-BallR)
			continue;
		Ball_Type type = (Ball_Type)(1 + rand() % 6);
		m_allBalls.push_back(Ball(type, newx, newy));
		m_allBalls.back().sprite = CCSprite::create(s_pics[m_allBalls.back().type].c_str());
		m_allBalls.back().sprite->setPosition(ccp(newx, newy));
		m_allBalls.back().sprite->setAnchorPoint(ccp(0.5,0.5));
		m_ballBatch->addChild(m_allBalls.back().sprite);
	}
}

void PlayLayer::circleAct()
{
	//create circle action...
	m_circle1->stopAllActions();
	m_circle1->setPosition(m_moveCenter);
	m_circle1->setVisible(true);
	m_circle1->setScale(1.0f);
	m_circle1->runAction(
		CCSpawn::createWithTwoActions(CCEaseExponentialOut::create(CCMoveTo::create(MOVE_TIME, realCenter)),
		/*CCEaseIn::create*/CCScaleBy::create(CIRCLE_TIME, 0.0f)));
}

void PlayLayer::stopLastMove()
{	
	for(std::list<Ball>::iterator iter=m_allBalls.begin();iter!=m_allBalls.end();++iter)
	{
		(*iter).sprite->stopAllActions();
		CCPoint pos = (*iter).sprite->getPosition();
		(*iter).x = pos.x;
		(*iter).y = pos.y;
	}
}

void PlayLayer::move()
{
	CCLog("move %f, %f", m_dp.x, m_dp.y);
	
	stopLastMove();

	if(!m_auto)
		circleAct();

	addNewBall();
	CCArray* children = m_ballBatch->getChildren();
	for(unsigned int i = 0; i < m_ballBatch->getChildrenCount(); i++)
	{
		CCSprite* ball = (CCSprite*)children->objectAtIndex(i);
		
		if(m_auto)
		{
			if(i == m_ballBatch->getChildrenCount()-1)
				ball->runAction(CCSequence::create(
				CCMoveBy::create(0.5f, m_dp),
				CCCallFunc::create(this, callfunc_selector(PlayLayer::moveEnd)),
				NULL));
			else
				ball->runAction(CCMoveBy::create(0.5f, m_dp));
		}
		else
		{
			if(i == m_ballBatch->getChildrenCount()-1)
				ball->runAction(CCSequence::create(
				CCEaseExponentialOut::create(CCMoveBy::create(MOVE_TIME, m_dp)),
				CCCallFunc::create(this, callfunc_selector(PlayLayer::moveEnd)),
				NULL));
			else
				ball->runAction(CCEaseExponentialOut::create(CCMoveBy::create(MOVE_TIME, m_dp)));
		}
	}

	//set all ball's x and y to new position...
	//setBallNewPosition();

	m_dis = DEFAULT_DIS;
	if(!m_auto)
		this->scheduleUpdate();

	//scheduleOnce(schedule_selector(PlayLayer::moveDone), 0.0f);
	//this->schedule(schedule_selector(PlayLayer::moving));
}

void PlayLayer::setTouchEnable(CCSprite* sprite, bool b)
{
	if(b)
	{
		sprite->setScale(1.2f);
	}
	else
	{
		sprite->setScale(1.0f);
	}
}

bool PlayLayer::getBallDisFromPoint(Ball& ball, float x, float y, float dis2)
{
	CCPoint p = ball.sprite->getPosition();
	return (p.x-x)*(p.x-x) + (p.y-y)*(p.y-y) <= dis2*dis2;
}

void PlayLayer::update(float t)
{
	//make circle distance small  and check if circle done...
	//float scale = m_circle1->getScale();
	//m_dis = scale * 300;
	m_dis -= DEFAULT_DIS / (CIRCLE_TIME*60);
	if(m_dis < BallR )
	{
		this->unscheduleUpdate();
	}
	
	//if in circle...
	for(std::list<Ball>::iterator iter=m_allBalls.begin();iter!=m_allBalls.end();++iter)
	{
		if(getBallDisFromPoint(*iter, BASE_X, BASE_Y, m_dis))
			setTouchEnable((*iter).sprite, true);
		else
			setTouchEnable((*iter).sprite, false);
	}

}

void PlayLayer::moveEnd()
{
	//get rid of balls which lower than bottom of screen...
	while(m_allBalls.front().y<-BallR)
	{
		m_allBalls.front().sprite->removeFromParentAndCleanup(true);
		m_allBalls.pop_front();
	}

	if(m_auto)
	{
		for(std::list<Ball>::iterator iter=m_allBalls.begin();iter!=m_allBalls.end();++iter)
		{
			CCPoint p = (*iter).sprite->getPosition();
			if(p.y <= BASE_Y)
				continue;
			if(getBallDisFromPoint(*iter, BASE_X, BASE_Y, DEFAULT_DIS))
			{
				m_circle1->setVisible(false);
				//move center
				m_moveCenter = p;
				//move delta
				m_dp = ccp(BASE_X-m_moveCenter.x, BASE_Y-m_moveCenter.y);
				CCLog("auto move %d,%d", m_moveCenter.x, m_moveCenter.y);
				m_autoCount++;
				if(m_autoCount>10)
				{
					m_auto = false;
					m_movable = true;
				}
				move();
				return;
			}
		}
		m_auto = false;
		m_movable = true;
	}
}

void PlayLayer::setBallNewPosition()
{
	for(std::list<Ball>::iterator iter=m_allBalls.begin();iter!=m_allBalls.end();++iter)
	{
		(*iter).x += m_dp.x;
		(*iter).y += m_dp.y;
	}
}

//---SpriteClassModel 按键-----------------
// 别忘记加入 TouchDispatcher  ,TouchDispatcher一般放在onenter 绑定，onexit 删除
bool PlayLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	if(!m_movable)
		return false;

	CCPoint touchPoint = touch->getLocation();
	if(touchPoint.y<BASE_Y-BallR)
		return false;

	for(std::list<Ball>::iterator iter=m_allBalls.begin();iter!=m_allBalls.end();++iter)
	{
		CCPoint p = (*iter).sprite->getPosition();
		if(p.y < BASE_Y)
			continue;
		if(p.y - BallR > touchPoint.y)
			break;
		if(p.y + BallR < touchPoint.y)
			continue;
		if(touchPoint.x > p.x-BallR && 
			touchPoint.x < p.x+BallR)
		{
			if(getBallDisFromPoint(*iter, BASE_X, BASE_Y, m_dis))
			{
				//stop ealier schedule...
				this->unscheduleUpdate();

				//move center...
				m_moveCenter = p;
				//move delta
				m_dp = ccp(BASE_X-m_moveCenter.x, BASE_Y-m_moveCenter.y);
				m_touchCount++;
				if(m_touchCount>2)
				{
					//auto fly...
					m_auto = true;
					m_touchCount = 0;
					m_autoCount = 0;
					m_movable = false;
				}

				//move...
				move();

				return true;
			}
		}
	}
	return false;
}

void PlayLayer::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
}

void PlayLayer::ccTouchEnded(CCTouch* touch, CCEvent* event)
{	
}

void PlayLayer::onEnter()
{   
	//JigsawSound::resumeMusic();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
	CCLayer::onEnter();
}

void PlayLayer::onExit()
{   
	//MySound::stopAll();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);  
	CCLayer::onExit();
}

void PlayLayer::menuStopCallback(CCObject* pSender)
{
	MySound::playSound(SOUND_MENU);
	StopLayer* stop = StopLayer::create();
	stop->initWithColor(ccc4(0,0,0,125));
	this->addChild(stop, 100);
	setStopVisible(false);
	//SceneManager::goStop(); 
}

void PlayLayer::setStopVisible(bool b)
{
	m_stopMenu->setVisible(b);
}
