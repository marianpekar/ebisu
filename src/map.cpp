#pragma once

#include "map.h"
#include "texture_loader.h"
#include "camera.h"
#include <SDL.h>

Map::Map(SDL_Renderer* renderer, const int tile_size, const int map_size, Camera* camera, std::vector<int> collision_map) :
	renderer(renderer), tile_size(tile_size), map_size(map_size), tiles_in_row(map_size / tile_size),
	camera(camera), collision_map(collision_map)
{
	src_rect = new SDL_Rect();
	dst_rect = new SDL_Rect();
}

void Map::AddLayer(const char* sprite_filepath, std::vector<int> tile_map)
{
	int image_width, image_height;
	SDL_Texture* sprite = TextureLoader::LoadTexture(sprite_filepath, renderer, image_width, image_height);
	int spritesheet_cols = image_width / tile_size;
	Layer* layer = new Layer(sprite, tile_map, tiles_in_row, spritesheet_cols);
	layers.emplace_back(layer);
}

void Map::Render()
{
	for (auto& layer : layers)
	{
		for (size_t j = 0; j < tiles_in_row; j++)
		{
			for (size_t i = 0; i < tiles_in_row; i++)
			{
				src_rect->x = (layer->tile_map[j * tiles_in_row + i] % layer->spritesheet_cols) * tile_size;
				src_rect->y = (layer->tile_map[j * tiles_in_row + i] / layer->spritesheet_cols) * tile_size;
				src_rect->w = tile_size;
				src_rect->h = tile_size;

				dst_rect->x = i * tile_size - camera->GetPosition().x;
				dst_rect->y = j * tile_size - camera->GetPosition().y;
				dst_rect->w = tile_size;
				dst_rect->h = tile_size;

				SDL_RenderCopy(renderer, layer->sprite, src_rect, dst_rect);
			}
		}
	}
}

const int& Map::GetCollisionAt(const int& i, const int& j)
{
	size_t index = j * tiles_in_row + i;

	if (index < 0 || index >= collision_map.size())
		return 0;

	return collision_map[index];
}

Map::~Map()
{
	delete src_rect;
	delete dst_rect;

	for (auto& layer : layers)
	{
		delete layer;
	}
}

Layer::~Layer()
{
	SDL_DestroyTexture(sprite);
}
