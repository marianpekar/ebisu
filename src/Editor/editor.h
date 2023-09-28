#pragma once

#include <vector>

#include "imgui.h"
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
	void DrawSelectedSpriteRect(const int& index, const ImVec2& image_screen_pos, const float& tiles_in_col, const ImColor& color) const;

private:
	bool is_bank_window_init_size_set = false;
	
	bool paint_collision_map = false;
	bool lock_canvas_position = true;
	
	int selected_sprite_index_lmb = 0;
	int selected_sprite_index_rmb = -1;
	Texture* bank_texture;
	static Texture* LoadTexture(const char* path);

	int tile_size = 64;
	int row_tile_count = 16;
	std::vector<int> tile_map;
	std::vector<int> collision_map;
	
	void DrawSpriteBank();
	void DrawCanvas();
};
