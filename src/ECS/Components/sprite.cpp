#pragma once

#include "../../texture_loader.h"
#include "../entity.h"
#include "transform.h"
#include "sprite.h"
#include <SDL.h>

void Sprite::Setup()
{
    transform = owner->GetComponent<Transform>();
	sprite = TextureLoader::LoadTexture(filepath, renderer, width, height);
    dst_rect = new SDL_Rect();
}

void Sprite::Render()
{
    dst_rect->x = transform->GetX();
    dst_rect->y = transform->GetY();
    dst_rect->w = width * transform->GetXScale();
    dst_rect->h = height * transform->GetYScale();

    SDL_RenderCopy(renderer, sprite, nullptr, dst_rect);
}