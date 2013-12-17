////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "Mod.h"

using namespace Urho3D; 

Mod::Mod() :
m_Id(StringHash::ZERO),
m_Directory(String::EMPTY),
m_Name(String::EMPTY),
m_Description(String::EMPTY),
m_Author(String::EMPTY),
m_Version(String::EMPTY)
{
}

Mod::Mod(const String& dir, XMLFile& file) :
m_Id(StringHash::ZERO),
m_Directory(String::EMPTY),
m_Name(String::EMPTY),
m_Description(String::EMPTY),
m_Author(String::EMPTY),
m_Version(String::EMPTY)
{
    XMLElement root = file.GetRoot();
    m_Id = root.GetAttribute("id");
    m_Directory = dir;
    m_Name = root.GetChild("name").GetValue();
    m_Author = root.GetChild("author").GetValue();
    m_Description = root.GetChild("description").GetValue();
    m_Version = root.GetChild("version").GetValue();
}

const StringHash& Mod::GetId()
{
    return m_Id;
}

const String& Mod::GetName()
{
    return m_Name;
}

const String& Mod::GetDirectory()
{
    return m_Directory;
}

const String& Mod::GetDescription()
{
    return m_Description;
}

const String& Mod::GetAuthor()
{
    return m_Author;
}

const String& Mod::GetVersion()
{
    return m_Version;
}
