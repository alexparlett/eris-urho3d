////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "Settings.h"
#include "Exceptions/Exception.h"
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
}

void Settings::Load(void)
{
    String fileName = GetSubsystem<FileSystem>()->GetUserDocumentsDir() + "My Games/Solarian Wars/settings.xml";

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
    String fileName = GetSubsystem<FileSystem>()->GetUserDocumentsDir() + "My Games/Solarian Wars/settings.xml";

    File file = File(context_, fileName, FILE_WRITE);
    if (file.IsOpen())
    {
        XMLFile xmlFile = XMLFile(context_);
        XMLElement root = xmlFile.CreateRoot("settings");

        SaveGraphics(root);
        SaveSound(root);
        SaveGame(root);

        if (!xmlFile.Save(file))
        {
            LOGERROR("Unable to save " + fileName);
        }
    }
    else
    {
        LOGERROR("Unable to open " + fileName);
    }
}

const Variant& Settings::GetSetting(const String& name, const Variant& default) const
{
    VariantMap::ConstIterator find = m_Settings.Find(name);
    return find != m_Settings.End() ? find->second_ : default;
}

void Settings::SetSetting(const String& name, const Variant& value)
{
    m_Settings[name] = value;
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

    XMLElement game = root.GetChild("game");
    LoadGame(game);
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

void Settings::LoadGame(const XMLElement& game)
{
    m_Settings["autosave"] = game.GetChild("autosave").GetValue();
    m_Settings["frequency"] = game.GetChild("frequency").GetValue();
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

void Settings::SaveGame(XMLElement& root)
{
    XMLElement game = root.CreateChild("game");

    game.CreateChild("autosave").SetValue(m_Settings["autosave"].GetString());
    game.CreateChild("frequency").SetValue(m_Settings["frequency"].GetString());
}