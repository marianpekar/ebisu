#pragma once

#include "../../texture_loader.h"
#include "../entity.h"
#include "transform.h"
#include "sprite_sheet.h"
#include <SDL.h>

void SpriteSheet::Setup()
{
    transform = owner->GetComponent<Transform>();

    sprite = TextureLoader::LoadTexture(filepath, renderer, sheet_width, sheet_height);

    this->rect_width = rect_width;
    this->rect_height = rect_height;

    src_rect = new SDL_Rect();
    src_rect->x = 0;
    src_rect->y = 0;
    src_rect->w = rect_width;
    src_rect->h = rect_height;

    dst_rect = new SDL_Rect();
}

void SpriteSheet::Render()
{
    dst_rect->x = transform->GetX();
    dst_rect->y = transform->GetY();
    dst_rect->w = rect_width * transform->GetXScale();
    dst_rect->h = rect_height * transform->GetYScale();

    SDL_RenderCopy(renderer, sprite, src_rect, dst_rect);
}

void SpriteSheet::SelectSprite(const int& row, const int& col)
{
    src_rect->x = col * rect_width;
    src_rect->y = row * rect_height;
}
