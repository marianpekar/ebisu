#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "editor.h"

#include <format>
#include <filesystem>

#include "json_reader.h"
#include "json_writer.h"
namespace fs = std::filesystem;

#include <SDL_opengl.h>

#include "imgui.h"

const std::string project_path =
#if _DEBUG
    "./../../";
#else
	"./";
#endif

const std::string assets_path = project_path + "assets";

size_t Editor::LoadTexture(const char* path)
{
    if (strlen(path) == 0)
    {
        return -1;
    }
    
    if (texture_indices_cache.contains(path))
    {
        return texture_indices_cache[path];
    }

    const std::string full_path = std::format("{}/{}", assets_path.c_str(), path);
    if(!std::filesystem::exists(full_path))
    {
        return -1;
    }
    
    const auto texture = new Texture();

    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(full_path.c_str(), &image_width, &image_height, nullptr, 4);

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

    textures.emplace_back(texture);

    const size_t texture_index = textures.size() - 1;
    texture_indices_cache.emplace(path, texture_index);
    return texture_index;
}

void Editor::Draw()
{
    DrawMainMenuBar();

    if (open_new_level_popup)
    {
        DrawNewLevelPopup();
    }

    if (open_save_as_popup)
    {
        DrawSaveAsPopup();
    }

    if (open_open_popup)
    {
        DrawOpenPopup();
    }
    
    if (open_select_asset_popup)
    {
        DrawSelectAssetPopup();
    }

    if (!is_level_loaded)
        return;

    for (size_t i = 0; i < tile_maps.size(); i++)
    {
        DrawSpriteBank(i);
    }

    DrawCanvas();
    DrawCanvasOptions();

    DrawEntitiesWindow();
    DrawSelectedEntityComponentsWindow();
}

void Editor::DrawMainMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Level"))
            {
                open_new_level_popup = true;
            }
            if (ImGui::MenuItem("Save As..."))
            {
                open_save_as_popup = true;
            }
            if (ImGui::MenuItem("Open"))
            {
                open_open_popup = true;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void Editor::DrawNewLevelPopup()
{
    ImGui::OpenPopup("Create New Level");
    if (ImGui::BeginPopupModal("Create New Level", &open_new_level_popup))
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
            if (ImGui::Button(std::format("...##{}", i).c_str()))
            {
                selected_tilemap_input_field_index = i;
                open_new_level_popup = false;
                open_previous_popup = &open_new_level_popup;
                open_select_asset_popup = true;
            }
            ImGui::SameLine();
            ImGui::InputText(std::format("Tilemap Path {}", i).c_str(), new_level_tilemap_paths[i], 256);
        }

        if (selected_asset_path_changed)
        {
            selected_asset_path_changed = false;
            UpdateSelectedNewLevelTilemapPath();
        }

        if (ImGui::Button("Confirm"))
        {
            is_level_loaded = false;
            DisposeCurrentLevel();
            CreateNewLevel();
            is_level_loaded = true;
            open_new_level_popup = false;
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
        {
            open_new_level_popup = false;
        }

        ImGui::EndPopup();
    }
}

