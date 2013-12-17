////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////


#include "Settings.h"
#include "Exceptions/Exception.h"

#include <File.h>
#include <XMLFile.h>
#include <FileSystem.h>
#include <XMLElement.h>
#include <PackageFile.h>

using namespace Urho3D;

Settings::Settings(Context* context) :
Object(context),
m_Settings(VariantMap())
{
}

void Settings::Load(void)
{
    String fileName = GetUserDirectory() + "settings.xml";

    if (GetSubsystem<FileSystem>()->FileExists(fileName))
    {
        LoadUserSettings(fileName);
    }
    else
    {
        LoadDefaults();
    }
}

void Settings::Save(void)
{
    String fileName = GetUserDirectory() + "settings.xml";

    File file = File(context_, fileName, FILE_WRITE);
    if (!file.IsOpen())
    {
        throw Exception("Unable to open " + fileName);
    }

    XMLFile xmlFile = XMLFile(context_);
    XMLElement root = xmlFile.CreateRoot("settings");

    SaveGraphics(root);
    SaveSound(root);

    if (!xmlFile.Save(file))
    {
        throw Exception("Unable to save " + fileName);
    }
}

const Variant& Settings::GetSetting(const String& name, const Variant& default)
{
    VariantMap::Iterator find = m_Settings.Find(name);

    if (find != m_Settings.End())
    {
        if (find->second_.IsEmpty())
        {
            find->second_ = default;
            return default;
        }

        return find->second_;
    }

    m_Settings[name] = default;
    return default;
}

void Settings::SetSetting(const String& name, const Variant& value)
{
    m_Settings[name] = value;
}

const String Settings::GetUserDirectory(void)
{
    return GetSubsystem<FileSystem>()->GetUserDocumentsDir() + "My Games/Solarian Wars/";
}

void Settings::LoadUserSettings(const String& fileName)
{
    File file = File(context_, fileName);
    if (!file.IsOpen())
    {
        throw Exception("Unable to open " + fileName);
    }

    XMLFile xmlFile = XMLFile(context_);
    if (xmlFile.Load(file))
    {
        XMLElement root = xmlFile.GetRoot();

        LoadFromXml(xmlFile.GetRoot("settings"));
    }
    else
    {
        throw Exception("Unable to load " + fileName);
    }
}

void Settings::LoadDefaults(void)
{
    PackageFile pak = PackageFile(context_, "CoreData.pak");
    File file = File(context_, &pak, "settings.xml");

    if (!file.IsOpen())
    {
        throw Exception("Unable to load settings.xml from CoreData.pak");
    }

    XMLFile xmlFile = XMLFile(context_);
    if (xmlFile.Load(file))
    {
        LoadFromXml(xmlFile.GetRoot("settings"));
    }
    else
    {
        throw Exception("Unable to load settings.xml from CoreData.pak");
    }
}

void Settings::LoadFromXml(const XMLElement& root)
{
    XMLElement graphics = root.GetChild("graphics");
    LoadGraphics(graphics);

    XMLElement sound = root.GetChild("sound");
    LoadSound(sound);
}

void Settings::LoadGraphics(const XMLElement& graphics)
{
    m_Settings["resolution"] = graphics.GetChild("resolution").GetValue();
    m_Settings["fullscreen"] = graphics.GetChild("fullscreen").GetValue();
    m_Settings["antialiasing"] = graphics.GetChild("antialiasing").GetValue();
    m_Settings["vsync"] = graphics.GetChild("vsync").GetValue();
    m_Settings["anisotropic"] = graphics.GetChild("anisotropic").GetValue();
    m_Settings["shadows"] = graphics.GetChild("shadows").GetValue();
    m_Settings["gamma"] = graphics.GetChild("gamma").GetValue();
    m_Settings["shaders"] = graphics.GetChild("shaders").GetValue();
    m_Settings["ssao"] = graphics.GetChild("ssao").GetValue();
}

void Settings::LoadSound(const XMLElement& sound)
{
    m_Settings["master"] = sound.GetChild("master").GetValue();
    m_Settings["music"] = sound.GetChild("music").GetValue();
    m_Settings["interface"] = sound.GetChild("interface").GetValue();
    m_Settings["ambient"] = sound.GetChild("ambient").GetValue();
    m_Settings["effects"] = sound.GetChild("effects").GetValue();
}

void Settings::SaveGraphics(XMLElement& root)
{
    XMLElement graphics = root.CreateChild("graphics");

    graphics.CreateChild("resolution").SetValue(m_Settings["resolution"].GetString());
    graphics.CreateChild("fullscreen").SetValue(m_Settings["fullscreen"].GetString());
    graphics.CreateChild("antialiasing").SetValue(m_Settings["antialiasing"].GetString());
    graphics.CreateChild("vsync").SetValue(m_Settings["vsync"].GetString());
    graphics.CreateChild("anisotropic").SetValue(m_Settings["anisotropic"].GetString());
    graphics.CreateChild("shadows").SetValue(m_Settings["shadows"].GetString());
    graphics.CreateChild("gamma").SetValue(m_Settings["gamma"].GetString());
    graphics.CreateChild("shaders").SetValue(m_Settings["shaders"].GetString());
    graphics.CreateChild("ssao").SetValue(m_Settings["ssao"].GetString());
}

void Settings::SaveSound(XMLElement& root)
{
    XMLElement sound = root.CreateChild("sound");

    sound.CreateChild("master").SetValue(m_Settings["master"].GetString());
    sound.CreateChild("music").SetValue(m_Settings["music"].GetString());
    sound.CreateChild("interface").SetValue(m_Settings["interface"].GetString());
    sound.CreateChild("ambient").SetValue(m_Settings["ambient"].GetString());
    sound.CreateChild("effects").SetValue(m_Settings["effects"].GetString());
}