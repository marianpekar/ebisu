#pragma once

#include "../../texture_loader.h"
#include "../entity.h"
#include "sprite.h"
#include <SDL.h>

void Sprite::Setup(const char* filepath, SDL_Renderer* renderer)
{
	transform = owner->GetComponent<Transform>();
	this->renderer = renderer;
	sprite = TextureLoader::LoadTexture(filepath, renderer, width, height);
}

void Sprite::Update(float delta_time) {}

void Sprite::Render()
{
    SDL_Rect dst_rect = { 
        transform->GetX(),
        transform->GetY(),
        width,
        height
    };

    SDL_RenderCopy(renderer, sprite, nullptr, &dst_rect);
}