#ifndef __LAKEDUNGEON_H__
#define __LAKEDUNGEON_H__

#include "Module.h"
#include "List.h"
#include "Point.h"
#include "PQueue.h"
#include "DynArray.h"

#define COST_MAP_SIZE 500

#include "PugiXml\src\pugixml.hpp"

// L04: DONE 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
struct TileSetL
{
	SString	name;
	int	firstgid;
	int margin;
	int	spacing;
	int	tileWidth;
	int	tileHeight;
	int columns;
	int tilecount;

	SDL_Texture* texture;

	// L05: DONE 7: Create a method that receives a tile id and returns it's Rectfind the Rect associated with a specific tile id
	SDL_Rect GetTileRect(int gid) const;
};

//  We create an enum for map type, just for convenience,
// NOTE: Platformer game will be of type ORTHOGONAL
enum MapTypesL
{
	MAPTYPEL_UNKNOWN = 0,
	MAPTYPEL_ORTHOGONAL,
	MAPTYPEL_ISOMETRIC,
	MAPTYPEL_STAGGERED
};

// L06: DONE 5: Create a generic structure to hold properties
struct PropertiesL
{
	struct Property
	{
		SString name;
		bool value;
	};

	~PropertiesL()
	{
		//...
		ListItem<Property*>* item;
		item = list.start;

		while (item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}

		list.Clear();
	}

	// L06: DONE 7: Method to ask for the value of a custom property
	Property* GetProperty(const char* name);

	List<Property*> list;
};

// L05: DONE 1: Create a struct for the map layer

struct ImageLayerL {

	SString name;
	int width;
	int height;

	const char* texturePath;
	SDL_Texture* bgtexture;
	float parallaxFactor;

};

struct MapLayerL
{
	SString	name;
	int id; 
	int width;
	int height;
	uint* data;

	// L06: DONE: Store custom properties
	PropertiesL properties;

	MapLayerL() : data(NULL)
	{}

	~MapLayerL()
	{
		RELEASE(data);
	}

	// L05: DONE 6: Short function to get the gid value of x,y
	inline uint Get(int x, int y) const
	{
		return data[(y * width) + x];
	}
};

// L04: DONE 1: Create a struct needed to hold the information to Map node
struct MapDataL
{
	int width;
	int	height;
	int	tileWidth;
	int	tileHeight;
	List<TileSetL*> tilesets;
	MapTypesL type = MapTypesL::MAPTYPEL_ORTHOGONAL;

	// L05: DONE 2: Add a list/array of layers to the map
	List<MapLayerL*> maplayers;
	List<ImageLayerL*> imagelayers;
};

enum ColTypesL {

	TERRAINL = 0,
	DAMAGEL,
	FLOORL,
	TRIGGERL
};

struct ColDataL
{
	int x;
	int	y;
	int width;
	int	height;
	ColTypesL type;
};

class LakeDungeon : public Module
{
public:

	LakeDungeon();

    // Destructor
    virtual ~LakeDungeon();

    // Called before render is available
    bool Awake(pugi::xml_node& conf);

    // Called each loop iteration
    void Draw();

    // Called before quitting
    bool CleanUp();

    // Load new map
    bool Load();

	// L05: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
	iPoint MapToWorld(int x, int y) const;

	iPoint LakeDungeon::WorldToMap(int x, int y);

	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;

	void ResetPath();
	void DrawPath();
	bool IsWalkable(int x, int y) const;

	// L10: Methods for BFS + Pathfinding and cost function for Dijkstra
	int MovementCost(int x, int y) const;
	void ComputePath(int x, int y);

	// Propagation methods
	void PropagateDijkstra(); //L10


private:

	bool LoadMap(pugi::xml_node mapFile);

	// L04: DONE 4: Create and call a private function to load a tileset
	bool LoadTileSet(pugi::xml_node mapFile);

	// L05
	bool LoadColliders(pugi::xml_node& node);
	void CreateColliders(ColDataL colliders);

	bool LoadLayer(pugi::xml_node& node, MapLayerL* layer);
	bool LoadAllLayers(pugi::xml_node mapNode);

	// L06: DONE 2
	TileSetL* GetTilesetFromTileId(int gid) const;

	// L06: DONE 6: Load a group of properties 
	bool LoadProperties(pugi::xml_node& node, PropertiesL& properties);

public:

	// L04: DONE 1: Declare a variable data of the struct MapData
	MapDataL mapData;
	// L09: BFS Pathfinding variables
	PQueue<iPoint> frontier;
	List<iPoint> visited;

	// L09 DONE 4: Define destionation point 
	iPoint destination;

	// L10: Additional variables
	List<iPoint> breadcrumbs;

	bool DestinationFound = false, DrawPathing = false;

private:

	SString mapFileName;
	SString mapFolder;
	bool mapLoaded;

	uint costSoFar[COST_MAP_SIZE][COST_MAP_SIZE];
	DynArray<iPoint> path;

	SDL_Texture* tileX = nullptr;
};

#endif // __LAKEDUNGEON_H__