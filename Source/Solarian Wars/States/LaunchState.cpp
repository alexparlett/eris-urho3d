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
#include <XMLFile.h>

#include "IO/Settings.h"
#include "Core/Events.h"
#include "MenuState.h"

using namespace Urho3D;

LaunchState::LaunchState(Context* context) :
    State(context),
    launchRoot_(NULL),
    timer_(Timer())
{
}

LaunchState::~LaunchState()
{
    if (launchRoot_)
        launchRoot_.Reset();
}

void LaunchState::Create()
{
	ResourceCache* rc = GetSubsystem<ResourceCache>();
	UI* ui =  GetSubsystem<UI>();
    Graphics* graphics = GetSubsystem<Graphics>();

    UIElement* root = ui->GetRoot();

    launchRoot_ = root->CreateChild<BorderImage>("LaunchBackground");
    launchRoot_->SetColor(Color::BLACK);
    launchRoot_->SetSize(graphics->GetWidth(), graphics->GetHeight());
    launchRoot_->SetVisible(false);


    BorderImage* bi = launchRoot_->CreateChild<BorderImage>("LaunchLogo");
    bi->SetTexture(rc->GetResource<Texture2D>("Textures/UI/LaunchLogo.png"));
    bi->SetSize(512, 512);
    bi->SetAlignment(HorizontalAlignment::HA_CENTER, VerticalAlignment::VA_CENTER);
    bi->SetBlendMode(BLEND_ALPHA);
}

void LaunchState::Start()
{
    SubscribeToEvent(E_ENDFRAME, HANDLER(LaunchState, HandleTimer));
    SubscribeToEvent(E_KEYDOWN, HANDLER(LaunchState, HandleKey));
    SubscribeToEvent(E_MOUSEBUTTONUP, HANDLER(LaunchState, HandleButton));

    AsyncLoadCoreData();

	launchRoot_->SetVisible(true);

    timer_.Reset();
}

void LaunchState::Stop()
{
    UnsubscribeFromAllEvents();

    launchRoot_->SetVisible(false);
	launchRoot_->SetDeepEnabled(false);
}

void LaunchState::Destroy()
{
    if (launchRoot_)
    {
        launchRoot_->Remove();
        launchRoot_.Reset();
    }
}

void LaunchState::HandleTimer(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
    if (timer_.GetMSec(false) > 5000 && !switching_)
    {
        SwitchToMenu();

    }
}

void LaunchState::HandleKey(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
    using namespace KeyDown;

    int scanCode = eventData[P_SCANCODE].GetInt();

    if ((scanCode == SCANCODE_SPACE || scanCode == SCANCODE_ESCAPE) && !switching_)
    {
        SwitchToMenu();
    }
}

void LaunchState::HandleButton(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
    using namespace MouseButtonDown;

    if (!switching_)
    {
        SwitchToMenu();
    }
}

void LaunchState::AsyncLoadCoreData()
{
    ResourceCache* rc = GetSubsystem<ResourceCache>();

    rc->BackgroundLoadResource<Model>("Models/Box.mdl");
    rc->BackgroundLoadResource<Model>("Models/Plane.mdl");
    rc->BackgroundLoadResource<Model>("Models/Sphere.mdl");
}

void LaunchState::SwitchToMenu()
{
    switching_ = true;

    VariantMap createData = GetEventDataMap();
    createData[StateCreate::P_STATE] = new MenuState(context_);
    createData[StateCreate::P_ID] = StringHash("MenuState");
    SendEvent(E_STATE_CREATE, createData);

    VariantMap changeData = GetEventDataMap();
    changeData[StateChange::P_ID] = StringHash("MenuState");
    SendEvent(E_STATE_CHANGE, changeData);

    VariantMap deleteData = GetEventDataMap();
    deleteData[StateChange::P_ID] = StringHash("LaunchState");
    SendEvent(E_STATE_DESTROY, deleteData);
}
