#pragma once

#include <string>

enum errorClass {AL_CREATE_DISPLAY_ERR, AL_CREATE_FONT_ERR, AL_CREATE_BITMAP_ERR, AL_CREATE_TIMER_ERR, AL_CREATE_EVQUEUE_ERR,
	AL_INIT_ERR, AL_INIT_DISPLAY_ERR, AL_INIT_PRIMITIVES_ERR, AL_INIT_KEYBOARD_ERR, AL_INIT_MOUSE_ERR, AL_INIT_IMAGE_ERR, NO_ERR, ERROR_N};


class lcdError
{
public:
	lcdError(int code);
	lcdError() = default;

	std::string getErrorName();
	std::string getErrorDescription();
	unsigned long getErrorCode();


private:

	std::string description;
	std::string name;
	int code;


};
