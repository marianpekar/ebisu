#include "../../texture_loader.h"
#include "../../camera.h"
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
    dst_rect->x = static_cast<int>(transform->GetPosition().x - camera->GetPosition().x);
    dst_rect->y = static_cast<int>(transform->GetPosition().y - camera->GetPosition().y);
    dst_rect->w = width * static_cast<int>(transform->GetScale().x);
    dst_rect->h = height * static_cast<int>(transform->GetScale().y);

    SDL_RenderCopy(renderer, sprite, nullptr, dst_rect);
}