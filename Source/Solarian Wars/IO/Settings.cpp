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
    settings_["version"] = String(version);
    settings_["userdir"] = GetSubsystem<FileSystem>()->GetUserDocumentsDir() + "My Games/Solarian Wars/";
}

void Settings::Load(void)
{
    String fileName = settings_["userdir"].GetString() + "settings.xml";

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
    String fileName = settings_["userdir"].GetString() + "settings.xml";

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
    VariantMap::ConstIterator find = settings_.Find(name);
    return find != settings_.End() ? find->second_ : default;
}

void Settings::SetSetting(const String& name, const Variant& value)
{
    settings_[name] = value;
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
    settings_["resolution"] = graphics.GetChild("resolution").GetValue();
    settings_["fullscreen"] = graphics.GetChild("fullscreen").GetValue();
    settings_["antialiasing"] = graphics.GetChild("antialiasing").GetValue();
    settings_["vsync"] = graphics.GetChild("vsync").GetValue();
    settings_["anisotropic"] = graphics.GetChild("anisotropic").GetValue();
    settings_["shadows"] = graphics.GetChild("shadows").GetValue();
    settings_["gamma"] = graphics.GetChild("gamma").GetValue();
    settings_["shaders"] = graphics.GetChild("shaders").GetValue();
    settings_["ssao"] = graphics.GetChild("ssao").GetValue();
}

void Settings::LoadSound(const XMLElement& sound)
{
    settings_["master"] = sound.GetChild("master").GetValue();
    settings_["music"] = sound.GetChild("music").GetValue();
    settings_["interface"] = sound.GetChild("interface").GetValue();
    settings_["ambient"] = sound.GetChild("ambient").GetValue();
    settings_["effects"] = sound.GetChild("effects").GetValue();
}

void Settings::LoadGame(const XMLElement& game)
{
    settings_["autosave"] = game.GetChild("autosave").GetValue();
    settings_["frequency"] = game.GetChild("frequency").GetValue();
}

void Settings::SaveGraphics(XMLElement& root)
{
    XMLElement graphics = root.CreateChild("graphics");

    graphics.CreateChild("resolution").SetValue(settings_["resolution"].GetString());
    graphics.CreateChild("fullscreen").SetValue(settings_["fullscreen"].GetString());
    graphics.CreateChild("antialiasing").SetValue(settings_["antialiasing"].GetString());
    graphics.CreateChild("vsync").SetValue(settings_["vsync"].GetString());
    graphics.CreateChild("anisotropic").SetValue(settings_["anisotropic"].GetString());
    graphics.CreateChild("shadows").SetValue(settings_["shadows"].GetString());
    graphics.CreateChild("gamma").SetValue(settings_["gamma"].GetString());
    graphics.CreateChild("shaders").SetValue(settings_["shaders"].GetString());
    graphics.CreateChild("ssao").SetValue(settings_["ssao"].GetString());
}

void Settings::SaveSound(XMLElement& root)
{
    XMLElement sound = root.CreateChild("sound");

    sound.CreateChild("master").SetValue(settings_["master"].GetString());
    sound.CreateChild("music").SetValue(settings_["music"].GetString());
    sound.CreateChild("interface").SetValue(settings_["interface"].GetString());
    sound.CreateChild("ambient").SetValue(settings_["ambient"].GetString());
    sound.CreateChild("effects").SetValue(settings_["effects"].GetString());
}

void Settings::SaveGame(XMLElement& root)
{
    XMLElement game = root.CreateChild("game");

    game.CreateChild("autosave").SetValue(settings_["autosave"].GetString());
    game.CreateChild("frequency").SetValue(settings_["frequency"].GetString());
}