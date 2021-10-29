#include <iostream>
#include "AppWindow.h"


int main()
{
	AppWindow app;
	if(app.init())
	{
		app.onCreate();

		while (app.isRun())
		{
			app.broadcast();
		}
	}

	return true;
}