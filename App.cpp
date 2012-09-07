//====================================================
// App.cpp
//====================================================
#include "DragonFireSDK.h"
#include "App.h"
#include <set>
#include <map>

//=============== sets and maps =======================
//set that holds all containers
std::set<int> containerSet;

//map holding state container relationships where the state is the key and the relevant container is the value
std::map<STATE, int> stateToContainerMap;

//map holding touchzone enum and view relationships where the touchzone enum is the key and the relevant view is the value
std::map<SRTouchZone, int> SRTouchZoneToViewMap;

//map holding touchzone enum as the key and a boolen signifying true (timer turned on) or false (timer turned off) as the value
std::map<SRTouchZone, bool> SRTouchZoneToBoolMap;

//map holding touchzone enum as the key and a long signifying the time as the value 
std::map<SRTouchZone, double> SRTouchZoneToTimeMap;

//map holding touchzone enum as the key and the time text handle as the value 
std::map<SRTouchZone, int> SRTouchZoneToTimeTextMap;

//map holding touchzone enum as the key and the text above the time 
std::map<SRTouchZone, char *> SRTouchZoneToTimeLabelTextMap;

//=====================================================================

//====================containers
int cont_SR;
int cont_SRJungle;
int cont_Main;

//true if start game slide button is pressed down
bool startGameSlideButtonDown;

//true if in start game slider
bool startGameSliderDown;

//true if the game has started, false if not
bool gameStarted;
int gameSliderText;
double gameTime;

void AppMain()
{
	init();
	goToState(STATE_SR);
	gameStarted = false;
}

void init() {


	//lock screen to portrait mode
	AutoRotateInit(0, 0);

	


	//fill the SR Container
	ViewAdd(cont_SR, ImageAdd("Images/SR/StartGameSlider.png"), 0, 0);
	TouchAdd(cont_SR, 3, 3, 314, 38,  OnTouchSRStartGameSlider, SR_StartGameSlider);
	startGameSliderDown = false;
	
	gameSliderText = TextAdd(cont_SR, 50, 10, "Slide to Start Game", FontAdd("Arial", "Bold", 20, 0xFFFFFF));
	SRTouchZoneToViewMap[SR_StartGameSlideButton] = ViewAdd(cont_SR, "Images/SR/startGameSlideButton.png", 3, 3);
	startGameSlideButtonDown = false;
	SRTouchZoneToViewMap[SR_DragonPanel] = ViewAdd(cont_SR, ImageAdd("Images/SR/DragonPanel.png"), 0, 45, OnTouchSR, SR_DragonPanel);
	SRTouchZoneToBoolMap[SR_DragonPanel] = false;
	SRTouchZoneToTimeMap[SR_DragonPanel] = -1.0;
	SRTouchZoneToTimeTextMap[SR_DragonPanel] = TextAdd(cont_SR, 33, 50, "  Dragon", FontAdd("Arial", "Bold", 20, 0xFFFFFF));
	SRTouchZoneToTimeLabelTextMap[SR_DragonPanel] = "Dragon in\n     ";

	SRTouchZoneToViewMap[SR_BaronPanel] = ViewAdd(cont_SR, ImageAdd("Images/SR/BaronPanel.png"), 160, 45, OnTouchSR, SR_BaronPanel);
	SRTouchZoneToBoolMap[SR_BaronPanel] = false;
	SRTouchZoneToTimeMap[SR_BaronPanel] = -1.0;
	SRTouchZoneToTimeTextMap[SR_BaronPanel] = TextAdd(cont_SR, 193, 50, "   Baron", FontAdd("Arial", "Bold", 20, 0xFFFFFF));
	SRTouchZoneToTimeLabelTextMap[SR_BaronPanel] = " Baron in\n     ";

	ViewAdd(cont_SR, ImageAdd("Images/SR/YoursLabel.png"), 0, 105); 

	SRTouchZoneToViewMap[SR_YourBlueGolemPanel] = ViewAdd(cont_SR, ImageAdd("Images/SR/BlueGolemPanel.png"), 0, 130, OnTouchSR, SR_YourBlueGolemPanel);
	SRTouchZoneToBoolMap[SR_YourBlueGolemPanel] = false;
	SRTouchZoneToTimeMap[SR_YourBlueGolemPanel] = -1.0;
	SRTouchZoneToTimeTextMap[SR_YourBlueGolemPanel] = TextAdd(cont_SR, 15, 132, "  Blue Golem", FontAdd("Arial", "Bold", 20, 0xFFFFFF));
	SRTouchZoneToTimeLabelTextMap[SR_YourBlueGolemPanel] = "Blue Golem in\n         ";

	SRTouchZoneToViewMap[SR_YourRedGolemPanel] = ViewAdd(cont_SR, ImageAdd("Images/SR/RedGolemPanel.png"), 160, 130, OnTouchSR, SR_YourRedGolemPanel);
	SRTouchZoneToBoolMap[SR_YourRedGolemPanel] = false;
	SRTouchZoneToTimeMap[SR_YourRedGolemPanel] = -1.0;
	SRTouchZoneToTimeTextMap[SR_YourRedGolemPanel] = TextAdd(cont_SR, 179, 132, "  Red Lizard", FontAdd("Arial", "Bold", 20, 0xFFFFFF));
	SRTouchZoneToTimeLabelTextMap[SR_YourRedGolemPanel] = "Red Lizard in\n        ";


	//dim all of the panels
	dimPanels();

	//add all of the containers to the container set 
	containerSet.insert(cont_SR);
	containerSet.insert(cont_Main);
	containerSet.insert(cont_SRJungle);

	//fill the state to container map
	stateToContainerMap[STATE_Main] = cont_Main;
	stateToContainerMap[STATE_SR] = cont_SR;
	stateToContainerMap[STATE_SRJungle] = cont_SRJungle;

}


