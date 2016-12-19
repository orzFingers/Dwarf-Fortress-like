#ifndef SOUND_H
#define SOUND_H
/*
Sound is saved on the "soundMap[][][]" which is a simplified version of the worldCells
map (probably 1/10th of the x,z axes).
Sounds move in (x,z) directions only.
Sounds are read and written using the functions in this header.

Technical:
Sound Fill:
-Sounds fill cells in a box each successive frame.
-Each sound corner (4 corners) has a volume, eaten away by wall/cell density.
-When sound corner volume hits 0, it no longer fills that corner.
-Each frame, a cell's sound pointers are overwritten, and "numSounds" is increased by 1.
-"getSound(x,y,z, Sound* container)" returns total sound for all sounds present in cell.
-SoundType is the actual sound data, and Sound is just the corner volumes with the sound type pointer.
-Sound volume corners are not actually corners. They're the top,bottom,left,right.
*/


#define SMAP_RATIO 1 /// 1/10th size of WORLD_SIZE defines
#define SMAP_SIZE_X  WORLD_SIZE_X / SMAP_RATIO
#define SMAP_SIZE_Y  WORLD_SIZE_Y / 1
#define SMAP_SIZE_Z  WORLD_SIZE_Z / SMAP_RATIO

#define NUM_SOUND_FREQUENCIES 4
#define NUM_SOUND_FRAMES 4

struct SoundType{
    char data[NUM_SOUND_FRAMES][NUM_SOUND_FREQUENCIES];
    ALLEGRO_SAMPLE *sample;
};
typedef struct SoundType SoundType;

enum SOUNDS{SOUND_NONE, SOUND_RAINDROP, SOUND_DOOR_OPEN, SOUND_DOOR_CLOSE, SOUND_SHOWER, NUM_SOUNDS};
SoundType soundTypes[NUM_SOUNDS];

void setSoundType(int ref, char data[NUM_SOUND_FRAMES][NUM_SOUND_FREQUENCIES], ALLEGRO_SAMPLE* sample){
    soundTypes[ref].sample = sample;

    for(int i=0; i<NUM_SOUND_FRAMES; ++i){

        for(int j=0; j<NUM_SOUND_FREQUENCIES; ++j){

            soundTypes[ref].data[i][j] = data[i][j];
        }
    }
}

void initSoundTypes(){
    char data0[NUM_SOUND_FRAMES][NUM_SOUND_FREQUENCIES] = {
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
    };
    setSoundType(SOUND_NONE, data0, SND_Door_Close);

    char data1[NUM_SOUND_FRAMES][NUM_SOUND_FREQUENCIES] = {
        { 1, 69, 70, 68 },
        { 3, 1, 7, 67 },
        { 0, 9, 1, 0 },
        { 0, 65,76, 1 },
    };
    setSoundType(SOUND_RAINDROP, data1, SND_Door_Close);

    char data2[NUM_SOUND_FRAMES][NUM_SOUND_FREQUENCIES] = {
        { 2, 0, 0, 0 },
        { 0, 2, 0, 0 },
        { 0, 0, 2, 0 },
        { 0, 0, 0, 2 },
    };
    setSoundType(SOUND_DOOR_OPEN, data2, SND_Door_Open);

    char data3[NUM_SOUND_FRAMES][NUM_SOUND_FREQUENCIES] = {
        { 3, 0, 0, 0 },
        { 0, 3, 0, 0 },
        { 0, 0, 3, 0 },
        { 0, 0, 0, 3 },
    };
    setSoundType(SOUND_DOOR_CLOSE, data3, SND_Door_Close);

    char data4[NUM_SOUND_FRAMES][NUM_SOUND_FREQUENCIES] = {
        { 4, 0, 0, 0 },
        { 0, 4, 0, 0 },
        { 0, 0, 4, 0 },
        { 0, 0, 0, 4 },
    };
    setSoundType(SOUND_SHOWER, data4, NULL); /// Shower sound is not called like this, because it loops. So here, it's null.


/*// Possible with C99?
    soundTypes[SOUND_NONE].data = (char [NUM_SOUND_FRAMES][NUM_SOUND_FREQUENCIES]) {
        { 1, 69, 70, 68 },
        { 3, 1, 7, 67 },
        { 0, 9, 1, 0 },
        { 0, 65,76, 1 },
    };
*/
}

