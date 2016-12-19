#ifndef ODOR_H
#define ODOR_H
/*
Smell:

Smell is a 3D map the size of worldCells of struct Smell which holds a few
int variables representing a particular Chemical intensity. Chemicals disappear at
"chemicalDecayRate".
A separate pointer list of the active smellCells is used to Update, Add and Remove
active cells.


*/
///#define MAX_ODOR_VALUE 50000
#define NUM_ODOR_COMPONENTS 4 /// Must be at least 3 to draw.

struct Odor{
    int data[NUM_ODOR_COMPONENTS];
    int ref; /// Used to check if in worldOdors update array. if -1, not present.
    int x, y, z; /// Used only to spread odors.
};
typedef struct Odor Odor;

enum ODORS{ODOR_NONE, ODOR_MUSTY, ODOR_BODY, NUM_ODORS};

Odor odorTypes[NUM_ODORS];

void setOdorType(int ref, char data[NUM_ODOR_COMPONENTS]){
    for(int i=0; i<NUM_ODOR_COMPONENTS; ++i){
        odorTypes[ref].data[i] = data[i];
    }
}

void initOdorTypes(){
    int data0[NUM_ODOR_COMPONENTS] = {
        0, 0, 0, 0,
    };
    setOdorType(ODOR_NONE, data0);

    int data1[NUM_ODOR_COMPONENTS] = {
        30, 0, 10, 0,
    };
    setOdorType(ODOR_MUSTY, data1);
    //20,20,30,20
    odorTypes[ODOR_BODY].data[0] =4000;
    odorTypes[ODOR_BODY].data[1] =4000;
    odorTypes[ODOR_BODY].data[2] =2000;
    odorTypes[ODOR_BODY].data[3] =2000;
}

Odor odorMap[WORLD_SIZE_X][WORLD_SIZE_Y][WORLD_SIZE_Z];

void initOdorMap(){
    for(int x=0; x<WORLD_SIZE_X; ++x){
        for(int y=0; y<WORLD_SIZE_Y; ++y){
            for(int z=0; z<WORLD_SIZE_Z; ++z){
                /// Not sure if this works.
                odorMap[x][y][z].ref = -1;
                odorMap[x][y][z].x = x;
                odorMap[x][y][z].y = y;
                odorMap[x][y][z].z = z;
                for(int i=0; i<NUM_ODOR_COMPONENTS; ++i)
                    odorMap[x][y][z].data[i] = odorTypes[ODOR_NONE].data[i];
                /*
                memcpy( odorMap[x][y][z].data,
                       odorTypes[ODOR_NONE].data,
                       sizeof( odorTypes[ODOR_NONE].data ) );*/
            }
        }
    }
}

#define MAX_ODORS 10000
Odor* worldOdors[MAX_ODORS];
int numOdors = 0;

void initWorldOdors(){
    for(int i=0; i<MAX_ODORS; ++i){
        worldOdors[i] = NULL;
    }
}

void InitOdor(){ /// Capital name means called from outside header.
    initOdorTypes();
    initOdorMap();
    initWorldOdors();
}

void removeOdor(int ref){
    if(ref == numOdors-1){ // last odor
        worldOdors[ref]->ref = -1;
        --numOdors;
        return;
    }
    // notify cell is no longer in array
    worldOdors[ref]->ref = -1;
    // move last cell to this location
    worldOdors[ref] = worldOdors[--numOdors];
    // update array ref
    worldOdors[ref]->ref = ref;
}

bool leakOdor(Odor* odor, int x, int y, int z){ /// Used to add an odor.
    if(numOdors >= MAX_ODORS-1)
        return false;
    Odor* oCell = &odorMap[x][y][z];
    /// Leak this odor into the cell
    for(int i=0; i<NUM_ODOR_COMPONENTS; ++i){ /// ToDo: Check if will be larger than MAX_INT
        oCell->data[i] += odor->data[i];
        if ( oCell->data[i] < 0) // if added outside bounds of value range.
            oCell->data[i] = INT_MAX;
    }
    /// Add this cell to worldOdors, if it's not already inside.
    if(oCell->ref == -1){
        oCell->ref = numOdors;
        worldOdors[numOdors++] = oCell;
    }
}

Odor tempOdor = { .data = { 0, 0, 0, 0}, .ref = 0 }; /// Used to spread, (UpdateOdor, spreadOdor)
float ODOR_SPREAD_PERCENT = 0.5f;

bool odorHasSomeLesserData(int x, int y, int z){
    if( isCellImpassable(x,y,z) )
        return false;

    Odor *lesserOdor = &odorMap[x][y][z];

    for(int i=0; i<NUM_ODOR_COMPONENTS; ++i){
        if( lesserOdor->data[i] < tempOdor.data[i] )
            return true;
    }
    return false; /// Does not have any lesser components.
}

