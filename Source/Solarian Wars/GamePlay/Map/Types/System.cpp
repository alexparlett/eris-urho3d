////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "System.h"

#include <Context.h>

using namespace Urho3D;

System::System(Context* context) :
	Component(context)
{
}

System::~System()
{
}

void System::RegisterObject(Context* context)
{
    context->RegisterFactory<System>("GAME_CATEGORY");
}
