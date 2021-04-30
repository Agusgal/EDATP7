#include "../lib/ImGui/imgui.h"
#include "../lib/ImGui/imgui_impl_allegro5.h"

#include "Gui.h"


int main(void)
{
	Gui myGui;

	myGui.initAll();
	while (!myGui.isOver() && myGui.noError())
	{
		if (myGui.getEvent())
		{
			myGui.dispatcher();
		}
	}
	myGui.destroyAll();

	return 0;
}