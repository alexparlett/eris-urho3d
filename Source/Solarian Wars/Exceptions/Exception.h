////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////


#pragma once

#include <Str.h>

class Exception
{
public:
    Exception(const Urho3D::String& cause) : cause_(cause) {}
    virtual ~Exception() {}

    virtual const Urho3D::String& GetCause(void) const;

private:
    Urho3D::String cause_;
};

