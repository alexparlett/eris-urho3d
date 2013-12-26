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
    void StateDestroyed(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);

    template<typename T>
    T* GetState(const Urho3D::StringHash& id) const;
    State* GetState(const Urho3D::StringHash& id) const;

private:
    Urho3D::HashMap<Urho3D::StringHash, Urho3D::SharedPtr<State>> states_;
    Urho3D::StringHash currentState_;
};

template<typename T>
T* StateManager::GetState(const Urho3D::StringHash& id) const
{
    return dynamic_cast<T*>(GetState(id));
}