
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Blacksmith.h"
#include "Physics.h"
#include "Window.h"
#include "Pathfinding.h"
#include "Scene.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>
#include "SDL_image/include/SDL_image.h"


Blacksmith::Blacksmith() : Module(), mapLoaded(false)
{
    name.Create("Blacksmith");
}

// Destructor
Blacksmith::~Blacksmith()
{}

// Called before render is available
bool Blacksmith::Awake(pugi::xml_node& config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    mapFileName = "Assets/Maps/Forge.tmx";
    mapFolder = "Assets/Maps/";

    ////Initialize the path
    //frontier.Push(iPoint(20, 14), 0);
    //visited.Add(iPoint(20, 14));
    //breadcrumbs.Add(iPoint(20, 14));

    //// L09 DONE 4: Initialize destination point
    //destination = iPoint(25, 14);

    return ret;
}

void Blacksmith::Draw()
{
    if(mapLoaded == false)
        return;

    ListItem<ImageLayerB*>* imageLayerItem;
    imageLayerItem = mapData.imagelayers.start;

    while (imageLayerItem != NULL) {

        for (int i = 0; i < 4; i++) {
            app->render->DrawTexture(imageLayerItem->data->bgtexture, -1 * (app->render->camera.x / (int)app->win->GetScale() * imageLayerItem->data->parallaxFactor) + i * 592, -1 * ((app->render->camera.y / (int)app->win->GetScale())) - 90);

        }

        imageLayerItem = imageLayerItem->next;
    }
   
    //app->render->DrawTexture(BGtexture,  -1 * (app->render->camera.x / (int)app->win->GetScale()),-1 * (app->render->camera.y / (int)app->win->GetScale()));
   // app->render->DrawRectangle({ -1 * (app->render->camera.x / (int)app->win->GetScale()),-1 * (app->render->camera.y / (int)app->win->GetScale()),app->render->camera.w,app->render->camera.h }, 0, 255, 0, 50);



    ListItem<MapLayerB*>* mapLayerItem;
    mapLayerItem = mapData.maplayers.start;

    while (mapLayerItem != NULL) {

        //L06: DONE 7: use GetProperty method to ask each layer if your Draw property is true.
        if (mapLayerItem->data->properties.GetProperty("Draw") != NULL && mapLayerItem->data->properties.GetProperty("Draw")->value) {

            for (int x = 0; x < mapLayerItem->data->width; x++)
            {
                for (int y = 0; y < mapLayerItem->data->height; y++)
                {
                    // L05: DONE 9: Complete the draw function
                    int gid = mapLayerItem->data->Get(x, y);

                    //L06: DONE 3: Obtain the tile set using GetTilesetFromTileId
                    TileSetB* tileset = GetTilesetFromTileId(gid);

                    SDL_Rect r = tileset->GetTileRect(gid);
                    iPoint pos = MapToWorld(x, y);

                    app->render->DrawTexture(tileset->texture,
                        pos.x,
                        pos.y-800,
                        &r);
                }
            }
        }
        mapLayerItem = mapLayerItem->next;

    }
    DrawPath();

}

// L12: Create walkability map for pathfinding
bool Blacksmith::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
    bool ret = false;
    ListItem<MapLayerB*>* item;
    item = mapData.maplayers.start;

    for (item = mapData.maplayers.start; item != NULL; item = item->next)
    {
        MapLayerB* layer = item->data;

        LOG("Layer: %d", layer->id);
        
        /*if (layer->properties.GetProperty("Navigation") != NULL && !layer->properties.GetProperty("Navigation")->value)
            continue;*/
        if (layer->id != 12)
            continue; 

        uchar* map = new uchar[layer->width * layer->height];
        memset(map, 1, layer->width * layer->height);
        LOG("Layer: %d", layer->id);
        for (int y = 0; y < mapData.height; ++y)
        {
            for (int x = 0; x < mapData.width; ++x)
            {
                int i = (y * layer->width) + x;
                
                int tileId = layer->data[i];
                /*LOG("i : %d", i);
                LOG("id : %u",layer->data[i]);
                */
                TileSetB* tileset = (tileId > -1) ? GetTilesetFromTileId(tileId) : NULL;

                if (tileset != NULL)
                {
                    //map[i] = (tileId - tileset->firstgid) > 0 ? 0 : 1;
                    if (mapData.type == MapTypesB::MAPTYPEB_ISOMETRIC && tileId == 25) map[i] = 1;
                    else if (mapData.type == MapTypesB::MAPTYPEB_ORTHOGONAL && tileId == 119) map[i] = 1;
                    else map[i] = 0;
                }
                else {
                    LOG("CreateWalkabilityMap: Invalid tileset found");
                    map[i] = 0;
                }
            }
            
        }

        *buffer = map;
        width = mapData.width;
        height = mapData.height;
        ret = true;

        break;
    }

    return ret;
}


