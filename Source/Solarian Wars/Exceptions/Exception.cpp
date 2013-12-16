#include "Exception.h"

using namespace Urho3D;

const String& Exception::GetCause(void) const
{
    return m_Cause;
}
