#pragma once

#include <map>
#include <string>
#include <vector>

#include "entity.h"
#include "uid_generator.h"
#include "imgui.h"
#include "SDL_opengl.h"

struct TilemapLayer
{
    std::vector<int> data;
    bool is_front = false;
};

typedef std::vector<TilemapLayer> TilemapLayers;

struct Texture
{
    GLuint id;
    int width;
    int height;
};

enum CanvasMode
{
    PaintTiles,
    PaintCollision,
    MoveEntities,
};

class Editor
{
public:
    Editor() = default;
    ~Editor();
    void Draw();

    float GetTileSize() const { return tile_size; }
    void SetTileSize(const float new_tile_size) { tile_size = new_tile_size; }
    
    int GetRowTileCount() const { return row_tile_count; }
    void SetRowTileCount(const int new_row_tile_count) { row_tile_count = new_row_tile_count; }
    
    int GetColumnTileCount() const { return col_tile_count; }
    void SetColumnTileCount(const int new_col_tile_count) { col_tile_count = new_col_tile_count; }
    
    std::vector<int> GetCollisionMap() const { return collision_map; }
    void SetCollisionMap(const std::vector<int>& new_collision_map) { collision_map = new_collision_map; }
    
    std::vector<TilemapLayer> GetTilemapLayers() const { return tile_maps; }
    std::vector<char*> GetTilemapPaths() const { return new_level_tilemap_paths; }
    std::vector<Entity*> GetEntities() const { return entities; }

    void AddEntityAt(Entity* entity, const size_t index);

    void AddTilemapLayer(const char* sprite_sheet_path, const bool is_front, const std::vector<int>& data);

private:
    UniqueIdGenerator uid_generator;
    bool is_bank_window_init_size_set = false;
    int selected_canvas_mode;
    bool show_entity_names_on_canvas = true;
    bool show_entity_sprites = true;

    int selected_sprite_index = 0;
    size_t selected_tile_map_index = 0;
    
    std::map<const char*, GLint> texture_indices_cache;
    std::vector<Texture*> textures;
    size_t LoadTexture(const char* path);

    float tile_size = 64.0f;
    int new_level_tile_size = 64;

    int new_level_row_tile_count = 16;
    int row_tile_count = 16;

    int new_level_col_tile_count = 16;
    int col_tile_count = 16;

    int tilemap_paths_count = 2;
    std::vector<char*> new_level_tilemap_paths;
    bool new_level_tilemap_paths_dirty = true;

    TilemapLayers tile_maps;
    std::vector<int> collision_map;

    void DrawMainMenuBar();
    void UpdateSelectedNewLevelTilemapPath();
    void DrawNewLevelPopup();
    void DrawSaveAsPopup();
    void DrawOpenPopup();

    bool is_level_loaded = false;
    
    bool open_new_level_popup = false;
    bool open_select_asset_popup = false;
    int selected_tilemap_input_field_index = 0;

    bool open_save_as_popup = false;
    char* level_file_name = new char[256]();

    bool open_open_popup = false;
    
    void InitTilemapPathsInputFields();

    int select_asset_popup_dir_level = 0;
    std::string current_assets_relative_subdir_path = "";
    std::string selected_asset_path;
    bool selected_asset_path_changed;
    size_t selected_path_property_index = 0;
    bool* open_previous_popup = nullptr;
    void DrawSelectAssetPopup();

    void DrawAddAndRemoveLayerButtons();

    void DisposeCurrentLevel();
    void CreateNewLevel();

    void DrawSpriteBank(const size_t& tile_map_index);
    void HandleSpriteSelection(const ImVec2& image_screen_pos, const float& tiles_in_col, const size_t& tile_map_index);
    bool IsPositionOutsideSpriteBank(ImVec2 mouse_pos_relative, const size_t& tile_map_index) const;
    void DrawSelectedSpriteRect(const int& index, const ImVec2& image_screen_pos, const float& tiles_in_col,
                                const ImColor& color) const;

    void DrawCanvas();
    void DrawTilemaps(ImVec2& canvas_screen_pos) const;
    void DrawTilemapLayer(const ImVec2& canvas_screen_pos, ImVec2 current_cursor_pos, size_t i) const;
    void CalculateSelectedTileUVs(size_t i, int row, int col, ImVec2& uv0, ImVec2& uv1) const;
    
    void DrawCanvasOptions();
    
    void DrawEntitiesOnCanvas(const ImVec2& canvas_screen_pos);
    void HandleCanvasMouseInteraction(ImVec2 canvas_screen_pos);
    bool IsPositionOutsideCanvas(ImVec2 mouse_pos_relative) const;
    bool IsMouseHoveringOverEntityLabel(const ImVec2& canvas_screen_pos) const;
    
    std::vector<Entity*> entities;
    size_t selected_entity_index = -1;

    void DrawEntitiesWindow();
    void DrawSelectedEntityGeneralProperties(Entity* entity);

    void DrawSelectedEntityComponentsWindow();
    void DrawSelectedEntityComponentProperties(Entity* entity);
    void DrawAddComponentDropdownAndAddButton(Entity* selected_entity);

    void DeleteBankTextures();
    void DeleteEntities();
};
