#pragma once

#include <vector>

struct Layer
{
	struct SDL_Texture* sprite = nullptr;
	std::vector<int> tilemap;
	Layer(SDL_Texture* sprite, std::vector<int> tilemap) : sprite(sprite), tilemap(tilemap) {}
	~Layer() 
	{ 
		delete sprite; 
	}
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
public:
	Map(SDL_Renderer* renderer, const int tile_size, const int map_size);
	~Map();
	void AddLayer(const char* sprite_filepath, std::vector<int> tilemap);
	void Render();
};