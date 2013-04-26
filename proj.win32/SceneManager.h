#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "cocos2d.h"
USING_NS_CC;

class SceneManager : public CCObject
{
public:
	SceneManager(void);
	~SceneManager(void);
	
    static void initLoading();
    static void goMenu(int transType = 0);
	static void goPlay();
	static void goStop();
    static void go(CCLayer* layer, int transType = 0);
    static void push(CCLayer* layer);  
	static void pop();
    static CCScene* wrap(CCLayer* layer);      
	
    //声明静态旗标   
    static CCTransitionScene* createTransition(CCScene* s, int index = 0);  
    //用来返回 CCTransitionScene 对象，具体看.cpp中的实现
public:
	static bool music;
	static bool sound;
};

#endif