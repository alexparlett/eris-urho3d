class MapGenerator : ScriptObject
{
	MapGenerator()
	{
	}
	
	void run()
	{
		SystemProperties props;
		props.name = String("Solari");
		props.position = Vector3(0,0,0);
		
		System@ solari = galaxy.CreateSystem(props);
	}
}