void spreadOdor(Odor *odor){
    int x, y, z;
    x = odor->x;
    y = odor->y;
    z = odor->z;
    bool left, right, top, bottom;
    left = right = top = bottom = false;
    /// Left
    if( x > 0 && odorHasSomeLesserData( x-1, y, z )){
        //leakOdor(&tempOdor, x-1, y, z);
        left = true;
    }
    /// Right
    if( x < WORLD_SIZE_X-1 && odorHasSomeLesserData( x+1, y, z )){
        //leakOdor(&tempOdor, x+1, y, z);
        right = true;
    }
    /// Top
    if( z > 0 && odorHasSomeLesserData( x, y, z-1 )){
        //leakOdor(&tempOdor, x, y, z-1);
        top = true;
    }
    /// Bottom
    if( z < WORLD_SIZE_Z-1 && odorHasSomeLesserData( x, y, z+1 )){
        //leakOdor(&tempOdor, x, y, z+1);
        bottom = true;
    }

    int totalAffected = (int)left + (int)right + (int)top + (int)bottom;
    if(totalAffected > 0){
        /// Subtract the odor amount we leak into another cell.
        /// AND Divide tempOdor by amount of affected cells
        for(int i=0; i<NUM_ODOR_COMPONENTS; ++i){
            odor->data[i] -= tempOdor.data[i];
            if(odor->data[i] < 0)
                odor->data[i] = 0;
            tempOdor.data[i] /= totalAffected;
        }

        if(left) leakOdor(&tempOdor, x-1, y, z);
        if(right) leakOdor(&tempOdor, x+1, y, z);
        if(top) leakOdor(&tempOdor, x, y, z-1);
        if(bottom) leakOdor(&tempOdor, x, y, z+1);
    }
}

void UpdateOdor(){
    /// For all odors, if >=0 decrease values by ceiling(1%), else remove
    Odor* odor;
    bool isEmpty;
    int i=0;
    int leakAmount=0;
    bool canSpread;
    int x, y, z;
    while( i<numOdors ){
        odor = worldOdors[i];
        isEmpty = true;
        canSpread = false;
        for(int j=0; j<NUM_ODOR_COMPONENTS; ++j){
            if(odor->data[j] >0){
                isEmpty = false;
                odor->data[j] -= ceil(odor->data[j]*0.02); /// never subtract less than 1.

                /// ---- Try to spread to other cells. Get Data.
                leakAmount = odor->data[j] * ODOR_SPREAD_PERCENT;
                if( leakAmount > 0 ){
                    canSpread = true;
                    //odor->data[j] -= leakAmount; // take out how much we leak
                    tempOdor.data[j] = leakAmount;
                }
                else{
                    tempOdor.data[j] = 0; // so we don't have old values, in case this odor spreads.
                }
            }
        }
        if(isEmpty)
            removeOdor(i);
        else
            ++i;
        /// ---- Try to spread to other cells.
        if(canSpread){
            /// Check nearby cells, if one is empty, spread there.
            spreadOdor(odor);
        }
    }
}

void DrawOdor(){
    Odor* odor;
    static int startX, startZ;
    startX = characterX - ADJUSTED_S_WIDTH/2;
    startZ = characterZ - ADJUSTED_S_HEIGHT/2;
    static int y;
    y = characterY;
    static int r, g, b, brightness;

    for (int x=startX; x<startX+ADJUSTED_S_WIDTH; ++x) {
        if( x<0 || x >= WORLD_SIZE_X )
            continue;

        for (int z=startZ; z<startZ+ADJUSTED_S_HEIGHT; ++z) {
            if( z<0 || z >= WORLD_SIZE_Z )
                continue;


            // Draw.
            odor = &odorMap[x][y][z];
            r = g = b = 0;
            if(odor->data[0] > 0){
                r = odor->data[0]*0.2;
                if(r > 155) r=155;
            }
            if(odor->data[1] > 0){
                g = odor->data[1]*0.2;
                if(g > 155) g=155;
            }
            if(odor->data[2] > 0){
                b = odor->data[2]*0.2;
                if(b > 155) b=155;
            }

            brightness = (r+g+b)*0.2;
            if(brightness > 100)
                brightness = 100;

            al_draw_filled_rectangle((x-startX)*ADJUSTED_TILE_SIZE, (z-startZ)*ADJUSTED_TILE_SIZE,
                                     (x-startX)*ADJUSTED_TILE_SIZE+ADJUSTED_TILE_SIZE, (z-startZ)*ADJUSTED_TILE_SIZE+ADJUSTED_TILE_SIZE,
                                     al_map_rgba(r,g,b, brightness ));

        }
    }
}

#endif // ODOR_H

