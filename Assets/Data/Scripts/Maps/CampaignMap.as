#include "Scripts/Globals.as"
#include "Scripts/Types/Galaxy.as"
#include "Scripts/Types/System.as"

class CampaignMap : Map, ScriptObject
{
	CampaignMap()
	{
        @galaxy = Galaxy();
	}
    
    void generate()
	{	
		System@ solari = galaxy.CreateSystem("Solari", Vector3(0,0,0));
	}
}