enum SOUND_WALL_VOLUMES{SW_TOP, SW_RIGHT, SW_BOTTOM, SW_LEFT, NUM_SW};
struct Sound{
    int x, y, z;
    SoundType* type;
    int volume[NUM_SW];
    int frame;
    int ref;
};
typedef struct Sound Sound;

#define MAX_SOUND_VOLUME 3
#define MAX_SOUND_FRAMES 50
#define MAX_SOUNDS 10000
Sound* worldSounds[MAX_SOUNDS];
int numSounds=0;

void initWorldSounds(){
    Sound* sound;
    for(int i=0; i<MAX_SOUNDS; ++i){
        sound = malloc( sizeof(Sound) );
        sound->type = SOUND_NONE;

        for(int j=0; j<NUM_SW; ++j)
            sound->volume[j] = 0;

        sound->frame = 0;

        worldSounds[i] = sound;
    }
}

void removeSound(int ref){
    if(ref == numSounds-1){ // last sound
        --numSounds;
        return;
    }

    Sound* temp;
    temp = worldSounds[ref];
    worldSounds[ref] = worldSounds[--numSounds];
    worldSounds[numSounds] = temp;
    worldSounds[ref]->ref = ref;
}

bool addSoundToWorldSounds(SoundType* type, int x, int y, int z){ /// Could be called makeSound()
    if(numSounds >= MAX_SOUNDS-1)
        return false;

    worldSounds[numSounds]->x = x;
    worldSounds[numSounds]->y = y;
    worldSounds[numSounds]->z = z;
    worldSounds[numSounds]->type = type;
    worldSounds[numSounds]->frame = 0;//frameNum;
    worldSounds[numSounds]->ref = numSounds;
    for(int i=0; i<NUM_SW; ++i)
         worldSounds[numSounds]->volume[i] = MAX_SOUND_VOLUME;

    ++numSounds;
    return true;
}

bool addSoundToWorldSounds_withUserSound(SoundType* type, int x, int y, int z){ /// Could be called makeSound()
    if(type->sample != NULL)
        al_play_sample(type->sample, SFXVolume, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);

    addSoundToWorldSounds(type, x, y, z);
}

#define MAX_CELL_SOUNDS 10 // could be set low to avoid AI hearing too much sound.
struct SoundCell{
    Sound* sounds[MAX_CELL_SOUNDS];
    int numSounds;
    int density;
    int frame; /// Used to tell which frame this data is from. If old, write over it(setnumSounds0)
};
typedef struct SoundCell SoundCell;

SoundCell soundMap[SMAP_SIZE_X][SMAP_SIZE_Y][SMAP_SIZE_Z];

void updateSoundMapDensity(){
    int density;
    int x2, z2;
    for(int x=0; x < SMAP_SIZE_X; ++x){

        for(int y=0; y < SMAP_SIZE_Y; ++y){

            for(int z=0; z < SMAP_SIZE_Z; ++z){

                /// Gather isWall from worldCells
                x2 = x*SMAP_RATIO;
                z2 = z*SMAP_RATIO;
                density = 0;
                for( int i=0; i<SMAP_RATIO; ++i ){

                    for( int j=0; j<SMAP_RATIO; ++j ){
                        density += TileTypes[
                        worldCells[ x2+i ][ y ][ z2+j ].tileType
                        ].isWall;
                    }
                }

                // Set density using intensively acquired knowledge.
                soundMap[x][y][z].density = density; //10;//rand()%3;
            }
        }
    }
}

void initSoundMap(){
    SoundCell* sndCell;

    for(int x=0; x < SMAP_SIZE_X; ++x){

        for(int y=0; y < SMAP_SIZE_Y; ++y){

            for(int z=0; z < SMAP_SIZE_Z; ++z){

                sndCell = &soundMap[x][y][z];
                sndCell->frame = 0;
                sndCell->numSounds = 0;
                ///sndCell->density = 0;

                for(int i=0; i<MAX_CELL_SOUNDS; ++i) // Initializing may be unnecessary.
                    sndCell->sounds[i] = NULL;
            }
        }
    }
}

void InitSound(){ ///---- Call this to initialize everything. Do it after tiles are generated.
    initSoundTypes();
    initSoundMap();
    updateSoundMapDensity();
    initWorldSounds();
}

