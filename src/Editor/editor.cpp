#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "editor.h"

#include <format>
#include <SDL_opengl.h>

#include "imgui.h"

Editor::Editor()
{
	//TODO: This is just for testing, user should be able to add a tilemap layer via UI
	LoadTexture("./../../assets/test_tilemap_16_tiles_256x256.png");
	LoadTexture("./../../assets/test_tilemap_4_tiles_128x128_alpha.png");

	tile_maps.emplace_back();
	tile_maps.emplace_back();
	
	for(auto& tile_map : tile_maps)
	{
		for(int i = 0; i < row_tile_count * row_tile_count; i++)
		{
			tile_map.push_back(0);
		}		
	}

	for(int i = 0; i < row_tile_count * row_tile_count; i++)
	{
		collision_map.push_back(0);
	}

}

void Editor::LoadTexture(const char* path)
{
	const auto texture = new Texture();
	
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(path, &image_width, &image_height, nullptr, 4);
	
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
	for(size_t i = 0; i < tile_maps.size(); i++)
	{
		DrawSpriteBank(i);
	}

	DrawCanvas();
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

	const ImVec2 image_size(static_cast<float>(bank_textures[tile_map_index]->width), static_cast<float>(bank_textures[tile_map_index]->height));
	const ImVec2 image_screen_pos = ImGui::GetCursorScreenPos();
	
	ImGui::Image(reinterpret_cast<ImTextureID>(bank_textures[tile_map_index]->id), image_size); // NOLINT(performance-no-int-to-ptr)

	const float tiles_in_col = static_cast<float>(bank_textures[tile_map_index]->height) / tile_size;
	HandleSpriteSelection(image_screen_pos, tiles_in_col, tile_map_index);

	if (selected_tile_map_index == tile_map_index)
	{
		DrawSelectedSpriteRect(selected_sprite_index, image_screen_pos, tiles_in_col, ImColor(1.0f, 0.0f, 0.0f));
	}
	
	ImGui::End();
}

void Editor::HandleSpriteSelection(const ImVec2& image_screen_pos, const float& tiles_in_col, const size_t& tile_map_index)
{
	const ImVec2 mouse_position = ImGui::GetMousePos();
	const auto mouse_pos_relative = ImVec2(mouse_position.x - image_screen_pos.x, mouse_position.y - image_screen_pos.y);
	
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
	return mouse_pos_relative.x < 0 || mouse_pos_relative.x >= static_cast<float>(bank_textures[tile_map_index]->width) ||
		   mouse_pos_relative.y < 0 || mouse_pos_relative.y >= static_cast<float>(bank_textures[tile_map_index]->height);
}

void Editor::DrawSelectedSpriteRect(const int& index, const ImVec2& image_screen_pos, const float& tiles_in_col, const ImColor& color) const
{
	if (index == -1)
		return;

	const int col = index % static_cast<int>(tiles_in_col);
	const int row = index / static_cast<int>(tiles_in_col);
	const auto selected_sprite_rect_tl_pos = ImVec2(image_screen_pos.x + static_cast<float>(col) * tile_size, image_screen_pos.y + static_cast<float>(row) * tile_size);
	const auto selected_sprite_rect_br_pos = ImVec2(selected_sprite_rect_tl_pos.x + tile_size, selected_sprite_rect_tl_pos.y + tile_size);
	
	ImGui::GetWindowDrawList()->AddRect(selected_sprite_rect_tl_pos, selected_sprite_rect_br_pos,color);
}

void Editor::DrawCanvas()
{
	const ImGuiWindowFlags window_flags = (lock_canvas_position ? ImGuiWindowFlags_NoMove : 0) |
		ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar;

	const auto canvas_size = ImVec2(tile_size * static_cast<float>(row_tile_count), tile_size * static_cast<float>(row_tile_count));
	ImGui::SetNextWindowContentSize(canvas_size);
	ImGui::Begin("Canvas", nullptr, window_flags);
	ImGui::Checkbox("Paint Collision Map", &paint_collision_map);
	ImGui::SameLine();
	ImGui::Checkbox("Lock Canvas Position", &lock_canvas_position);

	ImVec2 canvas_screen_pos;
	DrawTilemap(canvas_screen_pos);
	HandleTilePaint(canvas_screen_pos);
	
	ImGui::End();
}

