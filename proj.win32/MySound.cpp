#include "MySound.h"
#include "SceneManager.h"

int MySound::music_index = -1;

void MySound::loadSounds()
{
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("background1.mp3");
	for(int i = 0; i < int(SOUND_COUNT); i++)
		SimpleAudioEngine::sharedEngine()->preloadEffect(sound_effects[i]);	
}

void MySound::playBackGround(int index)
{
	char buf[20];
	sprintf(buf, "background%d.mp3",index);
	if(index != music_index || !SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(buf, true);
	music_index = index;
}

void MySound::playSound(Sound_Type type)
{
	if(SceneManager::sound)
		SimpleAudioEngine::sharedEngine()->playEffect(sound_effects[type]);
}

void MySound::stopMusic()
{
	if(SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
		SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}

void MySound::stopSound()
{
	SimpleAudioEngine::sharedEngine()->stopAllEffects();
}

void MySound::pauseMusic()
{
	SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

void MySound::resumeMusic()
{
	SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
