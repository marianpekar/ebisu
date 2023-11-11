#include "../../texture_loader.h"
#include "../entity.h"
#include "transform.h"
#include "sprite_sheet.h"
#include <SDL.h>
#include "../../renderer.h"

SpriteSheet::SpriteSheet(std::string filepath, const int rect_width, const int rect_height) :
    sheet_width(0), sheet_height(0), rect_width(rect_width), rect_height(rect_height), filepath(std::move(filepath))
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
    sprite = TextureLoader::LoadTexture(filepath.c_str(), Renderer::GetRenderer(), sheet_width, sheet_height);
}

void SpriteSheet::Render()
{
    dst_rect->x = static_cast<int>(transform->GetPosition().x - Renderer::TryGetCameraPosition()->x);
    dst_rect->y = static_cast<int>(transform->GetPosition().y - Renderer::TryGetCameraPosition()->y);
    dst_rect->w = rect_width * static_cast<int>(transform->GetScale().x);
    dst_rect->h = rect_height * static_cast<int>(transform->GetScale().y);

    SDL_RenderCopy(Renderer::GetRenderer(), sprite, src_rect, dst_rect);
}

void SpriteSheet::SelectSprite(const int& row, const int& col) const
{
    src_rect->x = col * rect_width;
    src_rect->y = row * rect_height;
}

SpriteSheet::~SpriteSheet()
{
    delete src_rect;
    delete dst_rect;
}