void Editor::DrawTilemap(ImVec2& current_cursor_pos) const
{
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

	static constexpr auto grey_tint_color = ImVec4(0.33f, 0.33f, 0.33f, 1.0f);
	static constexpr auto neutral_tint_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

	current_cursor_pos = ImGui::GetCursorScreenPos();
	const ImVec2 init_cursor_pos = ImGui::GetCursorScreenPos();
	for (size_t i = 0; i < tile_maps.size(); i++)
	{
		const auto& tile_map = tile_maps[i];
		for (int j = 0; j < row_tile_count * row_tile_count; j++)
		{
			if (j % row_tile_count == 0 && j > 0)
			{
				current_cursor_pos.y += tile_size;
				current_cursor_pos.x = init_cursor_pos.x;
			}
			
			const int tile_index = tile_map[j];

			if (tile_index == -1)
			{
				current_cursor_pos.x += tile_size;
				continue;
			}

			const int num_columns = static_cast<int>(static_cast<float>(bank_textures[i]->width) / tile_size);
			const int row = tile_index / num_columns;
			const int col = tile_index % num_columns;

			ImVec2 uv0;
			ImVec2 uv1;
			uv0.x = static_cast<float>(col) * (tile_size / static_cast<float>(bank_textures[i]->width));
			uv0.y = static_cast<float>(row) * (tile_size / static_cast<float>(bank_textures[i]->width));
			uv1.x = uv0.x + (tile_size / static_cast<float>(bank_textures[i]->width));
			uv1.y = uv0.y + (tile_size / static_cast<float>(bank_textures[i]->width));

			ImVec4 tint_color = paint_collision_map && collision_map[j] == 1 ? grey_tint_color : neutral_tint_color;
			
			ImGui::GetWindowDrawList()->AddImage(reinterpret_cast<ImTextureID>(bank_textures[i]->id), // NOLINT(performance-no-int-to-ptr)
				current_cursor_pos,
				ImVec2(current_cursor_pos.x + tile_size, current_cursor_pos.y + tile_size),
				uv0, uv1,
				ImColor(tint_color));
			
			current_cursor_pos.x += tile_size;
		}
		
		current_cursor_pos.x = init_cursor_pos.x;
		current_cursor_pos.y = init_cursor_pos.y;
	}

	ImGui::PopStyleVar();
}

void Editor::HandleTilePaint(const ImVec2 canvas_screen_pos)
{
	const ImVec2 mouse_position = ImGui::GetMousePos();
	const auto mouse_pos_relative = ImVec2(mouse_position.x - canvas_screen_pos.x, mouse_position.y - canvas_screen_pos.y);
	const int i = static_cast<int>(mouse_pos_relative.x / tile_size);
	const int j = static_cast<int>(mouse_pos_relative.y / tile_size);
	const int tile_index = j * row_tile_count + i;

	if (IsPositionOutsideCanvas(mouse_pos_relative))
		return;
	
	if (ImGui::IsMouseDown(0))
	{
		paint_collision_map ? collision_map[tile_index] = 1 : tile_maps[selected_tile_map_index][tile_index] = selected_sprite_index;
	} else if (ImGui::IsMouseDown(1))
	{
		paint_collision_map ? collision_map[tile_index] = 0 : tile_maps[selected_tile_map_index][tile_index] = -1;
	}
}

bool Editor::IsPositionOutsideCanvas(const ImVec2 mouse_pos_relative) const
{
	static const int canvas_length = row_tile_count * static_cast<int>(tile_size);
	return  mouse_pos_relative.x < 0 || static_cast<int>(mouse_pos_relative.x) >= canvas_length ||
		   	mouse_pos_relative.y < 0 || static_cast<int>(mouse_pos_relative.y) >= canvas_length;
}

Editor::~Editor()
{
	for (const auto texture : bank_textures)
	{
		glDeleteTextures(1, &texture->id);
		delete texture;		
	}
}