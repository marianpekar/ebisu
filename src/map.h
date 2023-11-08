#pragma once

#include <vector>

#include "path_node.h"

struct Layer
{
    struct SDL_Texture* sprite = nullptr;
    std::vector<int> tile_map;
    int tiles_in_row;
    int spritesheet_cols;
    Layer(SDL_Texture* sprite, std::vector<int> tile_map, const int& tiles_in_row, const int spritesheet_cols) :
        sprite(sprite), tile_map(std::move(tile_map)), tiles_in_row(tiles_in_row), spritesheet_cols(spritesheet_cols) {}
    ~Layer();
    const int& GetTileAt(const int& i, const int& j) const { return tile_map[j * tiles_in_row + i]; }
};

class Map
{
private:
    struct SDL_Rect* src_rect = nullptr;
    SDL_Rect* dst_rect = nullptr;
    struct SDL_Renderer* renderer;
    int tile_size;
    int map_width;
    int map_height;
    int tiles_in_row;
    int tiles_in_col;
    std::vector<Layer*> layers_back;
    std::vector<Layer*> layers_front;
    std::vector<int> collision_map;
    class Camera* camera;
    int zero_index = 0;
    const Vector2 zero_vector = Vector2(0.f,0.f);
    void Render(const Layer* layer) const;
    
    std::vector<PathNode*> path_nodes;
    static int GetDistance(const PathNode& node_a, const PathNode& node_b);
    PathNode* GetPathNodeFromWorldPosition(const Vector2& world_position) const;
    static std::vector<Vector2> SimplifyPath(std::vector<Vector2> path);
    static std::vector<Vector2> RetracePath(const PathNode* start_node, PathNode* end_node);
    const Vector2& TryGetCameraPosition() const;
public:
    Map(SDL_Renderer* renderer, int tile_size, int map_width, int map_height, std::vector<int> collision_map);
    ~Map();
    void AddLayer(const char* sprite_filepath, const std::vector<int>& tile_map, bool is_front);
    void RenderBackLayers() const;
    void RenderFrontLayers() const;
    const int& GetTileSize() const { return tile_size; }
    const int& GetMapWidth() const { return map_width; }
    const int& GetMapHeight() const { return map_height; }
    const int& GetCollisionAt(const int& x, const int& y) const;
    void SetCamera(Camera* new_camera) { this->camera = new_camera; }
    
    std::vector<Vector2> FindPath(const struct Vector2& start, const Vector2& end);

#if _DEBUG
    void Debug_RenderPathNodes() const;
#endif
    
    std::vector<Vector2> debug_current_path;
};
