#ifndef TILE_H
#define TILE_H

/// TILE----
// Part of the map. Land, soil, structures, etc.
struct Tile{
    int imageNum;
    bool isWall;
};
typedef struct Tile Tile;

enum TILE_TYPES{TILE_NONE, TILE_SOIL, TILE_ROCK, TILE_GRASS, TILE_TALL_GRASS, TILE_WALL_WOOD,
     NUM_TILE_TYPES};
Tile TileTypes[NUM_TILE_TYPES];

void setTileType(int ref, int imageNum, bool isWall){
    TileTypes[ref].imageNum = imageNum;
    TileTypes[ref].isWall = isWall;
}

void initTileTypes(){
    setTileType(TILE_NONE, 0, false);
    setTileType(TILE_SOIL, 1, false);
    setTileType(TILE_ROCK, 4, false);
    setTileType(TILE_GRASS, 5, false);
    setTileType(TILE_TALL_GRASS, 8, false);
    setTileType(TILE_WALL_WOOD, -1+16*35+11, true);
}

/// OBJECT & CELL----
// A movable object, separate from the land or structures (e.g. Character, Furniture, etc).
struct Object{
    int x, y, z;
    int type;
    void* data;
    void (*update)(struct Object*);
    int (*getImage)(struct Object*);
    void (*interact)(struct Object*);
    void (*cleanup)(struct Object*);
    int ref;
};
typedef struct Object Object;

#define MAX_CELL_OCCUPANTS 2
struct Cell{
    int tileType;
    int wallImage; // used for walls
    float wallRotation; // used for walls
    bool isImpassable;
    Object* occupants[MAX_CELL_OCCUPANTS]; // currently unsorted list.
    int numOccupants
};
typedef struct Cell Cell;


#define MAX_WORLD_OBJECTS 100
Object* worldObjects[MAX_WORLD_OBJECTS];
int numWorldObjects = 0;
Cell worldCells[WORLD_SIZE_X][WORLD_SIZE_Y][WORLD_SIZE_Z];

bool isOutsideWorldBounds(int x, int y, int z){
    if( x<0 || x >= WORLD_SIZE_X ||
        y<0 || y >= WORLD_SIZE_Y ||
        z<0 || z >= WORLD_SIZE_Z )
        return true;

    return false;
}

bool isCellImpassable(int x, int y, int z){
    if( x<0 || x >= WORLD_SIZE_X ||
        y<0 || y >= WORLD_SIZE_Y ||
        z<0 || z >= WORLD_SIZE_Z )
        return true;

    if( worldCells[x][y][z].isImpassable ||
        TileTypes[ worldCells[x][y][z].tileType ].isWall )
        return true;

    return false;
}

/*
bool addObjectToCell(Object* obj, int x, int y, int z){
    Cell* cell = &worldCells[x][y][z];
    for (int i=0; i<MAX_CELL_OCCUPANTS; ++i) {
        if(cell->occupants[i]==NULL){
            cell->occupants[i]=obj;
            obj->x = x;
            obj->y = y;
            obj->z = z;

            return true;
        }
    }
    return false; // failed
}
*/

bool addObjectToCell(Object* obj, int x, int y, int z){
    Cell* cell = &worldCells[x][y][z];
    if(cell->numOccupants >= MAX_CELL_OCCUPANTS)
        return false;

    cell->occupants[ cell->numOccupants++ ] = obj;
    obj->x = x;
    obj->y = y;
    obj->z = z;

    return true;
}

Object* createObject(int x, int y, int z, int type, void* data,
                     void (*update)(struct Object*),
                     int (*getImage)(struct Object*),
                     void (*interact)(struct Object*),
                     void (*cleanup)(struct Object*) ){

    Object* obj = malloc( sizeof(Object) );
    obj->x = x;
    obj->y = y;
    obj->z = z;
    obj->type = type;
    obj->data = data;
    obj->update = update;
    obj->getImage = getImage;
    obj->interact = interact;
    obj->cleanup = cleanup;
    obj->ref = numWorldObjects;

    worldObjects[numWorldObjects++] = obj;

    addObjectToCell(obj, x, y, z);

    return obj;
}


/*
bool removeObjectFromCurrentCell(Object* obj){
    Cell* cell = &worldCells[obj->x][obj->y][obj->z];
    for (int i=0; i<MAX_CELL_OCCUPANTS; ++i) {
        if(cell->occupants[i]==obj){
            cell->occupants[i]=NULL;
            return true;
        }
    }
    return false; // failed
}
*/

