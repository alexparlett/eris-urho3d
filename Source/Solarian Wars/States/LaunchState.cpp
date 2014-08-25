#include "LaunchState.h"

#include <Graphics.h>
#include <Context.h>
#include <ResourceCache.h>
#include <UI.h>
#include <Texture2D.h>
#include <UIEvents.h>
#include <Text.h>

#include "GameState.h"
#include "Core/Events.h"
#include "IO/Settings.h"

using namespace Urho3D;

LaunchState::LaunchState(Context* context) :
    State(context),
    loading_(NULL)
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
	root->SetDefaultStyle(rc->GetResource<XMLFile>("UI/Style.xml"));

	Text* version = root->CreateChild<Text>("Version");
	version->SetAlignment(HorizontalAlignment::HA_LEFT, VerticalAlignment::VA_TOP);
	version->SetTextAlignment(HorizontalAlignment::HA_LEFT);
	version->SetText(GetSubsystem<Settings>()->GetSetting("version").GetString());
	version->SetPriority(100);
	version->SetFocusMode(FocusMode::FM_NOTFOCUSABLE);
	version->SetPosition(0, 0);

    loading_ = root->CreateChild<UIElement>("LoadingScreen");

    BorderImage* bi = loading_->CreateChild<BorderImage>("Background");
    bi->SetTexture(rc->GetResource<Texture2D>("Textures/UI/Loading.png"));
    bi->SetPosition(graphics->GetWidth() / 2, graphics->GetHeight() / 2);
	bi->SetSize(graphics->GetWidth() / 3, graphics->GetWidth() / 3);
    bi->SetAlignment(HorizontalAlignment::HA_CENTER, VerticalAlignment::VA_CENTER);
	bi->SetFocusMode(FocusMode::FM_NOTFOCUSABLE);
}

void LaunchState::Start()
{
    loading_->SetEnabled(true);
	loading_->SetVisible(true);

    VariantMap createData = GetEventDataMap();
    createData[StateCreated::P_STATE] = new GameState(context_);
    createData[StateCreated::P_ID] = StringHash("GameState");
    SendEvent(E_STATE_CREATED, createData);

    VariantMap changeData = GetEventDataMap();
    changeData[StateChanged::P_ID] = StringHash("GameState");
    SendEvent(E_STATE_CHANGED, changeData);
}

void LaunchState::Stop()
{
    loading_->SetVisible(false);
	loading_->SetEnabled(false);
}

void LaunchState::Destroy()
{
    loading_->Remove();
}