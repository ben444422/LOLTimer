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

//map holding touchzone enum as the key and the appropriate text above the time WHEN THE TIMER IS ON
std::map<SRTouchZone, char *> SRTouchZoneToTimeLabelTextMap;

//map holding the touchzone enum as the key and the appropriate label text as the key WHEN THE TIMER IS OFF
std::map<SRTouchZone, char*> SRTouchZoneToDefaultTimeLabelTextMap;

//map holding the touchzone enum as the key and the appropriate label time as the key WHEN THE TIMER IS ON
std::map<SRTouchZone, int> SRTouchZoneToTimeTextMapTimerOn;
//=====================================================================

//====================containers
int cont_SR;
int cont_SRJungle;
int cont_Main;

//true if start game slide button is pressed down
bool startGameSlideButtonDown;

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


	//start game slider properties
	ViewAdd(cont_SR, ImageAdd("Images/SR/StartGameSlider.png"), 0, 0);
	TouchAdd(cont_SR, 3, 3, 314, 38,  OnTouchSRStartGameSlider, SR_StartGameSlider);
	
	gameSliderText = TextAdd(cont_SR, 50, 4, "Slide to Start Game", FontAdd("Arial", "Bold", 17, 0xFFFFFF));
	SRTouchZoneToViewMap[SR_StartGameSlideButton] = ViewAdd(cont_SR, "Images/SR/startGameSlideButton.png", 3, 3);
	startGameSlideButtonDown = false;

	//timer properties
	SRTouchZoneToViewMap[SR_DragonPanel] = ViewAdd(cont_SR, ImageAdd("Images/SR/DragonPanel.png"), 0, 45, OnTouchSR, SR_DragonPanel);
	SRTouchZoneToBoolMap[SR_DragonPanel] = false;
	SRTouchZoneToTimeMap[SR_DragonPanel] = -1.0;
	SRTouchZoneToTimeTextMap[SR_DragonPanel] = TextAdd(cont_SR, 33, 50, "  Dragon", FontAdd("Arial", "Bold", 20, 0xFFFFFF));
	SRTouchZoneToTimeLabelTextMap[SR_DragonPanel] = "Dragon in";
	SRTouchZoneToDefaultTimeLabelTextMap[SR_DragonPanel] = "  Dragon";
	SRTouchZoneToTimeTextMapTimerOn[SR_DragonPanel] = TextAdd(cont_SR, 33, 70, "      ", FontAdd("Arial", "Bold", 20, 0xFFFFFF));

	SRTouchZoneToViewMap[SR_BaronPanel] = ViewAdd(cont_SR, ImageAdd("Images/SR/BaronPanel.png"), 160, 45, OnTouchSR, SR_BaronPanel);
	SRTouchZoneToBoolMap[SR_BaronPanel] = false;
	SRTouchZoneToTimeMap[SR_BaronPanel] = -1.0;
	SRTouchZoneToTimeTextMap[SR_BaronPanel] = TextAdd(cont_SR, 193, 50, "   Baron", FontAdd("Arial", "Bold", 20, 0xFFFFFF));
	SRTouchZoneToTimeLabelTextMap[SR_BaronPanel] = " Baron in";
	SRTouchZoneToDefaultTimeLabelTextMap[SR_BaronPanel] = "   Baron";
	SRTouchZoneToTimeTextMapTimerOn[SR_BaronPanel] = TextAdd(cont_SR, 193, 70, " ", FontAdd("Arial", "Bold", 20, 0xFFFFFF));

	//add the "Yours" Label
	ViewAdd(cont_SR, ImageAdd("Images/SR/YoursLabel.png"), 0, 105); 

	SRTouchZoneToViewMap[SR_YourBlueGolemPanel] = ViewAdd(cont_SR, ImageAdd("Images/SR/BlueGolemPanel.png"), 0, 130, OnTouchSR, SR_YourBlueGolemPanel);
	SRTouchZoneToBoolMap[SR_YourBlueGolemPanel] = false;
	SRTouchZoneToTimeMap[SR_YourBlueGolemPanel] = -1.0;
	SRTouchZoneToTimeTextMap[SR_YourBlueGolemPanel] = TextAdd(cont_SR, 15, 132, "  Blue Golem", FontAdd("Arial", "Bold", 20, 0xFFFFFF));
	SRTouchZoneToTimeLabelTextMap[SR_YourBlueGolemPanel] = "Blue Golem in";
	SRTouchZoneToDefaultTimeLabelTextMap[SR_YourBlueGolemPanel] = "  Blue Golem";
	SRTouchZoneToTimeTextMapTimerOn[SR_YourBlueGolemPanel] = TextAdd(cont_SR, 33, 152, "      ", FontAdd("Arial", "Bold", 20, 0xFFFFFF));

	SRTouchZoneToViewMap[SR_YourRedGolemPanel] = ViewAdd(cont_SR, ImageAdd("Images/SR/RedGolemPanel.png"), 160, 130, OnTouchSR, SR_YourRedGolemPanel);
	SRTouchZoneToBoolMap[SR_YourRedGolemPanel] = false;
	SRTouchZoneToTimeMap[SR_YourRedGolemPanel] = -1.0;
	SRTouchZoneToTimeTextMap[SR_YourRedGolemPanel] = TextAdd(cont_SR, 179, 132, "  Red Lizard", FontAdd("Arial", "Bold", 20, 0xFFFFFF));
	SRTouchZoneToTimeLabelTextMap[SR_YourRedGolemPanel] = "Red Lizard in";
	SRTouchZoneToDefaultTimeLabelTextMap[SR_YourRedGolemPanel] = "  Red Lizard";
	SRTouchZoneToTimeTextMapTimerOn[SR_YourRedGolemPanel] = TextAdd(cont_SR, 179, 152, "      ", FontAdd("Arial", "Bold", 20, 0xFFFFFF));
	

	//add the GP10 label
	ViewAdd(cont_SR, ImageAdd("Images/SR/gp10panel.png"), 0, 185);
	//add the theirs label
	ViewAdd(cont_SR, ImageAdd("Images/SR/TheirsLabel.png"), 0, 240);

	//add the two golem panels for theirs
	SRTouchZoneToViewMap[SR_TheirBlueGolemPanel] = ViewAdd(cont_SR, ImageAdd("Images/SR/BlueGolemPanel.png"), 0, 265, OnTouchSR, SR_TheirBlueGolemPanel);
	SRTouchZoneToBoolMap[SR_TheirBlueGolemPanel] = false;
	SRTouchZoneToTimeMap[SR_TheirBlueGolemPanel] = -1.0;
	SRTouchZoneToTimeTextMap[SR_TheirBlueGolemPanel] = TextAdd(cont_SR, 15, 267, "  Blue Golem      ", FontAdd("Arial", "Bold", 20, 0xFFFFFF));
	SRTouchZoneToTimeLabelTextMap[SR_TheirBlueGolemPanel] = "Blue Golem in";
	SRTouchZoneToDefaultTimeLabelTextMap[SR_TheirBlueGolemPanel] = "  Blue Golem";
	SRTouchZoneToTimeTextMapTimerOn[SR_TheirBlueGolemPanel] = TextAdd(cont_SR, 33, 287, "          ", FontAdd("Arial", "Bold", 20, 0xFFFFFF));

	SRTouchZoneToViewMap[SR_TheirRedGolemPanel] = ViewAdd(cont_SR, ImageAdd("Images/SR/RedGolemPanel.png"), 160, 130, OnTouchSR, SR_TheirRedGolemPanel);
	SRTouchZoneToBoolMap[SR_TheirRedGolemPanel] = false;
	SRTouchZoneToTimeMap[SR_TheirRedGolemPanel] = -1.0;
	SRTouchZoneToTimeTextMap[SR_TheirRedGolemPanel] = TextAdd(cont_SR, 179, 267, "  Red Lizard", FontAdd("Arial", "Bold", 20, 0xFFFFFF));
	SRTouchZoneToTimeLabelTextMap[SR_TheirRedGolemPanel] = "Red Lizard in";
	SRTouchZoneToDefaultTimeLabelTextMap[SR_TheirRedGolemPanel] = "  Red Lizard";
	SRTouchZoneToTimeTextMapTimerOn[SR_TheirRedGolemPanel] = TextAdd(cont_SR, 179, 287, "      ", FontAdd("Arial", "Bold", 20, 0xFFFFFF));

	//add the summor spells panel
	ViewAdd(cont_SR, ImageAdd("Images/SR/SummonerSpells.png"), 0, 320);
	//end the game
	endGame();

	//add all of the containers to the container set 
	containerSet.insert(cont_SR);
	containerSet.insert(cont_Main);
	containerSet.insert(cont_SRJungle);

	//fill the state to container map
	stateToContainerMap[STATE_Main] = cont_Main;
	stateToContainerMap[STATE_SR] = cont_SR;
	stateToContainerMap[STATE_SRJungle] = cont_SRJungle;

}
//dims all of the panels (defaulting of the value occurs in startGame())
void endGame() {
	//dim all of the panels
	std::map<SRTouchZone, int>::iterator viewIter;
	for (viewIter = SRTouchZoneToViewMap.begin(); viewIter != SRTouchZoneToViewMap.end(); viewIter++) {
		int view = viewIter->second;
		ViewSetAlpha(view, 30);
	}
}

