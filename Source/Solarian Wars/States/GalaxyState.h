////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include "State.h"
#include "IO/Bindings.h"
#include "IO/Settings.h"

#include <Camera.h>
#include <Node.h>
#include <Ptr.h>
#include <UI.h>
#include <Timer.h>
#include <ScriptFile.h>

class GalaxyState : public State
{
    OBJECT(GalaxyState)
    BASEOBJECT(State)

public:
    GalaxyState(Urho3D::Context* context);
    virtual ~GalaxyState();

    virtual void Create ();
    virtual void Start ();
    virtual void Stop ();
    virtual void Destroy ();

private:
    void CreateScene();
    void CreateCamera();

    void HandleMouseMove(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void HandleMouseClick(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void HandleMouseWheel(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void HandleKeyDown(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void HandleGameFinished(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);

	Urho3D::SharedPtr<Urho3D::Scene> scene_;
    Urho3D::SharedPtr<Urho3D::Node> camera_;
    Bindings* bindings_;
    Settings* settings_;
    Urho3D::UI* ui_;
    Urho3D::Time* time_;
};

