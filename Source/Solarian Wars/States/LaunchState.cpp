#include "LaunchState.h"

#include <Graphics.h>
#include <Context.h>
#include <ResourceCache.h>
#include <UI.h>
#include <Texture2D.h>
#include <Text.h>
#include <Timer.h>
#include <Font.h>
#include <InputEvents.h>
#include <CoreEvents.h>
#include <Model.h>

#include "IO/Settings.h"
#include "Core/Events.h"
#include "MenuState.h"
#include "GalaxyState.h"

using namespace Urho3D;

LaunchState::LaunchState(Context* context) :
    State(context),
    loading_(NULL),
    timer_(Timer())
{
}

LaunchState::~LaunchState()
{
    if (version_)
        version_.Reset();

    if (loading_)
        loading_.Reset();
}

void LaunchState::Create()
{
	ResourceCache* rc = GetSubsystem<ResourceCache>();
	UI* ui =  GetSubsystem<UI>();
    Graphics* graphics = GetSubsystem<Graphics>();

    UIElement* root = ui->GetRoot();

    version_ = root->CreateChild<Text>("Version");
    version_->SetAlignment(HorizontalAlignment::HA_LEFT, VerticalAlignment::VA_TOP);
    version_->SetTextAlignment(HorizontalAlignment::HA_LEFT);
    version_->SetText("Rev: " + GetSubsystem<Settings>()->GetSetting("version").GetString().ToUpper());
    version_->SetFocusMode(FocusMode::FM_NOTFOCUSABLE);
    version_->SetPosition(0, 0);
    version_->SetSize(100, 50);
    version_->SetColor(Color::WHITE);
    version_->SetFont(rc->GetResource<Font>("Fonts/msyi.ttf"), 12);

    loading_ = root->CreateChild<UIElement>("LoadingScreen");

    BorderImage* bi = loading_->CreateChild<BorderImage>("Background");
    bi->SetTexture(rc->GetResource<Texture2D>("Textures/Icon.png"));
    bi->SetPosition(graphics->GetWidth() / 2, graphics->GetHeight() / 2);
    bi->SetSize(512, 512);
    bi->SetAlignment(HorizontalAlignment::HA_CENTER, VerticalAlignment::VA_CENTER);
	bi->SetFocusMode(FocusMode::FM_NOTFOCUSABLE);

}

void LaunchState::Start()
{
    SubscribeToEvent(E_ENDFRAME, HANDLER(LaunchState, HandleTimer));
    SubscribeToEvent(E_KEYDOWN, HANDLER(LaunchState, HandleKey));
    SubscribeToEvent(E_MOUSEBUTTONUP, HANDLER(LaunchState, HandleButton));

    AsyncLoadCoreData();

    loading_->SetEnabled(true);
	loading_->SetVisible(true);

    timer_.Reset();
}

void LaunchState::Stop()
{
    UnsubscribeFromAllEvents();

    loading_->SetVisible(false);
	loading_->SetEnabled(false);
}

void LaunchState::Destroy()
{
    if (version_)
    {
        version_->Remove();
        version_.Reset();    
    }

    if (loading_)
    {
        loading_->Remove();
        loading_.Reset();
    }
}

void LaunchState::HandleTimer(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
    if (timer_.GetMSec(false) > 5000 && !switching_)
    {
        switching_ = true;

        VariantMap createData = GetEventDataMap();
        createData[StateCreate::P_STATE] = new GalaxyState(context_);
        createData[StateCreate::P_ID] = StringHash("GalaxyState");
        SendEvent(E_STATE_CREATE, createData);

        VariantMap changeData = GetEventDataMap();
        changeData[StateChange::P_ID] = StringHash("GalaxyState");
        SendEvent(E_STATE_CHANGE, changeData);
    }
}

void LaunchState::HandleKey(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
    using namespace KeyDown;

    int scanCode = eventData[P_SCANCODE].GetInt();

    if ((scanCode == SCANCODE_SPACE || scanCode == SCANCODE_ESCAPE) && !switching_)
    {
        switching_ = true;

        VariantMap createData = GetEventDataMap();
        createData[StateCreate::P_STATE] = new GalaxyState(context_);
        createData[StateCreate::P_ID] = StringHash("GalaxyState");
        SendEvent(E_STATE_CREATE, createData);

        VariantMap changeData = GetEventDataMap();
        changeData[StateChange::P_ID] = StringHash("GalaxyState");
        SendEvent(E_STATE_CHANGE, changeData);
    }
}

void LaunchState::HandleButton(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
    using namespace MouseButtonDown;

    if (!switching_)
    {
        switching_ = true;

        VariantMap createData = GetEventDataMap();
        createData[StateCreate::P_STATE] = new GalaxyState(context_);
        createData[StateCreate::P_ID] = StringHash("GalaxyState");
        SendEvent(E_STATE_CREATE, createData);

        VariantMap changeData = GetEventDataMap();
        changeData[StateChange::P_ID] = StringHash("GalaxyState");
        SendEvent(E_STATE_CHANGE, changeData);
    }
}

void LaunchState::AsyncLoadCoreData()
{
    ResourceCache* rc = GetSubsystem<ResourceCache>();

    rc->BackgroundLoadResource<Model>("Models/Box.mdl");
    rc->BackgroundLoadResource<Model>("Models/Plane.mdl");
    rc->BackgroundLoadResource<Model>("Models/Sphere.mdl");
}
