#ifndef APP_H
#define APP_H

#include "Windows.h"



//application  states
enum STATE {
	STATE_Main,
	STATE_SR,
	STATE_SRJungle
};
enum STATE currentState;

//maps out the touch areas in the SR container
enum SRTouchZone {
	SR_StartGameSlideButton, SR_StartGameSlider, SR_StartGameSliderEnd,
	SR_DragonPanel,	SR_BaronPanel,
	SR_YourBlueGolemPanel, SR_YourRedGolemPanel,
	SR_Philo, SR_Kages, SR_Avarice, SR_HOG,
	SR_TheirBlueGolemPanel, SR_TheirRedGolemPanel,
	SR_Cleanse, SR_Flash, SR_Surge, SR_Exhaust, SR_CV,
	SR_Ignite, SR_Teleport, SR_Heal, SR_Smite, SR_Ghost
};

int AlertShow(char *title, char *message)
{
    MessageBox(NULL,message,title,MB_OK);
    return(1);
}



//convert seconds to ##:## format as a char array
void convertTime(char * str, int seconds) {
	int m = seconds/60;
	int s = seconds % 60;

	char mstr[10];
	itoa(m, mstr, 10);
	char sstr[10];
	itoa(s, sstr, 10);
	char mfinal[10];
	char sfinal[10];
	if (m < 10) {
		strcat(mfinal, "0");
		strcat(mfinal, mstr);
	} else {
		strcat(mfinal, mstr);
	}
	if (s < 10) {
		strcat(sfinal, "0");
		strcat(sfinal, sstr);
	} else {
		strcat(sfinal, sstr);
	}
	char timeFinal[20];
	strcat(timeFinal, mfinal);
	strcat(timeFinal, ":");
	strcat(timeFinal, sfinal);
	strcpy(str, timeFinal);
}

//true if x and y are within the coordinates of the view false other wise
bool contains(int view, int x, int y) {
	int minx = ViewGetx(view);
	int miny = ViewGety(view);
	int maxx = minx + ViewGetWidth(view);
	int maxy = miny + ViewGetHeight(view);
	return (x >= minx && x <= maxx) && (y >= miny && y <= maxy);
}

bool contains(int minx, int miny, int maxx, int maxy, int x, int y) {
	return (x >= minx && x <= maxx) && (y >= miny && y <= maxy);
}

void dimPanels();
void lightPanels();

int OnTouchSRStartGameSlider(int id, int event, int x, int y);
void updateTimers();
void goToState(STATE state);
void init();
void clearContainers() ;
void OnTimerSR();
int OnTouchSR(int id, int event, int x, int y);
#endif