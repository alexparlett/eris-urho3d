////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include <Object.h>

EVENT(E_STATE_CREATED, StateCreated)
{
    PARAM(P_STATE, State); //State*
    PARAM(P_ID, Id); //StringHash
}

EVENT(E_STATE_CHANGED, StateChanged)
{
    PARAM(P_ID, Id);  //StringHash
}

EVENT(E_STATE_DESTROYED, StateDestroyed)
{
    PARAM(P_ID, Id); //StringHash
}

EVENT(E_MOD_ACTIVATED, ModActivated)
{
    PARAM(P_ID, Id); //String
    PARAM(P_PRIORITY, Priority); //unsigned int
}

EVENT(E_MOD_DEACTIVATED, ModDeactivated)
{
    PARAM(P_ID, Id); // String
}

EVENT(E_MOD_ORDER_SAVED, ModOrderSaved)
{
}

EVENT(E_GAME_SAVED, GameSaved)
{
}