#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "editor.h"

#include <SDL_opengl.h>

#include "imgui.h"

Editor::Editor()
{
	bank_texture = LoadTexture("./../../assets/test_tilemap_4_tiles_128x128.png");
	
	for(int i = 0; i < row_tile_count * row_tile_count; i++)
	{
		tile_map.push_back(0);
		collision_map.push_back(0);
	}
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
		constexpr static auto offset_width = 64.0f;
		constexpr static auto offset_height = 136.0f;
		static auto window_size = ImVec2(bank_texture->width + offset_width, bank_texture->height + offset_height);
		ImGui::SetNextWindowSize(window_size);
		is_bank_window_init_size_set = true;
	}
	
	ImGui::Begin("Sprite Bank", nullptr);
	
	ImVec2 imageSize(bank_texture->width, bank_texture->height);
	ImVec2 image_screen_pos = ImGui::GetCursorScreenPos();
	ImGui::Image((void*)(intptr_t)bank_texture->id, imageSize);

	ImVec2 mouse_position = ImGui::GetMousePos();
	ImVec2 mouse_pos_relative = ImVec2(mouse_position.x - image_screen_pos.x, mouse_position.y - image_screen_pos.y);

	ImGui::Text("Image: %i %i", (int)image_screen_pos.x, (int)image_screen_pos.y);
	ImGui::Text("Mouse: %i %i", (int)mouse_position.x, (int)mouse_position.y);
	ImGui::Text("Mouse Relative: %i %i", (int)mouse_pos_relative.x, (int)mouse_pos_relative.y);

	float tiles_in_col = bank_texture->height / tile_size; // assume square spritesheet; TODO: assert on load
	
	if (mouse_pos_relative.x >= 0 && mouse_pos_relative.x < bank_texture->width &&
		mouse_pos_relative.y >= 0 && mouse_pos_relative.y < bank_texture->height)
	{
		int i = static_cast<int>(mouse_pos_relative.x / tile_size);
		int j = static_cast<int>(mouse_pos_relative.y / tile_size);
		int index = j * static_cast<int>(tiles_in_col) + i;

		ImGui::Text("Sprite Index: %d", index);

		if (ImGui::IsMouseClicked(0))
		{
			selected_sprite_index_lmb = index;
		}
		if (ImGui::IsMouseClicked(1))
		{
			selected_sprite_index_rmb = index;
		}
	}
	else
	{
		ImGui::Text("Cursor Outside Image");	
	}

	ImGui::Text("Selected Index LMB: %d", selected_sprite_index_lmb);
	ImGui::Text("Selected Index RMB: %d", selected_sprite_index_rmb);	

	DrawSelectedSpriteRect(selected_sprite_index_lmb, image_screen_pos, tiles_in_col, ImColor(1.0f, 0.0f, 0.0f));
	DrawSelectedSpriteRect(selected_sprite_index_rmb, image_screen_pos, tiles_in_col, ImColor(1.0f, 1.0f, 0.0f));
	
	ImGui::End();
}

void Editor::DrawSelectedSpriteRect(const int& index, const ImVec2& image_screen_pos, const float& tiles_in_col, const ImColor& color) const
{
	const auto draw_list = ImGui::GetWindowDrawList();

	int col = index % static_cast<int>(tiles_in_col);
	int row = index / static_cast<int>(tiles_in_col);
	ImVec2 selected_sprite_rect_tl_pos = ImVec2(image_screen_pos.x + col * tile_size, image_screen_pos.y + row * tile_size);
	ImVec2 selected_sprite_rect_br_pos = ImVec2(selected_sprite_rect_tl_pos.x + tile_size, selected_sprite_rect_tl_pos.y + tile_size);
	
	draw_list->AddRect(selected_sprite_rect_tl_pos, selected_sprite_rect_br_pos,color);
}

void Editor::DrawCanvas()
{
	ImGuiWindowFlags window_flags = lock_canvas_position ? ImGuiWindowFlags_NoMove : 0;
	
	ImGui::Begin("Canvas", nullptr, window_flags);

	ImGui::Checkbox("Paint Collision Map", &paint_collision_map);
	ImGui::SameLine();
	ImGui::Checkbox("Lock Canvas Position", &lock_canvas_position);
	
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

	static constexpr ImVec4 grey_tint_color = ImVec4(0.33f, 0.33f, 0.33f, 1.0f);
	static constexpr ImVec4 neutral_tint_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	
	ImVec2 canvas_screen_pos = ImGui::GetCursorScreenPos();
	for (int i = 0; i < row_tile_count * row_tile_count; i++)
	{
		if (i % row_tile_count != 0)
		{
			ImGui::SameLine();
		}

		int tile_index = tile_map[i];

		int numColumns = bank_texture->width / tile_size;
		int row = tile_index / numColumns;
		int col = tile_index % numColumns;

		ImVec2 uv0;
		ImVec2 uv1;
		uv0.x = static_cast<float>(col) * (tile_size / static_cast<float>(bank_texture->width));
		uv0.y = static_cast<float>(row) * (tile_size / static_cast<float>(bank_texture->width));
		uv1.x = uv0.x + (tile_size / static_cast<float>(bank_texture->width));
		uv1.y = uv0.y + (tile_size / static_cast<float>(bank_texture->width));

		ImVec4 tint_color = paint_collision_map && collision_map[i] == 1 ? grey_tint_color : neutral_tint_color;
		ImGui::Image((void*)bank_texture->id, ImVec2(tile_size, tile_size), uv0, uv1, tint_color);
	}

	ImGui::PopStyleVar();

	ImVec2 mouse_position = ImGui::GetMousePos();
	ImVec2 mouse_pos_relative = ImVec2(mouse_position.x - canvas_screen_pos.x, mouse_position.y - canvas_screen_pos.y);
	int i = static_cast<int>(mouse_pos_relative.x / tile_size);
	int j = static_cast<int>(mouse_pos_relative.y / tile_size);
	int tile_index = j * row_tile_count + i;
	
	ImGui::Text("Image: %i %i", (int)canvas_screen_pos.x, (int)canvas_screen_pos.y);
	ImGui::Text("Mouse: %i %i", (int)mouse_position.x, (int)mouse_position.y);
	ImGui::Text("Mouse Relative: %i %i", (int)mouse_pos_relative.x, (int)mouse_pos_relative.y);

	if (mouse_pos_relative.x >= 0 && mouse_pos_relative.x < row_tile_count * tile_size &&
		mouse_pos_relative.y >= 0 && mouse_pos_relative.y < row_tile_count * tile_size)
	{
		ImGui::Text("Tile Index: %i", tile_index);
		
		if (ImGui::IsMouseDown(0))
		{
			if (paint_collision_map)
			{
				collision_map[tile_index] = 1;
			}
			else
			{
				tile_map[tile_index] = selected_sprite_index_lmb;
			}
		}

		if (ImGui::IsMouseDown(1))
		{
			if (paint_collision_map)
			{
				collision_map[tile_index] = 0;
			}
			else
			{
				tile_map[tile_index] = selected_sprite_index_rmb;
			}
		}
	}
	else
	{
		ImGui::Text("Cursor Outside Canvas");
	}
	
	ImGui::End();
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

Editor::~Editor()
{
	glDeleteTextures(1, &bank_texture->id);
	delete bank_texture;
}