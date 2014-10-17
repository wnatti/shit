#pragma once
#include <glew.h>
#include <string>
#include "lodepng.h"
#include <Windows.h>
#include <assert.h>
class Sprite
{
public:
	Sprite(int x, int y,int width,int height);
	Sprite(){};
	~Sprite();
	void draw();
	void setTexture(std::string filename);
	void giveProgram(GLuint poo)
	{
		_pO = poo;
	}

private:
	int _top, _bot, _left, _right, _width, _height;
	GLfloat* _vertexData;
	GLuint _buffers[2];
	GLuint _texture;
	unsigned int _textureWidth, _textureHeight;
	std::vector<unsigned char> _imageData;
	GLint _positionIndex, _colorIndex, _texAttrib;
	GLuint _pO;


};

