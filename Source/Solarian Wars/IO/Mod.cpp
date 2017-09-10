////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "Module.h"

using namespace Urho3D; 

Module::Module(const String& dir, XMLFile& file)
{
    XMLElement root = file.GetRoot();
    id_ = root.GetAttribute("id");
    directory_ = dir;
    name_ = root.GetChild("name").GetValue();
    author_ = root.GetChild("author").GetValue();
    description_ = root.GetChild("description").GetValue();
    version_ = root.GetChild("version").GetValue();
}

const String& Module::GetId() const
{
    return id_;
}

const String& Module::GetName() const
{
    return name_;
}

const String& Module::GetDirectory() const
{
    return directory_;
}

const String& Module::GetDescription() const
{
    return description_;
}

const String& Module::GetAuthor() const
{
    return author_;
}

const String& Module::GetVersion() const
{
    return version_;
}
