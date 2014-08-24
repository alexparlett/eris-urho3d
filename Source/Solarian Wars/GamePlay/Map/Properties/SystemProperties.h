#pragma once

#include <RefCounted.h>
#include <Str.h>
#include <Vector3.h>

class SystemProperties : public Urho3D::RefCounted
{
public:
	SystemProperties();

	const Urho3D::String& GetName() const { return name_; }
	const Urho3D::Vector3& GetPosition() const { return position_; }

private:
	Urho3D::String name_;
	Urho3D::Vector3 position_;
};

