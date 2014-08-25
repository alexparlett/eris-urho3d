#pragma once

#include <Str.h>
#include <Vector3.h>

class SystemProperties
{
public:
    SystemProperties() :
        name_(Urho3D::String::EMPTY),
        position_(Urho3D::Vector3::ZERO),
        controller_(Urho3D::String::EMPTY)
    {
    }

    Urho3D::String GetName() const { return name_; }
    Urho3D::Vector3 GetPosition() const { return position_; }
    Urho3D::String GetController() const { return controller_; }

    void SetName(const Urho3D::String& name) { name_ = name; }
    void SetPosition(const Urho3D::Vector3& position) { position_ = position; }
    void SetController(const Urho3D::String& controller) { controller_ = controller; }

private:
	Urho3D::String name_;
	Urho3D::Vector3 position_;
	Urho3D::String controller_;
};

