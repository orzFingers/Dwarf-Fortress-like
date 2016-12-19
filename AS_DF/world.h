#ifndef WORLD_H
#define WORLD_H

void generateWorldCells(){
    int type;
    for (int x=0; x<WORLD_SIZE_X; ++x) {

        for (int y=0; y<WORLD_SIZE_Y; ++y) {

            for (int z=0; z<WORLD_SIZE_Z; ++z) {

                if(y==0)
                    type = TILE_NONE;
                else if(y==1)
                    type = TILE_SOIL;//rand()%(NUM_TILE_TYPES-2)+1;
                else if(y%2==0)
                    type = TILE_SOIL;
                else
                    type = TILE_ROCK;

                if(x==0 && z==0 )
                    type = TILE_NONE;

                setCell(x, y, z, type);

                worldCells[x][y][z].isImpassable = false;

                worldCells[x][y][z].numOccupants = 0;
                for(int i=0; i<MAX_CELL_OCCUPANTS; ++i)
                    worldCells[x][y][z].occupants[i] = NULL;

            }
        }
    }
    //setCell(0,0,0, TILE_DOORWAY);

    generateGrassOnMap(1);
    setCell(0,1,0, TILE_NONE);
}

void Init_World(){
    initWorldObjects();
    initTileTypes();
    generateWorldCells();
    InitSound();
    InitOdor();
    InitWeather();
    initMenus();
    initSaveLoadObjectArrays();

    createPlayer();
    createDoor(10,0,10);
    //createDoor(2,1,2);
    createShower(12,0,12);
    //worldCells[1][0][0].numOccupants = MAX_CELL_OCCUPANTS;

    Vision_Object vF[MAX_VISION_OBJECTS];
    Vision_Object vL[MAX_VISION_OBJECTS];
    Vision_Object vR[MAX_VISION_OBJECTS];
    int nF=0, nL=0, nR=0;

    ViewArea(10,10,10,
             vF, &nF,
             vL, &nL,
             vR, &nR,
             NORTH);

    /*
    setupShaders();
    //al_set_target_backbuffer(myDisplay);
    al_use_shader(defaultShader);
    al_set_shader_float("selection", -1);
    */
    al_reserve_samples(100);
}

void Draw_World(){
    //al_draw_bitmap(IMG_Button, m.x,m.y,0);
    draw_worldCells();
    DrawOdor();
    DrawSound();
    DrawWeather();
    drawMenus();
}

const int odorUpdateWait = 10;
int odorUpdateCount =0;
void Update_World(){
    ++frameNum;

    updateMenus();
    updatePlayer();
    update_worldObjects();
    updateInput();
    UpdateSound();
    if(++odorUpdateCount >= odorUpdateWait){
        odorUpdateCount = 0;
        UpdateOdor();
    }
    UpdateWeather();


    characterX = player->x;
    characterY = player->y;
    characterZ = player->z;
    ScreenY = player->y;

    /// DEBUG:
    if(input.Select.isNew)
        cleanup_worldObjects();
        //Load_Game();
    //else if(input.Back.isNew)
        //Save_Game();

}

void Cleanup_World(){
    cleanup_worldObjects();
}

#endif // WORLD_H
