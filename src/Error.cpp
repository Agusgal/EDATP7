#include "Error.h"

lcdError::lcdError()
{

	this->code = errorClass::NO_ERR;
	this->name = "No error.";
	this->description = "There is no error";

}

errorClass lcdError::getErrorCode(void)
{
	return this->code;
}

std::string lcdError::getErrorDescription(void)
{
	return this->description;
}

std::string lcdError::getErrorName(void)
{
	return this->name;
}


void lcdError::setErrorCode(errorClass code)
{
	this->code = code;

	switch (code)
	{
	case errorClass::AL_INIT_ERR:
		this->name = "Allegro init error";
		this->description = "Failed to initialize Allegro 5!";
		break;
	case errorClass::AL_INIT_DISPLAY_ERR:
		this->name = "Allegro display init error";
		this->description = "Failed to initialize Allegro display module!";
		break;
	case errorClass::AL_INIT_PRIMITIVES_ERR:
		this->name = "Allegro primitives error";
		this->description = "Failed to initialize Allegro primitives module!";
		break;
	case errorClass::AL_INIT_MOUSE_ERR:
		this->name = "Allegro Mouse error";
		this->description = "Failed to initialize Allegro Mouse module!";
		break;
	case errorClass::AL_INIT_IMAGE_ERR:
		this->name = "Allegro Image error";
		this->description = "Failed to initialize Allegro image module!";
		break;
	case errorClass::AL_INIT_KEYBOARD_ERR:
		this->name = "Allegro keyboard error";
		this->description = "Failed to initialize Allegro keyboard module!";
		break;
	case errorClass::AL_CREATE_DISPLAY_ERR:
		this->name = "Allegro display creation error";
		this->description = "Failed to create Allegro display!";
	case errorClass::AL_CREATE_TIMER_ERR:
		this->name = "Allegro timer creation error";
		this->description = "Failed to create Allegro timer!";
		break;
	case errorClass::AL_CREATE_EVQUEUE_ERR:
		this->name = "Allegro event queue creation error";
		this->description = "Failed to create Allegro event queue!";
		break;

	case errorClass::MOVEMENT_ERR:
		this->name = "Movement Error";
		this->description = "Not able to move the cursor!";
		break;
	case errorClass::NO_ERR:
		this->name = "No Error";
		this->description = "No Error";
		break;
	default:
		this->name = "Unspecified Error";
		this->description = "Unspecified Error";

		break;
	}
}
