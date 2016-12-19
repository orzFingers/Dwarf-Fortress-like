
#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

///FILE* file;  // This is in constants, because it needs to be accessed by objectSave/Load functions.

void File_saveTiles(){
    for (int x=0; x<WORLD_SIZE_X; ++x) {

        for (int y=0; y<WORLD_SIZE_Y; ++y) {

            for (int z=0; z<WORLD_SIZE_Z; ++z) {

                fprintf(file, "%d\n", worldCells[x][y][z].tileType);

            }
        }
    }
}
void File_loadTiles(){
    int type=0;

    for (int x=0; x<WORLD_SIZE_X; ++x) {

        for (int y=0; y<WORLD_SIZE_Y; ++y) {

            for (int z=0; z<WORLD_SIZE_Z; ++z) {

                /// Reset worldCell values to zero. Technically part of the "destroy world", but faster if done at same time.
                worldCells[x][y][z].isImpassable = false;
                worldCells[x][y][z].numOccupants = 0;
                for(int i=0; i<MAX_CELL_OCCUPANTS; ++i)
                    worldCells[x][y][z].occupants[i] = NULL;

                /// Read in and set cell type.
                fscanf(file, "%d\n", &type);
                setCell(x, y, z, type);

            }
        }
    }
}

void File_saveObjects(){
    fprintf(file, "%d\n", numWorldObjects);

    for (int i=0; i<numWorldObjects; ++i) {
        saveObject( worldObjects[i] );
    }
}

void File_loadObjects(){
    int numObjects;
    fscanf(file, "%d\n", &numObjects);

    for (int i=0; i<numObjects; ++i) {
        loadObject();
    }
}

void Save_Game(){
    file = fopen("save.txt", "w");

    File_saveTiles();
    File_saveObjects();
    fclose(file);
}

void Load_Game(){
    file = fopen("save.txt", "r");

    /// Destroy world
    cleanup_worldObjects();
    /// Load
    File_loadTiles();
    File_loadObjects();
    /// Reset Sensory Map
    updateSoundMapDensity();

    /// Set player
    player = worldObjects[0];

    fclose(file);
}


#endif