void removeObjectFromCurrentCell(Object* obj){
    Cell* cell = &worldCells[obj->x][obj->y][obj->z];
    for (int i=0; i < cell->numOccupants; ++i) {
        if(cell->occupants[i]==obj){
            cell->occupants[i] = cell->occupants[ --cell->numOccupants ]; // move last to this one
            cell->occupants[ cell->numOccupants ] = NULL;
        }
    }
}

bool moveObjectToCell(Object* obj, int x, int y, int z){
    if( x<0 || x >= WORLD_SIZE_X ||
        y<0 || y >= WORLD_SIZE_Y ||
        z<0 || z >= WORLD_SIZE_Z ||
        worldCells[x][y][z].numOccupants >= MAX_CELL_OCCUPANTS)
            return false;

    bool isFull = worldCells[x][y][z].tileType != TILE_NONE;
    //TileTypes[ worldCells[x][y][z].tileType ].isWall;
    if( isFull || worldCells[x][y][z].isImpassable )
        return false;


    removeObjectFromCurrentCell(obj);
    addObjectToCell(obj, x, y, z);
    return true;
}

bool deleteObject(Object* obj){ // Here we assume the "data" member is already freed.
    int ref = obj->ref;
    if( worldObjects[ref] != obj ) // could not find object in array.
        return false;

    free(obj);
    --numWorldObjects;
    if(ref == numWorldObjects){ // last object in worldObjects.
        worldObjects[ref] = NULL;
        return true;
    }
    // Move last to this one, and set other value to NULL to be safe.
    worldObjects[ref] = worldObjects[numWorldObjects];
    worldObjects[ref]->ref = ref;
    worldObjects[numWorldObjects] = NULL;
    return true;
}



void initWorldObjects(){
    for (int i=0; i<numWorldObjects; ++i) {
        worldObjects[i] = NULL;
    }
}

void update_worldObjects(){
    Object *obj;
    for (int i=0; i<numWorldObjects; ++i) {
        obj = worldObjects[i];
        obj->update(obj);
    }
};

enum WALL_IMAGES{
    W_D = 4,
    W_L_D = 1,  W_U_D = 2,
    W_L_U_D = 3,
    W_ALL = 0,
    W_NONE = 0//-1+16*35+16
};

const float R_0 = 0;
const float R_90 = M_PI/2.0;
const float R_180 = M_PI;
const float R_270 = M_PI*1.5;

void adjustWallTile(int x, int y, int z, int imageNum){
    int wallType = TileTypes[ worldCells[x][y][z].tileType ].imageNum;
    //-1+16*35+11;//-1+16*22+12;//-1+16*35+11;
    int variation;
    float rotation;
    bool isAbove, isBelow, isLeft, isRight;
    isAbove = isBelow = isLeft = isRight = false;

    if(z > 0)
        isAbove = TileTypes[ worldCells[x][y][z-1].tileType ].isWall;
    if(z < WORLD_SIZE_Z-1)
        isBelow = TileTypes[ worldCells[x][y][z+1].tileType ].isWall;
    if(x > 0)
        isLeft = TileTypes[ worldCells[x-1][y][z].tileType ].isWall;
    if(x < WORLD_SIZE_X-1)
        isRight = TileTypes[ worldCells[x+1][y][z].tileType ].isWall;


    if(isAbove){
        if(isBelow){
            if(isLeft){
                if(isRight){
                    variation = W_ALL;
                    rotation = R_0;
                }
                else{ // Material is Up,Down, and Left
                    variation = W_L_U_D;
                    rotation = R_90;
                }
            }
            else{ // Material is Up,Down but NOT Left
                if(isRight){
                    variation = W_L_U_D;
                    rotation = R_270;
                }
                else{ // Material is Up and Down only.
                    variation = W_U_D;
                    rotation = R_0;
                }
            }
        }
        else{ // Material is Above but NOT Below
            if(isLeft){
                if(isRight){
                    variation = W_L_U_D;
                    rotation = R_180;
                }
                else{ // Above and Left only.
                    variation = W_L_D;
                    rotation = R_180;
                }
            }
            else{ // Material is Above but NOT Below or Left
                if(isRight){
                    variation = W_L_D;
                    rotation = R_270;
                }
                else{ // Material is Above only.
                    variation = W_D;
                    rotation = R_180;
                }
            }
        }
    }
    // -------------------------------------
    else{ // no Material above
        if(isBelow){
            if(isLeft){
                if(isRight){
                    variation = W_L_U_D;
                    rotation = R_0;
                }
                else{ // Material is Down, and Left
                    variation = W_L_D;
                    rotation = R_90;
                }
            }
            else{ // Material is Down but NOT Left or Up
                if(isRight){
                    variation = W_L_D;
                    rotation = R_0;
                }
                else{ // Material is Down only.
                    variation = W_D;
                    rotation = R_0;
                }
            }
        }
        else{ // Material is NOT Above or Below
            if(isLeft){
                if(isRight){
                    variation = W_U_D;
                    rotation = R_90;
                }
                else{ // Left only.
                    variation = W_D;
                    rotation = R_90;
                }
            }
            else{ // Material is NOT Above, Below or Left
                if(isRight){
                    variation = W_D;
                    rotation = R_270;
                }
                else{ // Material is nowhere.
                    variation = W_NONE;
                    rotation = R_0;
                }
            }
        }
    }

    worldCells[x][y][z].wallImage = variation + wallType;
    worldCells[x][y][z].wallRotation = rotation;
}

