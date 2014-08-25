////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "TypeDefs.h"

#include <Context.h>

#include "Map/Types/Galaxy.h"
#include "Map/Types/System.h"


using namespace Urho3D;


void RegisterTypeLibrary(Context* context)
{
    System::RegisterObject(context);
    Galaxy::RegisterObject(context);
}