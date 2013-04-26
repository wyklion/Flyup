#ifndef __STOP_LAYER_H__
#define __STOP_LAYER_H__   
#include "cocos2d.h" 
using namespace cocos2d; 


class StopLayer : public CCLayerColor
{
public:	
	virtual bool init(); //CCLayer的儿子们都有这玩意儿。别忘了virtual关键字   

	CREATE_FUNC(StopLayer);   
	//千万别忘了这个风情万种的宏
	
    void onEnter();
    void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

public:
	void resume(CCObject* pSender);
	void retry(CCObject* pSender);
	void quit(CCObject* pSender);
	void music(CCObject *pSender);
	void sound(CCObject *pSender);

private:
    // 模态对话框菜单
    CCMenu* m_menu;
    // 记录菜单点击
    bool m_touchedMenu;
};

#endif