int addSoundToSoundMap(Sound* sound, int x, int y, int z){
    if(x < 0 || x >= SMAP_SIZE_X ||
       y < 0 || y >= SMAP_SIZE_Y ||
       z < 0 || z >= SMAP_SIZE_Z )
        return 0;

    SoundCell* sndCell = &soundMap[x][y][z];
    if( sndCell->frame != frameNum ){
        sndCell->frame = frameNum;
        sndCell->numSounds = 0;
    }

    sndCell->sounds[ sndCell->numSounds++ ] = sound;

    return sndCell->density;
}

bool updateSound(Sound* sound){
    /// Add sound to soundMap in a square.
    static int x, y, z, frame;
    x = sound->x;
    y = sound->y;
    z = sound->z;
    frame = sound->frame;
    ++sound->frame;

    bool hasVolume = false;

    //Top
    if(sound->volume[SW_TOP] > 0) {
        for (int i=0; i < frame*2; ++i)
            sound->volume[SW_TOP] -= addSoundToSoundMap(sound, x-frame+i, y, z-frame );
        hasVolume = true;
    }

    //Right
    if(sound->volume[SW_RIGHT] > 0) {
        for (int i=0; i < frame*2; ++i)
            sound->volume[SW_RIGHT] -= addSoundToSoundMap(sound, x+frame, y, z-frame+i);
        hasVolume = true;
    }

    //Bottom
    if(sound->volume[SW_BOTTOM] > 0) {
        for (int i=1; i <= frame*2; ++i)
            sound->volume[SW_BOTTOM] -= addSoundToSoundMap(sound, x-frame+i, y, z+frame );
        hasVolume = true;
    }

    //Left
    if(sound->volume[SW_LEFT] > 0){
        for (int i=1; i <= frame*2; ++i)
            sound->volume[SW_LEFT] -= addSoundToSoundMap(sound, x-frame, y, z-frame+i);
        hasVolume = true;
    }

    if(!hasVolume || frame >= MAX_SOUND_FRAMES){
        removeSound(sound->ref);
        return true; /// removed sound
    }

    return false; /// did not remove sound
}

void UpdateSound(){
    int i=0;
    while(i < numSounds){
        if( !updateSound( worldSounds[i] ) )
            ++i;
    }
}

void getSound(int x, int y, int z, Sound* container){
    /// Get all sounds from sndCell and add or average them, and put them in the container
    ///--- I don't know how to handle this yet. It depends on how the AI works.
    /// Also, sound values may end up being int instead of char.
}

void Sound_tileXZ_to_screenXZ(int* x, int* z){
    *x = *x + ADJUSTED_S_WIDTH/2 - characterX;
    *x = (*x) * ADJUSTED_TILE_SIZE;

    *z = *z + ADJUSTED_S_HEIGHT/2 - characterZ;
    *z = (*z) * ADJUSTED_TILE_SIZE;
}

void Sound_tX_to_sX(int x){
    int cXOffset = characterX * ADJUSTED_TILE_SIZE;
    //x = x
}

void DrawSound(){
    SoundCell* sndCell;
    static int startX, startZ;
    startX = characterX - ADJUSTED_S_WIDTH/2;
    startZ = characterZ - ADJUSTED_S_HEIGHT/2;
    static int y;
    int num;

    for (int x=startX; x<startX+ADJUSTED_S_WIDTH; ++x) {
        if( x<0 || x >= WORLD_SIZE_X )
            continue;

        for (int z=startZ; z<startZ+ADJUSTED_S_HEIGHT; ++z) {
            if( z<0 || z >= WORLD_SIZE_Z )
                continue;

            y = characterY;

            sndCell = &soundMap[x][y][z];
            num = sndCell->numSounds;//abs(sndCell->frame - frameNum) < 50;
            if(abs(sndCell->frame - frameNum) > 1)
                num = 0;

            al_draw_filled_rectangle((x-startX)*ADJUSTED_TILE_SIZE, (z-startZ)*ADJUSTED_TILE_SIZE,
                                     (x-startX)*ADJUSTED_TILE_SIZE+ADJUSTED_TILE_SIZE, (z-startZ)*ADJUSTED_TILE_SIZE+ADJUSTED_TILE_SIZE,
                                     al_map_rgba( num*100, num*100, num*100, num*100 ));

        }
    }
}




#endif // SOUND_H
