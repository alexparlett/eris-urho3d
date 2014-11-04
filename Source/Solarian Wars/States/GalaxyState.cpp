////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "GalaxyState.h"
#include "MenuState.h"

#include "Core/Events.h"
#include "GamePlay/Scene/Components/TurnResolver.h"

#include <ResourceCache.h>
#include <Scene.h>
#include <Script.h>
#include <ScriptFile.h>
#include <PhysicsWorld.h>
#include <Octree.h>
#include <NavigationMesh.h>
#include <Navigable.h>
#include <Log.h>
#include <Renderer.h>
#include <Viewport.h>
#include <Material.h>
#include <TextureCube.h>
#include <Skybox.h>
#include <Model.h>
#include <SoundListener.h>
#include <UI.h>
#include <Input.h>
#include <InputEvents.h>

#include <angelscript.h>

using namespace Urho3D;

GalaxyState::GalaxyState(Context* context) :
    State(context),
    scene_(NULL),
    camera_(NULL),
    bindings_(GetSubsystem<Bindings>()),
    settings_(GetSubsystem<Settings>()),
    ui_(GetSubsystem<UI>()),
    time_(GetSubsystem<Time>())
{
    TurnResolver::RegisterObject(context);
}

GalaxyState::~GalaxyState()
{
}

void GalaxyState::Create()
{
    ResourceCache* rc = GetSubsystem<ResourceCache>();

    CreateScene();
    CreateCamera();

    ScriptFile* mapFile = rc->GetResource<ScriptFile>("Scripts/Maps/CampaignMap.as");
    if (mapFile)
    {
        asIScriptObject* object = mapFile->CreateObject("Map", true);
        mapFile->Execute(object, "void Generate()");
    }

    ScriptFile* uiFile = rc->GetResource<ScriptFile>("Scripts/UI/GalaxyUI.as");
    if (uiFile)
    {
        asIScriptObject* object = uiFile->CreateObject("UserInterface", true);
        uiFile->Execute(object, "void Create()");
    }
}

void GalaxyState::Start ()
{
    SubscribeToEvent(E_MOUSEMOVE, HANDLER(GalaxyState, HandleMouseMove));
    SubscribeToEvent(E_MOUSEBUTTONUP, HANDLER(GalaxyState, HandleMouseClick));
    SubscribeToEvent(E_MOUSEWHEEL, HANDLER(GalaxyState, HandleMouseWheel));
    SubscribeToEvent(E_KEYDOWN, HANDLER(GalaxyState, HandleKeyDown));
    SubscribeToEvent(E_GAME_FINISHED, HANDLER(GalaxyState, HandleGameFinished));

    Viewport* viewport = GetSubsystem<Renderer>()->GetViewport(0);
    viewport->SetScene(scene_);
    viewport->SetCamera(scene_->GetDefaultCamera());

    scene_->GetComponent<NavigationMesh>()->Build();

	SendEvent(E_GAME_STARTED);

    scene_->SetUpdateEnabled(true);
    GetSubsystem<UI>()->GetCursor()->SetVisible(true);
}

void GalaxyState::Stop ()
{
    UnsubscribeFromAllEvents();
    scene_->SetUpdateEnabled(false);
}

void GalaxyState::Destroy ()
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

    GetSubsystem<ResourceCache>()->ReleaseAllResources();
}

void GalaxyState::CreateScene()
{
    ResourceCache* rc = GetSubsystem<ResourceCache>();

    scene_ = new Scene(context_);
    scene_->SetUpdateEnabled(false);
    scene_->CreateComponent<PhysicsWorld>();
    scene_->CreateComponent<Octree>();
    scene_->CreateComponent<NavigationMesh>();
    scene_->CreateComponent<TurnResolver>();

    Node* skyboxNode = scene_->CreateChild("Skybox");
    skyboxNode->SetScale(0.25f);

    Skybox* skybox = skyboxNode->CreateComponent<Skybox>();
    skybox->SetModel(rc->GetResource<Model>("Models/Box.mdl"));
    skybox->SetMaterial(rc->GetResource<Material>("Materials/Skybox.xml"));

    GetSubsystem<Script>()->SetDefaultScene(scene_);
}

