#pragma once

#include "../../texture_loader.h"
#include "../../camera.h"
#include "../entity.h"
#include "transform.h"
#include "sprite_sheet.h"
#include <SDL.h>

SpriteSheet::SpriteSheet(const char* filepath, SDL_Renderer* renderer, int rect_width, int rect_height, Camera* camera) :
    renderer(renderer), rect_width(rect_width), rect_height(rect_height),
    filepath(filepath), sheet_width(0), sheet_height(0), camera(camera) 
{
    src_rect = new SDL_Rect();
    dst_rect = new SDL_Rect();

    src_rect->x = 0;
    src_rect->y = 0;
    src_rect->w = rect_width;
    src_rect->h = rect_height;
}

void SpriteSheet::Setup()
{
    transform = owner->GetComponent<Transform>();
    sprite = TextureLoader::LoadTexture(filepath, renderer, sheet_width, sheet_height);
}

void SpriteSheet::Render()
{
    dst_rect->x = transform->GetPosition().x - camera->GetPosition().x;
    dst_rect->y = transform->GetPosition().y - camera->GetPosition().y;
    dst_rect->w = rect_width * transform->GetScale().x;
    dst_rect->h = rect_height * transform->GetScale().y;

    SDL_RenderCopy(renderer, sprite, src_rect, dst_rect);
}

void SpriteSheet::SelectSprite(const int& row, const int& col)
{
    src_rect->x = col * rect_width;
    src_rect->y = row * rect_height;
}

SpriteSheet::~SpriteSheet()
{
    delete src_rect;
    delete dst_rect;
}