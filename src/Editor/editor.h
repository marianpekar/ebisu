#pragma once

#include "SDL_opengl.h"

struct Texture
{
	GLuint id;
	int width;
	int height;
};

class Editor
{
public:
	Editor();
	~Editor();
	void Draw();
private:
	Texture* bank_texture;
	static Texture* LoadTexture(const char* path);
	
	void DrawSpriteBank();
};