void setCell(int x, int y, int z, int tileType){
    if(x<0 || x >= WORLD_SIZE_X ||
       y<0 || y >= WORLD_SIZE_Y ||
       z<0 || z >= WORLD_SIZE_Z )
       return;


    worldCells[x][y][z].tileType = tileType;

    // Set this cell.
    if ( TileTypes[tileType].isWall )
        adjustWallTile(x,y,z, TileTypes[tileType].imageNum);

    // Set surrounding cells.
    if(z > 0  &&  TileTypes[ worldCells[x][y][z-1].tileType ].isWall)
        adjustWallTile(x, y, z-1, TileTypes[tileType].imageNum);
    if(z < WORLD_SIZE_Z-1  &&  TileTypes[ worldCells[x][y][z+1].tileType ].isWall)
        adjustWallTile(x, y, z+1, TileTypes[tileType].imageNum);
    if(x > 0  &&  TileTypes[ worldCells[x-1][y][z].tileType ].isWall)
        adjustWallTile(x-1, y, z, TileTypes[tileType].imageNum);
    if(x < WORLD_SIZE_X-1  &&  TileTypes[ worldCells[x+1][y][z].tileType ].isWall)
        adjustWallTile(x+1, y, z, TileTypes[tileType].imageNum);
}

bool interactCell(int x, int y, int z, Object* self){
    if(x<0 || x >= WORLD_SIZE_X ||
       y<0 || y >= WORLD_SIZE_Y ||
       z<0 || z >= WORLD_SIZE_Z )
       return false;

    Object* obj;
    for(int i=0; i<MAX_CELL_OCCUPANTS; ++i){
        obj = worldCells[x][y][z].occupants[i];
        if(obj != NULL && obj != self){
            obj->interact(obj);
            return true;
        }
    }
    return false;
}