void dimPanels() {
	std::map<SRTouchZone, int>:: iterator iter;
	for (iter = SRTouchZoneToViewMap.begin(); iter != SRTouchZoneToViewMap.end(); iter++) {
		int view = iter->second;
		ViewSetAlpha(view, 30);
	}
	
}
void lightPanels() {
	std::map<SRTouchZone, int>:: iterator iter;
	for (iter = SRTouchZoneToViewMap.begin(); iter != SRTouchZoneToViewMap.end(); iter++) {
		int view = iter->second;
		ViewSetAlpha(view, 100);
	}
}



int OnTouchSRStartGameSlider(int id, int event, int x, int y) { //1 = down
	//if game has started this functionality should not work
	if (gameStarted) return 0;
	if (event == 1) {
		if (contains(SRTouchZoneToViewMap[SR_StartGameSlideButton], x, y)) {
			ViewSetxy(SRTouchZoneToViewMap[SR_StartGameSlideButton], 3, 3);
			startGameSlideButtonDown = true;
		}
	} else if (event == 2) {
		if (y >= 3 && y <= 41 && startGameSlideButtonDown) {
			//printf("in event 2");
			//draw the button moving with the finger
			ViewSetxy(SRTouchZoneToViewMap[SR_StartGameSlideButton], (x - 19), 3);
		} else {
			startGameSlideButtonDown = false;
			ViewSetxy(SRTouchZoneToViewMap[SR_StartGameSlideButton], 3, 3);
		}
	} else if (event == 3) {
		//printf("event 3============================================");
		if (contains(279, 3, 317, 41, x, y) && startGameSlideButtonDown) {
			//AlertShow("title", "it worked");
			ContainerSetVisible(cont_SR, 0);
			startGameSlideButtonDown = false;
			ViewSetxy(SRTouchZoneToViewMap[SR_StartGameSlideButton], 279, 3);
			gameStarted = true;
			lightPanels();
			gameTime = 0.0;
		} else {
			//AlertShow("title", "it didn't worked");
			//printf("ok then");
			startGameSlideButtonDown = false;
			ViewSetxy(SRTouchZoneToViewMap[SR_StartGameSlideButton], 3, 3);
		}
	}
	return (0);
}
	
