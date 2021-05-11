#include "micholcd.h"
using namespace std;


/// micho lcd Constructor
micholcd::micholcd() : share("OK", "Ok", OK) {
	cursor.column = 0;
	cursor.row = 0;
	for (int i = 0; i < MAX; ++i) {
		display[i] = 0;
	}
	fuentemicho = al_load_ttf_font(FONTPATHMICHO, 36, 0);
	if (fuentemicho == NULL)
	{
		share = lcdError("INITERR", "Error al cargar fuente", INITERR);
	}

}

//Destructor
micholcd::~micholcd() {
	al_destroy_font(fuentemicho);
}

//OK?
bool micholcd::lcdInitOk() {
	return true;
}

//ERROR?
lcdError& micholcd::lcdGetError() {
	return this->share;
}

//Limpio display
bool micholcd::lcdClear() {
	for (int i = 0; i < MAX; ++i) {
		display[i] = 0;
	}
	cursor.column = 0;
	cursor.row = 0;
	redraw();
	return true;
}

//Borrra hasta el cursor
bool micholcd::lcdClearToEOL() {
	for (int i = cursor.column; i < (MAX / 2); ++i) {
		display[cursor.row * (MAX / 2) + i] = 0;
	}
	redraw();
	return true;
}


basicLCD& micholcd::operator<<(const unsigned char word)
{
	if ((this->cursor.row == 1) && (this->cursor.column == ((MAX / 2) - 1))) {
		if ((word >= 32) && (word <= 126)) {
			this->display[MAX - 1] = word;
			this->cursor.column = 0;
			this->cursor.row = 0;
		}
		else
		{
			this->display[MAX - 1] = ' ';
			this->cursor.column = 0;
			this->cursor.row = 0;
		}
	}
	else {
		if ((word >= 32) && (word <= 126)) {
			this->display[(this->cursor.row * (MAX / 2)) + (this->cursor.column)] = word;
			if (this->lcdMoveCursorRight()) {
			}
			else {
				this->lcdMoveCursorDown();
				this->cursor.column = 0;
			}
		}
		else
		{
			this->display[(this->cursor.row * (MAX / 2)) + (this->cursor.column)] = ' ';
			if (this->lcdMoveCursorRight()) {
			}
			else {
				this->lcdMoveCursorDown();
				this->cursor.column = 0;
			}
		}
	}
	redraw();
	return *this;
}


basicLCD& micholcd::operator<<(const char* word)
{
	int len = strlen(word);
	int var = 1;
	if (len >= MAX) {
		int len_inicio = len - MAX;
		for (int i = len_inicio; (i < len) && var; i++) {
			if ((this->cursor.row == 1 && this->cursor.column == ((MAX / 2) - 1)) && this->display[(this->cursor.row * MAX / 2) + (this->cursor.column)] != '/0') {
				this->display[(this->cursor.row * (MAX / 2)) + (this->cursor.column)] = word[i];
				var = 0;

				this->cursor.column = 0;
				this->cursor.row = 0;
			}
			else {
				this->display[(this->cursor.row * (MAX / 2)) + (this->cursor.column)] = word[i];
				if (this->lcdMoveCursorRight()) {
				}
				else {
					this->lcdMoveCursorDown();
					this->cursor.column = 0;
				}
			}
		}
	}
	else {
		for (int i = 0; (word[i] != '\0'); i++) {
			if ((this->cursor.row && this->cursor.column == ((MAX / 2) - 1)) && this->display[(this->cursor.row * MAX / 2) + (this->cursor.column)] != '/0') {
				this->display[(this->cursor.row * (MAX / 2)) + (this->cursor.column)] = word[i];
				if (this->lcdMoveCursorRight()) {
				}
				else if (this->lcdMoveCursorDown()) {
					this->cursor.column = 0;
				}
				else {
					this->cursor.row = 0;
					this->cursor.column = 0;
				}
			}
			else {
				this->display[(this->cursor.row * (MAX / 2)) + (this->cursor.column)] = word[i];
				if (this->lcdMoveCursorRight()) {
				}
				else {
					this->lcdMoveCursorDown();
					this->cursor.column = 0;
				}
			}
		}
	}

	redraw();
	return *this;
}


