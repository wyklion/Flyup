#include "StopLayer.h"
#include "PlayLayer.h"
#include "SceneManager.h"
#include "Basic.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
#include "MySound.h"

bool StopLayer::init()
{
	if (!CCLayerColor::init())
	{  
		return false;  
	}  
	
    this->setTouchEnabled(true);  
    //启用触摸   

	// elastic effect
    CCSize s = CCDirector::sharedDirector()->getWinSize();
	
	//CCSprite* bg = CCSprite::create("stopBackground.png");  
    //bg->setPosition(ccp(s.width/2, s.height/2));
    //this->addChild(bg,0);

	/*
	CCScale9Sprite* backGround = CCScale9Sprite::create(PIC_BUTTON);
	backGround->setContentSize(CCSizeMake(400,500));
	backGround->setColor(ccc3(50,205,50));
	backGround->setPosition(ccp(s.width*0.5, s.height*0.5));
	addChild(backGround);*/

	/*
    CCScale9Sprite* btnNormal = CCScale9Sprite::create(PIC_BUTTON);
    CCScale9Sprite* btnDown = CCScale9Sprite::create(PIC_BUTTONHILIT);
	btnNormal->setColor(ccc3(255,0,0));
	btnDown->setColor(ccc3(255,0,0));
    CCLabelTTF* title = CCLabelTTF::create("Resume", "Marker Felt", 30);
    CCControlButton* controlBtn = CCControlButton::create(title, btnNormal);
    controlBtn->setBackgroundSpriteForState(btnDown, CCControlStateSelected);
    controlBtn->setPosition(ccp(s.width*0.5, s.height*0.5+200));
    this->addChild(controlBtn);*/

	CCSprite* musicOn = CCSprite::create(PIC_MUSIC_ON);
	CCSprite* musicOff = CCSprite::create(PIC_MUSIC_OFF);
	CCMenuItemSprite* music1 = CCMenuItemSprite::create(musicOn, musicOn);
	CCMenuItemSprite* music2 = CCMenuItemSprite::create(musicOff, musicOff);
	if(!SceneManager::music)
	{
		CCMenuItemSprite* tmp = music1;
		music1 = music2;
		music2 = tmp;
	}
	CCMenuItemToggle* music_toggle = CCMenuItemToggle::createWithTarget(this, menu_selector(StopLayer::music), music1, music2, NULL);
	/*
	CCSprite* soundOn = CCSprite::create(PIC_SOUND_ON);
	CCSprite* soundOff = CCSprite::create(PIC_SOUND_OFF);
	CCMenuItemSprite* sound1 = CCMenuItemSprite::create(soundOn, soundOn);
	CCMenuItemSprite* sound2 = CCMenuItemSprite::create(soundOff, soundOff);
	if(!SceneManager::sound)
	{
		CCMenuItemSprite* tmp = sound1;
		sound1 = sound2;
		sound2 = tmp;
	}
	CCMenuItemToggle* sound_toggle = CCMenuItemToggle::createWithTarget(this, menu_selector(StopLayer::sound), sound1, sound2, NULL);
	*/
	CCMenu *musicMenu = CCMenu::create(music_toggle,/*sound_toggle,*/NULL);
	musicMenu->setPosition(ccp(s.width/2,s.height/2+200));
	musicMenu->alignItemsHorizontally();
	this->addChild(musicMenu);
	
	CCMenuItemImage* resume = CCMenuItemImage::create(
            PIC_RESUME,
            PIC_RESUME_SELECTED,
            this,
            menu_selector(StopLayer::resume));
	
	CCMenuItemImage* quit = CCMenuItemImage::create(
            PIC_QUIT,
            PIC_QUIT_SELECTED,
            this,
            menu_selector(StopLayer::quit));

	/*CCMenuItemFont* resume = CCMenuItemFont::create("Resume",this, menu_selector(StopLayer::resume));
	resume->setFontSize(100);
	CCMenuItemFont* quit = CCMenuItemFont::create("Quit",this, menu_selector(StopLayer::quit));
	quit->setFontSize(100);*/
	//老问题 别忘记类名 还有这个只需要函数名就够了  
	m_menu = CCMenu::create(resume, quit, NULL);   
	//Items！ 单复数 要注意
	m_menu->setPosition(ccp(s.width*0.5, s.height*0.5));
	//m_menu->alignItemsVertically();
	m_menu->alignItemsHorizontally();
	m_menu->setContentSize(CCSizeMake(500,500));
	this->addChild(m_menu);

	/*
	CCParticleSystemQuad *system = CCParticleSystemQuad::create("mergeEffect.plist");
	system->setPosition(100,100);
	system->setTextureWithRect(CCTextureCache::sharedTextureCache()->addImage("particale.png"), CCRectMake(0,0,32,32));
	addChild(system, 10);
*/
	return true;
}

void StopLayer::onEnter()
{
    CCLayerColor::onEnter();
	((PlayLayer*)this->getParent())->setStopVisible(false);
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

void StopLayer::onExit()
{
    CCLayerColor::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool StopLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_touchedMenu = m_menu->ccTouchBegan(pTouch, pEvent);
    return true;
}

void StopLayer::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (m_touchedMenu) 
	{
        m_menu->ccTouchMoved(pTouch, pEvent);
    }
}

void StopLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (m_touchedMenu)
	{
        m_menu->ccTouchEnded(pTouch, pEvent);
        m_touchedMenu = false;
    }
}

void StopLayer::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (m_touchedMenu) 
	{
        m_menu->ccTouchEnded(pTouch, pEvent);
        m_touchedMenu = false;
    }
}

void StopLayer::resume(cocos2d::CCObject *pSender)
{  
	MySound::playSound(SOUND_MENU);
	((PlayLayer*)this->getParent())->setStopVisible(true);
	this->removeFromParentAndCleanup(false);
	//MySound::resumeMusic();
	//SceneManager::pop();
}

void StopLayer::quit(cocos2d::CCObject *pSender)  
{  
	//SceneManager::pop();
	//this->removeFromParentAndCleanup(false);
	MySound::playSound(SOUND_MENU);
	//MySound::stopMusic();
	SceneManager::goMenu();  
	//back就是回主菜单，这里调用了人见人爱 花见花开的静态函数 goMenu  
}

void StopLayer::music(CCObject *pSender)
{
	if(SceneManager::music)
	{
		MySound::playSound(SOUND_MENU);
		SceneManager::music = false;
		SceneManager::sound = false;
		MySound::stopMusic();
	}
	else
	{
		SceneManager::music = true;
		SceneManager::sound = true;
		MySound::playBackGround(1);
	}
	/*CCMenuItemToggle* toggle = (CCMenuItemToggle*)pSender;
	if(toggle->getSelectedIndex() == 0)
		CCLog("toggle 0000...");
	else if(toggle->getSelectedIndex() == 1)
		CCLog("toggle 1111...");*/
}

void StopLayer::sound(CCObject *pSender)
{
	if(SceneManager::sound)
	{
		MySound::playSound(SOUND_MENU);
		SceneManager::sound = false;
	}
	else
	{
		SceneManager::sound = true;
	}
}