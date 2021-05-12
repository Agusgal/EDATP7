#pragma once

#include <stdio.h>
#include <iostream>
#include <string>

#include "../lib/ImGui/imgui.h"
#include "../lib/ImGui/imgui_impl_allegro5.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "Error.h"
#include "BasicLCD.h"
#include "ConcreteLCD.h"
#include "lcd3.h"
#include "LCDB.h"

#include "Imgui_string.h"


#include "Twitter/TwitterHandler.h"
#include "UserData.h"

#include "Twitter/API_Request_err.h"

#include <vector>
#include <cmath>

#define WIDTH     800
#define HEIGHT    300
#define FPS       120 


enum class events{IDLE, REQUEST, CANCELREQUEST, ROLL, NEXT, PREVIOUS, CLOSE};

enum class state{TCnotLoaded, TCLoaded, TweetsRequested};


//#define DEMO //to see demo window and implemnt different widgets

class Gui {
public:
    //Constructor
    Gui();

    //Gui Windows
    void testWindow(void);
    void mainWindow(void);

    //Custom addons
    void helpMarker(const char* desc);
    void popup(const char* msg);

    //Show Functions
    bool showMainWindow(void);

    //
    int loadBitmaps(void);

    //Inits and Configs
    void initAll(void);
    void destroyAll(void);
    void resize(void);

    //Refresh and draw
    void refresh(void);

    //Dispatcher
    void dispatcher(void);
    events checkGuiEvent(void);

    //Setters and getters
    bool isOver(void);
    bool getEvent(void);
    bool noError(void);
    
    

private:
    //Allegro 

    ALLEGRO_DISPLAY* display;
    ALLEGRO_EVENT_QUEUE* queue;
    ALLEGRO_EVENT ev;
    ALLEGRO_TIMER* flipTimer;


    ALLEGRO_TIMER* requestTimer;
    ALLEGRO_EVENT requestEv;

    ALLEGRO_TIMER* rollTimer;


    void initAllegro(void);
    void initDisplay(void);
    void initTimer(void);
    void initEvents(void);
    void initImGui(void);

    //Draws
    void refreshImgui(void);
    void refreshLCDs(void);


    double displaySizeX;
    double displaySizeY;

    double fps;

    //Control flags
    bool running;
    bool popupOpen;

    //ComboBox atributes
    const char* comboItems[3];
    int currentItemId;
    
    bool idAvailable(std::string id);


    //Displays
    std::vector<basicLCD*> displays;
    std::vector <UserData*> userInput;
    TwitterHandler* tClient;
    int displayInUSe;

    //Misc
    int test;
    int getCurrentDisplay(std::string Id);
    bool rollTwitts;
    unsigned int positionRoll;

    float rollVar;
    
    //showtweets
    void roll(void);
    unsigned int tweetNumber;
    void showNextTweet();
    void showPreviousTweet();

    //States
    state loadState;
    events nextEvent;
    bool allegroEventComes;
    bool genericEventComes;

    //Error
    lcdError err;

    //PArsers

    void parseToClient(const char* username, int tweetN, std::string displayId);
    void performRequest(std::string displayId);

    void loading(int& sign, int id);


};