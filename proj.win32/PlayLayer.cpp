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
	m_movable = true;
	m_auto = false;
	m_autoCount = 0;
	m_dp = ccp(0,0);
	m_moveCenter = ccp(BASE_X, BASE_Y);
	initBalls();

	//init cricle...
	m_circle1 = CCSprite::create(PIC_CIRCLE1);
	m_circle1->setPosition(ccp(BASE_X, BASE_Y));
	m_circle1->setVisible(false);
	addChild(m_circle1, LAYER_CIRCLE);

	//height label
	m_height = 0;
	m_heightLabel = CCLabelTTF::create("0m", "Arial", 30, CCSizeMake(150,30), kCCTextAlignmentRight);
	m_heightLabel->setPosition(ccp(80,50));
	addChild(m_heightLabel, LAYER_MENU);
	m_calib = 2000;
	m_calibration = CCLabelTTF::create("1000m", "Arial", 20, CCSizeMake(100,20), kCCTextAlignmentRight);
	m_calibration->setPosition(ccp(60,2000+BASE_Y));
	addChild(m_calibration, LAYER_MENU);

	//renders node...
	m_renders = CCNode::create();
	addChild(m_renders);
	m_pBrush = CCSprite::create(PIC_BRUSH);
	m_pBrush->retain();

	return true;
}

PlayLayer::~PlayLayer()
{
	m_pBrush->release();
}

Ball* PlayLayer::createBall(Ball_Type type, int px, int py)
{
	m_allBalls.push_back(Ball(type));
	m_allBalls.back().sprite = CCSprite::create(s_pics[type].c_str());
	m_allBalls.back().sprite->setPosition(ccp(px, py));
	m_allBalls.back().sprite->setAnchorPoint(ccp(0.5,0.5));
	m_ballBatch->addChild(m_allBalls.back().sprite);
	return &(m_allBalls.back());
}

void PlayLayer::initBalls()
{
	//m_ballBatch = CCSpriteBatchNode::create(PIC_BALL1);
	m_ballBatch = CCNode::create();
	m_ballBatch->setAnchorPoint(ccp(0,0));
	m_ballBatch->setPosition(0,0);
	this->addChild(m_ballBatch, LAYER_BALL);
	
	m_lastTouch = createBall(BALL_NORMAL, BASE_X, BASE_Y);
	m_linkBalls.push_back(m_lastTouch);
	createBall(BALL_NORMAL, BASE_X, BASE_Y+250);
	m_baseBalls.insert(&(m_allBalls.back()));
	addNewBall();
}

struct cmp    
{    
	bool operator () (const Ball& a,const Ball& b)    
	{    
		return a.sprite->getPosition().y < b.sprite->getPosition().y; 
	}  
};    

void getRandomUpBallLeft(const CCPoint& pos, int& newx, int& newy)
{
	int dis = 140+(rand() % BallR)*2;
	float angle = 110+rand()%40;
	int dx = dis * cos(angle*PI/180);
	int dy = dis * sin(angle*PI/180);
	newx = pos.x + dx;
	newy = pos.y + dy;
}

void getRandomUpBallRight(const CCPoint& pos, int& newx, int& newy)
{
	int dis = 140+(rand() % BallR)*2;
	float angle = 30+rand()%40;
	int dx = dis * cos(angle*PI/180);
	int dy = dis * sin(angle*PI/180);
	newx = pos.x + dx;
	newy = pos.y + dy;
}

void getRandomUpBall(const CCPoint& pos, int& newx, int& newy)
{
	int dis = 140+(rand() % BallR)*2;
	float angle = 30+rand()%120;
	int dx = dis * cos(angle*PI/180);
	int dy = dis * sin(angle*PI/180);
	newx = pos.x + dx;
	newy = pos.y + dy;
}

bool PlayLayer::checkPosUpBall(Ball* ball, bool& left, bool& right)
{
	CCPoint ballPos = ball->sprite->getPosition();
	for(std::list<Ball>::iterator iter=m_allBalls.begin();iter!=m_allBalls.end();++iter)
	{
		if(ball == &(*iter))
			continue;
		
		CCPoint pos = (*iter).sprite->getPosition();
		if(pos.y < ballPos.y || pos.y > ballPos.y+200)
			continue;
		if( (ballPos.y-pos.y)*(ballPos.y-pos.y) + (ballPos.x-pos.x)*(ballPos.x-pos.x) < 48400)
		{
			if(pos.x < ballPos.x)
				left = true;
			else
				right = true;
		}
	}
	return left||right;
}

