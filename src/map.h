#pragma once

#include <vector>

struct Layer
{
	struct SDL_Texture* sprite = nullptr;
	std::vector<int> tile_map;
	const int tiles_in_row;
	const int spritesheet_cols;
	Layer(SDL_Texture* sprite, std::vector<int> tile_map, const int& tiles_in_row, const int spritesheet_cols) :
		sprite(sprite), tile_map(tile_map), tiles_in_row(tiles_in_row), spritesheet_cols(spritesheet_cols) {}
	~Layer();
	const int& GetTileAt(const int& i, const int& j) { return tile_map[j * tiles_in_row + i]; }
};

class Map
{
private:
	struct SDL_Rect* src_rect = nullptr;
	SDL_Rect* dst_rect = nullptr;
	struct SDL_Renderer* renderer;
	const int tile_size;
	const int map_size;
	const int tiles_in_row;
	std::vector<Layer*> layers;
	std::vector<int> collision_map;
	class Camera* camera;
public:
	Map(SDL_Renderer* renderer, const int tile_size, const int map_size, Camera* camera, std::vector<int> collision_map);
	~Map();
	void AddLayer(const char* sprite_filepath, std::vector<int> tile_map);
	void Render();
	const std::vector<Layer*>& GetLayers() { return layers; }
	const int& GetTileSize() { return tile_size; }
	const int& GetMapSize() { return map_size; }
	const int& GetCollisionAt(const int& i, const int& j);
};