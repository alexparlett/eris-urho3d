#include "Scripts/Types/System.as"

shared class Galaxy : ScriptObject
{
	Array<System@> systems;

	Galaxy()
	{
	}
	
	System@ CreateSystem(const String& name, const Vector3& position)
	{
		Node@ sysNode = scene.CreateChild(name);
		
		ScriptObject@ object = sysNode.CreateScriptObject("Scripts/Types/System.as", "System");
		if (object !is null)
		{
			System@ system = cast<System@>(object);
			systems.Push(system);
			return system;
		}
		
		return null;
	}
}