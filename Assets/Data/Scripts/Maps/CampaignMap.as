class MapGenerator : ScriptObject
{
	MapGenerator()
	{
	}
	
	void run()
	{
		SystemProperties props;
		props.name = "Solari";
		props.position = Vector3(0,0,0);
		props.controller = "Scripts/Controllers/SystemController.as";
		
		System@ solari = galaxy.CreateSystem(props);
	}
}