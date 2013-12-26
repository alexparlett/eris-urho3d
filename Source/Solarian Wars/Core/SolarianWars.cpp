////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "SolarianWars.h"
#include "StateManager.h"
#include "Exceptions/Exception.h"
#include "IO/Settings.h"
#include "States/LaunchState.h"
#include "States/LoadingState.h"
#include "Events.h"
#include "ModManager.h"
#include "SaveManager.h"

#include <Script.h>
#include <FileSystem.h>
#include <Input.h>
#include <Graphics.h>
#include <Audio.h>
#include <ResourceCache.h>
#include <StringHash.h>

using namespace Urho3D;

SolarianWars::SolarianWars(Context* context) :
    Application(context)
{
    context_->RegisterSubsystem(new Script(context));
    context_->RegisterSubsystem(new Settings(context));
    context_->RegisterSubsystem(new StateManager(context));
    context_->RegisterSubsystem(new ModManager(context));
    context_->RegisterSubsystem(new SaveManager(context));
}

void SolarianWars::Setup()
{
    try
    {
        Audio* audio = GetSubsystem<Audio>();
        Settings* settings = GetSubsystem<Settings>();
        ResourceCache* cache = GetSubsystem<ResourceCache>();
        Input* input = GetSubsystem<Input>();
        ModManager* mm = GetSubsystem<ModManager>();
        SaveManager* sm = GetSubsystem<SaveManager>();

        cache->SetAutoReloadResources(false);
        settings->Load();
        
        engineParameters_["WindowTitle"] = "Solarian Wars";
        engineParameters_["WindowIcon"] = "Textures/Icon.png";
        engineParameters_["LogName"] = GetSubsystem<Settings>()->GetSetting("userdir").GetString() + "sw.log";
        engineParameters_["WindowWidth"] = settings->GetSetting("resolution", "1024 768").GetIntVector2().x_;
        engineParameters_["WindowHeight"] = settings->GetSetting("resolution", "1024 768").GetIntVector2().y_;
        engineParameters_["FullScreen"] = settings->GetSetting("fullscreen", "true").GetBool();
        engineParameters_["VSync"] = settings->GetSetting("vsync", "false").GetBool();
        engineParameters_["MultiSample"] = settings->GetSetting("antialiasing", "1").GetInt();
        engineParameters_["Shadows"] = settings->GetSetting("shadows", "2").GetInt() > 0 ? true : false;
        engineParameters_["LowQualityShadows"] = settings->GetSetting("shadows", "2").GetInt() == 1 ? true : false;

        audio->SetMasterGain(SoundType::SOUND_MASTER, settings->GetSetting("master", "1.0").GetFloat());
        audio->SetMasterGain(SoundType::SOUND_AMBIENT, settings->GetSetting("ambient", "0.75").GetFloat());
        audio->SetMasterGain(SoundType::SOUND_MUSIC, settings->GetSetting("music", "0.75").GetFloat());
        audio->SetMasterGain(SoundType::SOUND_EFFECT, settings->GetSetting("effects", "0.75").GetFloat());
        audio->SetMasterGain(SoundType::SOUND_UI, settings->GetSetting("interface", "0.75").GetFloat());

        mm->Load();
        sm->LoadHeaders();

        VariantMap createLoadingState;
        createLoadingState[StateCreated::P_STATE] = new LoadingState(context_);
        createLoadingState[StateCreated::P_ID] = StringHash("LoadingState");
        SendEvent(E_STATE_CREATED, createLoadingState);
    }
    catch (const Exception& ex)
    {
        ErrorExit(ex.GetCause());
    }
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
    GetSubsystem<Settings>()->Save();
}

DEFINE_APPLICATION_MAIN(SolarianWars)