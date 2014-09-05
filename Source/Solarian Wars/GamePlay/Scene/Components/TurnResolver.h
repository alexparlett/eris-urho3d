#pragma once

#include <Component.h>

class TurnResolver : public Urho3D::Component
{
	OBJECT(TurnResolver)
	BASEOBJECT(Component)

public:
	TurnResolver(Urho3D::Context* context);
	virtual ~TurnResolver();
	static void RegisterObject(Urho3D::Context* context);

	void HandleTurn(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
};

