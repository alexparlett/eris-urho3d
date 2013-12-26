////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////


#include "StateManager.h"
#include "Events.h"

#include <Log.h>

using namespace Urho3D;

StateManager::StateManager(Urho3D::Context* context) :
    Object(context),
    currentState_(-1)
{
    SubscribeToEvent(E_STATE_CREATED, HANDLER(StateManager, StateCreated));
    SubscribeToEvent(E_STATE_CHANGED, HANDLER(StateManager, StateChanged));
    SubscribeToEvent(E_STATE_DESTROYED, HANDLER(StateManager, StateDestroyed));
}

StateManager::~StateManager()
{
    HashMap<StringHash, SharedPtr<State>>::Iterator iter = states_.Begin();
    while (iter != states_.End())
    {
        iter->second_->Destroy();
        iter->second_.Reset();
        iter++;
    }

    states_.Clear();
}

void StateManager::StateCreated(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
    using namespace StateCreated;

    State* state = reinterpret_cast<State*>(eventData[P_STATE].GetPtr());
    StringHash id = eventData[P_ID].GetStringHash();
    if (state && id != StringHash::ZERO)
    {
        if (!states_.Contains(id))
        {
            state->Create();
            states_[eventData[P_ID].GetStringHash()] = state;
        }
        else
        {
            LOGERROR("State already exists with id of " + id.ToString());
        }
    }
}

void StateManager::StateChanged(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
    using namespace StateChanged;

    StringHash id = eventData[P_ID].GetStringHash();
    if (id != StringHash::ZERO)
    {
        if (states_.Contains(id))
        {
            State* newActive = states_[id];

            if (currentState_ != StringHash::ZERO && states_.Contains(currentState_)) 
            {
                states_[currentState_]->Stop();
            }

            newActive->Start();
            currentState_ = id;
        }
        else
        {
            LOGERROR("State with id " + eventData[P_ID].ToString() + " does not exist.");
        }
    }
    else
    {
        LOGERROR("State with id " + eventData[P_ID].ToString() + " is not a valid id.");
    }
}

void StateManager::StateDestroyed(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
    using namespace StateDestroyed;

    StringHash id = eventData[P_ID].GetStringHash();
    if (id != StringHash::ZERO)
    {
        if (states_.Contains(id) && id != currentState_)
        {
            states_[id]->Destroy();
            states_[id].Reset();
            states_.Erase(id);
        }
        else
        {
            LOGERROR("Cannot destroy state " + id.ToString() + " either current or doesn't exist.");
        }
    }
    else
    {
        LOGERROR("State with id " + id.ToString() + " is not a valid id.");
    }
}

State* StateManager::GetState(const Urho3D::StringHash& id) const
{
    HashMap<StringHash, SharedPtr<State>>::ConstIterator find = states_.Find(id);
    return find != states_.End() ? find->second_.Get() : NULL;
}