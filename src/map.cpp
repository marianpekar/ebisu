#include "map.h"
#include "texture_loader.h"
#include "camera.h"
#include <SDL.h>

Map::Map(SDL_Renderer* renderer, const int tile_size, const int map_width, const int map_height, Camera* camera, std::vector<int> collision_map) :
    renderer(renderer), tile_size(tile_size),
    map_width(map_width), map_height(map_height),
    tiles_in_row(map_height / tile_size), tiles_in_col(map_width / tile_size),
    collision_map(std::move(collision_map)), camera(camera)
{
    src_rect = new SDL_Rect();
    dst_rect = new SDL_Rect();
}

void Map::AddLayer(const char* sprite_filepath, const std::vector<int>& tile_map, bool is_front)
{
    int image_width, image_height;
    SDL_Texture* sprite = TextureLoader::LoadTexture(sprite_filepath, renderer, image_width, image_height);
    const int spritesheet_cols = image_width / tile_size;
    auto layer = new Layer(sprite, tile_map, tiles_in_row, spritesheet_cols);

    if (is_front)
    {
        layers_front.emplace_back(layer);
    }
    else
    {
        layers_back.emplace_back(layer);
    }
}

void Map::RenderBackLayers() const
{
    for (const auto layer : layers_back)
    {
        Render(layer);
    }
}

void Map::RenderFrontLayers() const
{
    for (const auto layer : layers_front)
    {
        Render(layer);
    }
}

void Map::Render(const Layer* layer) const
{
    for (int j = 0; j < tiles_in_row; j++)
    {
        for (int i = 0; i < tiles_in_col; i++)
        {
            if (layer->tile_map[j * tiles_in_col + i] == -1)
                continue;
            
            src_rect->x = (layer->tile_map[j * tiles_in_col + i] % layer->spritesheet_cols) * tile_size;
            src_rect->y = (layer->tile_map[j * tiles_in_col + i] / layer->spritesheet_cols) * tile_size;
            src_rect->w = tile_size;
            src_rect->h = tile_size;

            dst_rect->x = i * tile_size - static_cast<int>(camera->GetPosition().x);
            dst_rect->y = j * tile_size - static_cast<int>(camera->GetPosition().y);
            dst_rect->w = tile_size;
            dst_rect->h = tile_size;

            SDL_RenderCopy(renderer, layer->sprite, src_rect, dst_rect);
        }
    }
}

const int& Map::GetCollisionAt(const int& i, const int& j) const
{
    const size_t index = j * tiles_in_col + i;

    if (index < 0 || index >= collision_map.size())
    {
        return zero_index;
    }

    return collision_map[index];
}

Map::~Map()
{
    delete src_rect;
    delete dst_rect;

    for (const auto& layer : layers_back)
    {
        delete layer;
    }
}

Layer::~Layer()
{
    SDL_DestroyTexture(sprite);
}
