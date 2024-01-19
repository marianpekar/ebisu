#pragma once

#include <string>

#include "component.h"

class MapExit : public Component
{
public:
    MapExit(const std::string& next_map_path, const struct Vector2* move_other_to_pos, class Game* game) :
    game(game), next_map_path(next_map_path), move_other_to_pos(move_other_to_pos) {}
    void Setup() override;
    ~MapExit() override;
private:
    class BoxCollider* collider = nullptr;
    Game* game;
    std::string next_map_path;
    const Vector2* move_other_to_pos;
    void ChangeLevel(const BoxCollider* other, Game* game);
    
    typedef void (*CollisionCallback)(const BoxCollider*, void*);
    static void OnCollisionStatic(const BoxCollider* other, void* user_data);
    
};