// L05: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
iPoint Blacksmith::MapToWorld(int x, int y) const
{
    iPoint ret;

    ret.x = x * mapData.tileWidth;
    ret.y = y * mapData.tileHeight;

    return ret;
}

// L08: DONE 3: Add method WorldToMap to obtain  map coordinates from screen coordinates
iPoint Blacksmith::WorldToMap(int x, int y)
{
    iPoint ret(0, 0);

    if (mapData.type == MAPTYPEB_ORTHOGONAL)
    {
        ret.x = x / mapData.tileWidth;
        ret.y = y / mapData.tileHeight;
    }
    else if (mapData.type == MAPTYPEB_ISOMETRIC)
    {
        float halfWidth = mapData.tileWidth * 0.5f;
        float halfHeight = mapData.tileHeight * 0.5f;
        ret.x = int((x / halfWidth + y / halfHeight) / 2);
        ret.y = int((y / halfHeight - x / halfWidth) / 2);
    }
    else
    {
        LOG("Unknown map type");
        ret.x = x; ret.y = y;
    }

    return ret;
}


// Get relative Tile rectangle
SDL_Rect TileSetB::GetTileRect(int gid) const
{
    SDL_Rect rect = { 0 };
    int relativeIndex = gid - firstgid;

    // L05: DONE 7: Get relative Tile rectangle
    rect.w = tileWidth;
    rect.h = tileHeight;
    rect.x = margin + (tileWidth + spacing) * (relativeIndex % columns);
    rect.y = margin + (tileWidth + spacing) * (relativeIndex / columns);

    return rect;
}


// L06: DONE 2: Pick the right Tileset based on a tile id
TileSetB* Blacksmith::GetTilesetFromTileId(int gid) const
{
    ListItem<TileSetB*>* item = mapData.tilesets.start;
    TileSetB* set = NULL;

    while (item)
    {
        set = item->data;
        if (gid < (item->data->firstgid + item->data->tilecount))
        {
            break;
        }
        item = item->next;
    }

    return set;
}

// Called before quitting
bool Blacksmith::CleanUp()
{
    LOG("Unloading map");

    // L04: DONE 2: Make sure you clean up any memory allocated from tilesets/map
	ListItem<TileSetB*>* item;
	item = mapData.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	mapData.tilesets.Clear();

    // L05: DONE 2: clean up all layer data
    // Remove all layers
    ListItem<MapLayerB*>* layerItem;
    layerItem = mapData.maplayers.start;

    while (layerItem != NULL)
    {
        RELEASE(layerItem->data);
        layerItem = layerItem->next;
    }

    return true;
}

