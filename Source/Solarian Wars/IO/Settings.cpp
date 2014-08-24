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
#include <StringUtils.h>

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
    File file(context_);

    if (GetSubsystem<FileSystem>()->FileExists(fileName))
        file.Open(fileName);
    else
        file.Open("settings.xml");


    if (!file.IsOpen())
    {
        LOGERROR("Unable to open settings file " + fileName);
        return;
    }

    XMLFile xmlFile = XMLFile(context_);
    if (xmlFile.Load(file))
    {
        XMLElement root = xmlFile.GetRoot();

        XMLElement graphics = root.GetChild("graphics");
        LoadGraphics(graphics);

        XMLElement sound = root.GetChild("sound");
        LoadSound(sound);

        XMLElement game = root.GetChild("game");
        LoadGame(game);
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
            LOGERROR("Unable to save " + fileName);
        
    }
    else
        LOGERROR("Unable to open " + fileName);
}

const Variant& Settings::GetSetting(const String& name, const Variant& default) const
{
    VariantMap::ConstIterator find = settings_.Find(name);
    
    if(find != settings_.End())
        return find->second_;
    
    const_cast<Settings&>(*this).SetSetting(name, default);

    return default;
}

void Settings::SetSetting(const String& name, const Variant& value)
{
    settings_[name] = value;
}

void Settings::LoadGraphics(const XMLElement& graphics)
{
    settings_["resolution"] = ToIntVector2(graphics.GetChild("resolution").GetValue());
    settings_["fullscreen"] = ToBool(graphics.GetChild("fullscreen").GetValue());
    settings_["borderless"] = ToBool(graphics.GetChild("borderless").GetValue());
    settings_["antialiasing"] = ToInt(graphics.GetChild("antialiasing").GetValue());
    settings_["vsync"] = ToBool(graphics.GetChild("vsync").GetValue());
    settings_["anisotropic"] = ToInt(graphics.GetChild("anisotropic").GetValue());
    settings_["shadows"] = ToInt(graphics.GetChild("shadows").GetValue());
    settings_["gamma"] = ToFloat(graphics.GetChild("gamma").GetValue());
    settings_["shaders"] = ToInt(graphics.GetChild("shaders").GetValue());
    settings_["ssao"] = ToInt(graphics.GetChild("ssao").GetValue());
    settings_["quality"] = ToInt(graphics.GetChild("quality").GetValue());
    settings_["bloom"] = ToBool(graphics.GetChild("bloom").GetValue());
}

void Settings::LoadSound(const XMLElement& sound)
{
    settings_["master"] = ToFloat(sound.GetChild("master").GetValue());
    settings_["music"] = ToFloat(sound.GetChild("music").GetValue());
    settings_["interface"] = ToFloat(sound.GetChild("interface").GetValue());
    settings_["ambient"] = ToFloat(sound.GetChild("ambient").GetValue());
    settings_["effects"] = ToFloat(sound.GetChild("effects").GetValue());
}

void Settings::LoadGame(const XMLElement& game)
{
    settings_["language"] = game.GetChild("language").GetValue();
    settings_["autosave"] = ToBool(game.GetChild("autosave").GetValue());
    settings_["frequency"] = ToInt(game.GetChild("frequency").GetValue());
}

void Settings::SaveGraphics(XMLElement& root)
{
    XMLElement graphics = root.CreateChild("graphics");

    graphics.CreateChild("resolution").SetValue(settings_["resolution"].ToString());
    graphics.CreateChild("fullscreen").SetValue(settings_["fullscreen"].ToString());
    graphics.CreateChild("borderless").SetValue(settings_["borderless"].ToString());
    graphics.CreateChild("antialiasing").SetValue(settings_["antialiasing"].ToString());
    graphics.CreateChild("vsync").SetValue(settings_["vsync"].ToString());
    graphics.CreateChild("anisotropic").SetValue(settings_["anisotropic"].ToString());
    graphics.CreateChild("shadows").SetValue(settings_["shadows"].ToString());
    graphics.CreateChild("gamma").SetValue(settings_["gamma"].ToString());
    graphics.CreateChild("shaders").SetValue(settings_["shaders"].ToString());
    graphics.CreateChild("ssao").SetValue(settings_["ssao"].ToString());
    graphics.CreateChild("quality").SetValue(settings_["quality"].ToString());
    graphics.CreateChild("bloom").SetValue(settings_["bloom"].ToString());
}

void Settings::SaveSound(XMLElement& root)
{
    XMLElement sound = root.CreateChild("sound");

    sound.CreateChild("master").SetValue(settings_["master"].ToString());
    sound.CreateChild("music").SetValue(settings_["music"].ToString());
    sound.CreateChild("interface").SetValue(settings_["interface"].ToString());
    sound.CreateChild("ambient").SetValue(settings_["ambient"].ToString());
    sound.CreateChild("effects").SetValue(settings_["effects"].ToString());
}

void Settings::SaveGame(XMLElement& root)
{
    XMLElement game = root.CreateChild("game");

    game.CreateChild("language").SetValue(settings_["language"].ToString());
    game.CreateChild("autosave").SetValue(settings_["autosave"].ToString());
    game.CreateChild("frequency").SetValue(settings_["frequency"].ToString());
}