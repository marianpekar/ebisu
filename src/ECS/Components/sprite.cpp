#include <SDL.h>
#include "../../texture_loader.h"
#include "../../renderer.h"
#include "../entity.h"
#include "transform.h"
#include "sprite.h"

void Sprite::Setup()
{
    transform = owner->GetComponent<Transform>();
    sprite = TextureLoader::LoadTexture(filepath, Renderer::GetRenderer(), width, height);
    dst_rect = new SDL_Rect();
}

Sprite::~Sprite()
{
    delete dst_rect;
}

void Sprite::Render()
{
    dst_rect->x = static_cast<int>(transform->GetPosition().x - Renderer::TryGetCameraPosition().x);
    dst_rect->y = static_cast<int>(transform->GetPosition().y - Renderer::TryGetCameraPosition().x);
    dst_rect->w = width * static_cast<int>(transform->GetScale().x);
    dst_rect->h = height * static_cast<int>(transform->GetScale().y);

    SDL_RenderCopy(Renderer::GetRenderer(), sprite, nullptr, dst_rect);
}