void setCellImpassable(int x, int y, int z, bool isImpassable){
    if(x<0 || x >= WORLD_SIZE_X ||
       y<0 || y >= WORLD_SIZE_Y ||
       z<0 || z >= WORLD_SIZE_Z )
       return;

    worldCells[x][y][z].isImpassable = isImpassable;
}
/*
void draw_worldCells2(){
    Object* obj;
    for(int x=0; x<WORLD_SIZE_X; x++){
        for(int z=0; z<WORLD_SIZE_Z; z++){
            obj = worldCells[x][0][z].occupants[0];
        }
    }
}
*/
void draw_worldCells(){
    Object* obj;
    static int sourceX, sourceY, imageNum;
    static float rotation;
    static int startX, startZ;
    startX = characterX - ADJUSTED_S_WIDTH/2;
    startZ = characterZ - ADJUSTED_S_HEIGHT/2;
    static int y;

    for (int x=startX; x<startX+ADJUSTED_S_WIDTH; ++x) {
        if( x<0 || x >= WORLD_SIZE_X )
            continue;

        for (int z=startZ; z<startZ+ADJUSTED_S_HEIGHT; ++z) {
            if( z<0 || z >= WORLD_SIZE_Z )
                continue;

            y = characterY;
            while(y < WORLD_SIZE_Y && worldCells[x][y][z].tileType == TILE_NONE){
                ++y;
            }

            rotation = 0;
            if(y < WORLD_SIZE_Y){
                int tileType = worldCells[x][y][z].tileType;
                imageNum = TileTypes[tileType].imageNum;
                if(TileTypes[tileType].isWall){
                   imageNum = worldCells[x][y][z].wallImage;
                   rotation = worldCells[x][y][z].wallRotation;
                }
            }
            else{
                imageNum = 0; // empty tile.
            }

            sourceX = (imageNum%16)*TILE_SIZE;
            sourceY = (imageNum/16)*TILE_SIZE;

            /*
            al_draw_scaled_bitmap(IMG_TileSheet,
                                  sourceX, sourceY, TILE_SIZE, TILE_SIZE,
                                  (x-startX)*ADJUSTED_TILE_SIZE, (z-startZ)*ADJUSTED_TILE_SIZE,
                                  ADJUSTED_TILE_SIZE, ADJUSTED_TILE_SIZE, 0);
            */
            custom_draw_scaled_rotated_bitmap(IMG_TileSheet,
                                  sourceX, sourceY, TILE_SIZE, TILE_SIZE,
                                  (x-startX)*ADJUSTED_TILE_SIZE, (z-startZ)*ADJUSTED_TILE_SIZE,
                                  ADJUSTED_TILE_SIZE, ADJUSTED_TILE_SIZE, rotation);

            // Draw occupants
            if (y < WORLD_SIZE_Y && y > 0){
                static int objImage;
                static int objSourceX, objSourceY;
                static int objY;
                objY = y-1;

                for (int i=0; i<MAX_CELL_OCCUPANTS; ++i) {

                    obj = worldCells[x][objY][z].occupants[i];
                    if( obj != NULL ){
                        objImage = obj->getImage(obj);
                        objSourceX = (objImage%16)*TILE_SIZE;
                        objSourceY = (objImage/16)*TILE_SIZE;

                        al_draw_scaled_bitmap(IMG_TileSheet,
                                  objSourceX, objSourceY, TILE_SIZE, TILE_SIZE,
                                  (x-startX)*ADJUSTED_TILE_SIZE, (z-startZ)*ADJUSTED_TILE_SIZE,
                                  ADJUSTED_TILE_SIZE, ADJUSTED_TILE_SIZE, 0);

                    }
                }
            }


            // Draw shadow for deep tiles.
            int shadowColor = 0;
            if(y-characterY > 1)
                shadowColor = y-characterY-1;
            ///if(y==characterY)
                ///shadowColor = 10;
            al_draw_filled_rectangle((x-startX)*ADJUSTED_TILE_SIZE, (z-startZ)*ADJUSTED_TILE_SIZE,
                                     (x-startX)*ADJUSTED_TILE_SIZE+ADJUSTED_TILE_SIZE, (z-startZ)*ADJUSTED_TILE_SIZE+ADJUSTED_TILE_SIZE,
                                     al_map_rgba(0,0,20*shadowColor, shadowColor*10 ));

        }
    }
}

void tileXZ_to_screenXZ(int* x, int* z){
    *x = *x + ADJUSTED_S_WIDTH/2 - characterX;
    *x = (*x) * ADJUSTED_TILE_SIZE;

    *z = *z + ADJUSTED_S_HEIGHT/2 - characterZ;
    *z = (*z) * ADJUSTED_TILE_SIZE;
}

void updateMouseTileValues(){
    m.tileX = m.x / ADJUSTED_TILE_SIZE + characterX - ADJUSTED_S_WIDTH/2;
    if(m.tileX < 0)
        m.tileX = 0;
    if(m.tileX >= WORLD_SIZE_X)
        m.tileX = WORLD_SIZE_X-1;

    m.tileZ = m.y / ADJUSTED_TILE_SIZE + characterZ - ADJUSTED_S_HEIGHT/2;
    if(m.tileZ < 0)
        m.tileZ = 0;
    if(m.tileZ >= WORLD_SIZE_Z)
        m.tileZ = WORLD_SIZE_Z-1;
}



void cleanup_worldObjects(){
    Object* obj;
    /// Must be deleted this way because deleting objects moves others up in the array.
    while(worldObjects[0] != NULL){
        obj = worldObjects[0];
        obj->cleanup(obj);
    }
}



#endif
