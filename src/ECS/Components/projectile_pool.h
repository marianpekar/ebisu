#pragma once

#include <stack>
#include "projectile.h"

class ProjectilePool
{
    std::stack<std::shared_ptr<Projectile>> pool;
    std::vector<std::shared_ptr<Projectile>> active_projectiles;
public:
    std::vector<std::shared_ptr<Projectile>>& GetActiveProjectiles() { return active_projectiles; }
    
    void Add(const size_t& count, const int& width, const int& height, const int& frames, const std::shared_ptr<class CollisionSolver>& collision_solver);
    std::shared_ptr<Projectile>& Get();
    void Return(const std::shared_ptr<Projectile>& proj);
    void ReturnAll();
    
    bool IsEmpty() const { return pool.empty(); }
};
