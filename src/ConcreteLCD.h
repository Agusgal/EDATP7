#pragma once


#include "allegro5/allegro.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include <allegro5/allegro_primitives.h>
#include "BasicLCD.h"
#include "Cursor.h"
#include "Error.h"


#define DISPLAY_WIDTH       550
#define DISPLAY_HEIGHT		200

#define MSG_X				20
#define MSG1_Y				20
#define MSG2_Y				MSG1_Y + 100

#define SCREEN_COLOR al_map_rgba_f(1, 1, 0.8, 1)


class LcdA : public basicLCD
{
public:
	LcdA();
	virtual ~LcdA();


	virtual bool lcdInitOk();
	virtual lcdError& lcdGetError();

	virtual bool lcdClear();
	virtual bool lcdClearToEOL();

	virtual basicLCD& operator<< (const char c);
	virtual basicLCD& operator<< (const char* c);

	virtual bool lcdMoveCursorUp();
	virtual bool lcdMoveCursorDown();
	virtual bool lcdMoveCursorRight();
	virtual bool lcdMoveCursorLeft();

	virtual bool lcdSetCursorPosition(const cursorPosition pos);
	virtual cursorPosition lcdGetCursorPosition();

private:
	ALLEGRO_DISPLAY* display;
	ALLEGRO_FONT* font;

	cursorPosition cursorP;

	lcdError error;

	std::string messages[2];

	std::string msg1;
	std::string msg2;

	void drawMessage(void);
	void drawCursor(void);
	void clearScreen(void);
};