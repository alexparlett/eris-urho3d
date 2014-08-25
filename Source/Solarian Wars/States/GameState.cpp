////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "GameState.h"

#include "GamePlay/Map/Types/Galaxy.h"
#include "GamePlay/Map/MapGenerator.h"

#include <ResourceCache.h>
#include <Scene.h>
#include <Script.h>
#include <ScriptFile.h>
#include <PhysicsWorld.h>
#include <Octree.h>
#include <NavigationMesh.h>
#include <Navigable.h>
#include <Log.h>

using namespace Urho3D;

GameState::GameState(Context* context) :
    State(context),
    scene_(NULL),
    camera_(NULL)
{
}

GameState::~GameState()
{
}

void GameState::Create ()
{
	scene_ = SharedPtr<Scene>(new Scene(context_));

	scene_->SetUpdateEnabled(false);
	scene_->CreateComponent<Galaxy>();
	scene_->CreateComponent<PhysicsWorld>();
	scene_->CreateComponent<Octree>();
	scene_->CreateComponent<NavigationMesh>();
	scene_->CreateComponent<Navigable>();

	GetSubsystem<Script>()->SetDefaultScene(scene_);

    ScriptFile* file = GetSubsystem<ResourceCache>()->GetResource<ScriptFile>("Scripts/Maps/CampaignMap.as");
    if (file)
    {
        MapGenerator gen(file);
        gen.Generate();
    }
}

void GameState::Start ()
{
    scene_->GetComponent<NavigationMesh>()->Build();
    scene_->SetUpdateEnabled(true);
}

void GameState::Stop ()
{
    scene_->SetUpdateEnabled(false);
}

void GameState::Destroy ()
{
    scene_->ReleaseRef();
}
