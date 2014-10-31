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

using namespace Urho3D;

MenuState::MenuState (Context* context) :
    State (context)
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
    viewport->SetCamera(camera_->GetChild("CameraNode")->GetComponent<Camera>());

    scene_->SetUpdateEnabled(true);

    GetSubsystem<UI>()->GetCursor()->SetVisible(true);
}

void MenuState::Stop()
{
    scene_->SetUpdateEnabled(false);
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
    camNode->CreateComponent<Camera>();
}

void MenuState::CreateUI()
{
    UI* ui = GetSubsystem<UI>();
}
