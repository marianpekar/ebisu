#include "projectile_pool.h"
#include "../collision_solver.h"

void ProjectilePool::Add(const size_t& count, const int& width, const int& height, const std::shared_ptr<class CollisionSolver>& collision_solver)
{
    for (size_t i = 0; i < count; ++i)
    {
        std::shared_ptr<Projectile> projectile = std::make_shared<Projectile>(width, height);
        collision_solver->AddCollider(projectile);
        pool.push(projectile);
    }
}

std::shared_ptr<Projectile>& ProjectilePool::Get()
{
    auto proj = pool.top();
    pool.pop();
    return proj;
}

void ProjectilePool::Return(const std::shared_ptr<Projectile>& proj)
{
    pool.push(proj);
}

void ProjectilePool::ReturnAll()
{
    auto it = active_projectiles.begin();
    while (it != active_projectiles.end())
    {
        Return(*it);
        it = active_projectiles.erase(it);
    }
}

