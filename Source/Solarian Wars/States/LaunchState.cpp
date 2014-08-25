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

#include "IO/Settings.h"
#include "Core/Events.h"
#include "GameState.h"

using namespace Urho3D;

LaunchState::LaunchState(Context* context) :
    State(context),
    loading_(NULL),
    timer_(Timer())
{
}

LaunchState::~LaunchState()
{
}

void LaunchState::Create()
{
	ResourceCache* rc = GetSubsystem<ResourceCache>();
	UI* ui =  GetSubsystem<UI>();
    Graphics* graphics = GetSubsystem<Graphics>();

    UIElement* root = ui->GetRoot();

    Text* version = root->CreateChild<Text>("Version");
    version->SetAlignment(HorizontalAlignment::HA_LEFT, VerticalAlignment::VA_TOP);
    version->SetTextAlignment(HorizontalAlignment::HA_LEFT);
    version->SetText("Rev: " + GetSubsystem<Settings>()->GetSetting("version").GetString().ToUpper());
    version->SetFocusMode(FocusMode::FM_NOTFOCUSABLE);
    version->SetPosition(0, 0);
    version->SetSize(100, 50);
    version->SetColor(Color::WHITE);
    version->SetFont(rc->GetResource<Font>("Fonts/BlueHighway.ttf"), 10);

    loading_ = root->CreateChild<UIElement>("LoadingScreen");

    BorderImage* bi = loading_->CreateChild<BorderImage>("Background");
    bi->SetTexture(rc->GetResource<Texture2D>("Textures/Icon.png"));
    bi->SetPosition(graphics->GetWidth() / 2, graphics->GetHeight() / 2);
    bi->SetSize(512, 512);
    bi->SetAlignment(HorizontalAlignment::HA_CENTER, VerticalAlignment::VA_CENTER);
	bi->SetFocusMode(FocusMode::FM_NOTFOCUSABLE);

    SubscribeToEvent("EndFrame", HANDLER(LaunchState, HandleTimer));
    SubscribeToEvent("KeyUp", HANDLER(LaunchState, HandleKey));
}

void LaunchState::Start()
{
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
    loading_->Remove();
}

void LaunchState::HandleTimer(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
    if (timer_.GetMSec(false) > 3000)
    {
        VariantMap createData = GetEventDataMap();
        createData[StateCreated::P_STATE] = new GameState(context_);
        createData[StateCreated::P_ID] = StringHash("GameState");
        SendEvent(E_STATE_CREATED, createData);

        VariantMap changeData = GetEventDataMap();
        changeData[StateChanged::P_ID] = StringHash("GameState");
        SendEvent(E_STATE_CHANGED, changeData);
    }
}

void LaunchState::HandleKey(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
    using namespace KeyUp;

    int scanCode = eventData[P_SCANCODE].GetInt();

    if (scanCode == SCANCODE_SPACE || scanCode == SCANCODE_ESCAPE)
    {
        VariantMap createData = GetEventDataMap();
        createData[StateCreated::P_STATE] = new GameState(context_);
        createData[StateCreated::P_ID] = StringHash("GameState");
        SendEvent(E_STATE_CREATED, createData);

        VariantMap changeData = GetEventDataMap();
        changeData[StateChanged::P_ID] = StringHash("GameState");
        SendEvent(E_STATE_CHANGED, changeData);
    }
}