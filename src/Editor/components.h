#pragma once

class Component
{
public:
    Component(const char* name) : name(name) {}
    const char* GetName() const { return name; }
private:
    const char* name;
};

template<typename T>
class ComponentProperty
{
public:
    ComponentProperty() = default;
    ComponentProperty(const char* name, T value) : name(name), value(value) {}
    T& GetValue() const { return value; }
    void SetValue(T& value) { this->value = value; }
    
private:
    const char* name;
    T value;
};

struct Transform : Component
{
    Transform(const float x, const float y) : Component("Transform")
    {
        this->x = ComponentProperty("X", x);
        this->y = ComponentProperty("Y", y);
    }
    
    ComponentProperty<float> x;
    ComponentProperty<float> y;
};

struct MapCollider : Component
{
    MapCollider(const float width, const float height) : Component("MapCollider")
    {
        this->width = ComponentProperty("Width", width);
        this->height = ComponentProperty("Height", height);
    }
    
    ComponentProperty<float> width;
    ComponentProperty<float> height;
};

struct BoxCollider : Component
{
    BoxCollider(const float width, const float height) : Component("BoxCollider")
    {
        this->width = ComponentProperty("Width", width);
        this->height = ComponentProperty("Height", height);
    }
    
    ComponentProperty<float> width;
    ComponentProperty<float> height;
};

struct SpriteSheet : Component
{
    SpriteSheet(const std::string& sprite_file_path, const float width, const float height) : Component("SpriteSheet")
    {
        this->sprite_file_path = ComponentProperty("FilePath", sprite_file_path);
        this->width = ComponentProperty("Width", width);
        this->height = ComponentProperty("Height", height);
    }

    ComponentProperty<std::string> sprite_file_path;
    ComponentProperty<float> width;
    ComponentProperty<float> height;
};

struct Rigidbody : Component
{
    Rigidbody(const float mass, const float drag) : Component("Rigidbody")
    {
        this->mass = ComponentProperty("Mass", mass);
        this->drag = ComponentProperty("Drag", drag);
    }
    
    ComponentProperty<float> mass;
    ComponentProperty<float> drag;
};

struct Animation
{
    Animation() = default;
    Animation(const int sprite_sheet_fow, const int start_frame, const int end_frame, const int frame_time, const bool is_loop, const bool play_on_setup)
    {
        this->sprite_sheet_fow = ComponentProperty("SpriteSheetRow", sprite_sheet_fow);
        this->start_frame = ComponentProperty("StartFrame", start_frame);
        this->end_frame = ComponentProperty("EndFrame", end_frame); 
        this->frame_time = ComponentProperty("FrameTime", frame_time);
        this->is_loop = ComponentProperty("IsLoop", is_loop);
        this->play_on_setup = ComponentProperty("PlayOnSetup", play_on_setup);
    }
    
    ComponentProperty<int> sprite_sheet_fow;
    ComponentProperty<int> start_frame;
    ComponentProperty<int> end_frame;
    ComponentProperty<int> frame_time;
    ComponentProperty<bool> is_loop;
    ComponentProperty<bool> play_on_setup;
};

struct Animator : Component
{
    Animator(const int sprite_sheet_fow, const int start_frame, const int end_frame, const int frame_time, const bool is_loop, const bool play_on_setup) : Component("Animator")
    {
        this->animation = ComponentProperty("Animation", 
            new Animation(0,0,0,1000,true, true));
    }
    
    ComponentProperty<Animation*> animation;
};