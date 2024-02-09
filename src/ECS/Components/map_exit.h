#pragma once

#include <memory>
#include <string>

#include "../../Math/vector2.h"
#include "component.h"

class MapExit : public Component
{
public:
    MapExit(std::string& next_map_path, float move_to_x, float move_to_y, class Game* game) :
    game(game), next_map_path(std::move(next_map_path)), move_other_to_pos(Vector2(move_to_x, move_to_y)) {}
    void Setup() override;
private:
    std::shared_ptr<class Collider> collider;
    Game* game;
    std::string next_map_path;
    Vector2 move_other_to_pos;
    void ChangeLevel(const std::shared_ptr<Collider>& other, Game* game) const;
    
    typedef void (*CollisionCallback)(const std::shared_ptr<class Collider>&, void*);
    static void OnCollisionStatic(const std::shared_ptr<Collider>& other, void* user_data);
    
};
