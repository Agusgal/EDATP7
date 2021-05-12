#define _CRT_SECURE_NO_WARNINGS

#include "LCDB.h"

#define FONT		"../res/Fonts/OpenSans-Semibold.ttf"
#define LCD			"../res/Images/lcd.png"
#define FPS			60

int LcdB::initAllegro()
{
	error = lcdError();
	//inicializo cada componente relacionado con allegro

	this->punteros.font = al_load_font(FONT, 45, 0);
	if (this->punteros.font)
	{
		this->punteros.display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
		al_set_target_backbuffer(punteros.display);
		if (this->punteros.display)
		{
			this->punteros.lcd = al_load_bitmap(LCD);

			if (this->punteros.lcd)
			{
			}
			else
			{
				error.setErrorCode(errorClass::AL_CREATE_BITMAP_ERR);
			}
		}
		else
		{
			error.setErrorCode(errorClass::AL_CREATE_DISPLAY_ERR);
		}
	}
	else
	{
		error.setErrorCode(errorClass::AL_CREATE_FONT_ERR);
	}

	if (error.getErrorCode() == errorClass::NO_ERR) //En caso de una inicialización correcta se dibuja el entorno
	{
		al_draw_bitmap(this->punteros.lcd, 0, 0, 0);
	}
	else // Si hubo errores destruyo los recursos
	{
		lcdError tempError = lcdError(error);
		this->error = tempError;

		destroyAllegro();
	}
	al_flip_display();
	return true;
}

//destruyo componentes de allegro
void LcdB::destroyAllegro()
{
	if (this->punteros.lcd)
		al_destroy_bitmap(this->punteros.lcd);
	if (this->punteros.display)
		al_destroy_display(this->punteros.display);
	if (this->punteros.font)
		al_destroy_font(this->punteros.font);
}

LcdB::LcdB()
{
	this->cursor.column = 0;
	this->cursor.row = 0;

	this->text[0] = "                ";
	this->text[1] = "                ";

	int error = initAllegro();

	this->error.setErrorCode(errorClass::NO_ERR);

	switch (error)
	{
	case 1:
		//error de fuentes
		break;
	case 2:
		//error de display
		break;
	case 3:
		//error de eventos
		break;
	}
}

LcdB::~LcdB()
{
	destroyAllegro();
}

void LcdB::redraw()
{
	al_set_target_backbuffer(punteros.display);
	al_draw_bitmap(this->punteros.lcd, 0, 0, 0);

	const char* texto1 = this->text[0].c_str();
	const char* texto2 = this->text[1].c_str();

	char txt1[17];
	strcpy(txt1, texto1);
	char txt2[17];
	strcpy(txt2, texto2);

	if (this->cursor.row == 0)
	{
		al_draw_text(this->punteros.font, green, 98, 85, 0, txt1);
		txt1[this->cursor.column] = '_';
		txt1[this->cursor.column + 1] = '\0';
		al_draw_text(this->punteros.font, green, 98, 85, 0, txt1);
		al_draw_text(this->punteros.font, green, 98, 140, 0, txt2);
	}
	else
	{
		al_draw_text(this->punteros.font, green, 98, 140, 0, txt2);
		txt2[this->cursor.column] = '_';
		txt2[this->cursor.column + 1] = '\0';
		al_draw_text(this->punteros.font, green, 98, 140, 0, txt2);
		al_draw_text(this->punteros.font, green, 98, 85, 0, txt1);
	}
	al_flip_display();
}

bool LcdB::lcdInitOk()
{
	if (this->error.getErrorCode() == errorClass::NO_ERR)
	{
		return true;
	}
	else
	{
		return false;
	}
}


lcdError& LcdB::lcdGetError()
{
	return this->error;
}

bool LcdB::lcdClear()
{
	this->text[0] = "                ";
	this->text[1] = "                ";

	this->cursor.column = 0;
	this->cursor.row = 0;

	redraw();

	return true;
}


bool LcdB::lcdClearToEOL()
{
	this->text[this->cursor.row].replace(this->cursor.column, 16 - this->cursor.column, 16 - this->cursor.column, ' ');
	redraw();

	return true;
}



basicLCD& LcdB::operator<<(const char c)
{
	if (c >= 32 && c <= 126)
	{
		if (this->cursor.column < 15)
		{
			this->text[this->cursor.row].replace(this->cursor.column, 1, 1, c);
			this->cursor.column += 1;
		}
		else if (this->cursor.row == 0)
		{
			this->text[this->cursor.row].replace(this->cursor.column, 1, 1, c);
			this->cursor.column = 0;
			this->cursor.row = 1;
		}
		else if (this->cursor.row == 1)
		{
			this->text[this->cursor.row].replace(this->cursor.column, 1, 1, c);
		}
	}
	redraw();

	return *this;
}

basicLCD& LcdB::operator<<(const char* c)
{
	int i = 0;
	while (i < strlen(c))
	{
		if (c[i] >= 32 && c[i] <= 126)
		{
			if (this->cursor.column < 15)
			{
				this->text[this->cursor.row].replace(this->cursor.column, 1, 1, c[i]);
				this->cursor.column += 1;
				i++;
			}
			else if (this->cursor.row == 0)
			{
				this->text[this->cursor.row].replace(this->cursor.column, 1, 1, c[i]);
				this->cursor.column = 0;
				this->cursor.row = 1;
				i++;
			}
			else if (this->cursor.row == 1 && this->cursor.column == 15)
			{
				this->text[this->cursor.row].replace(this->cursor.column, 1, 1, c[i]);
				break;
			}
		}
	}

	redraw();

	return *this;
}

bool LcdB::lcdMoveCursorUp()
{
	if (this->cursor.row == 0)
	{
		return false;
	}
	else
	{
		this->cursor.row = 0;
	}

	redraw();

	return true;
}

bool LcdB::lcdMoveCursorDown()
{
	if (this->cursor.row == 1)
	{
		return false;
	}
	else
	{
		this->cursor.row = 1;
	}

	redraw();

	return true;
}

bool LcdB::lcdMoveCursorRight()
{
	if (this->cursor.row == 0 && this->cursor.column == 15)
	{
		this->cursor.column = 0;
		this->cursor.row = 1;
	}
	else if (this->cursor.row == 1 && this->cursor.column == 15)
	{
		return false;
	}
	else
	{
		this->cursor.column += 1;
	}

	redraw();

	return true;
}

bool LcdB::lcdMoveCursorLeft()
{
	if (this->cursor.row == 1 && this->cursor.column == 0)
	{
		this->cursor.column = 15;
		this->cursor.row = 0;
	}
	else if (this->cursor.row == 0 && this->cursor.column == 0)
	{
		return false;
	}
	else
	{
		this->cursor.column -= 1;
	}

	redraw();

	return true;
}


cursorPosition LcdB::lcdGetCursorPosition()
{
	return this->cursor;
}


bool LcdB::lcdSetCursorPosition(const cursorPosition pos)
{
	if ((pos.column >= 0 && pos.column <= 16) && (pos.row == 0 || pos.row == 1))
	{
		this->cursor.column = pos.column;
		this->cursor.row = pos.row;
	}
	else
	{
		return false;
	}

	return true;
}