#pragma once
#include <Windows.h>

static const GLchar* VERTEX_SOURCE =
"attribute vec2 attrPosition;\n"

"void main()\n"
"{	\n"
"gl_Position = 	vec4(attrPosition, 0.0, 1.0);	\n"
"}	\n";
static const GLchar* FRAGMENT_SOURCE =
"void main()	\n"
"{	\n"
"gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);	\n"
"}	\n";

static const GLfloat VERTEX_DATA[] =
{
	0.0f, 0.75f,	//1st vertex
	-0.75, -0.75f,	//2nd vertex
	0.75, -0.75f	//3rd vertex
};

static const GLuint INDEX_DATA[] =
{
	0, 1, 2
};

void nope(char* charArr, int arrSize)
{
	std::string errString;

	for (int i = 0; i < arrSize; i++)
	{
		errString += charArr[i];
	}

	MessageBoxA(0, errString.c_str(), "ERROR", 0);
}


LRESULT CALLBACK proc(HWND hw, UINT msg, WPARAM param, LPARAM lparam)
{

	//Function that processes messages sent to any window of assiociated class

	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		DestroyWindow(hw);
		break;
	case WM_CREATE:
		return 0;
	default:
		return DefWindowProc(hw, msg, param, lparam);
		break;

	};

}


class Window
{
public:


	Window()
	{

		//Making all the things needed for a succesful Registeration

		wstr = L"the_window";					// name for the window for some use i dont know what, something about unique names maybe
		wind.cbClsExtra = 0;
		wind.cbWndExtra = 0;
		wind.hbrBackground = 0;
		wind.hCursor = 0;
		wind.hIcon = 0;
		wind.hIconSm = 0;

		wind.lpfnWndProc = proc;				// this procedure thing is really interesting stuff
		instans = GetModuleHandle(nullptr);		
		wind.hInstance = instans;			

		wind.lpszMenuName = nullptr;
		wind.lpszClassName = wstr;
		wind.cbSize = sizeof(WNDCLASSEX);
		wind.style = CS_OWNDC;
		
		RegisterClassEx(&wind);					//Registers a window class with the attributes of WNDCLASSEX structure

		rect.bottom = 1000;
		rect.left = 100;
		rect.right = 1000;
		rect.top = 100;

	};

	void create(wchar_t* name)
	{		
		
		//gibbing some parametters like title for example,
		//also giving some default sizes for the window

		hwnd = CreateWindowEx(0 ,wstr, name,
			WS_CAPTION | WS_MINIMIZEBOX|WS_SYSMENU,
			CW_USEDEFAULT,CW_USEDEFAULT,1000,1000,
			NULL, NULL, instans, NULL);

	}

	void show()
	{
		ShowWindow(hwnd, 1);
	}

	void run()
	{
		
		//message loop

		bool perkele = true;
		while (perkele)
		{
			while (PeekMessage(&msg, NULL, 0, 0,PM_REMOVE))
			{

				if (msg.message == WM_QUIT)
				{
					perkele = !perkele;
					break;
				}

				//clearing de window
				

				glClear(GL_COLOR_BUFFER_BIT);

				glUseProgram(pO);
				glVertexAttribPointer(positionIndex, 2, GL_FLOAT,GL_FALSE,0,
					VERTEX_DATA);

				glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, INDEX_DATA);
				
				SwapBuffers(hdc);

				TranslateMessage(&msg);
				DispatchMessage(&msg);

			}

		}
	}

	void createDC()
	{
		hdc = GetDC(hwnd);
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL; //Doublebuffer is good, no screen tearing
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 24;
		pfd.cDepthBits = 24;

		ipf = ChoosePixelFormat(hdc, &pfd);
		SetPixelFormat(hdc, ipf, &pfd);

		hglrc = wglCreateContext(hdc);
		wglMakeCurrent(hdc, hglrc);

		glClearColor(0.f, 0.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		SwapBuffers(hdc);

		GLint err;
		err = glewInit();


		if (err != GLEW_OK)
		{
			MessageBoxA(0, "Something went horribly wrong", "Oh shit son", 0);
		}


		pO = glCreateProgram();

		//	Creating vertex shader
		sOv = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(sOv, 1, &VERTEX_SOURCE, nullptr);
		glCompileShader(sOv);

		glGetShaderiv(sOv, GL_COMPILE_STATUS, &result);

		if (result != GL_TRUE)
		{
			glGetShaderInfoLog(sOv, 0, 0, errChar);
			nope(errChar, 1024);
		}
		//

		//	Creating fragment shader
		sOf = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(sOf, 1, &FRAGMENT_SOURCE, nullptr);
		glCompileShader(sOf);

		glGetShaderiv(sOf, GL_COMPILE_STATUS, &result);

		if (result != GL_TRUE)
		{
			glGetShaderInfoLog(sOf, 0, 0, errChar);
			nope(errChar, 1024);
		}
		//

		//	Attaching shaders
		glAttachShader(pO, sOv);
		glAttachShader(pO, sOf);
		//

		//	Linking program
		glLinkProgram(pO);
		glGetProgramiv(pO, GL_LINK_STATUS, &result);

		if (result != GL_TRUE)
		{
			glGetProgramInfoLog(pO, 1024, 0, errChar);
			nope(errChar, 1024);
		}
		//

		//	Enabling attribute array

		positionIndex = glGetAttribLocation(pO, "attrPosition");
		assert(positionIndex >= 0);
		glEnableVertexAttribArray(positionIndex);
		//
	};

	void destroyRC()
	{
		wglMakeCurrent(hdc, hglrc = NULL);
		wglDeleteContext(hglrc);
	}

	~Window(){};

private:

	HGLRC hglrc;				//OpenGL Rendering Context handle
	HDC hdc;					//Device Context Handle
	HINSTANCE instans;			//Window Instance Handle
	WNDCLASSEX wind;			//Defines Window's class's attributes
	HWND hwnd;					//Handle to window
	wchar_t *wstr;				
	MSG msg;
	PIXELFORMATDESCRIPTOR pfd;	//Self-explanotary
	int ipf;					//The most suitable pixelformat given by ChoosePixelformat(..)
	RECT rect;
	GLint result;
	GLuint sOv, sOf;
	GLuint pO;
	char errChar[1024];
	GLint positionIndex;
};

