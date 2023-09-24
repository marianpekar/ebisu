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
	bool is_bank_window_init_size_set = false;
	int selected_sprite_index = 0;
	Texture* bank_texture;
	static Texture* LoadTexture(const char* path);
	
	void DrawSpriteBank();
};
