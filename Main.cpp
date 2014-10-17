#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <assert.h>
#include <glew.h>
#include <string>
#include "Window.h"




int main()
{
	Window dow;
	dow.create(L"lol");
	dow.show();
	dow.createDC();

	//	Running loop
	dow.run();

	return 0;
}