// Load new map
bool Blacksmith::Load()
{
    bool ret = true;
    tileX = app->tex->Load("Assets/Maps/path_square.png");

    pugi::xml_document mapFileXML;
    pugi::xml_parse_result result = mapFileXML.load_file(mapFileName.GetString());

    if(result == NULL)
    {
        LOG("Could not load map xml file %s. pugi error: %s", mapFileName, result.description());
        ret = false;
    }

    if(ret == true)
    {
        ret = LoadMap(mapFileXML);
    }

    if (ret == true)
    {
        ret = LoadTileSet(mapFileXML);
    }

    // L05: DONE 4: Iterate all layers and load each of them
    if (ret == true)
    {
        ret = LoadAllLayers(mapFileXML.child("map"));
    }
    
    // L07 DONE 3: Create colliders
    // Later you can create a function here to load and create the colliders from the map

    if (ret == true)
    {
        ret = LoadColliders(mapFileXML.child("map"));
    }


    if(ret == true)
    {
        // L04: DONE 5: LOG all the data loaded iterate all tilesets and LOG everything
       
        LOG("Successfully parsed map XML file :%s", mapFileName.GetString());
        LOG("width : %d height : %d",mapData.width,mapData.height);
        LOG("tile_width : %d tile_height : %d",mapData.tileWidth, mapData.tileHeight);
        
        LOG("Tilesets----");

        ListItem<TileSetB*>* tileset;
        tileset = mapData.tilesets.start;

        while (tileset != NULL) {
            LOG("name : %s firstgid : %d",tileset->data->name.GetString(), tileset->data->firstgid);
            LOG("tile width : %d tile height : %d", tileset->data->tileWidth, tileset->data->tileHeight);
            LOG("spacing : %d margin : %d", tileset->data->spacing, tileset->data->margin);
            tileset = tileset->next;
        }

        // L05: DONE 4: LOG the info for each loaded layer
        ListItem<MapLayerB*>* mapLayer;
        mapLayer = mapData.maplayers.start;

        while (mapLayer != NULL) {
            LOG("id : %d name : %s", mapLayer->data->id, mapLayer->data->name.GetString());
            LOG("Layer width : %d Layer height : %d", mapLayer->data->width, mapLayer->data->height);
            mapLayer = mapLayer->next;
        }
    }

    if(mapFileXML) mapFileXML.reset();

    mapLoaded = ret;

    return ret;
}

// L04: DONE 3: Implement LoadMap to load the map properties
bool Blacksmith::LoadMap(pugi::xml_node mapFile)
{
    bool ret = true;
    pugi::xml_node map = mapFile.child("map");

    if (map == NULL)
    {
        LOG("Error parsing map xml file: Cannot find 'map' tag.");
        ret = false;
    }
    else
    {
        //Load map general properties
        mapData.height = map.attribute("height").as_int();
        mapData.width = map.attribute("width").as_int();
        mapData.tileHeight = map.attribute("tileheight").as_int();
        mapData.tileWidth = map.attribute("tilewidth").as_int();
    }

    return ret;
}

// L04: DONE 4: Implement the LoadTileSet function to load the tileset properties
bool Blacksmith::LoadTileSet(pugi::xml_node mapFile){

    bool ret = true; 

    pugi::xml_node tileset;
    for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
    {
        TileSetB* set = new TileSetB();

        // L04: DONE 4: Load Tileset attributes
        set->name = tileset.attribute("name").as_string();
        set->firstgid = tileset.attribute("firstgid").as_int();
        set->margin = tileset.attribute("margin").as_int();
        set->spacing = tileset.attribute("spacing").as_int();
        set->tileWidth = tileset.attribute("tilewidth").as_int();
        set->tileHeight = tileset.attribute("tileheight").as_int();
        set->columns = tileset.attribute("columns").as_int();
        set->tilecount = tileset.attribute("tilecount").as_int();

        // L04: DONE 4: Load Tileset image
        SString tmp("%s%s", mapFolder.GetString(), tileset.child("image").attribute("source").as_string());
        set->texture = app->tex->Load(tmp.GetString());

        mapData.tilesets.Add(set);
    }

    return ret;
}


// L05: DONE 3: Implement a function that loads a single layer layer
bool Blacksmith::LoadLayer(pugi::xml_node& node, MapLayerB* layer)
{
    bool ret = true;

    //Load the attributes
    layer->id = node.attribute("id").as_int();
    layer->name = node.attribute("name").as_string();
    layer->width = node.attribute("width").as_int();
    layer->height = node.attribute("height").as_int();

    //L06: DONE 6 Call Load Propoerties
    LoadProperties(node, layer->properties);

    //Reserve the memory for the data 
    layer->data = new uint[layer->width * layer->height];
    memset(layer->data, 0, layer->width * layer->height);

    //Iterate over all the tiles and assign the values
    pugi::xml_node tile;
    int i = 0;
    for (tile = node.child("data").child("tile"); tile && ret; tile = tile.next_sibling("tile"))
    {
        layer->data[i] = tile.attribute("gid").as_int();
        /*if (layer->name == "Navigation") {
            LOG(" i: %d", i);
            LOG("data: %u", layer->data[i]);
        }*/
        i++;
    }

    return ret;
}

