#pragma once

#include <Str.h>

class Exception
{
public:
	Exception(const Urho3D::String& cause) : m_Cause(cause) {}
	virtual ~Exception() {}

	virtual const Urho3D::String& GetCause(void) const;

private:
	Urho3D::String m_Cause;
};

