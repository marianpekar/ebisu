#pragma once

#include <vector>
#include <memory>
#include <string>

#include "path_node.h"

struct Layer
{
    struct SDL_Texture* sprite = nullptr;
    std::vector<int> tile_map;
    int tiles_in_row;
    int sprite_sheet_cols;
    Layer(SDL_Texture* sprite, std::vector<int> tile_map, const int& tiles_in_row, const int sprite_sheet_cols) :
        sprite(sprite), tile_map(std::move(tile_map)), tiles_in_row(tiles_in_row), sprite_sheet_cols(sprite_sheet_cols) {}
    ~Layer();
    const int& GetTileAt(const int& i, const int& j) const { return tile_map[j * tiles_in_row + i]; }
};

class Map
{
private:
    struct SDL_Rect* src_rect = nullptr;
    SDL_Rect* dst_rect = nullptr;
    
    int tile_size = 0;
    int map_width = 0;
    int map_height = 0;
    int tiles_in_row = 0;
    int tiles_in_col = 0;
    
    std::vector<std::shared_ptr<Layer>> layers_back;
    std::vector<std::shared_ptr<Layer>> layers_front;
    std::vector<int> collision_map;
    int zero_index = 0;
    
    void Render(const std::shared_ptr<Layer>& layer) const;
    
    std::vector<PathNode*> path_nodes;
    
    static int GetDistance(const PathNode& node_a, const PathNode& node_b);
    PathNode* GetPathNodeFromWorldPosition(const Vector2& world_position) const;
    static std::vector<Vector2> RetracePath(const PathNode* start_node, PathNode* end_node);
public:
    Map();
    ~Map();
    void SetDimensions(int new_tile_size, int new_map_width, int new_map_height);
    void SetCollisionMap(const std::vector<int>& collision_map_data);
    void GeneratePathNodes();
    void AddLayer(const std::string& sprite_filepath, const std::vector<int>& tile_map, bool is_front);
    void Clear();
    
    void RenderBackLayers() const;
    void RenderFrontLayers() const;
    
    const int& GetTileSize() const { return tile_size; }
    const int& GetMapWidth() const { return map_width; }
    const int& GetMapHeight() const { return map_height; }
    const int& GetCollisionAt(const int& x, const int& y) const;
    const bool& HasCollisionAt(const Vector2& position, const float& width, const float& height) const;
    
    std::vector<Vector2> FindPath(const struct Vector2& start, const Vector2& end) const;

#if _DEBUG
    void Debug_RenderPathNodes() const;
#endif
};
