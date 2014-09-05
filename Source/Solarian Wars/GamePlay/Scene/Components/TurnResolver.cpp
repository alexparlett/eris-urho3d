#include "TurnResolver.h"

#include "Core/Events.h"

using namespace Urho3D;

TurnResolver::TurnResolver(Context* context) :
	Component(context)
{
	SubscribeToEvent(E_GAME_TURN, HANDLER(TurnResolver, HandleTurn));
}

TurnResolver::~TurnResolver()
{
	UnsubscribeFromAllEvents();
}

void TurnResolver::RegisterObject(Context* context)
{
	context->RegisterFactory<TurnResolver>("GAME_CATEGORY");

	ACCESSOR_ATTRIBUTE(TurnResolver, VAR_BOOL, "Is Enabled", IsEnabled, SetEnabled, bool, true, AM_DEFAULT);
}

void TurnResolver::HandleTurn(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{

}