bool PlayLayer::checkAround(int newx, int newy)
{
	for(std::list<Ball>::iterator iter=m_allBalls.begin();iter!=m_allBalls.end();++iter)
	{
		if(getBallDisFromPoint(*iter, newx, newy, 100.0))
			return true;
	}
	return false;
}

Ball_Type PlayLayer::getNextType()
{
	if(rand()%10 == 1)
	{
		return (Ball_Type)(2 + rand() % 5);
	}
	else
		return BALL_NORMAL;
}

Ball* PlayLayer::createUpLeftBall(const CCPoint& pos, std::vector<Ball*>& newBalls)
{
	int newx, newy;
	//left up one...
	if(pos.x > -m_dp.x+100)
	{
		bool overlap = false;
		while(1)
		{
			getRandomUpBallLeft(pos, newx, newy);
			CCLog("left x,y: %d,%d", newx, newy);
			if(newx < -m_dp.x - BallR-300)
			{
				CCLog("x too left...");
				continue;
			}
			if(checkAround(newx, newy))
			{
				CCLog("overlap...");
				continue;
				//overlap = true;
			}
			break;
		}
		if(!overlap)
		{
			CCLog("create ball %d, %d", newx, newy);
			Ball_Type type = getNextType();
			Ball* ball = createBall(type, newx, newy);
			newBalls.push_back(ball);
			return ball;
		}
	}
	return NULL;
}

Ball* PlayLayer::createUpRightBall(const CCPoint& pos, std::vector<Ball*>& newBalls)
{
	int newx, newy;
	//right up one...
	if(pos.x < s.width-m_dp.x-100)
	{
		bool overlap = false;
		while(1)
		{
			getRandomUpBallRight(pos, newx, newy);
			CCLog("right x,y: %d,%d", newx, newy);
			if(newx > s.width-m_dp.x-BallR+300)
			{
				CCLog("x too right...");
				continue;
			}
			if(checkAround(newx, newy))
			{
				CCLog("overlap...");
				continue;
				//overlap = true;
			}
			break;
		}
		if(!overlap)
		{
			CCLog("create ball %d, %d", newx, newy);
			Ball_Type type = getNextType();
			Ball* ball = createBall(type, newx, newy);
			newBalls.push_back(ball);
			return ball;
		}
	}
	return NULL;
}

Ball* PlayLayer::createUpBall(const CCPoint& pos, std::vector<Ball*>& newBalls)
{
	int newx, newy;
	bool overlap = false;
	while(1)
	{
		getRandomUpBall(pos, newx, newy);
		CCLog("up x,y: %d,%d", newx, newy);
		if(newx < -m_dp.x - BallR-300 || newx > s.width-m_dp.x-BallR+300)
		{
			CCLog("too left or right...");
			continue;
		}
		if(checkAround(newx, newy))
		{
		CCLog("overlap...");
		//continue;
		overlap = true;
		}
		break;
	}
	if(!overlap)
	{
		Ball_Type type = getNextType();
		Ball* ball = createBall(type, newx, newy);
		newBalls.push_back(ball);
		return ball;
	}
	return NULL;
}

