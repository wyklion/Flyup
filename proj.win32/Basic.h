#ifndef __BASIC_H__
#define __BASIC_H__   

#define CCSetString CCUserDefault::sharedUserDefault()->setStringForKey  
  
#define CCSetInt CCUserDefault::sharedUserDefault()->setIntegerForKey  
  
#define CCSetBool CCUserDefault::sharedUserDefault()->setBoolForKey  
  
#define CCGetString CCUserDefault::sharedUserDefault()->getStringForKey  
  
#define CCGetInt CCUserDefault::sharedUserDefault()->getIntegerForKey  
  
#define CCGetBool CCUserDefault::sharedUserDefault()->getBoolForKey  

#define LAYER_BALL 20
#define LAYER_CIRCLE 10
#define LAYER_MENU 30

#define PIC_CIRCLE1 "circle1.png"

#define PIC_BG "bg.png"
#define PIC_BALL_NORMAL "ball_normal.png"
#define PIC_BALL_RED "ball_red.png"
#define PIC_BALL_GREEN "ball_green.png"
#define PIC_BALL_YELLOW "ball_yellow.png"
#define PIC_BALL_BLUE "ball_blue.png"
#define PIC_BALL_PURPLE "ball_purple.png"
#define PIC_BRUSH "brush.png"

#define PIC_ROUND "round.png"
#define PIC_BUTTON_BACK "backButton.png"
#define PIC_BUTTON_BACK_SELECTED "backButtonSelected.png"
#define PIC_STOPBUTTON "stopButton.png"
#define PIC_STOPBUTTON_SELECTED "stopButtonSelected.png"
#define PIC_MUSIC_ON "buttonMusic1.png"
#define PIC_MUSIC_OFF "buttonMusic2.png"
#define PIC_RESUME "resumeButton.png"
#define PIC_RESUME_SELECTED "resumeButtonSelected.png"
#define PIC_QUIT "quitButton.png"
#define PIC_QUIT_SELECTED "quitButtonSelected.png"

const int s_pics_num = 20;
const std::string s_pics[] = {
	PIC_BG,
	PIC_BALL_NORMAL,
	PIC_BALL_RED,
	PIC_BALL_GREEN,
	PIC_BALL_YELLOW,
	PIC_BALL_BLUE,
	PIC_BALL_PURPLE,
	PIC_BRUSH,

	PIC_ROUND,
	PIC_BUTTON_BACK,
	PIC_BUTTON_BACK_SELECTED,
	PIC_STOPBUTTON,
	PIC_STOPBUTTON_SELECTED,
	PIC_MUSIC_ON,
	PIC_MUSIC_OFF,
	PIC_RESUME,
	PIC_RESUME_SELECTED,
	PIC_QUIT,
	PIC_QUIT_SELECTED,

	PIC_CIRCLE1,
};

enum GAME_STATE
{
	GAME_STATE_NORMAL,
	GAME_STATE_WIN,
	GAME_STATE_LOSE,
	GAME_STATE_PROP,
	GAME_STATE_STOP,
	GAME_STATE_FOUND,
	GAME_STATE_FIRSTPLAY,
	GAME_STATE_NEWTHING,
};

struct GameProcess
{
	bool win;
	bool lose;
	GAME_STATE state;
};


#endif