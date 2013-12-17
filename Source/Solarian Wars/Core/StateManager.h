////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include <Object.h>
#include <HashMap.h>
#include <Ptr.h>
#include <Variant.h>
#include <StringHash.h>

#include "State.h"

class StateManager : public Urho3D::Object
{
    OBJECT(StateManager)

public:
    StateManager(Urho3D::Context* context);
    virtual ~StateManager();

    void StateCreated(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void StateChanged(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);

private:
    Urho3D::HashMap<Urho3D::StringHash, Urho3D::SharedPtr<State>> m_States;
    Urho3D::StringHash m_CurrentState;
};