void PlayLayer::addNewBall(int type)
{
	if(m_allBalls.back().sprite->getPosition().y>s.height+400)
		return;
	CCLog("================add start=================");
	bool enough = false;
	std::vector<Ball*> overUpBalls;
	while(!enough)
	{
		std::vector<Ball*> newBalls;
		for(std::set<Ball*>::iterator iter = m_baseBalls.begin(); iter != m_baseBalls.end(); iter++)
		{
			CCPoint pos = (*iter)->sprite->getPosition();			
			CCLog("For base x,y: %f,%f", pos.x, pos.y);
			bool left = false;
			bool right = false;
			bool up = checkPosUpBall(*iter, left, right);

			if(pos.x < -m_dp.x - BallR-300 || pos.x > s.width-m_dp.x-BallR+300)
			{
				CCLog("out of x...");
				continue;
			}

			if(type == 0)
			{
				if(!up)
					createUpBall(pos, newBalls);
			}
			else if(type == 1)
			{
				if(!left)
					createUpLeftBall(pos, newBalls);
				if(!right)
					createUpRightBall(pos, newBalls);
			}
		}
		if(newBalls.size() == 0)
		{
			break;
		}
		else
		{
			m_baseBalls.clear();
			enough = true;
			for(size_t i = 0; i < newBalls.size(); i++)
			{
				if(newBalls[i]->sprite->getPosition().y<s.height-m_dp.y+BallR)
				{
					m_baseBalls.insert(newBalls[i]);
					enough = false;
				}
				else
				{
					overUpBalls.push_back(newBalls[i]);
				}
			}
		}
	}
	for(size_t i = 0; i < overUpBalls.size(); i++)
	{
		m_baseBalls.insert(overUpBalls[i]);
	}
	m_allBalls.sort(cmp());
	CCLog("================add end=================");
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
	}
	m_calibration->stopAllActions();
}

void PlayLayer::move()
{
	//CCLog("move %f, %f", m_dp.x, m_dp.y);
	
	this->unscheduleUpdate();
	stopLastMove();
	
	int lastY = m_lastTouch->sprite->getPosition().y;

	m_lastTouch = m_touchBall;

	//create link...
	if(m_linkBalls.back()->sprite != m_touchBall->sprite)
	{
		m_linkBalls.push_back(m_touchBall);
		createLink();
	}
	
	while(1)
	{
		CCPoint pos = m_linkBalls.front()->sprite->getPosition();
		if(pos.y < -BallR)
		{
			if(m_linkBalls.front()->render!=NULL)
			{
				m_linkBalls.front()->render->removeFromParentAndCleanup(true);
				m_linkBalls.front()->render = NULL;
			}
			m_linkBalls.pop_front();
		}
		else
			break;
	}
	//get rid of balls which lower than bottom of screen...
	while(1)
	{
		CCPoint pos = m_allBalls.front().sprite->getPosition();
		if(pos.y < -BallR)
		{
			m_allBalls.front().sprite->removeFromParentAndCleanup(true);
			m_allBalls.pop_front();
		}
		else
			break;
	}
	
	//reset circle radius...
	m_dis = DEFAULT_DIS;
	//move center
	m_moveCenter = m_touchBall->sprite->getPosition();
	//move delta
	m_dp = ccp(BASE_X-m_moveCenter.x, BASE_Y-m_moveCenter.y);

	if(m_touchBall->type == BALL_GREEN)
	{
		m_auto = true;
		m_autoCount = 0;
		m_movable = false;
	}
	if(m_auto)
	{
		m_circle1->setVisible(false);
		m_autoCount++;
		if(m_autoCount>10)
		{
			m_auto = false;
			m_movable = true;
			circleAct();
		}
	}
	else
	{
		circleAct();
	}
	

	//update height label...
	m_height += (m_moveCenter.y - lastY);
	char buf[20];
	sprintf(buf, "%d um", m_height/2);
	m_heightLabel->setString(buf);


	//add new balls...
	addNewBall(1);

	float t = m_auto ? sqrt(m_dp.x*m_dp.x+m_dp.y*m_dp.y) / AUTO_SPEED : MOVE_TIME;
	//CCLog("t:%f",t);

	//move calibration...
	if(m_height-500 > m_calib)
	{
		m_calib+=2000;
		sprintf(buf, "%d um", m_calib/2);
		m_calibration->setString(buf);
		m_calibration->setPosition(ccp(60,m_calib-m_height+m_moveCenter.y));
		//CCLog("calib, pos: %d, %d", m_calib, m_calib-m_height+m_moveCenter.y);
	}
	if(m_auto)
		m_calibration->runAction(CCMoveBy::create(t, ccp(0, m_dp.y)));
	else
		m_calibration->runAction(CCEaseExponentialOut::create(CCMoveBy::create(t, ccp(0, m_dp.y))));
	
	CCArray* children = m_ballBatch->getChildren();
	//CCLog("children %d", m_ballBatch->getChildrenCount());
	for(unsigned int i = 0; i < m_ballBatch->getChildrenCount(); i++)
	{
		CCSprite* ball = (CCSprite*)children->objectAtIndex(i);
		if(m_auto)
		{
			if(i == m_ballBatch->getChildrenCount()-1)
				ball->runAction(CCSequence::create(
				CCMoveBy::create(t, m_dp),
				CCCallFunc::create(this, callfunc_selector(PlayLayer::moveEnd)),
				NULL));
			else
				ball->runAction(CCMoveBy::create(t, m_dp));
		}
		else
		{
			if(i == m_ballBatch->getChildrenCount()-1)
				ball->runAction(CCSequence::create(
				CCEaseExponentialOut::create(CCMoveBy::create(t, m_dp)),
				CCCallFunc::create(this, callfunc_selector(PlayLayer::moveEnd)),
				NULL));
			else
				ball->runAction(CCEaseExponentialOut::create(CCMoveBy::create(t, m_dp)));
		}
	}
	CCArray* rchildren = m_renders->getChildren();
	for(unsigned int i = 0; i < m_renders->getChildrenCount(); i++)
	{
		CCRenderTexture* render = (CCRenderTexture*)rchildren->objectAtIndex(i);
		if(m_auto)
		{
			render->runAction(CCMoveBy::create(t, m_dp));
		}
		else
		{
			render->runAction(CCEaseExponentialOut::create(CCMoveBy::create(t, m_dp)));
		}
	}

	//set all ball's x and y to new position...
	//setBallNewPosition();

	this->scheduleUpdate();

	//scheduleOnce(schedule_selector(PlayLayer::moveDone), 0.0f);
	//this->schedule(schedule_selector(PlayLayer::moving));
}

