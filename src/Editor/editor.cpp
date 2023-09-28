#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "editor.h"

#include <SDL_opengl.h>

#include "imgui.h"

Editor::Editor()
{
	bank_texture = LoadTexture("./../../assets/test_tilemap_16_tiles_256x256.png");
	
	for(int i = 0; i < row_tile_count * row_tile_count; i++)
	{
		tile_map.push_back(0);
		collision_map.push_back(0);
	}
}

Texture* Editor::LoadTexture(const char* path)
{
	const auto texture = new Texture();
	
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(path, &image_width, &image_height, NULL, 4);
	
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
	
	return texture;
}

void Editor::Draw()
{
	DrawSpriteBank();
	DrawCanvas();
}

void Editor::DrawSpriteBank()
{
	if (!is_bank_window_init_size_set)
	{
		constexpr static auto offset_width = 16.0f;
		constexpr static auto offset_height = 40.0f;
		static auto window_size = ImVec2(static_cast<float>(bank_texture->width) + offset_width, static_cast<float>(bank_texture->height) + offset_height);
		ImGui::SetNextWindowSize(window_size);
		is_bank_window_init_size_set = true;
	}
	
	ImGui::Begin("Sprite Bank", nullptr);

	const ImVec2 image_size(static_cast<float>(bank_texture->width), static_cast<float>(bank_texture->height));
	const ImVec2 image_screen_pos = ImGui::GetCursorScreenPos();
	
	ImGui::Image(reinterpret_cast<ImTextureID>(bank_texture->id), image_size); // NOLINT(performance-no-int-to-ptr)

	const float tiles_in_col = static_cast<float>(bank_texture->height) / tile_size;
	HandleSpriteSelection(image_screen_pos, tiles_in_col);

	DrawSelectedSpriteRect(selected_sprite_index_lmb, image_screen_pos, tiles_in_col, ImColor(1.0f, 0.0f, 0.0f));
	DrawSelectedSpriteRect(selected_sprite_index_rmb, image_screen_pos, tiles_in_col, ImColor(1.0f, 1.0f, 0.0f));
	
	ImGui::End();
}

void Editor::HandleSpriteSelection(const ImVec2& image_screen_pos, const float& tiles_in_col)
{
	const ImVec2 mouse_position = ImGui::GetMousePos();
	const auto mouse_pos_relative = ImVec2(mouse_position.x - image_screen_pos.x, mouse_position.y - image_screen_pos.y);
	
	if (IsPositionOutsideSpriteBank(mouse_pos_relative))
		return;

	const int i = static_cast<int>(mouse_pos_relative.x / tile_size);
	const int j = static_cast<int>(mouse_pos_relative.y / tile_size);
	const int index = j * static_cast<int>(tiles_in_col) + i;

	if (ImGui::IsMouseClicked(0))
	{
		selected_sprite_index_lmb = selected_sprite_index_lmb == index ? -1 : index;
	}
	else if (ImGui::IsMouseClicked(1))
	{
		selected_sprite_index_rmb = selected_sprite_index_rmb == index ? -1 : index;
	}
}

bool Editor::IsPositionOutsideSpriteBank(const ImVec2 mouse_pos_relative) const
{
	return mouse_pos_relative.x < 0 || mouse_pos_relative.x >= static_cast<float>(bank_texture->width) ||
		   mouse_pos_relative.y < 0 || mouse_pos_relative.y >= static_cast<float>(bank_texture->height);
}

void Editor::DrawSelectedSpriteRect(const int& index, const ImVec2& image_screen_pos, const float& tiles_in_col, const ImColor& color) const
{
	if (index == -1)
		return;
	
	const auto draw_list = ImGui::GetWindowDrawList();

	const int col = index % static_cast<int>(tiles_in_col);
	const int row = index / static_cast<int>(tiles_in_col);
	const auto selected_sprite_rect_tl_pos = ImVec2(image_screen_pos.x + static_cast<float>(col) * tile_size, image_screen_pos.y + static_cast<float>(row) * tile_size);
	const auto selected_sprite_rect_br_pos = ImVec2(selected_sprite_rect_tl_pos.x + tile_size, selected_sprite_rect_tl_pos.y + tile_size);
	
	draw_list->AddRect(selected_sprite_rect_tl_pos, selected_sprite_rect_br_pos,color);
}

void Editor::DrawCanvas()
{
	const ImGuiWindowFlags window_flags = lock_canvas_position ? ImGuiWindowFlags_NoMove : 0;
	
	ImGui::Begin("Canvas", nullptr, window_flags);

	ImGui::Checkbox("Paint Collision Map", &paint_collision_map);
	ImGui::SameLine();
	ImGui::Checkbox("Lock Canvas Position", &lock_canvas_position);

	ImVec2 canvas_screen_pos;
	DrawTilemap(canvas_screen_pos);
	HandleTilePaint(canvas_screen_pos);
	
	ImGui::End();
}

void Editor::DrawTilemap(ImVec2& canvas_screen_pos) const
{
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

	static constexpr auto grey_tint_color = ImVec4(0.33f, 0.33f, 0.33f, 1.0f);
	static constexpr auto neutral_tint_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

	canvas_screen_pos = ImGui::GetCursorScreenPos();
	for (int i = 0; i < row_tile_count * row_tile_count; i++)
	{
		if (i % row_tile_count != 0)
		{
			ImGui::SameLine();
		}

		const int tile_index = tile_map[i];

		if (tile_index == -1)
		{
			ImGui::Image(nullptr, ImVec2(tile_size, tile_size));
			continue;
		}

		const int num_columns = static_cast<int>(static_cast<float>(bank_texture->width) / tile_size);
		const int row = tile_index / num_columns;
		const int col = tile_index % num_columns;

		ImVec2 uv0;
		ImVec2 uv1;
		uv0.x = static_cast<float>(col) * (tile_size / static_cast<float>(bank_texture->width));
		uv0.y = static_cast<float>(row) * (tile_size / static_cast<float>(bank_texture->width));
		uv1.x = uv0.x + (tile_size / static_cast<float>(bank_texture->width));
		uv1.y = uv0.y + (tile_size / static_cast<float>(bank_texture->width));

		ImVec4 tint_color = paint_collision_map && collision_map[i] == 1 ? grey_tint_color : neutral_tint_color;
		ImGui::Image(reinterpret_cast<ImTextureID>(bank_texture->id), ImVec2(tile_size, tile_size), uv0, uv1, tint_color); // NOLINT(performance-no-int-to-ptr)
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
		paint_collision_map ? collision_map[tile_index] = 1 : tile_map[tile_index] = selected_sprite_index_lmb;
	} else if (ImGui::IsMouseDown(1))
	{
		paint_collision_map ? collision_map[tile_index] = 0 : tile_map[tile_index] = selected_sprite_index_rmb;
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
	glDeleteTextures(1, &bank_texture->id);
	delete bank_texture;
}