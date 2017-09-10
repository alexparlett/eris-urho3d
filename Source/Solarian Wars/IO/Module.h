////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include <Container/Str.h>
#include <Math/StringHash.h>
#include <Resource/XMLFile.h>
#include <Container/HashMap.h>

class Module
{
public:
    Module() { }
    Module(const Urho3D::String& dir, Urho3D::XMLFile& file);
    virtual ~Module() { }

    const Urho3D::String& GetId() const;
    const Urho3D::String& GetName() const;
    const Urho3D::String& GetDirectory() const;
    const Urho3D::String& GetDescription() const;
    const Urho3D::String& GetAuthor() const;
    const Urho3D::String& GetVersion() const;

private:
    Urho3D::String id_;
    Urho3D::String directory_;
    Urho3D::String name_;
    Urho3D::String description_;
    Urho3D::String author_;
    Urho3D::String version_;
};