//these touchareas refer only to those that are active when the game is running
int OnTouchSR(int id, int event, int x, int y) { //event: 1=down, 2=move, 3=up
	if (!gameStarted) return 0;

	if (event == 3) {
		switch (id) {
		case SR_StartGameSliderEnd:
			if (startGameSlideButtonDown && startGameSliderDown) {
				ContainerSetVisible(cont_SR, 0);
				startGameSlideButtonDown = false;
				startGameSliderDown = false;
				ViewSetxy(SRTouchZoneToViewMap[SR_StartGameSlideButton], 3, 3);
			}
		case SR_DragonPanel:
			if (SRTouchZoneToBoolMap[SR_DragonPanel]) {
				//if on already turn off
				ViewSetAlpha(SRTouchZoneToViewMap[SR_DragonPanel], 100);
				SRTouchZoneToBoolMap[SR_DragonPanel] = false;
				SRTouchZoneToTimeMap[SR_DragonPanel] = -1.0;
				TextSetText(SRTouchZoneToTimeTextMap[SR_DragonPanel], "  Dragon");
			} else {
				//if off already turn on
				ViewSetAlpha(SRTouchZoneToViewMap[SR_DragonPanel], 30);
				SRTouchZoneToBoolMap[SR_DragonPanel] = true;
				SRTouchZoneToTimeMap[SR_DragonPanel] = 100.0;
			}
			break;
		case SR_BaronPanel:
			if (SRTouchZoneToBoolMap[SR_BaronPanel]) {
				//if on already turn off
				ViewSetAlpha(SRTouchZoneToViewMap[SR_BaronPanel], 100);
				SRTouchZoneToBoolMap[SR_BaronPanel] = false;
				SRTouchZoneToTimeMap[SR_BaronPanel] = -1.0;
				TextSetText(SRTouchZoneToTimeTextMap[SR_BaronPanel], "   Baron");
			} else {
				//if off already turn on
				ViewSetAlpha(SRTouchZoneToViewMap[SR_BaronPanel], 30);
				SRTouchZoneToBoolMap[SR_BaronPanel] = true;
				SRTouchZoneToTimeMap[SR_BaronPanel] = 100.0;
			}
			break;
		case SR_YourBlueGolemPanel:
			if (SRTouchZoneToBoolMap[SR_YourBlueGolemPanel]) {
				//if on already turn off
				ViewSetAlpha(SRTouchZoneToViewMap[SR_YourBlueGolemPanel], 100);
				SRTouchZoneToBoolMap[SR_YourBlueGolemPanel] = false;
				SRTouchZoneToTimeMap[SR_YourBlueGolemPanel] = -1.0;
				TextSetText(SRTouchZoneToTimeTextMap[SR_YourBlueGolemPanel], "  Blue Golem");
			} else {
				//if off already turn on
				ViewSetAlpha(SRTouchZoneToViewMap[SR_YourBlueGolemPanel], 30);
				SRTouchZoneToBoolMap[SR_YourBlueGolemPanel] = true;
				SRTouchZoneToTimeMap[SR_YourBlueGolemPanel] = 100.0;
			}
			break;
		case SR_YourRedGolemPanel:
			if (SRTouchZoneToBoolMap[SR_YourRedGolemPanel]) {
				//if on already turn off
				ViewSetAlpha(SRTouchZoneToViewMap[SR_YourRedGolemPanel], 100);
				SRTouchZoneToBoolMap[SR_YourRedGolemPanel] = false;
				SRTouchZoneToTimeMap[SR_YourRedGolemPanel] = -1.0;
				TextSetText(SRTouchZoneToTimeTextMap[SR_YourRedGolemPanel], "  Red Lizard");
				
			} else {
				//if off already turn on
				ViewSetAlpha(SRTouchZoneToViewMap[SR_YourRedGolemPanel], 30);
				SRTouchZoneToBoolMap[SR_YourRedGolemPanel] = true;
				SRTouchZoneToTimeMap[SR_YourRedGolemPanel] = 110.0;
			}
			break;
		case SR_Philo:
		case SR_Kages:
		case SR_Avarice:
		case SR_HOG:
			break;
		}
	}
	return (0);

}



void OnTimer()
{
	switch (currentState) {
	case STATE_Main:
	case STATE_SR:
		updateTimers();
		break;
	case STATE_SRJungle:
		break;
	}
}



void updateTimers() {
	//only update timers when the game has started 
	if (gameStarted) {

		
		//update the main game timer
		gameTime += (1.0/30.0);
		int seconds = (int) gameTime + 0.5;
		char str[40];
		convertTime(str, seconds);
		TextSetText(gameSliderText, str);
		
		//update the panel timers
		std::map<enum SRTouchZone, double>::iterator iter;
		for (iter = SRTouchZoneToTimeMap.begin(); iter != SRTouchZoneToTimeMap.end(); iter++) {
			SRTouchZone key = iter->first;
			double time = iter->second;
			//comparing via less than is more accurate than comparing from equality between float values
			if (time > -0.5) {
				time -= (1.0/30.0);
				int roundedTime = (int) time + 0.5;
				char str[10];
				itoa(roundedTime, str, 10);

				char * first = SRTouchZoneToTimeLabelTextMap[key];
				char * final;
				final = (char *) malloc(strlen(first) + 10);
				strcpy(final, first);
				strcat(final, str);
				TextSetText(SRTouchZoneToTimeTextMap[key], final);

				//update the timer value in the map
				SRTouchZoneToTimeMap[key] = time;
			}
		}
	}
}


//this method sets the current state and sets the appropriate container to show up on the screen
void goToState(STATE state) {
	currentState = state;
	//first clear all of the containers
	clearContainers();
	//set the correct container
	ContainerSetVisible(stateToContainerMap[state], 1);
}

//makes all of the container invisible
void clearContainers() {
	std::set<int>::iterator setIter;
	for (setIter = containerSet.begin(); setIter != containerSet.end(); setIter++) {
		int container = *setIter;
		ContainerSetVisible(container, 0);
	}
}


void AppExit()
{
	// Application exit code goes here.  Perform any clean up your app might 
	// need to do in the event of interruption by a phone call or the user
	// pressing the Home button, or some other event that would cause your
	// application to terminate.
}