////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////


#include "StateManager.h"
#include "Events.h"

#include <Log.h>
#include <ForEach.h>

using namespace Urho3D;

StateManager::StateManager(Urho3D::Context* context) :
Object(context),
m_States(HashMap<StringHash, SharedPtr<State>>()),
m_CurrentState(-1)
{
    SubscribeToEvent(E_STATE_CREATED, HANDLER(StateManager, StateCreated));
    SubscribeToEvent(E_STATE_CHANGED, HANDLER(StateManager, StateChanged));
    SubscribeToEvent(E_STATE_DESTROYED, HANDLER(StateManager, StateDestroyed));
}

StateManager::~StateManager()
{
    foreach(StringHash state, m_States.Keys())
    {
        m_States[state]->Destroy();
        m_States[state].Reset();
    }

    m_States.Clear();
}

void StateManager::StateCreated(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
    using namespace StateCreated;

    State* state = reinterpret_cast<State*>(eventData[P_STATE].GetPtr());
    StringHash id = eventData[P_ID].GetStringHash();
    if (state && id != StringHash::ZERO)
    {
        if (!m_States.Contains(id))
        {
            state->Create();
            m_States[eventData[P_ID].GetStringHash()] = state;
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
        if (m_States.Contains(id))
        {
            State* newActive = m_States[id];

            if (m_CurrentState != StringHash::ZERO && m_States.Contains(m_CurrentState)) 
            {
                m_States[m_CurrentState]->Stop();
            }

            newActive->Start();
            m_CurrentState = id;
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
        if (m_States.Contains(id) && id != m_CurrentState)
        {
            m_States[id]->Destroy();
            m_States[id].Reset();
            m_States.Erase(id);
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
