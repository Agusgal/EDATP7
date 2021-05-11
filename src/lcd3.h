#ifndef __LCD3_H
#define __LCD3_H
#include "basicLCD.h"
#include "Error.h"

using namespace std;

#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h> 
#include <allegro5/allegro_primitives.h> 
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>


// --------------------------     Constantes display backend     -------------------------- //
#define ROWS 2
#define COLUMNS 16
#define CHAR_LIMIT  31

// --------------------------     display allegro      -------------------------- //
#define BUFFER_W        640
#define BUFFER_H        280
#define SCREEN_SCALE      1
#define SCREEN_W          (BUFFER_W * SCREEN_SCALE)
#define SCREEN_H          (BUFFER_H * SCREEN_SCALE)
#define INNER_SCREEN_X  545
#define INNER_SCREEN_Y  126
#define BORDER_X        20
#define BORDER_Y        100
#define CHAR_SIZE_X     40
#define CHAR_SIZE_Y     40
#define LINE_HEIGHT     80


// --------------------------     colores      -------------------------- //
#define BLACK           al_map_rgb(0,0,0)
#define WHITE           al_map_rgb(255,255,255)
#define PINK            al_map_rgb(255, 182, 193)
#define SEAFOAM         al_map_rgb(69, 35, 80)
#define BUTTER          al_map_rgb(253, 246, 197)

// --------------------------     Errores      -------------------------- //

enum errorList {NONE, INIT_ERROR, MOVEMENT_ERROR};

//Clase LCD3
class LCD3 : public basicLCD
{
public:
    //Constructor.
    LCD3();

    //Destructor.
    ~LCD3();

    //Metodos virtuales a implementar de basicLCD.
    virtual bool lcdInitOk();
    virtual lcdError& lcdGetError();
    virtual bool lcdClear();
    virtual bool lcdClearToEOL();
    virtual basicLCD& operator<<(const char c);
    virtual basicLCD& operator<<(const char* c);
    virtual bool lcdMoveCursorUp();
    virtual bool lcdMoveCursorDown();
    virtual bool lcdMoveCursorRight();
    virtual bool lcdMoveCursorLeft();
    virtual bool lcdSetCursorPosition(const cursorPosition pos);
    virtual cursorPosition lcdGetCursorPosition();

private:

    void initAllegro();
    void deinitAllegro();
    void writeLCD();
    void eraseCursorLCD();
    void drawCursorLCD();

   cursorPosition cp;
    char display[ROWS][COLUMNS];

    lcdError err;
    

    //allegro
    ALLEGRO_FONT* font;
    ALLEGRO_DISPLAY* screen;
    //ALLEGRO_EVENT_QUEUE* event_queue;
    ALLEGRO_BITMAP* back;
};

#endif