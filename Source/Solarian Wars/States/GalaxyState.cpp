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

using namespace Urho3D;

GalaxyState::GalaxyState(Context* context) :
    State(context),
    scene_(NULL),
    camera_(NULL)
{
}

GalaxyState::~GalaxyState()
{
}

void GalaxyState::Create ()
{
	ResourceCache* rc = GetSubsystem<ResourceCache>();

	scene_ = new Scene(context_);
	scene_->SetUpdateEnabled(false);
	scene_->CreateComponent<PhysicsWorld>();
	scene_->CreateComponent<Octree>();
	scene_->CreateComponent<NavigationMesh>();
	scene_->CreateComponent<TurnResolver>();

	camera_ = scene_->CreateChild("Camera");
	camera_->CreateComponent<Camera>();
	camera_->SetPosition(Vector3(0, 20.f, 0));

	Node* skyboxNode =  scene_->CreateChild("Skybox");
	Skybox* skybox = skyboxNode->CreateComponent<Skybox>();
	skybox->SetMaterial(rc->GetResource<Material>("Materials/Skybox.xml"));

	GetSubsystem<Script>()->SetDefaultScene(scene_);

    ScriptFile* file = GetSubsystem<ResourceCache>()->GetResource<ScriptFile>("Scripts/Maps/CampaignMap.as");
    if (file)
    {
        MapGenerator gen(file);
        gen.Generate();
    }
}

void GalaxyState::Start ()
{
    Renderer* rdr = GetSubsystem<Renderer>();
    rdr->GetViewport(0)->SetScene(scene_);
    rdr->GetViewport(0)->SetCamera(camera_->GetComponent<Camera>());

    scene_->GetComponent<NavigationMesh>()->Build();
    scene_->SetUpdateEnabled(true);

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
