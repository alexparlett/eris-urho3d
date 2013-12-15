////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "SolarianWars.h"
#include "Exceptions/Exception.h"

#include <Script.h>
#include <FileSystem.h>
#include <Input.h>
#include <Graphics.h>

DEFINE_APPLICATION_MAIN(SolarianWars)

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

		engineParameters_["WindowWidth"] = settings->GetSetting("resolution").GetIntVector2().x_;
		engineParameters_["WindowHeight"] = settings->GetSetting("resolution").GetIntVector2().y_;
		engineParameters_["FullScreen"] = settings->GetSetting("fullscreen").GetBool();
		engineParameters_["VSync"] = settings->GetSetting("vsync").GetBool();
		engineParameters_["MultiSample"] = settings->GetSetting("antialiasing").GetInt();

		int shadows = settings->GetSetting("shadows").GetInt();
		engineParameters_["Shadows"] = shadows > 0 ? true : false;
		engineParameters_["LowQualityShadows"] = shadows = 1 ? true : false;

		if (!settings->GetSetting("fullscreen").GetBool())
		{
			GetSubsystem<Input>()->SetMouseVisible(true);
		}
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