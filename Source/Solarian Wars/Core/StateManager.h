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
#include <UIElement.h>
#include <Cursor.h>

#include "States/State.h"

class StateManager : public Urho3D::Object
{
    OBJECT(StateManager)

public:
    StateManager(Urho3D::Context* context);
    virtual ~StateManager();

    void InitializeLoadingUI();

    template<typename T>
    T* GetState(const Urho3D::StringHash& id) const;
    State* GetState(const Urho3D::StringHash& id) const;
private:
    void HandleCreate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void HandleChange(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void HandleDestroy(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void HandleBeginFrame(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);

    void DeleteStates();
    void CreateStates();
    void SwitchToNext();

    Urho3D::HashMap<Urho3D::StringHash, Urho3D::SharedPtr<State>> states_;
    Urho3D::HashMap<Urho3D::StringHash, Urho3D::SharedPtr<State>> awaitingCreate_;
    Urho3D::HashMap<Urho3D::StringHash, Urho3D::SharedPtr<State>> awaitingDelete_;
    Urho3D::StringHash currentState_;
	Urho3D::StringHash nextState_;

    Urho3D::SharedPtr<Urho3D::UIElement> loadingRoot_;
    Urho3D::WeakPtr<Urho3D::Cursor> cursor_;
};

template<typename T>
T* StateManager::GetState(const Urho3D::StringHash& id) const
{
    return dynamic_cast<T*>(GetState(id));
}