////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////


#include "Exception.h"

using namespace Urho3D;

const String& Exception::GetCause(void) const
{
    return cause_;
}
