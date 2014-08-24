#pragma once

#include "System.h"

#include <Scene.h>
#include <Object.h>
#include <HashMap.h>
#include <Ptr.h>

#include "GamePlay/Map/Properties/SystemProperties.h"

class Galaxy : public Urho3D::Component
{
	OBJECT(Galaxy)
	BASEOBJECT(Component)

public:
	Galaxy(Urho3D::Context* context);
	virtual ~Galaxy();

	System* CreateSystem(const SystemProperties& properties);
	System* GetSystem(const Urho3D::String& name) const;

private:
	Urho3D::HashMap<Urho3D::String, Urho3D::SharedPtr<System> > systems_;
};

