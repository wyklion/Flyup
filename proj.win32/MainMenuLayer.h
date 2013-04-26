#ifndef __MAIN_MENU_LAYER_H__
#define __MAIN_MENU_LAYER_H__

#include "cocos2d.h"
USING_NS_CC;
#include "cocos-ext.h"
USING_NS_CC_EXT;


class MainMenuLayer : public CCLayer
{
public:
	virtual bool init();
	CREATE_FUNC(MainMenuLayer);

	void loadImage();

public:
	void menuCallbackPlay(CCObject* sender);
	void music(CCObject *pSender);
};

#endif