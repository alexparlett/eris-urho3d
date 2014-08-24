#include "LaunchState.h"

#include <Context.h>
#include <ResourceCache.h>
#include <UI.h>

using namespace Urho3D;

LaunchState::LaunchState(Context* context) :
    State(context)
{
}

LaunchState::~LaunchState()
{
}

void LaunchState::Create()
{
	ResourceCache* rc = GetSubsystem<ResourceCache>();
	UI* ui =  GetSubsystem<UI>();
	ui->LoadLayout(rc->GetResource<XMLFile>("UI/loading.xml"), rc->GetResource<XMLFile>("UI/style.xml"));
}

void LaunchState::Start()
{
	ResourceCache* rc = GetSubsystem<ResourceCache>();
}

void LaunchState::Stop()
{
}

void LaunchState::Destroy()
{
}
