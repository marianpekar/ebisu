#include "projectile_acceptor.h"

void ProjectileAcceptor::RegisterCallback(AcceptCallback callback, void* user_data)
{
    callbacks.emplace_back(callback, user_data);
}

void ProjectileAcceptor::Accept(const std::shared_ptr<Entity>& entity) const
{
    for(auto& callback : callbacks)
    {
        callback.first(entity, callback.second);
    }
}
