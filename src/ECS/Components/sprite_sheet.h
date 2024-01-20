#pragma once

#include <string>
#include <memory>
#include "component.h"

class SpriteSheet final : public Component
{
private:
    int sheet_width, sheet_height, rect_width, rect_height;
    struct SDL_Texture* sprite = nullptr;
    struct SDL_Rect* src_rect = nullptr;
    SDL_Rect* dst_rect = nullptr;
    std::string filepath;
    std::shared_ptr<class Transform> transform;
public:
    SpriteSheet(std::string filepath, int rect_width, int rect_height);
    ~SpriteSheet() override;
    void Setup() override;
    void Render() override;
    void SelectSprite(const int& row, const int& col) const;
};