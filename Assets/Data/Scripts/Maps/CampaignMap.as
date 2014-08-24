class MapGenerator : ScriptObject
{
	MapGenerator()
	{
	}
	
	void generate()
	{
		SystemProperties& props = SystemProperties();
		props.name_ = "Solarian";
		props.position_ = Vector3(0,0,0
		
		System@ solarian = galaxy.CreateSystem(props);
	}
}