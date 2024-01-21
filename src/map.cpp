#include <unordered_set>
#include <SDL.h>
#include "map.h"
#include "texture_loader.h"
#include "path_node.h"
#include "heap.h"
#include "renderer.h"

Map::Map(const int tile_size, const int map_width, const int map_height, std::vector<int> collision_map) :
    tile_size(tile_size), map_width(map_width), map_height(map_height),
    tiles_in_row(map_width / tile_size), tiles_in_col(map_height / tile_size),
    collision_map(std::move(collision_map))
{
    src_rect = new SDL_Rect();
    dst_rect = new SDL_Rect();

    for (int y = 0; y < tiles_in_col; y++)
    {
        for (int x = 0; x < tiles_in_row; x++)
        {
            const int cell_index = y * tiles_in_row + x;
            auto* path_node = new PathNode(this->collision_map[cell_index] == 0,
                                           static_cast<float>(x * tile_size) + static_cast<float>(tile_size) / 2.0f,
                                           static_cast<float>(y * tile_size) + static_cast<float>(tile_size) / 2.0f,
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

                    if (this->collision_map[neighbour_index] == 1)
                    {
                        node->SetPCost(1e6);
                    }
                    
                    node->AddNeighbours(path_nodes[neighbour_index]);
                }
            }
        }
    }
}

void Map::AddLayer(const std::string& sprite_filepath, const std::vector<int>& tile_map, bool is_front)
{
    int image_width, image_height;
    SDL_Texture* sprite = TextureLoader::LoadTexture(sprite_filepath, Renderer::GetRenderer(), image_width, image_height);
    const int spritesheet_cols = image_width / tile_size;
    std::shared_ptr<Layer> layer = std::make_shared<Layer>(sprite, tile_map, tiles_in_row, spritesheet_cols);

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
    for (const auto& layer : layers_back)
    {
        Render(layer);
    }
}

void Map::RenderFrontLayers() const
{
    for (const auto& layer : layers_front)
    {
        Render(layer);
    }
}

std::vector<Vector2> Map::FindPath(const Vector2& start, const Vector2& end) const
{
    std::vector<Vector2> path;

    PathNode* start_node = GetPathNodeFromWorldPosition(start);
    PathNode* end_node = GetPathNodeFromWorldPosition(end);

    Heap<PathNode*> open_set(map_width * map_height);
    std::unordered_set<PathNode*> closed_set;
    open_set.Add(start_node);

    while (open_set.Count() > 0)
    {
        PathNode* current_node = open_set.RemoveFirst();
        closed_set.emplace(current_node);

        if (current_node == end_node)
        {
            path = RetracePath(start_node, end_node);
            return path;
        }

        for (auto& neighbour : current_node->GetNeighbours())
        {
            if (!neighbour->GetIsWalkable() || closed_set.contains(neighbour))
                continue;

            const int new_g_cost = current_node->GetGCost() + GetDistance(*current_node, *neighbour);
            if (new_g_cost < neighbour->GetGCost() || !open_set.Contains(neighbour))
            {
                neighbour->SetGCost(new_g_cost);
                neighbour->SetHCost(GetDistance(*neighbour, *end_node));
                neighbour->SetParent(current_node);

                if (!open_set.Contains(neighbour))
                {
                    open_set.Add(neighbour);
                }
            }
        }
    }

    return path;
}

PathNode* Map::GetPathNodeFromWorldPosition(const Vector2& world_position) const
{
    const int x = static_cast<int>(world_position.x / static_cast<float>(tile_size));
    const int y = static_cast<int>(world_position.y / static_cast<float>(tile_size));
    const size_t index = y * tiles_in_row + x;

    return path_nodes[index];
}

std::vector<Vector2> Map::RetracePath(const PathNode* start_node, PathNode* end_node)
{
    std::vector<Vector2> path;
    const PathNode* current_node = end_node;

    while (current_node != start_node)
    {
        path.emplace_back(current_node->GetWorldPosition());
        current_node = current_node->GetParent();
    }
    
    path.emplace_back(start_node->GetWorldPosition());
    
    std::reverse(path.begin(), path.end());
    
    return path;
}

int Map::GetDistance(const PathNode& node_a, const PathNode& node_b)
{
    const int dist_x = abs(node_a.GetMapX() - node_b.GetMapX());
    const int dist_y = abs(node_a.GetMapY() - node_b.GetMapY());

    return dist_x > dist_y ? 14 * dist_y + 10 * (dist_x - dist_y) : 14 * dist_x + 10 * (dist_y - dist_x);
}

void Map::Render(const std::shared_ptr<Layer>& layer) const
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

            dst_rect->x = x * tile_size - static_cast<int>(Renderer::TryGetCameraPosition().x);
            dst_rect->y = y * tile_size - static_cast<int>(Renderer::TryGetCameraPosition().y);
            dst_rect->w = tile_size;
            dst_rect->h = tile_size;

            SDL_RenderCopy(Renderer::GetRenderer(), layer->sprite, src_rect, dst_rect);
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
    
    layers_front.clear();
    layers_back.clear();
    path_nodes.clear();
    TextureLoader::ClearTextureCache();
}

Layer::~Layer()
{
    SDL_DestroyTexture(sprite);
}

#if _DEBUG
void Map::Debug_RenderPathNodes() const
{
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(Renderer::GetRenderer(), &r, &g, &b, &a);
    
    static const int quarter_of_tile_size = tile_size / 4;

    for (auto& node : path_nodes)
    {
        if (node->GetIsWalkable())
        {
            SDL_SetRenderDrawColor(Renderer::GetRenderer(), 0, 255, 0, 255);
        }
        else
        {
            SDL_SetRenderDrawColor(Renderer::GetRenderer(), 255, 0, 0, 255);
        }

        const auto* rect = new SDL_Rect{
            static_cast<int>(node->GetWorldPosition().x - Renderer::TryGetCameraPosition().x) - quarter_of_tile_size / 2,
            static_cast<int>(node->GetWorldPosition().y - Renderer::TryGetCameraPosition().y) - quarter_of_tile_size / 2,
            quarter_of_tile_size, quarter_of_tile_size
        };
        SDL_RenderDrawRect(Renderer::GetRenderer(), rect);
        delete rect;
    }
    
    SDL_SetRenderDrawColor(Renderer::GetRenderer(), r, g, b, a);
}
#endif
