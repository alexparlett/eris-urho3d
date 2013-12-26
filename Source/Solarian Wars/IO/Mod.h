////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include <Str.h>
#include <StringHash.h>
#include <XMLFile.h>
#include <HashMap.h>

class Mod
{
public:
    Mod() { }
    Mod(const Urho3D::String& dir, Urho3D::XMLFile& file);
    ~Mod() { }

    const Urho3D::String& GetId();
    const Urho3D::String& GetName();
    const Urho3D::String& GetDirectory();
    const Urho3D::String& GetDescription();
    const Urho3D::String& GetAuthor();
    const Urho3D::String& GetVersion();

private:
    Urho3D::String id_;
    Urho3D::String directory_;
    Urho3D::String name_;
    Urho3D::String description_;
    Urho3D::String author_;
    Urho3D::String version_;
};

