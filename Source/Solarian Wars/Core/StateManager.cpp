////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////


#include "StateManager.h"
#include "Events.h"

#include <IO/Log.h>
#include <Core/CoreEvents.h>
#include <UI/UI.h>
#include <UI/BorderImage.h>
#include <Graphics/Texture2D.h>
#include <Graphics/Graphics.h>
#include <Resource/ResourceCache.h>
#include <Math/Color.h>

using namespace Urho3D;

StateManager::StateManager(Context* context) :
    Object(context),
	currentState_(StringHash::ZERO),
	nextState_(StringHash::ZERO),
    states_(HashMap<StringHash, SharedPtr<State>>()),
    awaitingCreate_(HashMap<StringHash, SharedPtr<State>>()),
    awaitingDelete_(HashMap<StringHash, SharedPtr<State>>())
{
    SubscribeToEvent(E_STATE_CREATE, URHO3D_HANDLER(StateManager, HandleCreate));
    SubscribeToEvent(E_STATE_CHANGE, URHO3D_HANDLER(StateManager, HandleChange));
    SubscribeToEvent(E_STATE_DESTROY, URHO3D_HANDLER(StateManager, HandleDestroy));
	SubscribeToEvent(E_BEGINFRAME, URHO3D_HANDLER(StateManager, HandleBeginFrame));
}

StateManager::~StateManager()
{
	UnsubscribeFromAllEvents();

    HashMap<StringHash, SharedPtr<State>>::Iterator states = states_.Begin();
    while (states != states_.End())
    {
        states->second_->Destroy();
        states++;
    }

    HashMap<StringHash, SharedPtr<State>>::Iterator creates = awaitingCreate_.Begin();
    while (creates != awaitingCreate_.End())
    {
        creates->second_->Destroy();
        creates++;
    }

    HashMap<StringHash, SharedPtr<State>>::Iterator deletes = awaitingDelete_.Begin();
    while (deletes != awaitingDelete_.End())
    {
        deletes->second_->Destroy();
        deletes++;
    }

    states_.Clear();
    awaitingCreate_.Clear();
    awaitingDelete_.Clear();
}

void StateManager::InitializeLoadingUI()
{
    ResourceCache* rc = GetSubsystem<ResourceCache>();
    UI* ui = GetSubsystem<UI>();
    Graphics* graphics = GetSubsystem<Graphics>();

    UIElement* root = ui->GetRoot();

    cursor_ = ui->GetCursor();

    loadingRoot_ = root->CreateChild<BorderImage>("LoadingBackground");
    loadingRoot_->SetColor(Color::BLACK);
    loadingRoot_->SetSize(graphics->GetWidth(), graphics->GetHeight());
    loadingRoot_->SetVisible(false);
    loadingRoot_->SetPriority(M_MAX_INT - 1);

    BorderImage* logo = loadingRoot_->CreateChild<BorderImage>("LoadingLogo");
    logo->SetTexture(rc->GetResource<Texture2D>("Textures/UI/LoadingLogo.png"));
    logo->SetSize(512, 512);
    logo->SetAlignment(HorizontalAlignment::HA_CENTER, VerticalAlignment::VA_CENTER);
    logo->SetBlendMode(BLEND_ALPHA);
    logo->SetPriority(M_MAX_INT - 1);
}


void StateManager::HandleCreate(StringHash eventType, VariantMap& eventData)
{
    using namespace StateCreate;

    State* state = reinterpret_cast<State*>(eventData[P_STATE].GetPtr());
    StringHash id = eventData[P_ID].GetStringHash();

    if (state && id != StringHash::ZERO)
    {
        if (!states_.Contains(id) && !awaitingCreate_.Contains(id))
        {
            awaitingCreate_[id] = state;
        }
        else
			URHO3D_LOGERROR("State already exists with id of " + id.ToString());
    }
}

void StateManager::HandleChange(StringHash eventType, VariantMap& eventData)
{
    using namespace StateChange;

    StringHash id = eventData[P_ID].GetStringHash();
    if (id != StringHash::ZERO)
    {
        if (states_.Contains(id) || awaitingCreate_.Contains(id))
        {
			nextState_ = id;
        }
        else
        {
			URHO3D_LOGERROR("State with id " + eventData[P_ID].ToString() + " does not exist.");
        }
    }
    else
    {
		URHO3D_LOGERROR("State with id " + eventData[P_ID].ToString() + " is not a valid id.");
    }
}

void StateManager::HandleDestroy(StringHash eventType, VariantMap& eventData)
{
	using namespace StateDestroy;

	StringHash id = eventData[P_ID].GetStringHash();
	if (id != StringHash::ZERO)
	{
		if (states_.Contains(id))
		{
            awaitingDelete_[id] = states_[id];
			states_.Erase(id);
		}
		else
			URHO3D_LOGERROR("Cannot destroy state " + id.ToString() + " either current, next or doesn't exist.");
	}
	else
		URHO3D_LOGERROR("State with id " + id.ToString() + " is not a valid id.");
}

void StateManager::HandleBeginFrame(StringHash eventType, VariantMap& eventData)
{
    if (awaitingCreate_.Size() > 0 || nextState_ != StringHash::ZERO || awaitingDelete_.Size() > 0)
    {
        if (loadingRoot_->IsVisible())
        {
            CreateStates();
            SwitchToNext();
            DeleteStates();

            loadingRoot_->SetVisible(false);
        }
        else
        {
            loadingRoot_->SetVisible(true);
            cursor_->SetVisible(false);
        }
    }
}

State* StateManager::GetState(const StringHash& id) const
{
    HashMap<StringHash, SharedPtr<State>>::ConstIterator find = states_.Find(id);
    return find != states_.End() ? find->second_.Get() : NULL;
}

void StateManager::SwitchToNext()
{
    if (nextState_ != StringHash::ZERO)
    {
        if (states_.Contains(nextState_))
        {
            State* newActive = states_[nextState_];

            if (currentState_ != StringHash::ZERO && states_.Contains(currentState_))
                states_[currentState_]->Stop();

            currentState_ = nextState_;
            nextState_ = StringHash::ZERO;
            newActive->Start();
        }
        else
			URHO3D_LOGERRORF("State with id %s is not a valid id.", nextState_);
    }
}

void StateManager::CreateStates()
{
    if (awaitingCreate_.Size() > 0)
    {
        HashMap<StringHash, SharedPtr<State>>::Iterator iter = awaitingCreate_.Begin();
        while (iter != awaitingCreate_.End())
        {
            HashMap<StringHash, SharedPtr<State>>::Iterator erase = iter;
            iter->second_->Create();
            states_[iter->first_] = iter->second_;
            iter++;
            awaitingCreate_.Erase(erase);
        }
    }
}

void StateManager::DeleteStates()
{
    if (awaitingDelete_.Size() > 0)
    {
        HashMap<StringHash, SharedPtr<State>>::Iterator iter = awaitingDelete_.Begin();
        while (iter != awaitingDelete_.End())
        {
            HashMap<StringHash, SharedPtr<State>>::Iterator erase = iter;
            iter->second_->Destroy();
            iter++;
            awaitingDelete_.Erase(erase);
        }
    }
}
