#pragma once

#include "Gui.h"

class App
{
public:

	App();
	~App();

	//methods
	bool getEvent(void);

private:

	Gui appgui;


};