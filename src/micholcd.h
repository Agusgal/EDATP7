#pragma once
#ifndef MICHO_H
#define MICHO_H
#include <string>
#include <iostream>
#include "allegro5/allegro.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include <allegro5/allegro_primitives.h>
#include "basicLCD.h"

#define FONTPATHMICHO "../res/Fonts/fuentemicho.TTF"
#define MAX 32

class micholcd :public basicLCD {
public:
	micholcd();
	~micholcd();
	virtual bool lcdInitOk();
	virtual lcdError& lcdGetError();
	virtual bool lcdClear();
	virtual bool lcdClearToEOL();
	virtual bool lcdMoveCursorUp();
	virtual bool lcdMoveCursorDown();
	virtual bool lcdMoveCursorRight();
	virtual bool lcdMoveCursorLeft();
	virtual bool lcdSetCursorPosition(const cursorPosition pos);
	virtual cursorPosition lcdGetCursorPosition();
	virtual basicLCD& operator<<(const unsigned char word);
	virtual basicLCD& operator<<(const char* word);

private:
	cursorPosition cursor;
	lcdError share;
	char display[MAX];
	void redraw();
	ALLEGRO_FONT* fuentemicho;
};





#endif

