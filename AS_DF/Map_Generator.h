#ifndef MAP_GENERATOR_H
#define MAP_GENERATOR_H

bool hasGrass[WORLD_SIZE_X][WORLD_SIZE_Z];

void growTileOnMap(int y, int type, int promotionType, int growChance, int numPasses){
    /*
    For all cells on a y-plane, if the current cell is of the type we're growing
    and we pass the (1 in growChance) rand() check:
    Promote cell to the promotionType(if not set to -1), and grow the base type in a random direction.
    Do this for a number of passes.
    */
    int direction;
    for(int i=0; i < numPasses; ++i){
        for(int x=0; x<WORLD_SIZE_X; ++x){
            for(int z=0; z<WORLD_SIZE_Z; ++z){
                if( worldCells[x][y][z].tileType == type && rand()%growChance == 0){
                    // If there is a promotion type, promote current tile.
                    if(promotionType != -1)
                        setCell(x, y, z, promotionType);
                    // Get a random direction and set the cell in that direction to the input type.
                    direction = rand()%4;
                    switch(direction){
                    case 0:
                        if(x<WORLD_SIZE_X-1){
                            setCell(x+1, y, z, type);
                            break;
                        }
                    case 1:
                        if(x>0){
                            setCell(x-1, y, z, type);
                            break;
                        }
                    case 2:
                        if(z<WORLD_SIZE_Z-1){
                            setCell(x, y, z+1, type);
                            break;
                        }
                    case 3:
                        if(z>0){
                            setCell(x, y, z-1, type);
                            break;
                        }
                    default:
                        break;
                    }
                }
            }
        }
    }
}

void generateTileOnMap(int y, int type, int seedChance){
    /*
    For all cells on a Y-Plane:
    Set that cell to the input type in a (1 in seedChance) chance.
    */
    for(int x=0; x<WORLD_SIZE_X; ++x){
        for(int z=0; z<WORLD_SIZE_Z; ++z){
            if( rand()%seedChance == 0  &&  worldCells[x][y][z].tileType != type )
                setCell(x, y, z, type);
        }
    }
}

void generateGrassOnMap(int y){
    /*
    Generate grass on a y-plane:
    First generate some grass as a seed.
    Then grow the grass into adjacent cells,
    and finally grow and promote our grass.
    Tweak growChance and numPasses to taste.
    */

    // High density grass. Roughly half Tall, half Short.
    generateTileOnMap(y, TILE_GRASS, 3);
    growTileOnMap(y, TILE_GRASS, -1,5, 20);//, 20);
    growTileOnMap(y, TILE_GRASS, TILE_TALL_GRASS, 5, 5);
}

#endif // MAP_GENERATOR_H

