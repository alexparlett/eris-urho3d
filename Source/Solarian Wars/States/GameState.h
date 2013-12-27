////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include "Core\State.h"

class GameState : public State
{
    OBJECT(GameState)

public:
    GameState(Urho3D::Context* context);
    virtual ~GameState();

    virtual void Create ();
    virtual void Start ();
    virtual void Stop ();
    virtual void Destroy ();

};

