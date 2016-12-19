#ifndef OBJECTS
#define OBJECTS
///-------------CHARACTER

struct CharacterData{
    int image;
    int xSpeed, ySpeed, zSpeed;
    int orientation;
};
typedef struct CharacterData CharacterData;


void Character_update(Object* obj){
    CharacterData* data = ((CharacterData*)obj->data);

    // Update Orientation
    if(data->xSpeed !=0){
        if(data->xSpeed < 0)
            data->orientation = EAST;
        else
            data->orientation = WEST;
    }
    else if(data->zSpeed !=0){
        if(data->zSpeed < 0)
            data->orientation = NORTH;
        else
            data->orientation = SOUTH;
    }

    // Move Character
    if(data->xSpeed !=0 || data->ySpeed != 0 || data->zSpeed != 0){
        moveObjectToCell(obj, obj->x + data->xSpeed, obj->y + data->ySpeed,
                         obj->z + data->zSpeed);
        // Leak body odor where I'm standing
        leakOdor(&odorTypes[ODOR_BODY], obj->x, obj->y, obj->z);
    }




}

void Character_move(Object* obj, int x, int y, int z){
    CharacterData* data = (CharacterData*)obj->data;
    data->xSpeed = getPN0(x);
    data->ySpeed = getPN0(y);
    data->zSpeed = getPN0(z);
}

int Character_getImage(Object* obj){
    return ((CharacterData*) obj->data)->image;
}

void Character_interact(Object* obj){
    //Do something.
}

void Character_cleanup(Object* obj){
    free(obj->data);
    deleteObject(obj);
}


Object* createCharacter(int x, int y, int z){

    /// Create Data
    CharacterData* data = malloc( sizeof(CharacterData) );
    data->image = IMAGE_CHARACTER;
    data->xSpeed = data->ySpeed = data->zSpeed = 0;
    data->orientation = NORTH; // doesn't matter.

    /// Create and return Object
    return createObject(x, y, z, OBJECT_CHARACTER, data,
                              Character_update,
                              Character_getImage,
                              Character_interact,
                              Character_cleanup);
}

Object* createRandomCharacter(){
    return createCharacter( rand() % WORLD_SIZE_X,
                           rand() % WORLD_SIZE_Y,
                           rand() % WORLD_SIZE_Z );
}

void Character_save(Object* obj){
    /// Nothin' ter' do~
}

void Character_load(int x, int y, int z){
    Object* obj = createCharacter(x, y, z);
}

///-------------DOOR

struct DoorData{
    bool isOpen;
};
typedef struct DoorData DoorData;


void Door_update(Object* obj){
}

void Door_move(Object* obj, int x, int y, int z){
}

int Door_getImage(Object* obj){
    if( ((DoorData*) obj->data)->isOpen )
        return IMAGE_DOORWAY;
    else
        return IMAGE_DOOR;
}

void Door_interact(Object* obj){
    //Do something.
    DoorData* data = (DoorData*) obj->data;
    data->isOpen = !data->isOpen;
    setCellImpassable(obj->x, obj->y, obj->z, !data->isOpen);

    if(data->isOpen)
        addSoundToWorldSounds_withUserSound(&soundTypes[SOUND_DOOR_OPEN], obj->x, obj->y, obj->z);
    else
        addSoundToWorldSounds_withUserSound(&soundTypes[SOUND_DOOR_CLOSE], obj->x, obj->y, obj->z);

}

void Door_cleanup(Object* obj){
    free(obj->data);
    deleteObject(obj);
}

Object* createDoor(int x, int y, int z){

    /// Create Data
    DoorData* data = malloc( sizeof(DoorData) );
    data->isOpen = true;

    /// Create and return Object
    return createObject(x, y, z, OBJECT_DOOR, data,
                              Door_update,
                              Door_getImage,
                              Door_interact,
                              Door_cleanup);
}

void Door_save(Object* obj){
    DoorData* data = (DoorData*) obj->data;
    fprintf(file, "%d\n", (int)data->isOpen);
}

