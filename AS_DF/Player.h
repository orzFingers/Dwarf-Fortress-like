#ifndef PLAYER_H
#define PLAYER_H

/*
updateButtonState(&input.Select);
    updateButtonState(&input.Back);
    updateButtonState(&input.Up);
    updateButtonState(&input.Down);
    updateButtonState(&input.Left);
    updateButtonState(&input.Right);
    updateButtonState(&input.Inventory);
*/
Object* player;

void createPlayer(){
    player = createCharacter(0,0,0);
}
int pTLimit=5;//10;
int pTX=0, pTY=0, pTZ=0; // Timers for movement
int pDX, pDY, pDZ; // Actual Movement values
void updatePlayer(){
    pDX = pDY = pDZ = 0;

    ++pTX;
    ++pTY;
    ++pTZ;
    // Set Move Values
    if(pTX >= pTLimit)
        pDX = -1*(int)input.A.current + 1*(int)input.D.current;
    if(pTY >= pTLimit)
        pDY = -1*(int)input.Q.current + 1*(int)input.E.current;
    if(pTZ >= pTLimit)
        pDZ = -1*(int)input.W.current + 1*(int)input.S.current;

    // Reset Timers
    if(pDX != 0)
        pTX = 0;
    if(pDY != 0)
        pTY = 0;
    if(pDZ != 0)
        pTZ = 0;

    Character_move(player, pDX, pDY, pDZ);

    ///Try to interact with object
    if(input.R.isNew){
       // addSoundToWorldSounds(&soundTypes[SOUND_RAINDROP], player->x, player->y, player->z);
        if( interactCell(player->x, player->y, player->z, player) == true )
            return;

        switch( ((CharacterData*) player->data)->orientation ){
        case NORTH:
            interactCell(player->x, player->y, player->z-1, player);
            break;
        case SOUTH:
            interactCell(player->x, player->y, player->z+1, player);
            break;
        case EAST:
            interactCell(player->x-1, player->y, player->z, player);
            break;
        case WEST:
            interactCell(player->x+1, player->y, player->z, player);
            break;
        }
    }


};



#endif // PLAYER_H