// L05: DONE 4: Iterate all layers and load each of them
bool Blacksmith::LoadAllLayers(pugi::xml_node mapNode) {
    bool ret = true;

    for (pugi::xml_node imageNode = mapNode.child("imagelayer"); imageNode && ret; imageNode = imageNode.next_sibling("imagelayer")) {

        ImageLayerB* imageLayer = new ImageLayerB();

        imageLayer->name = imageNode.attribute("name").as_string();
        imageLayer->texturePath = (const char*)imageNode.child("image").attribute("source").as_string();
        imageLayer->parallaxFactor = imageNode.attribute("parallaxx").as_float();
        if (imageLayer->parallaxFactor == NULL) {
            imageLayer->parallaxFactor = 1.0f;
        }

        mapData.imagelayers.Add(imageLayer);

        imageLayer->bgtexture = app->tex->Load(imageLayer->texturePath); 

        LOG("%s", imageLayer->texturePath);

    }

    for (pugi::xml_node layerNode = mapNode.child("layer"); layerNode && ret; layerNode = layerNode.next_sibling("layer"))
    {
        //Load the layer
        MapLayerB* mapLayer = new MapLayerB();
        ret = LoadLayer(layerNode, mapLayer);

        //add the layer to the map
        mapData.maplayers.Add(mapLayer);
    }

    return ret;
}

// L06: DONE 6: Load a group of properties from a node and fill a list with it
bool Blacksmith::LoadProperties(pugi::xml_node& node, PropertiesB& properties)
{
    bool ret = false;

    for (pugi::xml_node propertieNode = node.child("properties").child("property"); propertieNode; propertieNode = propertieNode.next_sibling("property"))
    {
        PropertiesB::Property* p = new PropertiesB::Property();
        p->name = propertieNode.attribute("name").as_string();
        p->value = propertieNode.attribute("value").as_bool(); // (!!) I'm assuming that all values are bool !!

        properties.list.Add(p);
    }

    return ret;
}


// L06: DONE 7: Ask for the value of a custom property
PropertiesB::Property* PropertiesB::GetProperty(const char* name)
{
    ListItem<Property*>* item = list.start;
    Property* p = NULL;

    while (item)
    {
        if (item->data->name == name) {
            p = item->data;
            break;
        }
        item = item->next;
    }

    return p;
}


bool Blacksmith::LoadColliders(pugi::xml_node& node) {

    bool ret = true;
    
    for (pugi::xml_node colLayerNode = node.child("objectgroup"); colLayerNode; colLayerNode = colLayerNode.next_sibling("objectgroup")) {
        ColTypesB ct = (ColTypesB)colLayerNode.child("properties").child("property").attribute("value").as_int();
        for (pugi::xml_node colNode = colLayerNode.child("object"); colNode; colNode = colNode.next_sibling("object")) {

            ColDataB col;

            col.x = colNode.attribute("x").as_int();
            col.y = colNode.attribute("y").as_int();
            col.width = colNode.attribute("width").as_int()-1;
            col.height = colNode.attribute("height").as_int()-1;
            col.type = ct;

            CreateColliders(col);

        }
    }
   

    return ret;
}

