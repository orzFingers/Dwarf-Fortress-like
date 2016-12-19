#ifndef VISION_H
#define VISION_H

struct Vision_Object{
    Object* object;
    int distance; /// Basically a right angle from the viewer's position
};
typedef struct Vision_Object Vision_Object;

#define MAX_VISION_OBJECTS 10
#define MAX_VISION_DISTANCE 5 /// This is the horizontal/lateral distance, not walking distance

/// Note: The different "viewDirection" functions are the same except for the 3 "x-d+i, y, z-d", etc, statements
int getXZDistance(int x1, int z1, int x2, int z2){ // The right angle distance / walking distance.
    return abs(x1 - x2) + abs(z1 - z2);
}

int viewTop(int x, int y, int z, Vision_Object* v){
    Cell* cell;
    int numObjects = 0;

    for (int d = 0; d < MAX_VISION_DISTANCE; ++d){ /// lateral distance

        for (int i=0; i < d*2; ++i){ ///each cell on that line

            if( isOutsideWorldBounds(x-d+i, y, z-d) )
                continue;

            /// add Each object in cell
            cell = &worldCells[x-d+i][y][z-d];
            for(int j=0; j < cell->numOccupants; ++j){
                v[ numObjects ].distance = getXZDistance(x,z, x-d+i, z-d);
                v[ numObjects ].object = cell->occupants[j];
                ++numObjects;
                if(numObjects >= MAX_VISION_OBJECTS)
                    return numObjects;
            }
        }
    }
    return numObjects;
}
int viewBottom(int x, int y, int z, Vision_Object* v){
    Cell* cell;
    int numObjects = 0;

    for (int d = 0; d < MAX_VISION_DISTANCE; ++d){ /// lateral distance

        for (int i=0; i < d*2; ++i){ ///each cell on that line

            if( isOutsideWorldBounds(x-d+i, y, z+d) )
                continue;

            /// add Each object in cell
            cell = &worldCells[x-d+i][y][z+d];
            for(int j=0; j < cell->numOccupants; ++j){
                v[ numObjects ].distance = getXZDistance(x,z, x-d+i, z+d);
                v[ numObjects ].object = cell->occupants[j];
                ++numObjects;
                if(numObjects >= MAX_VISION_OBJECTS)
                    return numObjects;
            }
        }
    }
    return numObjects;
}
int viewLeft(int x, int y, int z, Vision_Object* v){
    Cell* cell;
    int numObjects = 0;

    for (int d = 0; d < MAX_VISION_DISTANCE; ++d){ /// lateral distance

        for (int i=0; i < d*2; ++i){ ///each cell on that line

            if( isOutsideWorldBounds(x-d, y, z-d+i) )
                continue;

            /// add Each object in cell
            cell = &worldCells[x-d][y][z-d+i];
            for(int j=0; j < cell->numOccupants; ++j){
                v[ numObjects ].distance = getXZDistance(x,z, x-d, z-d+i);
                v[ numObjects ].object = cell->occupants[j];
                ++numObjects;
                if(numObjects >= MAX_VISION_OBJECTS)
                    return numObjects;
            }
        }
    }
    return numObjects;
}
int viewRight(int x, int y, int z, Vision_Object* v){
    Cell* cell;
    int numObjects = 0;

    for (int d = 0; d < MAX_VISION_DISTANCE; ++d){ /// lateral distance

        for (int i=0; i < d*2; ++i){ ///each cell on that line

            if( isOutsideWorldBounds(x+d, y, z-d+i) )
                continue;

            /// add Each object in cell
            cell = &worldCells[x+d][y][z-d+i];
            for(int j=0; j < cell->numOccupants; ++j){
                v[ numObjects ].distance = getXZDistance(x,z, x+d, z-d+i);
                v[ numObjects ].object = cell->occupants[j];
                ++numObjects;
                if(numObjects >= MAX_VISION_OBJECTS)
                    return numObjects;
            }
        }
    }
    return numObjects;
}

/// !!! Remember to call this with "&intVar" so it doesn't crash.
void ViewArea(int x, int y, int z,
              Vision_Object* vForward, int* numForwardObjects,
              Vision_Object* vLeft, int* numLeftObjects,
              Vision_Object* vRight, int* numRightObjects,
              int characterOrientation){
    switch (characterOrientation){
        case NORTH:
            *numForwardObjects = viewTop(x, y, z, vForward);
            *numLeftObjects = viewLeft(x, y, z, vLeft);
            *numRightObjects = viewRight(x, y, z, vRight);
            break;
        case SOUTH:
            *numForwardObjects = viewBottom(x, y, z, vForward);
            *numLeftObjects = viewRight(x, y, z, vLeft);
            *numRightObjects = viewLeft(x, y, z, vRight);
            break;
        case EAST:
            *numForwardObjects = viewRight(x, y, z, vForward);
            *numLeftObjects = viewTop(x, y, z, vLeft);
            *numRightObjects = viewBottom(x, y, z, vRight);
            break;
        case WEST:
            *numForwardObjects = viewLeft(x, y, z, vForward);
            *numLeftObjects = viewBottom(x, y, z, vLeft);
            *numRightObjects = viewTop(x, y, z, vRight);
            break;
    }
}




#endif // VISION_H
