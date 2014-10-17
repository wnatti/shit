#pragma once
#include <Windows.h>
#include "lodepng.h"
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include "Sprite.h"

using namespace glm;

//VERTEX SOURCEEN TEXTUURIKORDINAATIT
//VÄLITYS FRAGMENT SHADERILLE
//UNIFORM SAMPLER 2D MUUTTUJA FRAGMENTILLE
//SAMPLERILLA VÄRI KÄYTTÄEN TEXTURIKORDINAATTIA
//<VERTEX DATAAN TEXTUURIKORDINAATIT
//<KÄYTÄ texture(...)
//<PIIRROSSA ENNEN GLDRAWELEMENTSIÄ BINDAA TEKSTUURI
//<SAMPLER UNIFORM NOLLAAN
//

static const GLchar* VERTEX_SOURCE =

"attribute vec2 attrPosition;\n"
"attribute vec3 attrColor;\n"
"attribute vec2 attrTex;\n"
"varying vec2 texCoord;\n"
"varying vec3 varyColor;\n"

"uniform mat4 unifProjection;"

"void main()\n"
"{	\n"
"	texCoord = attrTex;\n"
"	varyColor = attrColor;\n"
"	gl_Position = unifProjection * vec4(attrPosition, 0.0, 1.0);\n" //row-major vektori * matriisi, column-major matriisi * vektori
"}	\n";
static const GLchar* FRAGMENT_SOURCE =
"varying vec3 varyColor;\n"
"varying vec2 texCoord;\n"
"uniform sampler2D texture1;\n"

"void main()	\n"
"{	\n"
"	gl_FragColor = texture(texture1, texCoord) * vec4(varyColor,1.0f); \n"
"}	\n";

static const GLfloat VERTEX_DATA[] =
{
	
	0.0f, 0.0f,		//1st vertex
	1.0f, 0.0f, 0.0f,
	0.0f, 0.0f,

	0.0f, 1000.0f,		//2nd vertex
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f,

	1000.0f, 1000.0f,		//3rd vertex
	0.0f, 0.0f, 1.0f,
	1.0f, 1.0f,

	1000.0f, 0.0f,		//4th vertex
	0.5f,0.5f,0.5f,
	1.0f, 0.0f

};

static const GLuint INDEX_DATA[] =
{
	0, 1, 2,
	0, 2, 3
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
		rect.left = 0;
		rect.right = 1000;
		rect.top = 0;
		AdjustWindowRect(&rect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, true);
		//rect.bottom = 0;

	};

	void create(wchar_t* name)
	{		
		
		//gibbing some parametters like title for example,
		//also giving some default sizes for the window

		hwnd = CreateWindowEx(0 ,wstr, name,
			WS_CAPTION | WS_MINIMIZEBOX|WS_SYSMENU,
			0,0,rect.right-rect.left,rect.bottom-rect.top,
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
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{

				if (msg.message == WM_QUIT)
					perkele = !perkele;

				//clearing de window and drawing


				glClear(GL_COLOR_BUFFER_BIT);

				
				glUseProgram(pO);

			///	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
				
			//	glVertexAttribPointer(positionIndex, 2, GL_FLOAT, GL_FALSE, 28,
			//		reinterpret_cast<GLvoid*>(0)); // 0 tells which byte we want to start from when reading position

			//	glVertexAttribPointer(colorIndex, 3, GL_FLOAT, GL_FALSE, 28,
			//		reinterpret_cast<GLvoid*>(8)); // 8 tells which byte we want to start from when reading color

			////	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);

			//	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 28,
			//		reinterpret_cast<GLvoid*>(20)); // 20 tells which byte we want to start from when reading texture data

			//	glBindTexture(GL_TEXTURE_2D, texture);

			//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, //Triangles
			//		reinterpret_cast<GLvoid*>(0));

				sprite.draw();

			//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			//	glBindBuffer(GL_ARRAY_BUFFER, 0);
			//	glBindTexture(GL_TEXTURE_2D, 0);
				glUseProgram(0);

				SwapBuffers(hdc);

				TranslateMessage(&msg);
				DispatchMessage(&msg);

			}
			
		}
		glDeleteBuffers(2, buffers);
		glDeleteTextures(1, &texture);
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

		//	Enabling attribute array for position, color and texture coords

		//positionIndex = glGetAttribLocation(pO, "attrPosition");
		//assert(positionIndex >= 0);
		//glEnableVertexAttribArray(positionIndex);

		//colorIndex = glGetAttribLocation(pO, "attrColor");
		//assert(colorIndex >= 0);
		//glEnableVertexAttribArray(colorIndex);

		//texAttrib = glGetAttribLocation(pO, "attrTex");
		//assert(texAttrib >= 0);
		//glEnableVertexAttribArray(texAttrib);


		//

		//	Create uniform locations

		const GLint projectionLocation = glGetUniformLocation(pO, "unifProjection");
		assert(projectionLocation >= 0);

		//const GLint samplerLocation = glGetUniformLocation(pO, "unifDiffuseSample");
		//assert(samplerLocation >= 0);

		//

		//	Creating buffers
	/*	glGenBuffers(2, buffers);
		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		glBufferData(GL_ARRAY_BUFFER, 28 * sizeof(GLfloat), VERTEX_DATA, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), INDEX_DATA, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	*/	//For safety reasons, can mess something up if not binded to 0 (null)
		//

		//	Creating texture

		//filename = { "test.png" };
		//unsigned error = lodepng::decode(image, width, height, filename);
		//if (error != 0)
		//{
		//	MessageBoxA(hwnd, lodepng_error_text(error), "LODEPNG_ERROR", 0);
		//}

		//glGenTextures(1, &texture);
		//glBindTexture(GL_TEXTURE_2D, texture);
		//glTexImage2D(GL_TEXTURE_2D, 0, 4,width ,height,0,GL_RGBA,GL_UNSIGNED_BYTE,&image[0]);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //dis is importanssio
		//glBindTexture(GL_TEXTURE_2D, 0);
		//

		//

		//	Create Projection

		projection = ortho(0.0f, 1000.0f, 1000.0f,0.0f,-1.0f,1.0f);
		glUseProgram(pO);

		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, reinterpret_cast<const float*>(&projection));

		glUseProgram(0);


		//

		//	Enabling transparency for the texture
		sprite = Sprite(0, 0, 200, 300);
		sprite.giveProgram(pO);
		sprite.setTexture("test.png");
		
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		


	};

	void destroyRC()
	{
		wglMakeCurrent(hdc, hglrc = NULL);
		wglDeleteContext(hglrc);
	}

	~Window(){};

private:

	Sprite sprite;
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
	GLint positionIndex,colorIndex;
	GLint texAttrib;
	GLuint buffers[2];
	GLuint texture;
	mat4 projection;
	const char* filename;
	std::vector<unsigned char> image;
	unsigned width, height;
};

