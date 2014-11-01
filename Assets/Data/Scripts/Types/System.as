#include "Scripts/Types/Planet.as"

shared class System : ScriptObject
{
    Array<Planet@> planets;

	System()
	{
	}
    
    void AddPlanet(Planet@ planet)
    {
        planets.Push(planet);
    }
 }