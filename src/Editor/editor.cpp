#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "editor.h"

#include <SDL_opengl.h>

#include "imgui.h"

Editor::Editor()
{
	bank_texture = LoadTexture("./../../assets/test_tilemap_4_tiles_128x128.png");
}

void Editor::Draw()
{
	DrawSpriteBank();
}

void Editor::DrawSpriteBank()
{
	if (!is_bank_window_init_size_set)
	{
		constexpr static auto offset_width = 15.0f;
		constexpr static auto offset_height = 35.0f;
		static auto window_size = ImVec2(bank_texture->width + offset_width, bank_texture->height + offset_height);
		ImGui::SetNextWindowSize(window_size);
		is_bank_window_init_size_set = true;
	}
	
	ImGui::Begin("Sprite Bank", nullptr);

	glBindTexture(GL_TEXTURE_2D, bank_texture->id);
	
	ImVec2 imageSize(bank_texture->width, bank_texture->height);
	ImGui::Image((void*)(intptr_t)bank_texture->id, imageSize);
	
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