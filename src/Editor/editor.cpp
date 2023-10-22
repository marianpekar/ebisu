#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "editor.h"

#include <format>
#include <filesystem>
namespace fs = std::filesystem;

#include <SDL_opengl.h>

#include "imgui.h"

const std::string project_path =
#if _DEBUG
    "./../../";
#else
	"./"
#endif

const std::string assets_path = project_path + "assets";

void Editor::LoadTexture(const char* path)
{
    const auto texture = new Texture();

    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(std::format("{}/{}", assets_path.c_str(), path).c_str(),
                                          &image_width, &image_height, nullptr, 4);

    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    texture->id = texture_id;
    texture->width = image_width;
    texture->height = image_height;

    bank_textures.emplace_back(texture);
}

void Editor::Draw()
{
    DrawMainMenuBar();
    
    if (openNewLevelPopup)
    {
        DrawNewLevelPopup();
    }
    
    if (openSelectAssetPopup)
    {
        DrawSelectAssetPopup();
    }

    if (tile_maps.empty())
        return;

    for (size_t i = 0; i < tile_maps.size(); i++)
    {
        DrawSpriteBank(i);
    }

    DrawCanvas();
    DrawCanvasOptions();

    DrawEntityList();
    DrawSelectedEntityComponentsList();
}

void Editor::DrawMainMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Level"))
            {
                openNewLevelPopup = true;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void Editor::DrawNewLevelPopup()
{
    ImGui::OpenPopup("Create New Level");
    if (ImGui::BeginPopupModal("Create New Level", &openNewLevelPopup))
    {
        ImGui::SliderInt("Rows", &new_level_row_tile_count, 1, 256);
        ImGui::SliderInt("Columns", &new_level_col_tile_count, 1, 256);
        ImGui::SliderInt("Tile Size", &new_level_tile_size, 8, 64);

        DrawAddAndRemoveLayerButtons();
        if (new_level_tilemap_paths_dirty)
        {
            InitTilemapPathsInputFields();
            new_level_tilemap_paths_dirty = false;
        }

        for (int i = 0; i < tilemap_paths_count; ++i)
        {
            if (ImGui::Button(std::format("...###{}", i).c_str()))
            {
                selected_tilemap_input_field_index = i;
                openNewLevelPopup = false;
                openSelectAssetPopup = true;
            }
            ImGui::SameLine();
            ImGui::InputText(std::format("Tilemap Path {}", i).c_str(), new_level_tilemap_paths[i], 256);
        }

        if (ImGui::Button("Confirm"))
        {
            DisposeCurrentLevel();
            CreateNewLevel();
            openNewLevelPopup = false;
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
        {
            openNewLevelPopup = false;
        }

        ImGui::EndPopup();
    }
}

void Editor::DrawAddAndRemoveLayerButtons()
{
    if (ImGui::Button("Add Layer"))
    {
        tilemap_paths_count++;
        new_level_tilemap_paths_dirty = true;
    }

    if (tilemap_paths_count > 1)
    {
        ImGui::SameLine();
        if (ImGui::Button("Remove Layer"))
        {
            tilemap_paths_count--;
            new_level_tilemap_paths_dirty = true;
        }
    }
}

void Editor::DrawSelectAssetPopup()
{
    ImGui::OpenPopup("Select Asset");
    if (ImGui::BeginPopupModal("Select Asset", &openSelectAssetPopup))
    {
        if (select_asset_popup_dir_level > 0)
        {
            if (ImGui::Button(".."))
            {
                const size_t found = current_assets_relative_subdir_path.find_last_of('/');
                if (found != std::string::npos)
                {
                    current_assets_relative_subdir_path = current_assets_relative_subdir_path.substr(0, found);
                    select_asset_popup_dir_level--;
                    ImGui::EndPopup();
                    return;
                }
            }
        }

        const std::string current_asset_path = assets_path + current_assets_relative_subdir_path;
        for (const auto& entry : fs::directory_iterator(current_asset_path))
        {
            if (is_regular_file(entry))
            {
                std::string filename = entry.path().filename().string();
                if (ImGui::Button(filename.c_str()))
                {
                    std::string selected_asset_path = current_assets_relative_subdir_path.empty()
                                                          ? filename
                                                          : std::format("{}/{}", current_assets_relative_subdir_path,
                                                                        filename);

                    if (selected_asset_path[0] == '/')
                    {
                        selected_asset_path = selected_asset_path.substr(1);
                    }

                    const size_t source_length = strlen(selected_asset_path.c_str());
                    strncpy_s(new_level_tilemap_paths[selected_tilemap_input_field_index],
                              source_length + 1,
                              selected_asset_path.c_str(),
                              source_length);

                    openSelectAssetPopup = false;
                    openNewLevelPopup = true;
                }
            }
            else if (is_directory(entry))
            {
                std::string dir_name = entry.path().filename().string();
                if (ImGui::Button(std::format("[D] {}", dir_name).c_str()))
                {
                    select_asset_popup_dir_level++;
                    current_assets_relative_subdir_path += std::format("/{}", dir_name);
                }
            }
        }
        ImGui::EndPopup();
    }
}

void Editor::InitTilemapPathsInputFields()
{
    for (const auto& new_level_tilemap_path : new_level_tilemap_paths)
    {
        delete new_level_tilemap_path;
    }
    new_level_tilemap_paths.clear();

    new_level_tilemap_paths = std::vector<char*>(tilemap_paths_count);
    for (auto& new_level_tilemap_path : new_level_tilemap_paths)
    {
        new_level_tilemap_path = new char[256];
        for (int i = 0; i < 256; i++)
        {
            new_level_tilemap_path[i] = '\0';
        }
    }
}

void Editor::DisposeCurrentLevel()
{
    for (auto& tile_map : tile_maps)
    {
        tile_map.data.clear();
    }
    tile_maps.clear();
    collision_map.clear();

    DeleteEntities();
    DeleteBankTextures();
}

void Editor::CreateNewLevel()
{
    row_tile_count = new_level_row_tile_count;
    col_tile_count = new_level_col_tile_count;
    tile_size = static_cast<float>(new_level_tile_size);
    
    for (const auto& path : new_level_tilemap_paths)
    {
        LoadTexture(path);
        tile_maps.emplace_back();
    }

    for (int j = 0; j < tile_maps.size(); j++)
    {
        int initial_tile_index = j == 0 ? 0 : -1;
        for (int i = 0; i < row_tile_count * col_tile_count; i++)
        {
            tile_maps[j].data.push_back(initial_tile_index);
        }
    }

    for (int i = 0; i < row_tile_count * col_tile_count; i++)
    {
        collision_map.push_back(0);
    }
}

void Editor::DrawSpriteBank(const size_t& tile_map_index)
{
    if (!is_bank_window_init_size_set)
    {
        constexpr static auto offset_width = 16.0f;
        constexpr static auto offset_height = 40.0f;
        static auto window_size = ImVec2(static_cast<float>(bank_textures[tile_map_index]->width) + offset_width,
                                         static_cast<float>(bank_textures[tile_map_index]->height) + offset_height);
        ImGui::SetNextWindowSize(window_size);
        is_bank_window_init_size_set = true;
    }

    ImGui::Begin(std::format("Sprite Bank #{}", tile_map_index).c_str(), nullptr);

    const ImVec2 image_size(static_cast<float>(bank_textures[tile_map_index]->width),
                            static_cast<float>(bank_textures[tile_map_index]->height));
    const ImVec2 image_screen_pos = ImGui::GetCursorScreenPos();

    ImGui::Image(reinterpret_cast<ImTextureID>(bank_textures[tile_map_index]->id), image_size);
    // NOLINT(performance-no-int-to-ptr)

    const float tiles_in_col = static_cast<float>(bank_textures[tile_map_index]->height) / tile_size;
    HandleSpriteSelection(image_screen_pos, tiles_in_col, tile_map_index);

    if (selected_tile_map_index == tile_map_index)
    {
        DrawSelectedSpriteRect(selected_sprite_index, image_screen_pos, tiles_in_col, ImColor(1.0f, 0.0f, 0.0f));
    }

    ImGui::End();
}

void Editor::HandleSpriteSelection(const ImVec2& image_screen_pos, const float& tiles_in_col,
                                   const size_t& tile_map_index)
{
    const ImVec2 mouse_position = ImGui::GetMousePos();
    const auto mouse_pos_relative = ImVec2(mouse_position.x - image_screen_pos.x,
                                           mouse_position.y - image_screen_pos.y);

    if (IsPositionOutsideSpriteBank(mouse_pos_relative, tile_map_index))
        return;

    const int i = static_cast<int>(mouse_pos_relative.x / tile_size);
    const int j = static_cast<int>(mouse_pos_relative.y / tile_size);
    const int index = j * static_cast<int>(tiles_in_col) + i;

    if (ImGui::IsMouseClicked(0))
    {
        selected_sprite_index = selected_sprite_index == index ? -1 : index;
        selected_tile_map_index = tile_map_index;
    }
}

bool Editor::IsPositionOutsideSpriteBank(const ImVec2 mouse_pos_relative, const size_t& tile_map_index) const
{
    return mouse_pos_relative.x < 0 || mouse_pos_relative.x >= static_cast<float>(bank_textures[tile_map_index]->width)
        ||
        mouse_pos_relative.y < 0 || mouse_pos_relative.y >= static_cast<float>(bank_textures[tile_map_index]->height);
}

void Editor::DrawSelectedSpriteRect(const int& index, const ImVec2& image_screen_pos, const float& tiles_in_col,
                                    const ImColor& color) const
{
    if (index == -1)
        return;

    const int col = index % static_cast<int>(tiles_in_col);
    const int row = index / static_cast<int>(tiles_in_col);
    const auto selected_sprite_rect_tl_pos = ImVec2(image_screen_pos.x + static_cast<float>(col) * tile_size,
                                                    image_screen_pos.y + static_cast<float>(row) * tile_size);
    const auto selected_sprite_rect_br_pos = ImVec2(selected_sprite_rect_tl_pos.x + tile_size,
                                                    selected_sprite_rect_tl_pos.y + tile_size);

    ImGui::GetWindowDrawList()->AddRect(selected_sprite_rect_tl_pos, selected_sprite_rect_br_pos, color);
}

void Editor::DrawCanvasOptions()
{
    ImGui::SetNextWindowContentSize(ImVec2(256, 128));
    ImGui::Begin("Canvas Options");

    ImGui::SeparatorText("Misc");
    ImGui::Checkbox("Paint Collision Map", &paint_collision_map);
    ImGui::Checkbox("Lock Canvas Position", &lock_canvas_position);
    
    ImGui::SeparatorText("Render After Entities");
    for (size_t i = 0; i < tile_maps.size(); i++)
    {
        ImGui::Checkbox(std::format("Layer #{}", i).c_str(), &tile_maps[i].is_front);
    }

    ImGui::End();
}

void Editor::DrawCanvas()
{
    const ImGuiWindowFlags window_flags = (lock_canvas_position ? ImGuiWindowFlags_NoMove : 0) |
        ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar;

    const auto canvas_size = ImVec2(tile_size * static_cast<float>(col_tile_count),
                                    tile_size * static_cast<float>(row_tile_count));
    ImGui::SetNextWindowContentSize(canvas_size);
    ImGui::Begin("Canvas", nullptr, window_flags);

    ImVec2 canvas_screen_pos;
    DrawTilemaps(canvas_screen_pos);
    HandleTilePaint(canvas_screen_pos);

    ImGui::End();
}

void Editor::DrawTilemaps(ImVec2& canvas_screen_pos) const
{
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    const ImVec2 current_cursor_pos = ImGui::GetCursorScreenPos();
    canvas_screen_pos = ImGui::GetCursorScreenPos();
    for (size_t i = 0; i < tile_maps.size(); i++)
    {
        DrawTilemapLayer(canvas_screen_pos, current_cursor_pos, i);
    }

    ImGui::PopStyleVar();
}

void Editor::DrawTilemapLayer(const ImVec2& canvas_screen_pos, ImVec2 current_cursor_pos, size_t i) const
{
    const auto& [data, is_front] = tile_maps[i];
    for (int j = 0; j < row_tile_count; j++)
    {
        for (int k = 0; k < col_tile_count; k++)
        {
            const int tile_index = j * col_tile_count + k;
            const int tile_value = data[tile_index];

            if (tile_value == -1)
            {
                current_cursor_pos.x += tile_size;
                continue;
            }

            const int num_columns = static_cast<int>(static_cast<float>(bank_textures[i]->width) / tile_size);
            const int row = tile_value / num_columns;
            const int col = tile_value % num_columns;

            ImVec2 uv0;
            ImVec2 uv1;
            CalculateSelectedTileUVs(i, row, col, uv0, uv1);

            static constexpr auto grey_tint_color = ImVec4(0.33f, 0.33f, 0.33f, 1.0f);
            static constexpr auto neutral_tint_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
            ImVec4 tint_color = paint_collision_map && collision_map[tile_index] == 1
                                    ? grey_tint_color
                                    : neutral_tint_color;

            ImGui::GetWindowDrawList()->AddImage(reinterpret_cast<ImTextureID>(bank_textures[i]->id),
                                                 // NOLINT(performance-no-int-to-ptr)
                                                 current_cursor_pos,
                                                 ImVec2(current_cursor_pos.x + tile_size,
                                                        current_cursor_pos.y + tile_size),
                                                 uv0, uv1,
                                                 ImColor(tint_color));

            current_cursor_pos.x += tile_size;
        }

        current_cursor_pos.y += tile_size;
        current_cursor_pos.x = canvas_screen_pos.x;
    }

    current_cursor_pos.y = canvas_screen_pos.y;
}

void Editor::CalculateSelectedTileUVs(const size_t i, const int row, const int col, ImVec2& uv0, ImVec2& uv1) const
{
    uv0.x = static_cast<float>(col) * (tile_size / static_cast<float>(bank_textures[i]->width));
    uv0.y = static_cast<float>(row) * (tile_size / static_cast<float>(bank_textures[i]->width));
    uv1.x = uv0.x + (tile_size / static_cast<float>(bank_textures[i]->width));
    uv1.y = uv0.y + (tile_size / static_cast<float>(bank_textures[i]->width));
}

void Editor::HandleTilePaint(const ImVec2 canvas_screen_pos)
{
    const ImVec2 mouse_position = ImGui::GetMousePos();
    const auto mouse_pos_relative = ImVec2(mouse_position.x - canvas_screen_pos.x,
                                           mouse_position.y - canvas_screen_pos.y);
    const int i = static_cast<int>(mouse_pos_relative.x / tile_size);
    const int j = static_cast<int>(mouse_pos_relative.y / tile_size);
    const int tile_index = j * col_tile_count + i;

    if (IsPositionOutsideCanvas(mouse_pos_relative))
        return;

    if (ImGui::IsMouseDown(0))
    {
        paint_collision_map
            ? collision_map[tile_index] = 1
            : tile_maps[selected_tile_map_index].data[tile_index] = selected_sprite_index;
    }
    else if (ImGui::IsMouseDown(1))
    {
        paint_collision_map ? collision_map[tile_index] = 0 : tile_maps[selected_tile_map_index].data[tile_index] = -1;
    }
}

bool Editor::IsPositionOutsideCanvas(const ImVec2 mouse_pos_relative) const
{
    const int canvas_height = row_tile_count * static_cast<int>(tile_size);
    const int canvas_width = col_tile_count * static_cast<int>(tile_size);
    return mouse_pos_relative.x < 0 || static_cast<int>(mouse_pos_relative.x) >= canvas_width ||
        mouse_pos_relative.y < 0 || static_cast<int>(mouse_pos_relative.y) >= canvas_height;
}

void Editor::DrawEntityList()
{
    ImGui::Begin("Entities");

    if (ImGui::Button("Create Entity"))
    {
        const auto entity = new Entity("Entity");
        entities.push_back(entity);
    }

    ImGui::SeparatorText("Entities");
    for (size_t i = 0; i < entities.size(); i++)
    {
        if (ImGui::Button(std::format("{}###{}", entities[i]->GetName(), i).c_str()))
        {
            selected_entity_index = i;
        }
    }

    ImGui::End();

    //TODO: Remove entity (destroy incl. all its components)
}

void Editor::DrawSelectedEntityComponentsList() const
{
    ImGui::Begin("Components");

    if (selected_entity_index == -1)
    {
        ImGui::Text("No entity selected...");
        ImGui::End();
        return;
    }
    
    Entity* entity = entities[selected_entity_index];

    ImGui::SeparatorText("General");
    
    ImGui::Checkbox(std::format("{}###{}", "Is Active", entity->GetName()).c_str(), &entity->is_active);
    
    const std::string current_name = entity->GetName();
    char new_name_buffer[256];
    strncpy_s(new_name_buffer, current_name.c_str(), sizeof(new_name_buffer));
    ImGui::InputText("Name", new_name_buffer, IM_ARRAYSIZE(new_name_buffer));
    if (ImGui::IsKeyPressed(ImGuiKey_Enter))
    {
        entity->SetName(new_name_buffer);
    }
    
    DrawAddComponentDropdownAndAddButton();
    
    ImGui::SeparatorText("Components");
    for (auto& component : entity->GetComponents())
    {
        if (ImGui::CollapsingHeader(std::format("{}###{}", component->GetName(), entity->GetName()).c_str()))
        {
            // TODO: Add tune section for each component
            // TODO: Add remove component button
        }
    }
    
    
    ImGui::End();
}

void Editor::DrawAddComponentDropdownAndAddButton() const
{
    static const char* components[] = {
        "Map Collider",
        "Box Collider",
        "Sprite Sheet",
        "Rigidbody",
        "Animator",
    };
    const char* current_item = components[0];

    ImGui::SeparatorText("Add Component");
    
    if (ImGui::BeginCombo("##components", current_item))
    {
        for (auto& component : components)
        {
            const bool is_selected = current_item == component;
            if (ImGui::Selectable(component, is_selected))
            {
                current_item = component;
            }
            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    
    ImGui::SameLine();

    if (ImGui::Button("Add"))
    {
        // TODO: Add component to selected entity
    }
}

Editor::~Editor()
{
    DeleteBankTextures();
    DeleteEntities();
}

void Editor::DeleteBankTextures()
{
    for (const auto texture : bank_textures)
    {
        glDeleteTextures(1, &texture->id);
        delete texture;
    }
    bank_textures.clear();
}

void Editor::DeleteEntities()
{
    for (const auto entity : entities)
    {
        delete entity;
    }
    entities.clear();
    selected_entity_index = -1;
}
