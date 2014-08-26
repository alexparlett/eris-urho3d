#include "../Types/Galaxy.as"
#include "../Types/System.as"

Galaxy@ galaxy;

class MapGenerator : ScriptObject
{
	MapGenerator()
	{
		galaxy = Galaxy();
	}
	
	void run()
	{	
		System@ solari = galaxy.CreateSystem("Solari", Vector3(0,0,0));
	}
}