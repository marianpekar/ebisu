﻿#pragma once
#include <unordered_map>

class Component
{
public:
    Component(const char* name) : name(name) {}
    const char* GetName() const { return name; }

    float GetFloat(const char* property_name) { return float_properties[property_name]; }
    void SetFloat(const char* property_name, float value) { float_properties[property_name] = value; }
    
    int GetInt(const char* property_name) { return int_properties[property_name]; }
    bool GetBool(const char* property_name) { return bool_properties[property_name]; }
    std::string GetString(const char* property_name) { return string_properties[property_name]; }
    std::string GetPath(const char* property_name) { return path_properties[property_name]; }
    
private:
    const char* name;
public:
    std::unordered_map<const char*, float> float_properties;
    std::unordered_map<const char*, int> int_properties;
    std::unordered_map<const char*, bool> bool_properties;
    std::unordered_map<const char*, std::string> string_properties;
    std::unordered_map<const char*, std::string> path_properties;
};

struct Transform : Component
{
    Transform(const float x, const float y) : Component("Transform")
    {
        float_properties.insert({"X", x});
        float_properties.insert({"Y", y});
    }
};

struct MapCollider : Component
{
    MapCollider(const float width, const float height) : Component("MapCollider")
    {
        float_properties.insert({"Width", width});
        float_properties.insert({"Height", height});
    }
};

struct BoxCollider : Component
{
    BoxCollider(const float width, const float height) : Component("BoxCollider")
    {
        float_properties.insert({"Width", width});
        float_properties.insert({"Height", height});
    }
};

struct SpriteSheet : Component
{
    size_t texture_index = -1;
    
    SpriteSheet(const std::string& sprite_file_path, const float width, const float height) : Component("SpriteSheet")
    {
        path_properties.insert({"FilePath", sprite_file_path});
        float_properties.insert({"Width", width});
        float_properties.insert({"Height", height});
    }
};

struct Rigidbody : Component
{
    Rigidbody(const float mass, const float drag) : Component("Rigidbody")
    {
        float_properties.insert({"Mass", mass});
        float_properties.insert({"Drag", drag});
    }
};

struct Animator : Component
{
    Animator(const int sprite_sheet_fow,
        const int start_frame, const int end_frame, const int frame_time,
        const bool is_loop, const bool play_on_setup) : Component("Animator")
    {
        int_properties.insert({"SpriteSheetRow", sprite_sheet_fow});
        int_properties.insert({"StartFrame", start_frame});
        int_properties.insert({"EndFrame", end_frame}); 
        int_properties.insert({"FrameTime", frame_time});
        bool_properties.insert({"IsLoop", is_loop});
        bool_properties.insert({"PlayOnSetup", play_on_setup});
    }
};

struct CharacterAnimator : Component
{
    CharacterAnimator(const float idle_animation_frame_time,
        const int idle_animation_start_frame, const int idle_animation_end_frame,
        const float move_animation_frame_time,
        const int move_animation_start_frame, const int move_animation_end_frame) : Component("CharacterAnimator")
    {
        float_properties.insert({"IdleAnimationFrameTime", idle_animation_frame_time});
        int_properties.insert({"IdleAnimationStartFrame", idle_animation_start_frame});
        int_properties.insert({"IdleAnimationEndFrame", idle_animation_end_frame});
        float_properties.insert({"MoveAnimationFrameTime", move_animation_frame_time});
        int_properties.insert({"MoveAnimationStartFrame", move_animation_start_frame});
        int_properties.insert({"MoveAnimationEndFrame", move_animation_end_frame});
    }
};

struct Agent : Component
{
    Agent(const int target_entity_id, const float speed, const float stopping_distance) : Component("Agent")
    {
        int_properties.insert({"TargetEntityId", target_entity_id});
        float_properties.insert({"Speed", speed});
        float_properties.insert({"StoppingDistance", stopping_distance});
    }
};

struct MainCamera : Component
{
    MainCamera() : Component("MainCamera") {}
};

struct PlayerController : Component
{
    PlayerController(float speed) : Component("PlayerController")
    {
        float_properties.insert({"Speed", speed});
    }
};

struct MapExit : Component
{
    MapExit(const std::string& next_map_path, float move_other_to_x, float move_other_to_y) : Component("MapExit")
    {
        path_properties.insert({"NextMapPath", next_map_path});
        float_properties.insert({"MoveOtherToX", move_other_to_x});
        float_properties.insert({"MoveOtherToY", move_other_to_y});
    }
};

struct ProjectileEmitter : Component
{
    ProjectileEmitter(const std::string& sprite_sheet_filepath, float proj_width, float proj_height, float proj_speed,
        float proj_lifetime, int pool_size, float emit_delay) : Component("ProjectileEmitter")
    {
        path_properties.insert({"ProjectileSpriteSheetFilePath", sprite_sheet_filepath });
        float_properties.insert({"ProjectileWidth", proj_width});
        float_properties.insert({"ProjectileHeight", proj_height});
        float_properties.insert({"ProjectileSpeed", proj_speed});
        float_properties.insert({"ProjectileLifeTime", proj_lifetime});
        int_properties.insert({"PoolSize", pool_size});
        float_properties.insert({"EmitDelay", emit_delay});
    }
};

struct ProjectileAcceptor : Component
{
    ProjectileAcceptor() : Component("ProjectileAcceptor") { }
};

struct DamageReceptor : Component
{
    DamageReceptor(float on_projectile_hit) : Component("DamageReceptor")
    {
        float_properties.insert({"OnProjectileHit", on_projectile_hit});
    }
};

struct Health : Component
{
    Health(float init_health, float max_health) : Component("Health")
    {
        float_properties.insert({"InitialHealth", init_health});
        float_properties.insert({"MaxHealth", max_health});
    }
};
