#ifndef __MYSOUND_H__
#define __MYSOUND_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
#define PLAY_EFFECT(A) SimpleAudioEngine::sharedEngine()->playEffect(A)

enum Sound_Type
{
	SOUND_MENU,
	SOUND_START,
	SOUND_SELECT_CHESS,
	SOUND_MOVEDONE_CHESS,
	SOUND_WIN,
	SOUND_LOSE,

	SOUND_COUNT
};

const char* const sound_effects[SOUND_COUNT] = {
	"sound_menu.wav",
	"sound_start.wav",
	"sound_select.wav",
	"sound_movedone.wav",
	"sound_win.mp3",
	"sound_lose.mp3",
};

class MySound
{
public:
	MySound() {}
public:
	static void loadSounds();

	static void playBackGround(int index);
	static void playSound(Sound_Type type);

	static void stopMusic();
	static void stopSound();
	static void pauseMusic();
	static void resumeMusic();
public:
	static int music_index;
};

#endif