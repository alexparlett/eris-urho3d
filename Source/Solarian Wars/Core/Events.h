////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include <Object.h>


EVENT(E_STATE_CREATE, StateCreate)
{
    PARAM(P_STATE, State);              // State Ptr
    PARAM(P_ID, StateId);               // StringHash
}

EVENT(E_STATE_CHANGE, StateChange)
{
    PARAM(P_ID, StateId);               // StringHash
}

EVENT(E_STATE_DESTROY, StateDestroy)
{
    PARAM(P_ID, StateId);               // StringHash
}

EVENT(E_MOD_ACTIVATE, ModActivate)
{
    PARAM(P_ID, ModId);                 // String
    PARAM(P_PRIORITY, ModPriority);     // unsigned int
}

EVENT(E_MOD_DEACTIVATE, ModDeactivate)
{
    PARAM(P_ID, ModId);                 // String
}

EVENT(E_MOD_ORDER_SAVE, ModOrderSave)
{
}

EVENT(E_GAME_STARTED, GameStarted)
{
}

EVENT(E_GAME_FINISHED, GameFinished)
{
}

EVENT(E_GAME_TURN, GameTurn)
{
}