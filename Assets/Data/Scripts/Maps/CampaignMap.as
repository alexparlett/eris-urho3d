#include "Scripts/Types/System.as"

class CampaignMap : Map, ScriptObject
{
	CampaignMap()
	{
	}
    
    void Generate()
	{	
        CreateSolari();
	}
    
	void CreateSolari()
	{
		Node@ sysNode = scene.CreateChild("Solari");
        sysNode.position = Vector3(0,0,0);

        ScriptObject@ object = sysNode.CreateScriptObject("Scripts/Types/System.as", "System");
        if (object !is null)
        {
            System@ system = cast<System>(object);       
            system.AddPlanet(CreatePlanet(sysNode, "Solari Prime", Vector3(0,0,0)));
        }
	}
    
    Planet@ CreatePlanet(Node@ system, const String& name, const Vector3& position)
    {
        Node@ planetNode = system.CreateChild(name);
        planetNode.position = position;
        
        StaticModel@ model = planetNode.CreateComponent("StaticModel");
        model.model = cast<Model@>(cache.GetResource("Model","Models/Sphere.mdl"));
        model.material = cast<Material@>(cache.GetResource("Material","Materials/Planets/SolariPrime.xml"));
        
        ScriptObject@ object = planetNode.CreateScriptObject("Scripts/Types/Planet.as", "Planet");
        if (object !is null)
        {
            Planet@ planet = cast<Planet>(planetNode.scriptObject);
            return planet;
        }
   
        return null;
    }
}