//lights up all of the panels and defaults the values
void startGame() {
	//light up all of the panels
	std::map<SRTouchZone, int>:: iterator iter;
	for (iter = SRTouchZoneToViewMap.begin(); iter != SRTouchZoneToViewMap.end(); iter++) {
		int view = iter->second;
		ViewSetAlpha(view, 100);
	}

	//set all timers to off
	//sets times to -1.0
	//reset the default time text when the timer is off
	std::map<SRTouchZone, bool>::iterator timerIter;
	for (timerIter = SRTouchZoneToBoolMap.begin(); timerIter != SRTouchZoneToBoolMap.end(); timerIter++) {
		SRTouchZone tz = timerIter->first;
		SRTouchZoneToBoolMap[tz] = false;
		SRTouchZoneToTimeMap[tz] = -1.0;
		TextSetText(SRTouchZoneToTimeTextMap[tz], SRTouchZoneToDefaultTimeLabelTextMap[tz]);

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
			startGame();
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
		SRTouchZone panel = (SRTouchZone) id;
		if (SRTouchZoneToBoolMap[panel]) {
			//if on already turn off
			ViewSetAlpha(SRTouchZoneToViewMap[panel], 100);
			SRTouchZoneToBoolMap[panel] = false;
			SRTouchZoneToTimeMap[panel] = -1.0;
			TextSetText(SRTouchZoneToTimeTextMap[panel], SRTouchZoneToDefaultTimeLabelTextMap[panel]);
			TextSetVisible(SRTouchZoneToTimeTextMapTimerOn[panel], false);
		} else {
			//if off already turn on
			ViewSetAlpha(SRTouchZoneToViewMap[panel], 30);
			SRTouchZoneToBoolMap[panel] = true;
			SRTouchZoneToTimeMap[panel] = 100.0;
			TextSetVisible(SRTouchZoneToTimeTextMapTimerOn[panel], true);
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
		char str[15] = { '\0' };
		convertTime(str, seconds);
		char strFinal[100] = { '\0' };
		strcpy(strFinal, "Slide Back to End Game\n       ");
		strcat(strFinal, str);
		
		TextSetText(gameSliderText, strFinal);
		TextSetColor(gameSliderText, 0xFFFF00);

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
				sprintf(str, "%d", roundedTime);
				int timeValueHandle = SRTouchZoneToTimeTextMapTimerOn[key];
				TextSetText(timeValueHandle, str);
				
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