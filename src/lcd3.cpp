#include "LCD3.h"
LCD3::LCD3()
{
	try
	{
		this->err = lcdError();
		this->cp = cursorPosition();
		this->cp.column = 0;
		this->cp.row = 0;
		initAllegro();

	}
	catch (errorClass code)
	{
		this->err.setErrorCode(code);
	}
	
	
	
	//cp.row = 0;
	//cp.column = 0;
	//font = NULL;
	//screen = NULL;
	//back = NULL;
	//memset(display, 0, sizeof(display));
}
LCD3::~LCD3()
{
	deinitAllegro();
}


//Devuelve en su nombre "true" si el display se inicializó correctamente (el constructor no tuvo errores) o "false" en caso contrario.
bool LCD3::lcdInitOk()
{
	if (this->err.getErrorCode() == errorClass::NO_ERR)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//Devuelve en su nombre un lcdError&
lcdError& LCD3::lcdGetError()
{
	return err;
}


//Borra el display y el cursor va a HOME.
//Devuelve en su nombre "true" si fue satisfactoria "false" en caso contrario.
bool LCD3::lcdClear()
{
	al_set_target_backbuffer(this->screen);
	for (int i = 0; i < ROWS; ++i)		//Limpia el display. 
	{
		for (int j = 0; j < COLUMNS; ++j)
		{
			display[i][j] = 0;
		}
	}
	cp.row = 0;		//Resetea al cursor.
	cp.column = 0;
	writeLCD();
	drawCursorLCD();
	return true;
}

//Borra el display desde la posición actual del cursor hasta el final de la línea.
//Devuelve en su nombre "true" si fue satisfactoria "false" en caso contrario.
bool LCD3::lcdClearToEOL()
{
	al_set_target_backbuffer(this->screen);
	for (int i = cp.row; i < ROWS; ++i)		//Limpia el display. 
	{
		for (int j = cp.column; j < COLUMNS; ++j)
		{
			display[i][j] = 0;
		}
	}
	writeLCD();
	return true;
}

//Devuelve en su nombre una referencia a un basicLCD que permite encascar la función.
basicLCD& LCD3::operator<<(const char c)
{
	al_set_target_backbuffer(this->screen);
	if (c > CHAR_LIMIT)	//Si es un caracter imprimible: 
	{
		display[cp.row][cp.column] = c;	//Inserta el caracter en la posicion en la que se encuentra el cursor actualmente. 

		//Ahora hay que mover el cursor una posicion hacia la derecha:
		bool res = lcdMoveCursorRight();
	}

	writeLCD();
	drawCursorLCD();
	return *this;
}

//Devuelve en su nombre una referencia a un basicLCD que permite encascar la función
basicLCD& LCD3::operator<<(const char* c)
{
	al_set_target_backbuffer(this->screen);
	int i = 0;
	bool res = true;
	int cant = 0;	//Contendra la cantidad de caracteres de la cadena recibida. 
	int pos = 0;	//Se ira moviendo en la cadena ingresada. 

	while (c[i] != NULL)	//Calcula cantidad de caracteres de la cadena.
	{
		++cant;
		++i;
	}

	if (cant >= (ROWS * COLUMNS))		//En caso de superar el tamaño del display, escribe los ultimos 32.
	{
		pos = cant - 32;
	}

	while ((pos < cant) && res)	//Se van imprimiendo los caracteres de la cadena a partir de donde se encuentre el cursor. 
	{
		if (c[pos] > CHAR_LIMIT)	//Si es un caracter imprimible:
		{
			display[cp.row][cp.column] = c[pos];	//Lo imprime.
			res = lcdMoveCursorRight();		//Mueve el cursor una posicion hacia la derecha en caso de ser posible. 
		}
		++pos;	//Avanza al siguiente caracter de la cadena. 
	}

	writeLCD();
	drawCursorLCD();
	return *this;
}

//Pasa el cursor a la primera línea del display sin alterar la columna en la que estaba.
//Devuelve en su nombre "true" si fue satisfactoria "false" en caso contrario.
bool LCD3::lcdMoveCursorUp()
{
	bool movement = false;
	//Si estaba en la segunda fila, pasa a la primera.
	if (cp.row == 1)
	{
		eraseCursorLCD();
		--cp.row;
		movement = true;
		drawCursorLCD();
	}
	if (!movement)
	{
		this->err.setErrorCode(errorClass::MOVEMENT_ERR);
	}
	//En cualquier otro caso, devuelve error.
	return movement;
}

//Pasa el cursor a la segunda línea del display sin alterar la columna en la que estaba.
//Devuelve en su nombre “true” si fue satisfactoria “false” en caso contrario.
bool LCD3::lcdMoveCursorDown()
{
	bool movement = false;
	//Si estaba en la primera fila, pasa a abajo.
	if (cp.row == 0)
	{
		eraseCursorLCD();
		++cp.row;
		movement = true;
		drawCursorLCD();
	}
	if (!movement)
	{
		this->err.setErrorCode(errorClass::MOVEMENT_ERR);
	}
	//En cualquier otro caso, devuelve error.
	return movement;
}

//Avanza el cursor una posición hacia la derecha. 
///Devuelve en su nombre "true" si fue satisfactoria "false" en caso contrario.
bool LCD3::lcdMoveCursorRight()
{
bool movement = false;
	//Si hay una columna libre a la derecha, se mueve una posicion.
	if ((cp.column >= 0) && (cp.column <= COLUMNS - 1))
	{
		eraseCursorLCD();
		//Si esta en la ultima columna de la primera fila, lo regresa al principio.
		if (cp.column == COLUMNS - 1 && cp.row == 0)
		{
			cp.column = 0;
			++cp.row;
			movement = true;
		}
		else {
			++cp.column;	//Avanza una posicion el cursor. 
			movement = true;
		}
		drawCursorLCD();
	}
	if (!movement)
	{
		this->err.setErrorCode(errorClass::MOVEMENT_ERR);
	}
	//En cualquier otro caso, devuelve error.
	return movement;
}

//Retrocede el cursor una posición.
//Devuelve en su nombre "true" si fue satisfactoria "false" en caso contrario.
bool LCD3::lcdMoveCursorLeft()
{
	bool movement = false;
	//Si hay una columna libre a la izquierda, se mueve una posicion.
	if ((cp.column >= 0) && (cp.column < COLUMNS))
	{
		eraseCursorLCD();
		//Si esta en la ultima fila primera columna, pasa al renglon anterior.
		if (cp.column == 0 && cp.row == ROWS - 1)
		{
			cp.column = COLUMNS - 1;
			--cp.row;
			movement = true;
		}
		else
		{
			--cp.column;	//Avanza una posicion el cursor. 
			movement = true;
		}
		drawCursorLCD();

	}
	//En cualquier otro caso, devuelve error.
	if (!movement)
	{
		this->err.setErrorCode(errorClass::MOVEMENT_ERR);
	}
	return movement;
}

//Recibe una estructura tipo cursorPosition.
//Posiciona el cursor en la posición dada por row y column. row[0-1] col[0-15]. Ante un valor inválido de row y/o column ignora la instrucción (no hace nada).
//Devuelve en su nombre "true" si fue satisfactoria "false" en caso contrario.
bool LCD3::lcdSetCursorPosition(const cursorPosition pos)
{
	bool check = false;
	//De ser valida la expresion, posiciona el cursor.
	if (pos.row >= 0 && pos.row < ROWS && pos.column >= 0 && pos.column < COLUMNS) {
		eraseCursorLCD();
		cp.row = pos.row;
		cp.column = pos.column;
		check = true;
		drawCursorLCD();
	}
	return check;
}

//Devuelve la posición actual del cursor.
//Devuelve una estructura tipo cursorPosition.
cursorPosition LCD3::lcdGetCursorPosition()
{
	cursorPosition cursor;
	cursor.column = cp.column;
	cursor.row = cp.row;

	return cursor;
}

//Inicializamos Allegro
void LCD3::initAllegro()
{
	//Init display
	screen = al_create_display(SCREEN_W, SCREEN_H);
	if (screen == NULL) 
	{
		throw(errorClass::AL_CREATE_DISPLAY_ERR);
	}

	//Tipo y tamano de font
	font = al_load_ttf_font("../res/Fonts/fontpixels.ttf", 15, 0);
	if (font == NULL) 
	{
		throw(errorClass::AL_CREATE_FONT_ERR);
	}

}

//Destruye buffer, screen y font.
void LCD3::deinitAllegro()
{
	al_destroy_display(screen);
	al_destroy_font(font);
}

void LCD3::writeLCD()
{
	al_clear_to_color(PINK);
	string str = "";
	for (int i = 0; i < ROWS; ++i)
	{
		for (int j = 0; j < COLUMNS; ++j)
		{
			if (display[i][j] != 0)
			{
				str.push_back(display[i][j]);
				al_draw_text(font, SEAFOAM, j * CHAR_SIZE_X + BORDER_X, i * LINE_HEIGHT + BORDER_Y, ALLEGRO_ALIGN_LEFT, str.c_str());
				str.erase();
			}
		}
	}

	al_flip_display();
}
void LCD3::drawCursorLCD()
{
	al_draw_line(cp.column * CHAR_SIZE_X + BORDER_X, (cp.row + 1) * LINE_HEIGHT + BORDER_Y, (cp.column + 1) * CHAR_SIZE_X + BORDER_X, (cp.row + 1) * LINE_HEIGHT + BORDER_Y, BUTTER, 2);
	al_flip_display();
}

void LCD3::eraseCursorLCD()
{
	al_draw_line(cp.column * CHAR_SIZE_X + BORDER_X, (cp.row + 1) * LINE_HEIGHT + BORDER_Y, (cp.column + 1) * CHAR_SIZE_X + BORDER_X, (cp.row + 1) * LINE_HEIGHT + BORDER_Y, PINK, 2);
}