void Blacksmith::CreateColliders(ColDataB c) {

    PhysBody* collider1;

    if (c.type == ColTypesB::FLOORB) {
        collider1 = app->physics->CreateRectangleSensor((c.x + c.width / 2)+2, (c.y + c.height / 2)-800, c.width, c.height, bodyType::STATIC);
    }
    else if (c.type == 4) {
        collider1 = app->physics->CreateRectangleSensor((c.x + c.width / 2)+2, (c.y + c.height / 2)-800, c.width, c.height, bodyType::STATIC);
    }
    else if (c.type == 235) {
        collider1 = app->physics->CreateCircle((c.x + c.width / 2) + 2, (c.y + c.height / 2), c.width / 2, bodyType::STATIC);
    }
    else {
        collider1 = app->physics->CreateRectangle((c.x + c.width / 2)+2, (c.y + c.height / 2)-800, c.width, c.height, bodyType::STATIC);
    }

    if (c.type == 0) {
        collider1->ctype = ColliderType::PLATFORM;
    }
    else if (c.type == 1) {
        collider1->ctype = ColliderType::SPIKES; 
    }
    else if (c.type == 2) {
        collider1->ctype = ColliderType::FLOOR;
    }
    else if (c.type == 3) {
        collider1->ctype = ColliderType::WALL;
    }
    else if (c.type == 8) {
        collider1->ctype = ColliderType::TAVERN;
    }
    else if (c.type == 9) {
        collider1->ctype = ColliderType::TOWNTAVERN;
    }
    else if (c.type == 10) {
        collider1->ctype = ColliderType::BLACKSMITH;
    }
    else if (c.type == 11) {
        collider1->ctype = ColliderType::TOWNBLACKSMITH;
    }
    else if (c.type == 12) {
        collider1->ctype = ColliderType::HOUSE;
    }
    else if (c.type == 13) {
        collider1->ctype = ColliderType::TOWNHOUSE;
    }
    else if (c.type == 14) {
        collider1->ctype = ColliderType::LAKEDUNGEON;
    }
    else if (c.type == 15) {
        collider1->ctype = ColliderType::TOWNLAKEDUNGEON;
    }
    else if (c.type == 16) {
        collider1->ctype = ColliderType::FORESTDUNGEON;
    }
    else if (c.type == 17) {
        collider1->ctype = ColliderType::TOWNFORESTDUNGEON;
    }
    else if (c.type == 18) {
        collider1->ctype = ColliderType::CAVEDUNGEON;
    }
    else if (c.type == 19) {
        collider1->ctype = ColliderType::TOWNCAVEDUNGEON;
    }
    else if (c.type == 23) {
        collider1->ctype = ColliderType::NPC2;
    }
}

//TEMPORARY PATHFINDING -----------------------------------------------------------------------------------------------------------------

void Blacksmith::ResetPath()
{
    frontier.Clear();
    visited.Clear();
    breadcrumbs.Clear();
    path.Clear();

    frontier.Push(app->scene->AuxEnemy, 0);
    visited.Add(app->scene->AuxEnemy); 
    breadcrumbs.Add(app->scene->AuxEnemy);
    destination = app->scene->AuxPlayer;

    //initailize the cost matrix
    memset(costSoFar, 0, sizeof(uint) * COST_MAP_SIZE * COST_MAP_SIZE);

}

//Draw the visited nodes
void Blacksmith::DrawPath()
{
    iPoint point;

    // Draw visited
    ListItem<iPoint>* item = visited.start;
    if (app->blacksmith->DrawPathing == true) {

        while (item)
        {
            point = item->data;
            TileSetB* tileset = GetTilesetFromTileId(119);

            SDL_Rect rec = tileset->GetTileRect(119);
            iPoint pos = MapToWorld(point.x, point.y);

            app->render->DrawTexture(tileset->texture, pos.x, pos.y, &rec);

            item = item->next;
        }

        // Draw frontier
        for (uint i = 0; i < frontier.Count(); ++i)
        {
            point = *(frontier.Peek(i));
            TileSetB* tileset = GetTilesetFromTileId(118);

            SDL_Rect rec = tileset->GetTileRect(118);
            iPoint pos = MapToWorld(point.x, point.y);

            app->render->DrawTexture(tileset->texture, pos.x, pos.y, &rec);
        }

        // L09 DONE 4: Draw destination point
        iPoint posDestination = MapToWorld(destination.x, destination.y);
        TileSetB* tileset = GetTilesetFromTileId(118);
        SDL_Rect rec = tileset->GetTileRect(118);
        //app->render->DrawRectangle({ posDestination.x, posDestination.y, 16,16 }, 150, 150, 0, 200);
        app->render->DrawTexture(tileset->texture, posDestination.x, posDestination.y, &rec);

        // Draw path
        for (uint i = 0; i < path.Count(); ++i)
        {
            iPoint pos = MapToWorld(path[i].x, path[i].y);
            app->render->DrawTexture(tileX, pos.x, pos.y);
        }
    }
    iPoint Enemy = MapToWorld(app->scene->AuxEnemy.x, app->scene->AuxEnemy.y);
    iPoint Player = MapToWorld(app->scene->AuxPlayer.x, app->scene->AuxPlayer.y);
    //app->render->DrawLine(Enemy.x,Enemy.y,Player.x,Player.y, 100, 0, 100);
    float ModulVec = sqrt((pow(Player.x - Enemy.x, 2)) + (pow(Player.y - Enemy.y, 2)));
    /*LOG("VEC %f", ModulVec);*/
    if (ModulVec < 16*10) {
        LOG("player in enmey radius");
        app->scene->pathActive = true;
    }


}

