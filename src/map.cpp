#include "map.h"
#include "texture_loader.h"
#include "camera.h"
#include "path_node.h"
#include <SDL.h>

Map::Map(SDL_Renderer* renderer, const int tile_size, const int map_width, const int map_height, Camera* camera,
         std::vector<int> collision_map) :
    renderer(renderer), tile_size(tile_size),
    map_width(map_width), map_height(map_height),
    tiles_in_row(map_width / tile_size), tiles_in_col(map_height / tile_size),
    collision_map(std::move(collision_map)), camera(camera)
{
    src_rect = new SDL_Rect();
    dst_rect = new SDL_Rect();

    for (int y = 0; y < tiles_in_col; y++)
    {
        for (int x = 0; x < tiles_in_row; x++)
        {
            const int cell_index = y * tiles_in_row + x;
            auto* path_node = new PathNode(this->collision_map[cell_index] == 0,
                                           static_cast<float>(x * tile_size),
                                           static_cast<float>(y * tile_size),
                                           x, y);
            
            path_nodes.emplace_back(path_node);
        }
    }

    for (const auto& node : path_nodes)
    {
        for (int y = -1; y <= 1; y++)
        {
            for (int x = -1; x <= 1; x++)
            {
                if (y == 0 && x == 0)
                    continue;

                const int neighbour_map_x = node->GetMapX() + x;
                const int neighbour_map_y = node->GetMapY() + y;

                if (neighbour_map_x >= 0 && neighbour_map_x < tiles_in_row &&
                    neighbour_map_y >= 0 && neighbour_map_y < tiles_in_col)
                {
                    const int neighbour_index = neighbour_map_y * tiles_in_row + neighbour_map_x;
                    node->AddNeighbours(path_nodes[neighbour_index]);
                }
            }
        }
    }
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

void Map::Debug_RenderPathNodes() const
{
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

    for (auto& node : path_nodes)
    {
        if (node->GetIsWalkable())
        {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        }

        const int quarter_of_tile_size = tile_size / 4;
        const auto* rect = new SDL_Rect{
            static_cast<int>(node->GetWorldPosition().x - camera->GetPosition().x + static_cast<float>(quarter_of_tile_size) * 1.5f),
            static_cast<int>(node->GetWorldPosition().y - camera->GetPosition().y + static_cast<float>(quarter_of_tile_size) * 1.5f),
            quarter_of_tile_size, quarter_of_tile_size 
        };
        SDL_RenderDrawRect(renderer, rect);

        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    }
    
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void Map::Render(const Layer* layer) const
{
    for (int y = 0; y < tiles_in_col; y++)
    {
        for (int x = 0; x < tiles_in_row; x++)
        {
            if (layer->tile_map[y * tiles_in_row + x] == -1)
                continue;

            src_rect->x = (layer->tile_map[y * tiles_in_row + x] % layer->spritesheet_cols) * tile_size;
            src_rect->y = (layer->tile_map[y * tiles_in_row + x] / layer->spritesheet_cols) * tile_size;
            src_rect->w = tile_size;
            src_rect->h = tile_size;

            dst_rect->x = x * tile_size - static_cast<int>(camera->GetPosition().x);
            dst_rect->y = y * tile_size - static_cast<int>(camera->GetPosition().y);
            dst_rect->w = tile_size;
            dst_rect->h = tile_size;

            SDL_RenderCopy(renderer, layer->sprite, src_rect, dst_rect);
        }
    }
}

const int& Map::GetCollisionAt(const int& x, const int& y) const
{
    const size_t index = y * tiles_in_row + x;

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
    layers_front.clear();

    for (const auto& layer : layers_front)
    {
        delete layer;
    }
    layers_back.clear();

    for (const auto& node : path_nodes)
    {
        delete node;
    }
    path_nodes.clear();
}

Layer::~Layer()
{
    SDL_DestroyTexture(sprite);
}