void GalaxyState::CreateCamera()
{
    camera_ = scene_->CreateChild("CameraPivot");
    camera_->SetPosition(Vector3(0.0f, 0.0f, -15.0f));

    Node* camNode = camera_->CreateChild("CameraNode");
    Camera* cam = camNode->CreateComponent<Camera>();
    cam->SetFarClip(100.f);
    cam->SetFov(75.f);

    SoundListener* listener = camNode->CreateComponent<SoundListener>();

    camNode->GetScene()->SetDefaultCamera(cam);
}

void GalaxyState::HandleMouseMove(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
    using namespace MouseMove;

    int buttons = eventData[P_BUTTONS].GetInt();
    int dx = eventData[P_DX].GetInt();
    int dy = eventData[P_DY].GetInt();

    if (buttons & MOUSEB_RIGHT)
    {
        float x = dx * time_->GetTimeStep() * settings_->GetSetting("rotateSpeed", 10.f).GetFloat();
        float y = dy * time_->GetTimeStep() * settings_->GetSetting("rotateSpeed;", 10.f).GetFloat();

        if (settings_->GetSetting("inverted", false).GetBool())
            camera_->Rotate(Quaternion(-y, -x, 0));
        else
            camera_->Rotate(Quaternion(y, x, 0));
    }
    else if (buttons & MOUSEB_MIDDLE)
    {
        float x = dx * time_->GetTimeStep() * settings_->GetSetting("panSpeed", 10.f).GetFloat();
        float y = dy * time_->GetTimeStep() * settings_->GetSetting("panSpeed;", 10.f).GetFloat();


        if (settings_->GetSetting("inverted", false).GetBool())
            camera_->Translate(Vector3(x, -y, 0));
        else
            camera_->Translate(Vector3(-x, y, 0));
    }
}

void GalaxyState::HandleMouseClick(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{

}

void GalaxyState::HandleMouseWheel(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
    using namespace MouseWheel;

    float value = eventData[P_WHEEL].GetInt() * time_->GetTimeStep() * settings_->GetSetting("scrollSpeed", 150.f).GetFloat();

    camera_->Translate(Vector3(0, 0, value));
}

void GalaxyState::HandleKeyDown(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
    using namespace KeyDown;

    int scanCode = eventData[P_SCANCODE].GetInt();
    int qualifiers = eventData[P_QUALIFIERS].GetInt();

    if (scanCode == SCANCODE_ESCAPE && !GetSubsystem<UI>()->GetFocusElement())
    {
        SendEvent(E_TOGGLE_ESCAPE_MENU);
    }
    else if (scanCode == bindings_->GetActionScanCode("eventWindowToggle"))
    {
        SendEvent(E_TOGGLE_EVENT_WINDOW);
    }
    else if (scanCode == bindings_->GetActionScanCode("diplomacyWindowToggle"))
    {
        SendEvent(E_TOGGLE_DIPLOMACY_WINDOW);
    }
    else if (scanCode == bindings_->GetActionScanCode("endTurn"))
    {
        SendEvent(E_END_TURN);
    }
}

void GalaxyState::HandleGameFinished(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
    VariantMap createData = GetEventDataMap();
    createData[StateCreate::P_STATE] = new MenuState(context_);
    createData[StateCreate::P_ID] = StringHash("MenuState");
    SendEvent(E_STATE_CREATE, createData);

    VariantMap changeData = GetEventDataMap();
    changeData[StateChange::P_ID] = StringHash("MenuState");
    SendEvent(E_STATE_CHANGE, changeData);

    VariantMap deleteData = GetEventDataMap();
    deleteData[StateChange::P_ID] = StringHash("GalaxyState");
    SendEvent(E_STATE_DESTROY, deleteData);
}