void Editor::UpdateSelectedNewLevelTilemapPath()
{
    if (selected_asset_path[0] == '/')
    {
        selected_asset_path = selected_asset_path.substr(1);
    }

    const size_t source_length = strlen(selected_asset_path.c_str());
    strncpy_s(new_level_tilemap_paths[selected_tilemap_input_field_index],
              source_length + 1,
              selected_asset_path.c_str(),
              source_length);
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
    if (ImGui::BeginPopupModal("Select Asset", &open_select_asset_popup))
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
                    selected_asset_path = current_assets_relative_subdir_path.empty()
                                                          ? filename
                                                          : std::format("{}/{}", current_assets_relative_subdir_path, filename);

                    open_select_asset_popup = false;
                    selected_asset_path_changed = true;
                    if (open_previous_popup != nullptr)
                    {
                        *open_previous_popup = true;
                    }
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

    for (int j = 0; j < static_cast<int>(tile_maps.size()); j++)
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

void Editor::DrawSaveAsPopup()
{
    ImGui::OpenPopup("Save Level As");
    if (ImGui::BeginPopupModal("Save Level As", &open_save_as_popup))
    {
        ImGui::InputText("Filename", level_file_name,  256);
        if(ImGui::Button("Save"))
        {
            JsonWriter::SaveLevelToFile(level_file_name, this);
        }
        ImGui::EndPopup();
    }
}

void Editor::DrawOpenPopup()
{
    ImGui::OpenPopup("Open Level");
    if (ImGui::BeginPopupModal("Open Level", &open_open_popup))
    {
        ImGui::InputText("Filename", level_file_name,  256);
        ImGui::SameLine();
        if (ImGui::Button(std::format("...##OpenLevelFilename").c_str()))
        {
            open_open_popup = false;
            open_previous_popup = &open_open_popup;
            open_select_asset_popup = true;
        }

        if (selected_asset_path_changed)
        {
            selected_asset_path_changed = false;
            strncpy_s(level_file_name, 256, selected_asset_path.c_str(), _TRUNCATE);
        }
        
        if(ImGui::Button("Open"))
        {
            is_level_loaded = false;
            DisposeCurrentLevel();
            JsonReader::LoadLevelFromFile(std::format("{}/{}", assets_path, level_file_name).c_str(), this);
            is_level_loaded = true;
            open_open_popup = false;
        }
        ImGui::EndPopup();
    }
}

void Editor::DrawSpriteBank(const size_t& tile_map_index)
{
    if (!is_bank_window_init_size_set)
    {
        constexpr static auto offset_width = 16.0f;
        constexpr static auto offset_height = 40.0f;
        static auto window_size = ImVec2(static_cast<float>(textures[tile_map_index]->width) + offset_width,
                                         static_cast<float>(textures[tile_map_index]->height) + offset_height);
        ImGui::SetNextWindowSize(window_size);
        is_bank_window_init_size_set = true;
    }

    ImGui::Begin(std::format("Sprite Bank #{}", tile_map_index).c_str(), nullptr);

    const ImVec2 image_size(static_cast<float>(textures[tile_map_index]->width),
                            static_cast<float>(textures[tile_map_index]->height));
    const ImVec2 image_screen_pos = ImGui::GetCursorScreenPos();

    ImGui::Image(reinterpret_cast<ImTextureID>(textures[tile_map_index]->id), image_size); // NOLINT(performance-no-int-to-ptr)

    const float tiles_in_col = static_cast<float>(textures[tile_map_index]->height) / tile_size;
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
    return mouse_pos_relative.x < 0 || mouse_pos_relative.x >= static_cast<float>(textures[tile_map_index]->width)
        ||
        mouse_pos_relative.y < 0 || mouse_pos_relative.y >= static_cast<float>(textures[tile_map_index]->height);
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

    ImGui::SeparatorText("Modes");
    ImGui::RadioButton("Paint Tiles", &selected_canvas_mode, PaintTiles);
    ImGui::RadioButton("Paint Collision", &selected_canvas_mode, PaintCollision);
    ImGui::RadioButton("Move Entities", &selected_canvas_mode, MoveEntities);
    
    ImGui::SeparatorText("Entities");
    ImGui::Checkbox("Show Names", &show_entity_names_on_canvas);
    ImGui::Checkbox("Show Sprites", &show_entity_sprites);

    ImGui::SeparatorText("Render After Entities");
    for (size_t i = 0; i < tile_maps.size(); i++)
    {
        ImGui::Checkbox(std::format("Layer #{}", i).c_str(), &tile_maps[i].is_front);
    }

    ImGui::End();
}

void Editor::DrawCanvas()
{
    constexpr ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_AlwaysVerticalScrollbar |
        ImGuiWindowFlags_AlwaysHorizontalScrollbar |
        ImGuiWindowFlags_NoBringToFrontOnFocus;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    
    const auto canvas_size = ImVec2(tile_size * static_cast<float>(col_tile_count),
                                    tile_size * static_cast<float>(row_tile_count));
    ImGui::SetNextWindowContentSize(canvas_size);
    ImGui::Begin("Canvas", nullptr, window_flags);

    ImVec2 canvas_screen_pos;
    DrawTilemaps(canvas_screen_pos);
    HandleCanvasMouseInteraction(canvas_screen_pos);
    DrawEntitiesOnCanvas(canvas_screen_pos);

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

            const int num_columns = static_cast<int>(static_cast<float>(textures[i]->width) / tile_size);
            const int row = tile_value / num_columns;
            const int col = tile_value % num_columns;

            ImVec2 uv0;
            ImVec2 uv1;
            CalculateSelectedTileUVs(i, row, col, uv0, uv1);

            static constexpr auto grey_tint_color = ImVec4(0.33f, 0.33f, 0.33f, 1.0f);
            static constexpr auto neutral_tint_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
            ImVec4 tint_color = selected_canvas_mode == CanvasMode::PaintCollision && collision_map[tile_index] == 1
                                    ? grey_tint_color
                                    : neutral_tint_color;

            ImGui::GetWindowDrawList()->AddImage(reinterpret_cast<ImTextureID>(textures[i]->id), // NOLINT(performance-no-int-to-ptr)
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
    uv0.x = static_cast<float>(col) * (tile_size / static_cast<float>(textures[i]->width));
    uv0.y = static_cast<float>(row) * (tile_size / static_cast<float>(textures[i]->width));
    uv1.x = uv0.x + (tile_size / static_cast<float>(textures[i]->width));
    uv1.y = uv0.y + (tile_size / static_cast<float>(textures[i]->width));
}

void Editor::HandleCanvasMouseInteraction(const ImVec2 canvas_screen_pos)
{
    if(!ImGui::IsWindowHovered())
        return;
    
    const ImVec2 mouse_position = ImGui::GetMousePos();
    const auto mouse_pos_relative = ImVec2(mouse_position.x - canvas_screen_pos.x,
                                           mouse_position.y - canvas_screen_pos.y);
    const int i = static_cast<int>(mouse_pos_relative.x / tile_size);
    const int j = static_cast<int>(mouse_pos_relative.y / tile_size);
    const int tile_index = j * col_tile_count + i;

    if (IsPositionOutsideCanvas(mouse_pos_relative))
        return;

    if (selected_canvas_mode == PaintCollision)
    {
        if (ImGui::IsMouseDown(0) && !IsMouseHoveringOverEntityLabel(canvas_screen_pos))
        {
            collision_map[tile_index] = 1;
        }
        else if (ImGui::IsMouseDown(1) && !IsMouseHoveringOverEntityLabel(canvas_screen_pos))
        {
            collision_map[tile_index] = 0;
        }   
    }

    if (selected_canvas_mode == PaintTiles)
    {
        if (ImGui::IsMouseDown(0) && !IsMouseHoveringOverEntityLabel(canvas_screen_pos))
        {
            tile_maps[selected_tile_map_index].data[tile_index] = selected_sprite_index;
        }
        else if (ImGui::IsMouseDown(1) && !IsMouseHoveringOverEntityLabel(canvas_screen_pos))
        {
            tile_maps[selected_tile_map_index].data[tile_index] = -1;
        } 
    }

    if (selected_canvas_mode == MoveEntities && static_cast<int>(selected_entity_index) != -1 &&
        ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !IsMouseHoveringOverEntityLabel(canvas_screen_pos))
    {
        Component* transform = entities[selected_entity_index]->GetComponent("Transform");
        transform->SetFloat("X", mouse_pos_relative.x);
        transform->SetFloat("Y", mouse_pos_relative.y);
    }
}

bool Editor::IsMouseHoveringOverEntityLabel(const ImVec2& canvas_screen_pos) const
{
    for (const auto entity : entities)
    {
        Component* transform = entity->GetComponent("Transform");
        const float x = transform->GetFloat("X");
        const float y = transform->GetFloat("Y");
        const auto component_on_canvas_pos = ImVec2(canvas_screen_pos.x + x, canvas_screen_pos.y + y);
        const ImVec2 text_size = ImGui::CalcTextSize(entity->GetName().c_str());
        constexpr size_t margin = 2;
        const auto bg_min = ImVec2(component_on_canvas_pos.x - margin, component_on_canvas_pos.y - margin);
        const auto bg_max = ImVec2(component_on_canvas_pos.x + text_size.x + margin, component_on_canvas_pos.y + text_size.y + margin);
        if (ImGui::IsMouseHoveringRect(bg_min, bg_max))
        {
            return true;
        }
    }

    return false;
}

bool Editor::IsPositionOutsideCanvas(const ImVec2 mouse_pos_relative) const
{
    const int canvas_height = row_tile_count * static_cast<int>(tile_size);
    const int canvas_width = col_tile_count * static_cast<int>(tile_size);
    return mouse_pos_relative.x < 0 || static_cast<int>(mouse_pos_relative.x) >= canvas_width ||
        mouse_pos_relative.y < 0 || static_cast<int>(mouse_pos_relative.y) >= canvas_height;
}

void Editor::DrawEntitiesOnCanvas(const ImVec2& canvas_screen_pos)
{
    for (const auto& entity : entities)
    {
        Component* transform = entity->GetComponent("Transform");
        const float x = transform->GetFloat("X");
        const float y = transform->GetFloat("Y");

        auto component_on_canvas_pos = ImVec2(canvas_screen_pos.x + x, canvas_screen_pos.y + y);
        
        for (const auto& component : entity->GetComponents())
        {
            if (strcmp(component->GetName(), "SpriteSheet") == 0 && show_entity_sprites)
            {
                const auto sprite_sheet = reinterpret_cast<SpriteSheet*>(component);
                if (static_cast<int>(sprite_sheet->texture_index) != -1)
                {
                    const Texture* texture = textures[sprite_sheet->texture_index];
                    
                    const float width = component->GetFloat("Width");
                    const float height = component->GetFloat("Height");

                    constexpr ImVec2 uv0(0,0);
                    ImVec2 uv1;
                    uv1.x = width / static_cast<float>(texture->width);
                    uv1.y = height / static_cast<float>(texture->height);
                    
                    ImGui::GetWindowDrawList()->AddImage(reinterpret_cast<ImTextureID>(texture->id), // NOLINT(performance-no-int-to-ptr)
                         component_on_canvas_pos,
                         ImVec2(component_on_canvas_pos.x + width, component_on_canvas_pos.y + height),
                         uv0, uv1);
                }
                else
                {
                    sprite_sheet->texture_index = LoadTexture(component->GetPath("FilePath").c_str());
                }
            }
        }

        if (show_entity_names_on_canvas)
        {
            const ImVec2 text_size = ImGui::CalcTextSize(entity->GetName().c_str());
            constexpr size_t margin = 2;
            const auto bg_min = ImVec2(component_on_canvas_pos.x - margin, component_on_canvas_pos.y - margin);
            const auto bg_max = ImVec2(component_on_canvas_pos.x + text_size.x + margin, component_on_canvas_pos.y + text_size.y + margin);
            ImColor label_bg_color = selected_entity_index == entity->GetIndex() ? ImColor(0.f, 0.8f, 0.8f) : ImColor(0.f, 0.f, 0.f);
            ImGui::GetWindowDrawList()->AddRectFilled(bg_min, bg_max, label_bg_color); 
            ImGui::GetWindowDrawList()->AddText(component_on_canvas_pos, ImColor(1.f, 1.f, 1.f), entity->GetName().c_str());

            if (ImGui::IsMouseHoveringRect(bg_min, bg_max) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                selected_entity_index = entity->GetIndex();
            }
        }
    }
}

void Editor::DrawEntitiesWindow()
{
    ImGui::Begin("Entities");

    if (ImGui::Button("Create Entity"))
    {
        const auto entity = new Entity("Entity", entities.size());
        entities.push_back(entity);
    }

    ImGui::SeparatorText("Entities");
    for (size_t i = 0; i < entities.size(); i++)
    {
        if (ImGui::Button(std::format("{}##{}", entities[i]->GetName(), i).c_str()))
        {
            selected_entity_index = i;
        }

        ImGui::SameLine();

        if (ImGui::Button(std::format("[X]##{}{}", entities[i]->GetName(), i).c_str()))
        {
            if (selected_entity_index == i)
            {
                selected_entity_index = -1;
            }
            
            for (size_t j = 0; j < entities[i]->GetComponents().size(); j++)
            {
                entities[i]->RemoveComponent(j);
            }

            delete entities[i];
            entities.erase(entities.begin() + static_cast<int>(i));
        }
    }

    ImGui::End();
}

void Editor::DrawSelectedEntityComponentsWindow()
{
    ImGui::Begin("Components");

    if (selected_entity_index == static_cast<size_t>(-1) || entities.empty())
    {
        ImGui::Text("No entity selected...");
        ImGui::End();
        return;
    }

    Entity* entity = entities[selected_entity_index];
    DrawSelectedEntityGeneralProperties(entity);
    DrawAddComponentDropdownAndAddButton(entity);
    DrawSelectedEntityComponentProperties(entity);        
    
    ImGui::End();
}

void Editor::DrawSelectedEntityGeneralProperties(Entity* entity)
{
    ImGui::SeparatorText("General");

    ImGui::Checkbox(std::format("{}##{}", "Is Active", entity->GetName()).c_str(), &entity->is_active);

    const std::string current_name = entity->GetName();
    char new_name_buffer[256];
    strncpy_s(new_name_buffer, current_name.c_str(), sizeof(new_name_buffer));
    ImGui::InputText("Name", new_name_buffer, IM_ARRAYSIZE(new_name_buffer));
    if (ImGui::IsKeyPressed(ImGuiKey_Enter))
    {
        entity->SetName(new_name_buffer);
    }
}

void Editor::DrawSelectedEntityComponentProperties(Entity* entity)
{
    ImGui::SeparatorText("Components");
    const auto components = entity->GetComponents();
    for (size_t i = 0; i < components.size(); i++)
    {
        Component* component = components[i];
        if (ImGui::CollapsingHeader(std::format("{}##{}", component->GetName(), i).c_str()))
        {
            ImGui::Indent();
            for (auto& [name, value] : component->float_properties)
            {
                const std::string current_value = std::to_string(value);
                char new_value_buffer[256];
                strncpy_s(new_value_buffer, current_value.c_str(), sizeof(new_value_buffer));
                ImGui::InputText(std::format("{}##{}{}", name, component->GetName(), i).c_str(),
                                 new_value_buffer, IM_ARRAYSIZE(new_value_buffer));
                if (ImGui::IsKeyPressed(ImGuiKey_Enter))
                {
                    value = std::stof(new_value_buffer);
                }
            }

            for (auto& [name, value] : component->int_properties)
            {
                const std::string current_value = std::to_string(value);
                char new_value_buffer[256];
                strncpy_s(new_value_buffer, current_value.c_str(), sizeof(new_value_buffer));
                ImGui::InputText(std::format("{}##{}{}", name, component->GetName(), i).c_str(),
                                 new_value_buffer, IM_ARRAYSIZE(new_value_buffer));
                if (ImGui::IsKeyPressed(ImGuiKey_Enter))
                {
                    value = std::stoi(new_value_buffer);
                }
            }

            for (auto& [name, value] : component->string_properties)
            {
                const std::string current_value = value;
                char new_value_buffer[256];
                strncpy_s(new_value_buffer, current_value.c_str(), sizeof(new_value_buffer));
                ImGui::InputText(std::format("{}##{}{}", name, component->GetName(), i).c_str(),
                                 new_value_buffer, IM_ARRAYSIZE(new_value_buffer));
                if (ImGui::IsKeyPressed(ImGuiKey_Enter))
                {
                    value = new_value_buffer;
                }
            }

            for (auto& [name, value] : component->path_properties)
            {
                if (ImGui::Button(std::format("...##{}", i).c_str()))
                {
                    open_previous_popup = nullptr;
                    open_select_asset_popup = true;
                }

                if (selected_asset_path_changed)
                {
                    selected_asset_path_changed = false;
                    value = selected_asset_path;
                }

                ImGui::SameLine();
                
                std::string current_value = value;
                char new_value_buffer[256];
                strncpy_s(new_value_buffer, current_value.c_str(), sizeof(new_value_buffer));
                ImGui::InputText(std::format("{}##{}{}", name, component->GetName(), i).c_str(), new_value_buffer, IM_ARRAYSIZE(new_value_buffer));
                
                if (ImGui::IsKeyPressed(ImGuiKey_Enter))
                {
                    value = new_value_buffer;
                }
            }

            for (auto& [name, value] : component->bool_properties)
            {
                ImGui::Checkbox(std::format("{}##{}{}", name, component->GetName(), i).c_str(), &value);
            }

            if (strcmp(components[i]->GetName(), "Transform") != 0)
            {
                if (ImGui::Button(std::format("{}##{}", "Remove Component", i).c_str()))
                {
                    entity->RemoveComponent(i);
                }
            }

            ImGui::Unindent();
        }
    }
}

void Editor::DrawAddComponentDropdownAndAddButton(Entity* selected_entity)
{
    // Don't change the order of items! (see switch below)
    static const char* components[] = {
        "MapCollider", // 0
        "BoxCollider", // 1
        "SpriteSheet", // 2
        "Rigidbody", // 3
        "Animator", // 4
        "CharacterAnimator", // 5
        "Agent", // 6
        "PlayerController", // 7
        "MainCamera" // 8
    };
    static int current_item_index = 0;

    ImGui::SeparatorText("Add Component");

    if (ImGui::BeginCombo("##components", components[current_item_index]))
    {
        for (int i = 0; i < IM_ARRAYSIZE(components); i++)
        {
            const bool is_selected = (current_item_index == i);
            if (ImGui::Selectable(components[i], is_selected))
            {
                current_item_index = i;
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
        switch (current_item_index)
        {
        case 0:
            selected_entity->AddComponent<MapCollider>(tile_size, tile_size);
            break;
        case 1:
            selected_entity->AddComponent<BoxCollider>(tile_size, tile_size);
            break;
        case 2:
            selected_entity->AddComponent<SpriteSheet>("", tile_size, tile_size);
            break;
        case 3:
            selected_entity->AddComponent<Rigidbody>(1.f, 0.1f);
            break;
        case 4:
            selected_entity->AddComponent<Animator>(0, 0, 0, 1000, true, true);
            break;
        case 5:
            selected_entity->AddComponent<CharacterAnimator>(600.0f, 0, 1, 100.0f, 500.0f, 2, 5);
            break;
        case 6:
            selected_entity->AddComponent<Agent>(0, 1000.0f, tile_size * 2.0f);
            break;
        case 7:
            selected_entity->AddComponent<PlayerController>();
            break;
        case 8:
            selected_entity->AddComponent<MainCamera>();
            break;
        default:
            break;
        }
    }
}

void Editor::AddEntityAt(Entity* entity, const size_t index)
{
    if (index >= entities.size())
    {
        entities.resize(index + 1);
    }

    entities[index] = entity;
}

void Editor::AddTilemapLayer(const char* sprite_sheet_path, const bool is_front, const std::vector<int>& data)
{
    LoadTexture(sprite_sheet_path);
    TilemapLayer tilemap_layer;
    tilemap_layer.is_front = is_front;
    tilemap_layer.data = data;
    tile_maps.push_back(tilemap_layer);
}

Editor::~Editor()
{
    DeleteBankTextures();
    DeleteEntities();
    delete[] level_file_name;
}

void Editor::DeleteBankTextures()
{
    for (const auto texture : textures)
    {
        glDeleteTextures(1, &texture->id);
        delete texture;
    }
    textures.clear();
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
