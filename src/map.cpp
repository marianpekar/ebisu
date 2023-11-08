#include "map.h"
#include "texture_loader.h"
#include "ECS/Components/camera.h"
#include "path_node.h"
#include <unordered_set>
#include <SDL.h>
#include "heap.h"

Map::Map(SDL_Renderer* renderer, const int tile_size, const int map_width, const int map_height, std::vector<int> collision_map) :
    renderer(renderer), tile_size(tile_size),
    map_width(map_width), map_height(map_height),
    tiles_in_row(map_width / tile_size), tiles_in_col(map_height / tile_size),
    collision_map(std::move(collision_map)), camera(nullptr)
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

std::vector<PathNode*> Map::FindPath(const Vector2& start, const Vector2& end)
{
    std::vector<PathNode*> path;

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
            debug_current_path = RetracePath(start_node, end_node);
            path = debug_current_path; // TODO: This is temporary, later debug draw path in Agent instead
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

std::vector<PathNode*> Map::RetracePath(PathNode* start_node, PathNode* end_node)
{
    std::vector<PathNode*> path;
    PathNode* current_node = end_node;

    while (current_node != start_node)
    {
        path.emplace_back(current_node);
        current_node = current_node->GetParent();
    }
    
    path = SimplifyPath(path);
    
    path.emplace_back(start_node);
    
    std::reverse(path.begin(), path.end());
    
    return path;
}

std::vector<PathNode*> Map::SimplifyPath(std::vector<PathNode*> path)
{
    std::vector<PathNode*> simplified_path;
    auto current_direction = Vector2(0.f,0.f);
    for (size_t i = 1; i < path.size(); i++)
    {
        auto next_direction = Vector2(path[i-1]->GetWorldPosition().x - path[i]->GetWorldPosition().x,path[i-1]->GetWorldPosition().y - path[i]->GetWorldPosition().y);
        if (next_direction != current_direction)
        {
            simplified_path.emplace_back(path[i]);
        }
        current_direction = next_direction;
    }
    return simplified_path;
}

const Vector2& Map::TryGetCameraPosition() const
{
    if(camera == nullptr)
        return zero_vector;

    return camera->GetPosition();
}

int Map::GetDistance(const PathNode& node_a, const PathNode& node_b)
{
    const int dist_x = abs(node_a.GetMapX() - node_b.GetMapX());
    const int dist_y = abs(node_a.GetMapY() - node_b.GetMapY());

    return dist_x > dist_y ? 14 * dist_y + 10 * (dist_x - dist_y) : 14 * dist_x + 10 * (dist_y - dist_x);
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

            dst_rect->x = x * tile_size - static_cast<int>(TryGetCameraPosition().x);
            dst_rect->y = y * tile_size - static_cast<int>(TryGetCameraPosition().y);
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

#if _DEBUG
void Map::Debug_RenderPathNodes() const
{
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
    const int quarter_of_tile_size = tile_size / 4;

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

        const auto* rect = new SDL_Rect{
            static_cast<int>(node->GetWorldPosition().x - TryGetCameraPosition().x) - quarter_of_tile_size / 2,
            static_cast<int>(node->GetWorldPosition().y - TryGetCameraPosition().y) - quarter_of_tile_size / 2,
            quarter_of_tile_size, quarter_of_tile_size
        };
        SDL_RenderDrawRect(renderer, rect);
        delete rect;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    for (size_t i = 0; i < debug_current_path.size() - 1; i++)
    {
        SDL_RenderDrawLine(renderer,
                           debug_current_path[i]->GetWorldPosition().x - TryGetCameraPosition().x,
                           debug_current_path[i]->GetWorldPosition().y - TryGetCameraPosition().y - quarter_of_tile_size
                           / 2,
                           debug_current_path[i + 1]->GetWorldPosition().x - TryGetCameraPosition().x,
                           debug_current_path[i + 1]->GetWorldPosition().y - TryGetCameraPosition().y -
                           quarter_of_tile_size / 2);
    }

    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}
#endif
