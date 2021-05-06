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

#include "Imgui_string.h"

#include <cmath>

#define WIDTH     800
#define HEIGHT    500
#define FPS       120 

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

    bool openedDisplays[3];

    //ComboBox atributes
    const char* comboItems[3];
    int currentItemId;
    
    //User input
    std::string TwitterUsers[3];
    int TwittN[3];


    //Misc
    int test;

    //Error
    lcdError err;
};