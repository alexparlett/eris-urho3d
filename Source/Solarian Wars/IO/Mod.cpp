////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "Mod.h"

using namespace Urho3D; 

Mod::Mod(const String& dir, XMLFile& file)
{
    XMLElement root = file.GetRoot();
    id_ = root.GetAttribute("id");
    directory_ = dir;
    name_ = root.GetChild("name").GetValue();
    author_ = root.GetChild("author").GetValue();
    description_ = root.GetChild("description").GetValue();
    version_ = root.GetChild("version").GetValue();
}

const String& Mod::GetId() const
{
    return id_;
}

const String& Mod::GetName() const
{
    return name_;
}

const String& Mod::GetDirectory() const
{
    return directory_;
}

const String& Mod::GetDescription() const
{
    return description_;
}

const String& Mod::GetAuthor() const
{
    return author_;
}

const String& Mod::GetVersion() const
{
    return version_;
}
