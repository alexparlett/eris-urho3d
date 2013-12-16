////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "SolarianWars.h"
#include "Exceptions/Exception.h"
#include "IO/Settings.h"

#include <Script.h>
#include <FileSystem.h>
#include <Input.h>
#include <Graphics.h>
#include <Audio.h>

using namespace Urho3D;

SolarianWars::SolarianWars(Context* context) :
Application(context)
{
    context_->RegisterSubsystem(new Script(context_));
    context_->RegisterSubsystem(new Settings(context_));
}

void SolarianWars::Setup()
{
    try
    {
        engineParameters_["WindowTitle"] = "Solarian Wars";
        engineParameters_["WindowIcon"] = "Textures/Icon.png";

        Settings* settings = GetSubsystem<Settings>();
        settings->Load();

        engineParameters_["LogName"] = GetSubsystem<FileSystem>()->GetUserDocumentsDir() + "My Games/Solarian Wars/sw.log";

        engineParameters_["WindowWidth"] = settings->GetSetting("resolution", "1024 768").GetIntVector2().x_;
        engineParameters_["WindowHeight"] = settings->GetSetting("resolution", "1024 768").GetIntVector2().y_;
        engineParameters_["FullScreen"] = settings->GetSetting("fullscreen", "true").GetBool();
        engineParameters_["VSync"] = settings->GetSetting("vsync", "false").GetBool();
        engineParameters_["MultiSample"] = settings->GetSetting("antialiasing", "1").GetInt();

        int shadows = settings->GetSetting("shadows", "2").GetInt();
        engineParameters_["Shadows"] = shadows > 0 ? true : false;
        engineParameters_["LowQualityShadows"] = shadows = 1 ? true : false;

        if (!settings->GetSetting("fullscreen", "true").GetBool())
        {
            GetSubsystem<Input>()->SetMouseVisible(true);
        }

        Audio* audio = GetSubsystem<Audio>();

        audio->SetMasterGain(SoundType::SOUND_MASTER, settings->GetSetting("master", "1.0").GetFloat());
        audio->SetMasterGain(SoundType::SOUND_AMBIENT, settings->GetSetting("ambient", "0.75").GetFloat());
        audio->SetMasterGain(SoundType::SOUND_MUSIC, settings->GetSetting("music", "0.75").GetFloat());
        audio->SetMasterGain(SoundType::SOUND_EFFECT, settings->GetSetting("effects", "0.75").GetFloat());
    }
    catch (const Exception& ex)
    {
        ErrorExit(ex.GetCause());
    }
}

void SolarianWars::Start()
{
}

void SolarianWars::Stop()
{
}

DEFINE_APPLICATION_MAIN(SolarianWars)