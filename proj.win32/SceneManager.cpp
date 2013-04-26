#include "SceneManager.h"
#include "MySound.h"
#include "MainMenuLayer.h"
#include "PlayLayer.h"
#include "StopLayer.h"

bool SceneManager::music = false;
bool SceneManager::sound = false;

//--------------Translation��Ķ���----------------------   
//���涨�������ֲ�ͬ��ת����   
class FadeWhiteTransition :public CCTransitionFade  
{  
public:  
	static CCTransitionFade* transitionWithDuration (float t ,CCScene* s)  
	{  
		return CCTransitionFade::create(t, s,ccBLACK);  
	}  
};  

class ZoomFlipXLeftOver : public CCTransitionZoomFlipX  
{  
public:  
	static CCTransitionZoomFlipX* transitionWithDuration(float t, CCScene* s)  
	{  
		return CCTransitionZoomFlipX::create(t, s, kCCTransitionOrientationLeftOver);  
	}  

};  

class FlipYDownOver :public CCTransitionZoomFlipY  
{  
public:  
	static CCTransitionZoomFlipY* transitionWithDuration(float t, CCScene* s)  
	{  
		return CCTransitionZoomFlipY::create(t, s, kCCTransitionOrientationDownOver);  
	}  
};

CCTransitionScene* SceneManager::createTransition(cocos2d::CCScene *s, int index)  
{  
	//�������ת����ȡ�࣬ȷ���������0-3֮�����   
	switch (index) {  
	case 0: return CCTransitionFade::create(1.0f, s);
	case 1: return CCTransitionSlideInR::create(0.8f, s);
	case 2: return CCTransitionSlideInL::create(0.8f, s);
		break;            
	default:  
		break;  
	}  
	//�ֱ𷵻������ֲ�ͬ��ת��   
	return NULL;       
}  

SceneManager::SceneManager(void)
{
}


SceneManager::~SceneManager(void)
{
}

void SceneManager::initLoading()
{
	//CCLayer* layer = InitLoading::create();
	//SceneManager::go(layer);
}

void SceneManager::goMenu(int transType)  
{
	if(music)
		MySound::playBackGround(1);
	CCLayer* layer = MainMenuLayer::create();  
	SceneManager::go(layer, transType);  
}

void SceneManager::goPlay()
{
	PlayLayer* layer = PlayLayer::create();
	//����һ��PlayLayer�Ľڵ�   
	SceneManager::go(layer);  
	//����go�����Ķ���...���layerֱ�ӱ����ӵ�Scene����  
} 

void SceneManager::goStop()
{
	CCLayerColor* layer = StopLayer::create();
	layer->initWithColor(ccc4(0, 0, 0, 125));
	SceneManager::push(layer);
}

void SceneManager::go(CCLayer* layer, int transType)  
{
	CCDirector* director = CCDirector::sharedDirector();
	CCScene* newScene = SceneManager::wrap(layer);
	if (director->getRunningScene()) //��仰 ���п���д�Ĳ��԰�  ����λ����֪����ôд ��֪��...   
	{
		CCTransitionScene* transScene = SceneManager::createTransition(newScene, transType);
		director->replaceScene(transScene);
	}
	else
	{
		director->runWithScene(newScene);
	}
}

CCScene* SceneManager::wrap(CCLayer* layer)  
{  
	CCScene* newScene = CCScene::create();  
	newScene->addChild(layer);  
	return newScene;
}  

void SceneManager::push(CCLayer* layer)
{
	CCDirector* director = CCDirector::sharedDirector();
	CCScene* newScene = SceneManager::wrap(layer);
	if (director->getRunningScene()) //��仰 ���п���д�Ĳ��԰�  ����λ����֪����ôд ��֪��...   
	{  
		director->pushScene( newScene);
	}
}

void SceneManager::pop()
{
	CCDirector* director = CCDirector::sharedDirector();  
	if (director->getRunningScene())
	{  
		director->popScene();
	}
}