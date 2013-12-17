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
    Mod();
    Mod(const Urho3D::String& dir, Urho3D::XMLFile& file);
    ~Mod() { }

    const Urho3D::StringHash& GetId();
    const Urho3D::String& GetName();
    const Urho3D::String& GetDirectory();
    const Urho3D::String& GetDescription();
    const Urho3D::String& GetAuthor();
    const Urho3D::String& GetVersion();

private:
    Urho3D::StringHash m_Id;
    Urho3D::String m_Directory;
    Urho3D::String m_Name;
    Urho3D::String m_Description;
    Urho3D::String m_Author;
    Urho3D::String m_Version;
};

