////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include "Core\State.h"

class MenuState : public State
{
    OBJECT (MenuState)

public:
    MenuState (Urho3D::Context* context);
    virtual ~MenuState ();
};