void Door_load(int x, int y, int z){
    Object* obj = createDoor(x, y, z);
    /// Load and set Data variables
    int temp; // temp is used because bool value is saved as int.
    fscanf(file, "%d\n", &temp);
    ((DoorData*) obj->data)->isOpen = (bool)temp;
}

///-------------SHOWER

struct ShowerData{
    bool isOn;
    ALLEGRO_SAMPLE_ID *sampleID;
};
typedef struct ShowerData ShowerData;


void Shower_update(Object* obj){
}

void Shower_move(Object* obj, int x, int y, int z){
}

int Shower_getImage(Object* obj){
    return IMAGE_SHOWER;
}

void Shower_interact(Object* obj){
    //Do something.
    ShowerData* data = (ShowerData*) obj->data;
    data->isOn = !data->isOn;

    if(data->isOn){
        addSoundToWorldSounds(&soundTypes[SOUND_DOOR_OPEN], obj->x, obj->y, obj->z);
        al_play_sample(SND_Shower, SFXVolume, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,data->sampleID);
    }
    else
        al_stop_sample(data->sampleID);

}

void Shower_cleanup(Object* obj){
    free( ((ShowerData*) obj->data)->sampleID );
    free(obj->data);
    deleteObject(obj);
}


Object* createShower(int x, int y, int z){

    /// Create Data
    ShowerData* data = malloc( sizeof(ShowerData) );
    data->isOn = false;
    data->sampleID = malloc(sizeof(ALLEGRO_SAMPLE_ID));

    /// Create and return Object
    return createObject(x, y, z, OBJECT_SHOWER, data,
                              Shower_update,
                              Shower_getImage,
                              Shower_interact,
                              Shower_cleanup);
}

void Shower_save(Object* obj){
    ShowerData* data = (ShowerData*) obj->data;
    fprintf(file, "%d\n", (int)data->isOn);
}

void Shower_load(int x, int y, int z){
    Object* obj = createShower(x, y, z);
    /// Load and set Data variables
    int temp; // temp is used because bool value is saved as int.
    fscanf(file, "%d\n", &temp);
    ((ShowerData*) obj->data)->isOn = (bool)temp;
}



///------------ Object Save/Load Functions.
//void (*update)(struct Object*)
/// Don't call these functions outside saveObject() and loadObject()
void (*saveObjectType[NUM_OBJECT_TYPES])(Object*);
void (*loadObjectType[NUM_OBJECT_TYPES])(int x, int y, int z);



//enum OBJECT_TYPES {OBJECT_NONE, OBJECT_CHARACTER, OBJECT_DOOR, OBJECT_SHOWER, NUM_OBJECT_TYPES};//,  NUM_OBJECT_TYPES};
void initSaveLoadObjectArrays(){
    saveObjectType[OBJECT_NONE] = NULL;
    loadObjectType[OBJECT_NONE] = NULL;

    saveObjectType[OBJECT_CHARACTER] = Character_save;
    loadObjectType[OBJECT_CHARACTER] = Character_load;

    saveObjectType[OBJECT_DOOR] = Door_save;
    loadObjectType[OBJECT_DOOR] = Door_load;

    saveObjectType[OBJECT_SHOWER] = Shower_save;
    loadObjectType[OBJECT_SHOWER] = Shower_load;
}



void saveObject(Object* obj){
    /// Save general Object variables.
    fprintf(file, "%d\n", obj->x);
    fprintf(file, "%d\n", obj->y);
    fprintf(file, "%d\n", obj->z);
    fprintf(file, "%d\n", obj->type);

    /// ToDo: Call type-specific saving (save data)
    saveObjectType[ obj->type ]( obj );
}

void loadObject(){
    int x, y, z, type;

    /// Get general Object variables.
    fscanf(file, "%d\n", &x);
    fscanf(file, "%d\n", &y);
    fscanf(file, "%d\n", &z);
    fscanf(file, "%d\n", &type);

    /// ToDo: Call type-specific loading (set function pointers, load data)
    loadObjectType[ type ](x, y, z);
}

#endif // OBJECTS