bool micholcd::lcdMoveCursorUp() {
	bool estado = true;
	cursorPosition temp;
	temp.row = cursor.row;
	temp.row--;
	if (temp.row != 1 && temp.row != 0) {
		estado = false;
	}
	else {
		cursor.row = temp.row;
	}
	return estado;
}


bool micholcd::lcdMoveCursorDown() {
	bool estado = true;
	cursorPosition temp;
	temp.row = cursor.row;
	temp.row++;
	if (temp.row != 1 && temp.row != 0) {
		estado = false;
	}
	else {
		cursor.row = temp.row;
	}
	return estado;
}


bool micholcd::lcdMoveCursorRight() {
	bool estado = true;
	cursorPosition temp;
	temp.column = cursor.column;
	temp.column++;
	if (temp.column >= (MAX / 2) || temp.column < 0) {
		estado = false;
	}
	else {
		cursor.column = temp.column;
	}
	return estado;
}


bool micholcd::lcdMoveCursorLeft() {
	bool estado = true;
	cursorPosition temp;
	temp.column = cursor.column;
	temp.column--;
	if (temp.column >= (MAX / 2) || temp.column < 0) {
		estado = false;
	}
	else {
		cursor.column = temp.column;
	}
	return estado;
}


bool micholcd::lcdSetCursorPosition(const cursorPosition pos) {
	bool estado = true;
	if (pos.row != 1 && pos.row != 0) {
		estado = false;
	}
	else {
		cursor.row = pos.row;
		if (pos.column >= (MAX / 2) || pos.column < 0) {
			estado = false;
		}
		else {
			cursor.column = pos.column;
			redraw();
		}
	}
	return estado;
}


cursorPosition micholcd::lcdGetCursorPosition() {
	return this->cursor;
}



void micholcd::redraw() {

	al_clear_to_color(al_map_rgb(0, 0, 255));

	/*LCD*/
	for (int j = 0; j < 70; j++) {
		al_draw_textf(fuentemicho, al_map_rgb(255, 255, 255), 40 * (MAX / 2) + 10, j, ALLEGRO_ALIGN_LEFT, "%c", 'l');
	}
	for (int j = 0; j < 72; j++) {
		al_draw_textf(fuentemicho, al_map_rgb(0, 0, 0), 40 * (MAX / 2) + 15, j, ALLEGRO_ALIGN_LEFT, "%c", 'l');
	}
	for (int j = 0; j < 40 * (MAX / 2) - 4; j++) {
		al_draw_textf(fuentemicho, al_map_rgb(255, 255, 255), j, 70, ALLEGRO_ALIGN_LEFT, "%c", '_');
	}
	for (int j = 0; j < 40 * (MAX / 2) + 4; j++) {
		al_draw_textf(fuentemicho, al_map_rgb(0, 0, 0), j, 72, ALLEGRO_ALIGN_LEFT, "%c", '_');
	}
	///

	/*WORDS*/
	char curse = '_';

	cout << " * ";
	for (int i = 0; i < MAX; ++i) {
		float word_column_now = (float)(40 * i);
		cout << display[i];
		if (i < (MAX / 2)) {
			al_draw_textf(fuentemicho, al_map_rgb(255, 255, 255), word_column_now, 0, ALLEGRO_ALIGN_LEFT, "%c", display[i]);
		}
		else {
			word_column_now -= 40 * (MAX / 2);
			al_draw_textf(fuentemicho, al_map_rgb(255, 255, 255), word_column_now, 46, ALLEGRO_ALIGN_LEFT, "%c", display[i]);
		}
#ifdef _DEBUG
		if (i == MAX / 2 - 1)
		{
			cout << endl << " * ";
		}
#endif // DEBUG
	}
	cout << endl << endl;
	float cursor_x_now = (float)(40 * (this->cursor.column));
	float cursor_y_now = (float)(40 * (this->cursor.row) + 17);
	al_draw_textf(fuentemicho, al_map_rgb(0, 0, 0), cursor_x_now, cursor_y_now, ALLEGRO_ALIGN_LEFT, "%c", curse);

	al_flip_display();
}

