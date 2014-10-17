#include "Sprite.h"


Sprite::Sprite(int x, int y, int width, int height)
{
	_left = x;
	_top = y;
	_width = width;
	_height = height;
	_bot = _top + _height;
	_right = _left + width;

	GLfloat vertexData[] =
	{
		_left, _top,		//1st vertex
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f,

		_left, _bot,		//2nd vertex
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f,

		_right, _bot,		//3rd vertex
		0.0f, 0.0f, 1.0f,
		1.0f, 1.0f,

		1000.0f, 0.0f,		//4th vertex
		0.5f, 0.5f, 0.5f,
		_right, _top
	};

	_vertexData = vertexData;

	GLuint indexData[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	glGenBuffers(2, _buffers);
	glBindBuffer(GL_ARRAY_BUFFER, _buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, 28 * sizeof(GLfloat), _vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}


void Sprite::setTexture(std::string filename)
{

	_positionIndex = glGetAttribLocation(_pO, "attrPosition");
	assert(_positionIndex >= 0);
	glEnableVertexAttribArray(_positionIndex);

	_colorIndex = glGetAttribLocation(_pO, "attrColor");
	assert(_colorIndex >= 0);
	glEnableVertexAttribArray(_colorIndex);

	_texAttrib = glGetAttribLocation(_pO, "attrTex");
	assert(_texAttrib >= 0);
	glEnableVertexAttribArray(_texAttrib);



	unsigned error = lodepng::decode(_imageData, _textureWidth, _textureHeight, filename);
	
	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, _textureWidth, _textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &_imageData[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //dis is importanssio
	glBindTexture(GL_TEXTURE_2D, 0);


	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Sprite::draw()
{



	glVertexAttribPointer(_positionIndex, 2, GL_FLOAT, GL_FALSE, 28,
		reinterpret_cast<GLvoid*>(0));


	glVertexAttribPointer(_colorIndex, 3, GL_FLOAT, GL_FALSE, 28,
		reinterpret_cast<GLvoid*>(8));

	glVertexAttribPointer(_texAttrib, 2, GL_FLOAT, GL_FALSE, 28,
		reinterpret_cast<GLvoid*>(20));

	glBindBuffer(GL_ARRAY_BUFFER, _buffers[0]);

	glBindTexture(GL_TEXTURE_2D, _texture);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, //Triangles
		reinterpret_cast<GLvoid*>(0));

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


}


Sprite::~Sprite()
{
}
