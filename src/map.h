#pragma once

#include <vector>

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
    int map_size;
    int tiles_in_row;
    std::vector<Layer*> layers_back;
    std::vector<Layer*> layers_front;
    std::vector<int> collision_map;
    class Camera* camera;
    int zero_index = 0;
    void Render(const Layer* layer) const;
public:
    Map(SDL_Renderer* renderer, int tile_size, int map_size, Camera* camera, std::vector<int> collision_map);
    ~Map();
    void AddLayer(const char* sprite_filepath, const std::vector<int>& tile_map, bool is_front);
    void RenderBackLayers() const;
    void RenderFrontLayers() const;
    const int& GetTileSize() const { return tile_size; }
    const int& GetMapSize() const { return map_size; }
    const int& GetCollisionAt(const int& i, const int& j) const;
};