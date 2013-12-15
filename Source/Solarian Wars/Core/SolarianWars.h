#pragma once

#include "SolarianWarsConfig.h"
#include "IO/Settings.h"

#include <Application.h>
#include <Engine.h>

using namespace Urho3D;

class SolarianWars : public Application
{
	OBJECT(SolarianWars)

public:
	SolarianWars(Context* context);

	virtual void Setup();
	virtual void Start();
	virtual void Stop();
};

