#include "Scripts/Types/Planet.as"
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
            system.AddPlanet(CreatePlanet(sysNode, "Solari Prime", Vector3(0,0,0), "Materials/Planets/SolariPrime.xml"));
        }
	}
    
    Planet@ CreatePlanet(Node@ system, const String& name, const Vector3& position, const String& material)
    {
        Node@ planetNode = system.CreateChild(name);
        planetNode.position = position;
        
        StaticModel@ model = planetNode.CreateComponent("StaticModel");
        model.model = cast<Model@>(cache.GetResource("Model","Models/Sphere.mdl"));
        model.material = cast<Material@>(cache.GetResource("Material", material));
        
        ScriptObject@ object = planetNode.CreateScriptObject("Scripts/Types/Planet.as", "Planet");
        if (object !is null)
        {
            Planet@ planet = cast<Planet>(object);
            return planet;
        }
   
        return null;
    }
}