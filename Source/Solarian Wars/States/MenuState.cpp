////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "MenuState.h"
#include "Core/Events.h"
#include "GalaxyState.h"

#include <ResourceCache.h>
#include <Octree.h>
#include <Skybox.h>
#include <SoundListener.h>
#include <Renderer.h>
#include <UI.h>
#include <Camera.h>
#include <Vector3.h>
#include <Model.h>
#include <Material.h>
#include <Viewport.h>
#include <BorderImage.h>
#include <Font.h>
#include <Text.h>
#include <UIEvents.h>
#include <Texture2D.h>
#include <Graphics.h>
#include <Button.h>
#include <InputEvents.h>

using namespace Urho3D;

MenuState::MenuState (Context* context) :
    State (context),
    locale_(GetSubsystem<Locale>())
{
}

MenuState::~MenuState ()
{
}

void MenuState::Create()
{
    CreateScene();
    CreateCamera();
    CreateUI();
}

void MenuState::Start()
{
    Viewport* viewport = GetSubsystem<Renderer>()->GetViewport(0);
    viewport->SetScene(scene_);
    viewport->SetCamera(scene_->GetDefaultCamera());

    scene_->SetUpdateEnabled(true);
    menuRoot_->SetVisible(true);
    GetSubsystem<UI>()->GetCursor()->SetVisible(true);
}

void MenuState::Stop()
{
    scene_->SetUpdateEnabled(false);
    menuRoot_->SetVisible(false);
    menuRoot_->SetDeepEnabled(false);
}

void MenuState::Destroy()
{
    if (camera_)
    {
        camera_->Remove();
        camera_.Reset();
    }

    if (scene_)
    {
        scene_->ResetScene();
        scene_.Reset();
    }

    if (menuRoot_)
    {
        menuRoot_->Remove();
        menuRoot_.Reset();
    }
}

void MenuState::CreateScene()
{
    ResourceCache* rc = GetSubsystem<ResourceCache>();

    scene_ = new Scene(context_);
    scene_->SetUpdateEnabled(false);
    scene_->CreateComponent<Octree>();

    Node* skyboxNode = scene_->CreateChild("Skybox");
    skyboxNode->SetScale(0.25f);

    Skybox* skybox = skyboxNode->CreateComponent<Skybox>();
    skybox->SetModel(rc->GetResource<Model>("Models/Box.mdl"));
    skybox->SetMaterial(rc->GetResource<Material>("Materials/Skybox.xml"));
}

void MenuState::CreateCamera()
{
    camera_ = scene_->CreateChild("CameraPivot");
    camera_->SetPosition(Vector3(0.0f, 5.0f, 0.0f));
    camera_->CreateComponent<SoundListener>();

    Node* camNode = camera_->CreateChild("CameraNode");
    Camera* cam = camNode->CreateComponent<Camera>();

    scene_->SetDefaultCamera(cam);
}

void MenuState::CreateUI()
{
    ResourceCache* rc = GetSubsystem<ResourceCache>();
    UI* ui = GetSubsystem<UI>();
    Graphics* graphics = GetSubsystem<Graphics>();

    UIElement* root = ui->GetRoot();

    menuRoot_ = root->CreateChild<UIElement>("MenuRoot");
    menuRoot_->SetSize(graphics->GetWidth(), graphics->GetHeight());
    menuRoot_->SetVisible(false);

    Texture2D* atlas = rc->GetResource<Texture2D>("Textures/UI/UIAtlas.png");
    Font* font =rc->GetResource<Font>("Fonts/CaviarDreams.ttf");

    CreateBottomBrace(atlas);
    CreateButtons(font);
}

void MenuState::CreateBottomBrace(Texture2D* atlas)
{
    BorderImage* bottomBrace = menuRoot_->CreateChild<BorderImage>("BottomBrace");
    bottomBrace->SetTexture(atlas);
    bottomBrace->SetImageRect(IntRect(174, 37, 294, 81));
    bottomBrace->SetSize(120, 45);
    bottomBrace->SetAlignment(HorizontalAlignment::HA_CENTER, VerticalAlignment::VA_BOTTOM);
    bottomBrace->SetBlendMode(BLEND_ALPHA);
}

void MenuState::CreateButtons(Font* font)
{
    UIElement* buttonRoot = new UIElement(context_);
    buttonRoot->SetAlignment(HorizontalAlignment::HA_CENTER, VerticalAlignment::VA_CENTER);
    buttonRoot->SetLayoutMode(LM_VERTICAL);
    buttonRoot->SetFixedSize(230, 355);
    buttonRoot->SetEnabled(false);

    menuRoot_->AddChild(buttonRoot);

    Button* playButton = BuildMenuButton(locale_->Localize(0, 1), font);
    Button* loadButton = BuildMenuButton(locale_->Localize(0, 2), font);
    Button* modsButton = BuildMenuButton(locale_->Localize(0, 3), font);
    Button* optionsButton = BuildMenuButton(locale_->Localize(0, 4), font);
    Button* exitButton = BuildMenuButton(locale_->Localize(0, 5), font);

    buttonRoot->AddChild(playButton);
    buttonRoot->AddChild(loadButton);
    buttonRoot->AddChild(modsButton);
    buttonRoot->AddChild(optionsButton);
    buttonRoot->AddChild(exitButton);

    SubscribeToEvent(playButton, E_RELEASED, HANDLER(MenuState, PlayClicked));
    SubscribeToEvent(loadButton, E_RELEASED, HANDLER(MenuState, LoadClicked));
    SubscribeToEvent(modsButton, E_RELEASED, HANDLER(MenuState, ModsClicked));
    SubscribeToEvent(optionsButton, E_RELEASED, HANDLER(MenuState, OptionsClicked));
    SubscribeToEvent(exitButton, E_RELEASED, HANDLER(MenuState, ExitClicked));
}

Urho3D::Button* MenuState::BuildMenuButton(const String& content, Font* font)
{
    Button* button = new Button(context_);
    button->SetStyleAuto();
    button->SetFocusMode(FM_RESETFOCUS);

    Text* text = new Text(context_);
    text->SetFont(font, 18);
    text->SetText(content);
    text->SetAlignment(HorizontalAlignment::HA_CENTER, VerticalAlignment::VA_CENTER);
    text->SetTextAlignment(HorizontalAlignment::HA_CENTER);

    button->AddChild(text);

    return button;
}

void MenuState::PlayClicked(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
    VariantMap createData = GetEventDataMap();
    createData[StateCreate::P_STATE] = new GalaxyState(context_);
    createData[StateCreate::P_ID] = StringHash("GalaxyState");
    SendEvent(E_STATE_CREATE, createData);

    VariantMap changeData = GetEventDataMap();
    changeData[StateChange::P_ID] = StringHash("GalaxyState");
    SendEvent(E_STATE_CHANGE, changeData);

    VariantMap deleteData = GetEventDataMap();
    deleteData[StateChange::P_ID] = StringHash("MenuState");
    SendEvent(E_STATE_DESTROY, deleteData);
}

void MenuState::LoadClicked(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
}

void MenuState::ModsClicked(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
}

void MenuState::OptionsClicked(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
}

void MenuState::ExitClicked(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
    SendEvent(E_EXITREQUESTED);
}
