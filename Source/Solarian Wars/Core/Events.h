////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include <Core/Object.h>


URHO3D_EVENT(E_STATE_CREATE, StateCreate)
{
	URHO3D_PARAM(P_STATE, State);              // State Ptr
	URHO3D_PARAM(P_ID, StateId);               // StringHash
}

URHO3D_EVENT(E_STATE_CHANGE, StateChange)
{
	URHO3D_PARAM(P_ID, StateId);               // StringHash
}

URHO3D_EVENT(E_STATE_DESTROY, StateDestroy)
{
	URHO3D_PARAM(P_ID, StateId);               // StringHash
}

URHO3D_EVENT(E_MOD_ACTIVATE, ModActivate)
{
	URHO3D_PARAM(P_ID, ModId);                 // String
	URHO3D_PARAM(P_PRIORITY, ModPriority);     // unsigned int
}

URHO3D_EVENT(E_MOD_DEACTIVATE, ModDeactivate)
{
	URHO3D_PARAM(P_ID, ModId);                 // String
}

URHO3D_EVENT(E_MOD_ORDER_SAVE, ModOrderSave)
{
}

URHO3D_EVENT(E_GAME_STARTED, GameStarted)
{
}

URHO3D_EVENT(E_GAME_FINISHED, GameFinished)
{
}

URHO3D_EVENT(E_ANIMATION_FINISHED, AnimationFinished)
{
}