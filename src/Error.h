#pragma once

#include <string>

enum class errorClass {AL_CREATE_DISPLAY_ERR, AL_CREATE_FONT_ERR, AL_CREATE_BITMAP_ERR, AL_CREATE_TIMER_ERR, AL_CREATE_EVQUEUE_ERR,
	AL_INIT_ERR, AL_INIT_DISPLAY_ERR, AL_INIT_PRIMITIVES_ERR, AL_INIT_KEYBOARD_ERR, AL_INIT_MOUSE_ERR, AL_INIT_IMAGE_ERR, AL_INIT_FONT_ERR,
	AL_INIT_TTF_ERR, NO_ERR, MOVEMENT_ERR,ERROR_N};


class lcdError
{
public:
	lcdError();

	std::string getErrorName();
	std::string getErrorDescription();
	errorClass getErrorCode();

	void setErrorCode(errorClass code);

private:

	std::string description;
	std::string name;
	errorClass code;


};
