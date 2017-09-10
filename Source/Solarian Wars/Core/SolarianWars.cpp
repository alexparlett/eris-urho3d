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
#include "Script/ScriptAPI.h"
#include "IO/Bindings.h"

#include <AngelScript/APITemplates.h>
#include <AngelScript/Script.h>
#include <IO/FileSystem.h>
#include <Input/Input.h>
#include <Graphics/Graphics.h>
#include <Audio/Audio.h>
#include <Resource/ResourceCache.h>
#include <Math/StringHash.h>
#include <Engine/DebugHud.h>
#include <Engine/Console.h>
#include <IO/Log.h>
#include <Graphics/Renderer.h>
#include <Graphics/Viewport.h>
#include <UI/UI.h>
#include <UI/UIElement.h>
#include <UI/Cursor.h>
#include <Math/Color.h>
#include <Graphics/Zone.h>
#include <UI/Font.h>

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
    context->RegisterSubsystem(new Bindings(context));

    RegisterScriptAPI(GetSubsystem<Script>()->GetScriptEngine());
}

void SolarianWars::Setup()
{
    Audio* audio = GetSubsystem<Audio>();
    Settings* settings = GetSubsystem<Settings>();
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    Input* input = GetSubsystem<Input>();
    Log* log = GetSubsystem<Log>();
    Bindings* bindings = GetSubsystem <Bindings>();

    log->Open(settings->GetSetting("userdir").GetString() + "sw.log");
    log->WriteRaw("[" + Time::GetTimeStamp() + "] Solarian Wars " + settings->GetSetting("version").GetString() + "\n");

    ParseArgs();

    cache->SetAutoReloadResources(false);

    input->SetMouseVisible(false);
    input->SetToggleFullscreen(true);

    settings->Load();
    bindings->Load();

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
    engineParameters_["RenderPath"] = "RenderPaths/Deferred.xml";

    audio->SetMasterGain("master", settings->GetSetting("master", 1.0f).GetFloat());
    audio->SetMasterGain("ambient", settings->GetSetting("ambient", 0.6f).GetFloat());
    audio->SetMasterGain("music", settings->GetSetting("music", 0.4f).GetFloat());
    audio->SetMasterGain("effects", settings->GetSetting("effects", 0.6f).GetFloat());
    audio->SetMasterGain("interface", settings->GetSetting("interface", 0.6f).GetFloat());
}

void SolarianWars::Start()
{
    DefineCursor();
    DefineVersion();

    GetSubsystem<StateManager>()->InitializeLoadingUI();
    GetSubsystem<ModManager>()->Load();
    GetSubsystem<Locale>()->Load(GetSubsystem<Settings>()->GetSetting("language", "enGB").GetString());
    GetSubsystem<Renderer>()->SetViewport(0, new Viewport(context_));

    VariantMap createData = GetEventDataMap();
    createData[StateCreate::P_STATE] = new LaunchState(context_);
    createData[StateCreate::P_ID] = StringHash("LaunchState");
    SendEvent(E_STATE_CREATE, createData);

    VariantMap changeData = GetEventDataMap();
    changeData[StateChange::P_ID] = StringHash("LaunchState");
    SendEvent(E_STATE_CHANGE, changeData);
}

void SolarianWars::Stop()
{
    if (version_)
    {
        version_->Remove();
        version_.Reset();
    }

    context_->RemoveSubsystem<StateManager>();
    context_->RemoveSubsystem<Locale>();
    context_->RemoveSubsystem<ModManager>();
    context_->RemoveSubsystem<Settings>();
    context_->RemoveSubsystem<Bindings>();
}

void SolarianWars::ParseArgs()
{
    if (GetArguments().Contains("-debug"))
        GetSubsystem<Log>()->SetLevel(LOG_DEBUG);
}

void SolarianWars::DefineCursor()
{
    UI* ui = GetSubsystem<UI>();
    ResourceCache* rc = GetSubsystem<ResourceCache>();

    Cursor* cursor = new Cursor(context_);
    cursor->SetVisible(false);

    Image* image = rc->GetResource<Image>("Textures/UI/Cursors.png");
    if (image)
    {
        cursor->DefineShape(CS_NORMAL, image, IntRect(96, 32, 128, 64), IntVector2(0, 0));
        cursor->DefineShape(CS_REJECTDROP, image, IntRect(0, 0, 32, 32), IntVector2(0, 0));
        cursor->DefineShape(CS_RESIZEDIAGONAL_TOPLEFT, image, IntRect(0, 32, 32, 64), IntVector2(0, 0));
        cursor->DefineShape(CS_RESIZEDIAGONAL_TOPRIGHT, image, IntRect(96, 0, 128, 32), IntVector2(0, 0));
        cursor->DefineShape(CS_RESIZEHORIZONTAL, image, IntRect(64, 0, 96, 32), IntVector2(0, 0));
        cursor->DefineShape(CS_RESIZEVERTICAL, image, IntRect(32, 0, 64, 32), IntVector2(0, 0));
        cursor->DefineShape(CS_ACCEPTDROP, image, IntRect(32, 32, 64, 64), IntVector2(0, 0));
        cursor->DefineShape(CS_BUSY, image, IntRect(0, 64, 32, 96), IntVector2(0, 0));
    }

    ui->SetCursor(cursor);
}

void SolarianWars::DefineVersion()
{
    UI* ui = GetSubsystem<UI>();
    ResourceCache* rc = GetSubsystem<ResourceCache>();

    UIElement* root = ui->GetRoot();
    root->SetDefaultStyle(rc->GetResource<XMLFile>("UI/Style.xml"));

    version_ = root->CreateChild<Text>("Version");
    version_->SetAlignment(HorizontalAlignment::HA_RIGHT, VerticalAlignment::VA_TOP);
    version_->SetTextAlignment(HorizontalAlignment::HA_LEFT);
    version_->SetText("Rev: " + GetSubsystem<Settings>()->GetSetting("version").GetString().ToUpper());
    version_->SetColor(Color::WHITE);
    version_->SetFont(rc->GetResource<Font>("Fonts/msyi.ttf"), 12);
    version_->SetPriority(M_MAX_INT);
}

URHO3D_DEFINE_APPLICATION_MAIN(SolarianWars)