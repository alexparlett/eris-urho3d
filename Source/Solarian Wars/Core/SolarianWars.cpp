////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "SolarianWars.h"
#include "StateManager.h"
#include "IO/Settings.h"
#include "States/LaunchState.h"
#include "Events.h"
#include "ModManager.h"
#include "SaveManager.h"
#include "IO/Locale.h"
#include "ScriptDefs.h"

#include <APITemplates.h>
#include <Script.h>
#include <FileSystem.h>
#include <Input.h>
#include <Graphics.h>
#include <Audio.h>
#include <ResourceCache.h>
#include <StringHash.h>
#include <DebugHud.h>
#include <Console.h>

#include <windows.h>

using namespace Urho3D;

SolarianWars::SolarianWars(Context* context) :
    Application(context)
{
    context->RegisterSubsystem(new Script(context));
    context->RegisterSubsystem(new Settings(context));
    context->RegisterSubsystem(new StateManager(context));
    context->RegisterSubsystem(new ModManager(context));
    context->RegisterSubsystem(new SaveManager(context));
    context->RegisterSubsystem(new Locale(context));
}

void SolarianWars::Setup()
{
    Audio* audio = GetSubsystem<Audio>();
    Settings* settings = GetSubsystem<Settings>();
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    Input* input = GetSubsystem<Input>();
    ModManager* mm = GetSubsystem<ModManager>();
    SaveManager* sm = GetSubsystem<SaveManager>();
    Locale* locale = GetSubsystem<Locale>();

    input->SetMouseVisible(true);
    cache->SetAutoReloadResources(false);

    settings->Load();
    mm->Load();
    locale->Load(settings->GetSetting("language", "enGB").GetString());

    engineParameters_["Headless"] = false;
    engineParameters_["ResourcePaths"] = "00;Data";
    engineParameters_["LogName"] = GetSubsystem<Settings>()->GetSetting("userdir").GetString() + "sw.log";

    engineParameters_["WindowTitle"] = "Solarian Wars";
    engineParameters_["WindowIcon"] = "Textures/Icon.png";
    engineParameters_["WindowWidth"] = settings->GetSetting("resolution", 0).GetIntVector2().x_;
    engineParameters_["WindowHeight"] = settings->GetSetting("resolution", 0).GetIntVector2().y_;
    engineParameters_["FullScreen"] = settings->GetSetting("fullscreen", true).GetBool();
    engineParameters_["Borderless"] = settings->GetSetting("borderless", false).GetBool();
    engineParameters_["WindowResizable"] = !(settings->GetSetting("fullscreen", true).GetBool() && settings->GetSetting("borderless", false).GetBool());
    engineParameters_["VSync"] = settings->GetSetting("vsync", false).GetBool();
    engineParameters_["MultiSample"] = settings->GetSetting("antialiasing", 2).GetInt();
    engineParameters_["Anisotropy"] = settings->GetSetting("anisotropy", 4).GetInt();
    engineParameters_["TextureQuality"] = settings->GetSetting("quality", 2).GetInt();
    engineParameters_["MaterialQuality"] = settings->GetSetting("shaders", 2).GetInt();
    engineParameters_["Shadows"] = settings->GetSetting("shadows", 2).GetInt() > 0 ? true : false;
    engineParameters_["LowQualityShadows"] = settings->GetSetting("shadows", 2).GetInt() == 1 ? true : false;

    audio->SetMasterGain(SoundType::SOUND_MASTER, settings->GetSetting("master", 1.0f).GetFloat());
    audio->SetMasterGain(SoundType::SOUND_AMBIENT, settings->GetSetting("ambient", 0.75f).GetFloat());
    audio->SetMasterGain(SoundType::SOUND_MUSIC, settings->GetSetting("music", 0.75f).GetFloat());
    audio->SetMasterGain(SoundType::SOUND_EFFECT, settings->GetSetting("effects", 0.75f).GetFloat());
    audio->SetMasterGain(SoundType::SOUND_UI, settings->GetSetting("interface", 0.75f).GetFloat());

    RegisterScriptAPI(GetSubsystem<Script>()->GetScriptEngine());

    ParseArgs();
}

void SolarianWars::Start()
{
    VariantMap createData;
    createData[StateCreated::P_STATE] = new LaunchState(context_);
    createData[StateCreated::P_ID] = StringHash("LaunchState");
    SendEvent(E_STATE_CREATED, createData);

    VariantMap changeData;
    changeData[StateChanged::P_ID] = StringHash("LaunchState");
    SendEvent(E_STATE_CHANGED, changeData);
}

void SolarianWars::Stop()
{
}

void SolarianWars::ParseArgs()
{
    if (GetArguments().Contains("-debug"))
    {
    }

    if (GetArguments().Contains("-api"))
    {
        File output(context_, "api.doxy");
        GetSubsystem<Script>()->DumpAPI(output);
    }
}

DEFINE_APPLICATION_MAIN(SolarianWars)