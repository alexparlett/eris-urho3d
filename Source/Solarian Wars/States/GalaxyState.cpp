////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "GalaxyState.h"

#include "Core/Events.h"
#include "GamePlay/Scene/MapGenerator.h"
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

using namespace Urho3D;

GalaxyState::GalaxyState(Context* context) :
    State(context),
    scene_(NULL),
    camera_(NULL)
{
    TurnResolver::RegisterObject(context);
}

GalaxyState::~GalaxyState()
{
}

void GalaxyState::Create ()
{
    CreateScene();
    CreateCamera();

    ScriptFile* file = GetSubsystem<ResourceCache>()->GetResource<ScriptFile>("Scripts/Maps/CampaignMap.as");
    if (file)
    {
        MapGenerator gen(file);
        gen.Generate();
    }
}

void GalaxyState::Start ()
{
    Viewport* viewport = GetSubsystem<Renderer>()->GetViewport(0);
    viewport->SetScene(scene_);
    viewport->SetCamera(camera_->GetChild("CameraNode")->GetComponent<Camera>());

    scene_->GetComponent<NavigationMesh>()->Build();
    scene_->SetUpdateEnabled(true);

    GetSubsystem<UI>()->GetCursor()->SetVisible(true);

	SendEvent(E_GAME_STARTED);
}

void GalaxyState::Stop ()
{
	SendEvent(E_GAME_FINISHED);

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
    camera_->SetPosition(Vector3(0.0f, 0.0f, -5.0f));

    Node* camNode = camera_->CreateChild("CameraNode");
    camNode->CreateComponent<Camera>();
    camNode->CreateComponent<SoundListener>();
}
