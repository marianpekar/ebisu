#pragma once

#include "map.h"
#include "texture_loader.h"
#include <SDL.h>

Map::Map(SDL_Renderer* renderer, const int tile_size, const int map_size) :
renderer(renderer), tile_size(tile_size), map_size(map_size), tiles_in_row(map_size / tile_size)
{
	src_rect = new SDL_Rect();
	dst_rect = new SDL_Rect();
}

void Map::AddLayer(const char* sprite_filepath, std::vector<int> tilemap)
{
	SDL_Texture* sprite = TextureLoader::LoadTexture(sprite_filepath, renderer);
	Layer* layer = new Layer(sprite, tilemap);
	layers.emplace_back(layer);
}

void Map::Render()
{
	for (auto& layer : layers)
	{
		for (size_t y = 0; y < tiles_in_row; y++)
		{
			for (size_t x = 0; x < tiles_in_row; x++)
			{
				src_rect->x = layer->tilemap[y * tiles_in_row + x] * tile_size;
				src_rect->y = 0;
				src_rect->w = tile_size;
				src_rect->h = tile_size;

				dst_rect->x = x * tile_size;
				dst_rect->y = y * tile_size;
				dst_rect->w = tile_size;
				dst_rect->h = tile_size;

				SDL_RenderCopy(renderer, layer->sprite, src_rect, dst_rect);
			}
		}
	}
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
