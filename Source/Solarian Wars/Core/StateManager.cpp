////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////


#include "StateManager.h"
#include "Events.h"

#include <Log.h>
#include <CoreEvents.h>

using namespace Urho3D;

StateManager::StateManager(Context* context) :
    Object(context),
	currentState_(StringHash::ZERO),
	nextState_(StringHash::ZERO),
    states_(HashMap<StringHash, SharedPtr<State>>())
{
    SubscribeToEvent(E_STATE_CREATE, HANDLER(StateManager, HandleCreate));
    SubscribeToEvent(E_STATE_CHANGE, HANDLER(StateManager, HandleChange));
    SubscribeToEvent(E_STATE_DESTROY, HANDLER(StateManager, HandleDestroy));
	SubscribeToEvent(E_BEGINFRAME, HANDLER(StateManager, HandleFrame));
}

StateManager::~StateManager()
{
	UnsubscribeFromAllEvents();

    HashMap<StringHash, SharedPtr<State>>::Iterator iter = states_.Begin();
    while (iter != states_.End())
    {
        iter->second_->Destroy();
        iter++;
    }

    states_.Clear();
}

void StateManager::HandleCreate(StringHash eventType, VariantMap& eventData)
{
    using namespace StateCreate;

    State* state = reinterpret_cast<State*>(eventData[P_STATE].GetPtr());
    StringHash id = eventData[P_ID].GetStringHash();
    if (state && id != StringHash::ZERO)
    {
        if (!states_.Contains(id))
        {
            state->Create();
            states_[id] = state;
        }
        else
            LOGERROR("State already exists with id of " + id.ToString());
    }
}

void StateManager::HandleChange(StringHash eventType, VariantMap& eventData)
{
    using namespace StateChange;

    StringHash id = eventData[P_ID].GetStringHash();
    if (id != StringHash::ZERO)
    {
        if (states_.Contains(id))
        {
			nextState_ = id;
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

void StateManager::HandleDestroy(StringHash eventType, VariantMap& eventData)
{
	using namespace StateDestroy;

	StringHash id = eventData[P_ID].GetStringHash();
	if (id != StringHash::ZERO)
	{
		if (states_.Contains(id) && id != currentState_ && id != nextState_)
		{
			states_[id]->Destroy();
			states_[id].Reset();
			states_.Erase(id);
		}
		else
			LOGERROR("Cannot destroy state " + id.ToString() + " either current, next or doesn't exist.");
	}
	else
		LOGERROR("State with id " + id.ToString() + " is not a valid id.");
}

void StateManager::HandleFrame(StringHash eventType, VariantMap& eventData)
{
	if (nextState_ != StringHash::ZERO)
	{
		State* newActive = states_[nextState_];

		if (currentState_ != StringHash::ZERO && states_.Contains(currentState_))
			states_[currentState_]->Stop();

		currentState_ = nextState_;
		nextState_ = StringHash::ZERO;
		newActive->Start();
	}
}

State* StateManager::GetState(const StringHash& id) const
{
    HashMap<StringHash, SharedPtr<State>>::ConstIterator find = states_.Find(id);
    return find != states_.End() ? find->second_.Get() : NULL;
}