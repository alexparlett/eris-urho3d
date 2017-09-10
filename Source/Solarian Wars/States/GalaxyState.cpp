////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "GalaxyState.h"
#include "MenuState.h"

#include "Core/Events.h"

#include <Resource/ResourceCache.h>
#include <Scene/Scene.h>
#include <AngelScript/Script.h>
#include <AngelScript/ScriptFile.h>
#include <Physics/PhysicsWorld.h>
#include <Graphics/Octree.h>
#include <Navigation/NavigationMesh.h>
#include <Navigation/Navigable.h>
#include <IO/Log.h>
#include <Graphics/Renderer.h>
#include <Graphics/Viewport.h>
#include <Graphics/Material.h>
#include <Graphics/TextureCube.h>
#include <Graphics/Skybox.h>
#include <Graphics/Model.h>
#include <Audio/SoundListener.h>
#include <UI/UI.h>
#include <Input/Input.h>
#include <Input/InputEvents.h>

#include <AngelScript/angelscript.h>

#include <utility>

using namespace Urho3D;

GalaxyState::GalaxyState(Context* context) :
    State(context),
    scene_(),
    camera_(),
    bindings_(GetSubsystem<Bindings>()),
    settings_(GetSubsystem<Settings>()),
    ui_(GetSubsystem<UI>()),
    time_(GetSubsystem<Time>())
{
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
    SubscribeToEvent(E_MOUSEMOVE, URHO3D_HANDLER(GalaxyState, HandleMouseMove));
    SubscribeToEvent(E_MOUSEBUTTONUP, URHO3D_HANDLER(GalaxyState, HandleMouseClick));
    SubscribeToEvent(E_MOUSEWHEEL, URHO3D_HANDLER(GalaxyState, HandleMouseWheel));
    SubscribeToEvent(E_GAME_FINISHED, URHO3D_HANDLER(GalaxyState, HandleGameFinished));

    Viewport* viewport = GetSubsystem<Renderer>()->GetViewport(0);
    viewport->SetScene(scene_);
    viewport->SetCamera(camera_->GetComponent<Camera>());

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
