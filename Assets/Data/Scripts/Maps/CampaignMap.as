#include "Scripts/Types/Galaxy.as"
#include "Scripts/Types/System.as"

class Map : ScriptObject
{
    Galaxy@ galaxy;

	Map()
	{
        @galaxy = Galaxy();
	}
    
    void run()
	{	
		System@ solari = galaxy.CreateSystem("Solari", Vector3(0,0,0));
	}
}