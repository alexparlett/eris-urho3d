////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include <Object.h>

EVENT(E_STATECREATED, StateCreated)
{
    PARAM(P_STATE, State); //State*
    PARAM(P_ID, Id); //StringHash
}

EVENT(E_STATECHANGED, StateChanged)
{
    PARAM(P_ID, Id);  //StringHash
}