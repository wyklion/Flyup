#include "MainMenuLayer.h"
#include "SceneManager.h"
#include "MySound.h"
#include "Basic.h"

	
bool MainMenuLayer::init()
{
	if (!CCLayer::init())  
    {  
        return false;  
    }

    this->setTouchEnabled(true);//启用触摸
      
    CCSize s = CCDirector::sharedDirector()->getWinSize();

	//background...
	CCScale9Sprite* bg = CCScale9Sprite::create(PIC_ROUND);
	bg->setPreferredSize(CCSizeMake(600, 600));
	bg->setAnchorPoint(ccp(0.5,0.5));
	//bg->setColor(ccc3(50,30,50));
	bg->setPosition(ccp(s.width*0.5, s.height*0.5));
	addChild(bg);
	
	//music button...
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
	CCMenuItemToggle *music_toggle = CCMenuItemToggle::createWithTarget(this, menu_selector(MainMenuLayer::music), music1, music2, NULL);
	CCMenu *musicMenu = CCMenu::create(music_toggle,NULL);
	musicMenu->setPosition(ccp(s.width-54,s.height-52));
	this->addChild(musicMenu);

	/*CCMenuItemImage* item1 = CCMenuItemImage::create(PIC_BTN_2PLAYER1, PIC_BTN_2PLAYER2, this, menu_selector(MainMenuLayer::menuCallbackPlay) ); 
	CCMenuItemImage* item2 = CCMenuItemImage::create(PIC_BTN_COMPUTER1, PIC_BTN_COMPUTER2, this, menu_selector(MainMenuLayer::menuCallbackPlay2) ); 
	CCMenuItemImage* item3 = CCMenuItemImage::create(PIC_BTN_ONLINE1, PIC_BTN_ONLINE2, this, menu_selector(MainMenuLayer::menuCallbackPlayNet) ); 
	CCMenuItemImage* item4 = CCMenuItemImage::create(PIC_BTN_LEADBOARD1, PIC_BTN_LEADBOARD2, this, menu_selector(MainMenuLayer::menuCallbackRecording) ); */
		
    CCMenuItemFont *item1 = CCMenuItemFont::create("Play", this, menu_selector(MainMenuLayer::menuCallbackPlay) );  
	item1->setFontSizeObj(100);
	/*
    CCMenuItemFont *item2 = CCMenuItemFont::create("Vs Computer", this, menu_selector(MainMenuLayer::menuCallbackPlay2) );  
	item2->setFontSizeObj(100);
    CCMenuItemFont *item3 = CCMenuItemFont::create("Play online", this, menu_selector(MainMenuLayer::menuCallbackPlayNet) );  
    item3->setFontSizeObj(100);
    CCMenuItemFont *item4 = CCMenuItemFont::create("Recording", this, menu_selector(MainMenuLayer::menuCallbackRecording) );  
    item4->setFontSizeObj(100);*/


	//创建CCMenu菜单，其他可认为是菜单项   
    CCMenu* menu = CCMenu::create( item1,/* item2, item3, item4,*/ NULL);  
    menu->alignItemsVertically();//可以理解为设置成垂直排列   
    //menu->alignItemsHorizontally();//水平排列   

	addChild(menu);
    menu->setPosition(ccp(s.width/2, s.height/2));  
	
	loadImage();
	return true;

}

void MainMenuLayer::loadImage()
{
	for(int i = 0; i < s_pics_num; i++)
	{
		CCTextureCache::sharedTextureCache()->addImage(s_pics[i].c_str());
	}
}

void MainMenuLayer::menuCallbackPlay(CCObject* sender) 
{
	MySound::playSound(SOUND_MENU);
	
	SceneManager::goPlay();
}

void MainMenuLayer::music(CCObject *pSender)
{
	MySound::playSound(SOUND_MENU);
	if(SceneManager::music)
	{
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
}
