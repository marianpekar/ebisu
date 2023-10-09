#pragma once

#include <vector>

#include "imgui.h"
#include "SDL_opengl.h"

struct TilemapLayer
{
	std::vector<int> data;
	bool is_front = false;
};

typedef std::vector<TilemapLayer> TilemapLayers;

struct Texture
{
	GLuint id;
	int width;
	int height;
};

class Editor
{
public:
	Editor() = default;
	~Editor();
	void Draw();
	
private:
	bool is_bank_window_init_size_set = false;
	
	bool paint_collision_map = false;
	bool lock_canvas_position = true;
	
	int selected_sprite_index = 0;
	size_t selected_tile_map_index = 0;
	
	std::vector<Texture*> bank_textures;
	void LoadTexture(const char* path);

	float tile_size = 64.0f;
	int new_level_tile_size = 64;

	int new_level_row_tile_count = 32;
	int row_tile_count = 16;

	int tilemap_paths_count = 2;
	std::vector<char*> new_level_tilemap_paths;
	bool new_level_tilemap_paths_dirty = true;
	
	TilemapLayers tile_maps;
	std::vector<int> collision_map;

	void DrawMainMenuBar();
	void DrawNewLevelPopup();

	bool openNewLevelPopup = false;
	
	void InitTilemapPathsInputFields();
	void DrawAddAndRemoveLayerButtons();
	void DisposeCurrentLevel();
	void CreateNewLevel();
	
	void DrawSpriteBank(const size_t& tile_map_index);
	void HandleSpriteSelection(const ImVec2& image_screen_pos, const float& tiles_in_col, const size_t& tile_map_index);
	bool IsPositionOutsideSpriteBank(ImVec2 mouse_pos_relative, const size_t& tile_map_index) const;
	void DrawSelectedSpriteRect(const int& index, const ImVec2& image_screen_pos, const float& tiles_in_col, const ImColor& color) const;

	void DrawCanvas();
	void DrawCanvasOptions();
	void DrawTilemap(ImVec2& current_cursor_pos) const;
	void HandleTilePaint(ImVec2 canvas_screen_pos);
	bool IsPositionOutsideCanvas(ImVec2 mouse_pos_relative) const;
	void DeleteBankTextures();
};
