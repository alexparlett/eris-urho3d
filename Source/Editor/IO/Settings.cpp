////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "Settings.h"
#include "../gitversion.h"

#include <File.h>
#include <XMLFile.h>
#include <FileSystem.h>
#include <XMLElement.h>
#include <PackageFile.h>
#include <Log.h>

using namespace Urho3D;

Settings::Settings(Context* context) :
Object(context)
{
    m_Settings["version"] = String(version);
    m_Settings["userdir"] = GetSubsystem<FileSystem>()->GetUserDocumentsDir() + "My Games/Solarian Wars/";
}

bool Settings::Load(void)
{
    String fileName = m_Settings["userdir"].GetString() + "editor-settings.xml";
}

bool Settings::Save(void)
{
    String fileName = m_Settings["userdir"].GetString() + "editor-settings.xml";
}

const Variant& Settings::GetSetting(const String& name, const Variant& default) const
{
    VariantMap::ConstIterator find = m_Settings.Find(name);
    return (find != m_Settings.End() && (find->second_ .IsZero() || find->second_.IsEmpty()) ? find->second_ : default);
}

void Settings::SetSetting(const String& name, const Variant& value)
{
    m_Settings[name] = value;
}