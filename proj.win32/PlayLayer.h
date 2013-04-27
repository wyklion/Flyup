#ifndef __PLAY_LAYER_H__
#define __PLAY_LAYER_H__   
#include "cocos2d.h" 
USING_NS_CC;

#include <set>
#include <list>

enum Ball_Type
{
	BALL_NORMAL = 1,
	BALL_RED,
	BALL_GREEN,
	BALL_YELLOW,
	BALL_BLUE,
	BALL_PURPLE,
};

#define PI 3.1416
#define BallR 40
#define BASE_X 320
#define BASE_Y 330
#define DEFAULT_DIS 300.0f
#define MOVE_TIME 3.0f
#define CIRCLE_TIME 4.0f
#define AUTO_SPEED 600.0f

const CCPoint realCenter = ccp(BASE_X, BASE_Y);

struct Ball
{
	Ball(Ball_Type type)
		:type(type), sprite(NULL)
	{
	}
	Ball_Type type;
	CCSprite* sprite;	
};

class PlayLayer : public CCLayer  
{  
public:
	~PlayLayer();
	virtual bool init(); //CCLayer�Ķ����Ƕ������������������virtual�ؼ���
	CREATE_FUNC(PlayLayer); //ǧ�����������������ֵĺ�

	//��д������غ���----
	virtual void onEnter();
	virtual void onExit();  
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);    
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);  
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
	
public:
	void back (CCObject* pSender);  
	void menuStopCallback(CCObject* pSender);
	void setStopVisible(bool b);
	void update(float t);

private:
	void createBall(Ball_Type BALL_NORMAL, int px, int py);
	void initBalls();
	void move();
	void moveEnd();
	void addNewBall();
	
	bool getBallDisFromPoint(Ball& ball, float x, float y, float dis2);
	void setTouchEnable(CCSprite* sprite, bool b);
	void circleAct();
	void stopLastMove();

private:
	CCMenu* m_stopMenu;
	CCSize s;
	//CCSpriteBatchNode* m_ballBatch;
	CCNode* m_ballBatch;
	std::list<Ball> m_allBalls;
	std::set<Ball*> m_aroundBalls;

	float m_dis;
	CCPoint m_dp;

	bool m_movable;

	bool m_auto;
	int m_autoCount;

	int m_touchCount;
	CCPoint m_moveCenter;

	CCSprite* m_circle1;

	Ball* m_touchBall;
	int m_height;
	CCLabelTTF* m_heightLabel;
	int m_calib;
	CCLabelTTF* m_calibration;

	Ball* m_lastTouch;

};

#endif

