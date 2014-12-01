#include "Scripts/Types/Planet.as"
#include "Scripts/Types/System.as"

class CampaignMap : Map, ScriptObject
{
	CampaignMap()
	{
	}
    
    void Generate()
	{	
        CreateSolariSystem();
	}
    
	void CreateSolariSystem()
	{
		Node@ sysNode = scene.CreateChild("Solari");
        sysNode.position = Vector3(0,0,0);

        ScriptObject@ object = sysNode.CreateScriptObject("Scripts/Types/System.as", "System");
        if (object !is null)
        {
            System@ system = cast<System>(object);  
            system.AddPlanet(CreatePlanet(sysNode, "Solari Prime", Vector3(30,0,30), "SolariPrime"));
        }
	}
    
    Planet@ CreatePlanet(Node@ system, const String& name, const Vector3& position, const String& typeName)
    {
        Node@ planetNode = system.CreateChild(name);
        planetNode.position = position;
        
        PlanetType@ type = typeCache.GetPlanetType(typeName);
        
        StaticModel@ model = planetNode.CreateComponent("StaticModel");
        model.model = type.GetModel();
        model.material = type.GetMaterial();
        
        ScriptObject@ object = planetNode.CreateScriptObject(type.GetScriptFile(), type.GetScriptClass());
        if (object !is null)
        {
            Planet@ planet = cast<Planet>(object);
            return planet;
        }
   
        return null;
    }
}