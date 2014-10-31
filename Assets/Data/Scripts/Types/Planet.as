class Planet : ScriptObject
{
    System@ system;

	Planet()
	{
	}
    
    void Start()
    {
        @system = cast<System@>(node.parent.scriptObject);
    }
 }