int Blacksmith::MovementCost(int x, int y) const
{
    int ret = -1;

    if ((x >= 0) && (x < mapData.width) && (y >= 0) && (y < mapData.height))
    {
        int id = mapData.maplayers.start->next->data->Get(x, y); //TODO - Look for the navigation layer iteratinng all layers

        if (id == 25) ret = 10;
        else ret = 1;
    }

    return ret;
}

bool Blacksmith::IsWalkable(int x, int y) const
{
    bool isWalkable = false;

    // L09: DONE 3: return true only if x and y are within map limits
    // and the tile is walkable (tile id 0 in the navigation layer)

    ListItem<MapLayerB*>* mapLayerItem;
    mapLayerItem = mapData.maplayers.start;
    MapLayerB* navigationLayer = mapLayerItem->data;

    //Search the layer in the map that contains information for navigation
    while (mapLayerItem != NULL) {

        if (mapLayerItem->data->properties.GetProperty("Navigation") != NULL && mapLayerItem->data->properties.GetProperty("Navigation")->value) {
            navigationLayer = mapLayerItem->data;
        }

        mapLayerItem = mapLayerItem->next;
    }

    //Set isWalkable to true if the position is inside map limits and is a position that is not blocked in the navigation layer
    if (x >= 0 && y >= 0 && x < mapData.width && y < mapData.height && navigationLayer->Get(x, y) != NULL) {
        isWalkable = true;
    }

    return isWalkable;
}

void Blacksmith::ComputePath(int x, int y)
{
    path.Clear();
    iPoint goal = iPoint(x, y);

    // L10: DONE 2: Follow the breadcrumps to goal back to the origin
    // add each step into "path" dyn array (it will then draw automatically)

    path.PushBack(goal);
    int index = visited.Find(goal);

    while ((index >= 0) && (goal != breadcrumbs[index]))
    {
        goal = breadcrumbs[index];
        path.PushBack(goal);
        index = visited.Find(goal);
    }

}

void Blacksmith::PropagateDijkstra()
{
    // L10: DONE 3: Taking BFS as a reference, implement the Dijkstra algorithm
    // use the 2 dimensional array "costSoFar" to track the accumulated costs
    // on each cell (is already reset to 0 automatically)
    iPoint currentTile;
    bool foundDestination = false;

    if (frontier.Count() > 0) {
        iPoint frontierPoint = *(frontier.Peek(0));
        if (frontierPoint == destination) {
            foundDestination = true;
            DestinationFound = true;
            ComputePath(destination.x, destination.y);
        }
    }

    if (!foundDestination && frontier.Pop(currentTile))
    {

        List<iPoint> neighbors;
        if (IsWalkable(currentTile.x + 1, currentTile.y)) {
            iPoint p;
            neighbors.Add(p.Create(currentTile.x + 1, currentTile.y));
        }
        if (IsWalkable(currentTile.x, currentTile.y + 1)) {
            iPoint p;
            neighbors.Add(p.Create(currentTile.x, currentTile.y + 1));
        }
        if (IsWalkable(currentTile.x - 1, currentTile.y)) {
            iPoint p;
            neighbors.Add(p.Create(currentTile.x - 1, currentTile.y));
        }
        if (IsWalkable(currentTile.x, currentTile.y - 1)) {
            iPoint p;
            neighbors.Add(p.Create(currentTile.x, currentTile.y - 1));
        }

        ListItem<iPoint>* item = neighbors.start;

        while (item != NULL)
        {
            int cost = MovementCost(item->data.x, item->data.y);

            if (cost >= 0)
            {
                cost += costSoFar[currentTile.x][currentTile.y];
                if (visited.Find(item->data) == -1 || cost < costSoFar[item->data.x][item->data.y])
                {
                    costSoFar[item->data.x][item->data.y] = cost;
                    frontier.Push(item->data, cost);
                    visited.Add(item->data);
                    breadcrumbs.Add(currentTile);
                }
            }
            item = item->next;
        }

    }
}

