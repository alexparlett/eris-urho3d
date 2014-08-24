////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include <Component.h>

class System : public Urho3D::Component
{
	OBJECT(System)
	BASEOBJECT(Component)

public:
	System(Urho3D::Context* context);
	virtual ~System();
};

