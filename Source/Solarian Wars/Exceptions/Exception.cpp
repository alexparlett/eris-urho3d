#include "Exception.h"

const Urho3D::String& Exception::GetCause(void) const
{
	return m_Cause;
}
