#include "Scripts/Types/Galaxy.as"

class CampaignMap : Map, ScriptObject
{
    Galaxy@ galaxy;

	CampaignMap()
	{
        @galaxy = Galaxy();
	}
    
    void Start()
    {
        RegisterGlobalScriptObject("ScriptObject@ galaxyObject", galaxy);
    }
    
    void Generate()
	{	
		System@ solari = galaxy.CreateSystem("Solari", Vector3(0,0,0));
	}
}