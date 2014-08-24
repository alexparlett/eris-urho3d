////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "GameState.h"

#include "GamePlay/Map/Types/Galaxy.h"
#include "GamePlay/Map/MapGenerator.h"

#include <Script.h>
#include <PhysicsWorld.h>
#include <Octree.h>
#include <NavigationMesh.h>
#include <Navigable.h>

using namespace Urho3D;

GameState::GameState(Context* context) :
    State(context)
{
}

GameState::~GameState()
{
}

void GameState::Create ()
{
	if (scene_)
		scene_->ResetScene();
	else
		scene_ = new Scene(context_);

	scene_->SetEnabled(false);
	scene_->CreateComponent<Galaxy>();
	scene_->CreateComponent<PhysicsWorld>();
	scene_->CreateComponent<Octree>();
	scene_->CreateComponent<NavigationMesh>();
	scene_->CreateComponent<Navigable>();

	GetSubsystem<Script>()->SetDefaultScene(scene_);
}

void GameState::Start ()
{
	ScriptFile* file = GetSubsystem<ResourceCache>()->GetResource<ScriptFile>("Scripts/Map/CampaignMap.as");
	MapGenerator gen(file);

	gen.Generate();

	scene_->GetComponent<NavigationMesh>()->Build();
	scene_->SetEnabled(true);
}

void GameState::Stop ()
{
}

void GameState::Destroy ()
{
}