void PlayLayer::createLink()
{
	if(m_linkBalls.size() < 2)
		return;

	std::list<Ball*>::reverse_iterator it = m_linkBalls.rbegin();
	Ball* ball2 = *it;
	it++;
	Ball* ball1 = *it;

	ball2->front = ball1->sprite->getPosition();
	ball2->now = ball2->sprite->getPosition();
	ball2->renderPoint = ball2->front;
	ball2->render = CCRenderTexture::create(s.width, s.height, kCCTexture2DPixelFormat_RGBA8888);
	ball2->render->setPosition(ccp(s.width*0.5, s.height*0.5));
	m_renders->addChild(ball2->render);
	float dist = ccpDistance(ball2->front, ball2->now);
	int repeat = dist/5;
    this->schedule(schedule_selector(PlayLayer::linking), 0.01f,repeat,0);
}

void PlayLayer::linking(float t)
{
	Ball* ball = m_linkBalls.back();
	float dist = ccpDistance(ball->front, ball->now);
	int repeat = dist/5;

	ball->render->begin();
	ball->renderPoint = ccp(ball->renderPoint.x + (ball->now.x-ball->front.x)/repeat,
							ball->renderPoint.y + (ball->now.y-ball->front.y)/repeat);
    m_pBrush->setPosition(ball->renderPoint);
    m_pBrush->setColor(ccc3(rand() % 127 + 128, 255, 255));
    // Call visit to draw the brush, don't call draw..
    m_pBrush->visit();
	ball->render->end();
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
	if(!m_auto)
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
		CCPoint movePos = m_touchBall->sprite->getPosition();
		for(std::list<Ball>::iterator iter=m_allBalls.begin();iter!=m_allBalls.end();++iter)
		{
			if(getBallDisFromPoint(*iter, movePos.x, movePos.y, m_dis))
				setTouchEnable((*iter).sprite, true);
			else
				setTouchEnable((*iter).sprite, false);
		}
	}

}

void PlayLayer::moveEnd()
{
	if(m_auto)
	{
		std::vector<Ball*> upBalls;
		for(std::list<Ball>::iterator iter=m_allBalls.begin();iter!=m_allBalls.end();++iter)
		{
			CCPoint p = (*iter).sprite->getPosition();
			if(p.y <= BASE_Y)
				continue;
			if(getBallDisFromPoint(*iter, BASE_X, BASE_Y, DEFAULT_DIS))
			{
				upBalls.push_back(&*iter);
			}
		}
		size_t num = upBalls.size();
		if(num>0)
		{
			m_touchBall = upBalls[(rand()%num)];
			move();
		}
		else
		{
			m_auto = false;
			m_movable = true;
		}
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
				m_touchBall = &*iter;
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
