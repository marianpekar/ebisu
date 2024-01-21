#pragma once

#include <string>
#include <memory>
#include "component.h"

class Sprite : public Component
{
private:
    int width, height;
    std::string filepath;
    struct SDL_Texture* sprite = nullptr;
    struct SDL_Rect* dst_rect = nullptr;
    std::shared_ptr<Transform> transform;
public:
    Sprite(std::string filepath) : width(0), height(0), filepath(std::move(filepath)) {}
    ~Sprite() override;
    void Setup() override;
    void Render() override;
};