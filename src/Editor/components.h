#pragma once
#include <unordered_map>

class Component
{
public:
    Component(const char* name) : name(name) {}
    const char* GetName() const { return name; }

    float GetFloat(const char* property_name) { return float_properties[property_name]; }
    int GetInt(const char* property_name) { return int_properties[property_name]; }
    bool GetBool(const char* property_name) { return bool_properties[property_name]; }
    std::string GetString(const char* property_name) { return string_properties[property_name]; }
    
private:
    const char* name;
public:
    std::unordered_map<const char*, float> float_properties;
    std::unordered_map<const char*, int> int_properties;
    std::unordered_map<const char*, bool> bool_properties;
    std::unordered_map<const char*, std::string> string_properties;
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
    SpriteSheet(const std::string& sprite_file_path, const float width, const float height) : Component("SpriteSheet")
    {
        string_properties.insert({"FilePath", sprite_file_path});
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

// TODO: When serializing animations, nest them per entity under Animator component (see example map1.json)
struct Animation : Component
{
    Animation(const int sprite_sheet_fow,
        const int start_frame, const int end_frame, const int frame_time,
        const bool is_loop, const bool play_on_setup) : Component("Animation")
    {
        int_properties.insert({"SpriteSheetRow", sprite_sheet_fow});
        int_properties.insert({"StartFrame", start_frame});
        int_properties.insert({"EndFrame", end_frame}); 
        int_properties.insert({"FrameTime", frame_time});
        bool_properties.insert({"IsLoop", is_loop});
        bool_properties.insert({"PlayOnSetup", play_on_setup});
    }
};