#pragma once

#include <Str.h>
#include <Vector3.h>

struct SystemProperties
{
	SystemProperties() : 
		name_(Urho3D::String::EMPTY), 
		position_(Urho3D::Vector3::ZERO), 
		controller_(Urho3D::String::EMPTY) 
	{}

	Urho3D::String name_;
	Urho3D::Vector3 position_;
	Urho3D::String controller_;
};

