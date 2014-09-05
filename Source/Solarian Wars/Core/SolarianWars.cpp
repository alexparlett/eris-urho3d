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
#include <Log.h>
#include <Renderer.h>
#include <Viewport.h>

#include <windows.h>

using namespace Urho3D;

SolarianWars::SolarianWars(Context* context) :
    Application(context)
{
    context->RegisterSubsystem(new Script(context));
    context->RegisterSubsystem(new Settings(context));
    context->RegisterSubsystem(new StateManager(context));
    context->RegisterSubsystem(new ModManager(context));
    context->RegisterSubsystem(new Locale(context));

    RegisterScriptAPI(GetSubsystem<Script>()->GetScriptEngine());
}

void SolarianWars::Setup()
{
    Audio* audio = GetSubsystem<Audio>();
    Settings* settings = GetSubsystem<Settings>();
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    Input* input = GetSubsystem<Input>();
    Log* log = GetSubsystem<Log>();
    ModManager* mm = GetSubsystem<ModManager>();
    Locale* locale = GetSubsystem<Locale>();

    log->Open(settings->GetSetting("userdir").GetString() + "sw.log");
    log->WriteRaw("[" + Time::GetTimeStamp() + "] Solarian Wars " + settings->GetSetting("version").GetString() + "\n");

    ParseArgs();

    input->SetMouseVisible(true);
    cache->SetAutoReloadResources(false);

    settings->Load();

    engineParameters_["Headless"] = false;
    engineParameters_["ResourcePaths"] = "00;Data";

    engineParameters_["WindowTitle"] = "Solarian Wars";
    engineParameters_["WindowIcon"] = "Textures/Icon.png";
    engineParameters_["WindowWidth"] = settings->GetSetting("resolution", 0).GetIntVector2().x_;
    engineParameters_["WindowHeight"] = settings->GetSetting("resolution", 0).GetIntVector2().y_;
    engineParameters_["FullScreen"] = settings->GetSetting("fullscreen", true).GetBool();
    engineParameters_["Borderless"] = settings->GetSetting("borderless", false).GetBool();
    engineParameters_["WindowResizable"] = !(settings->GetSetting("fullscreen", true).GetBool() && settings->GetSetting("borderless", false).GetBool());
    engineParameters_["VSync"] = settings->GetSetting("vsync", false).GetBool();
    engineParameters_["MultiSample"] = settings->GetSetting("antialiasing", 2).GetInt();
    engineParameters_["TextureAnisotropy"] = settings->GetSetting("anisotropy", 4).GetInt();
    engineParameters_["TextureQuality"] = settings->GetSetting("quality", 2).GetInt();
    engineParameters_["MaterialQuality"] = settings->GetSetting("shaders", 2).GetInt();
    engineParameters_["Shadows"] = settings->GetSetting("shadows", 2).GetInt() > 0 ? true : false;
    engineParameters_["LowQualityShadows"] = settings->GetSetting("shadows", 2).GetInt() == 1 ? true : false;

    audio->SetMasterGain(SoundType::SOUND_MASTER, settings->GetSetting("master", 1.0f).GetFloat());
    audio->SetMasterGain(SoundType::SOUND_AMBIENT, settings->GetSetting("ambient", 0.6f).GetFloat());
    audio->SetMasterGain(SoundType::SOUND_MUSIC, settings->GetSetting("music", 0.4f).GetFloat());
    audio->SetMasterGain(SoundType::SOUND_EFFECT, settings->GetSetting("effects", 0.6f).GetFloat());
    audio->SetMasterGain(SoundType::SOUND_UI, settings->GetSetting("interface", 0.6f).GetFloat());
}

void SolarianWars::Start()
{
    GetSubsystem<ModManager>()->Load();
    GetSubsystem<Locale>()->Load(GetSubsystem<Settings>()->GetSetting("language", "enGB").GetString());

	Viewport* port = new Viewport(context_);
	port->SetRenderPath(GetSubsystem<ResourceCache>()->GetResource<XMLFile>("RenderPaths/Deferred.xml"));
	GetSubsystem<Renderer>()->SetViewport(0, port);

    VariantMap createData = GetEventDataMap();
    createData[StateCreated::P_STATE] = new LaunchState(context_);
    createData[StateCreated::P_ID] = StringHash("LaunchState");
    SendEvent(E_STATE_CREATED, createData);

    VariantMap changeData = GetEventDataMap();
    changeData[StateChanged::P_ID] = StringHash("LaunchState");
    SendEvent(E_STATE_CHANGED, changeData);
}

void SolarianWars::Stop()
{
    context_->RemoveSubsystem<StateManager>();
    context_->RemoveSubsystem<Locale>();
    context_->RemoveSubsystem<ModManager>();
    context_->RemoveSubsystem<Settings>();
}

void SolarianWars::ParseArgs()
{
    if (GetArguments().Contains("-debug"))
        GetSubsystem<Log>()->SetLevel(LOG_DEBUG);
}

DEFINE_APPLICATION_MAIN(SolarianWars)