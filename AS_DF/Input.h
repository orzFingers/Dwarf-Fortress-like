#ifndef INPUT_H
#define INPUT_H

typedef struct{
    bool isNew, current, previous;
} ButtonState;

typedef struct{
    ButtonState Select, Back;
    ButtonState Up, Down, Left, Right;
    ButtonState W, A, S, D;
    ButtonState Q, E, R;
    ButtonState Inventory;
} InputStates;

typedef struct{
    int x, y;
    ButtonState m1, m2;

    int action, primaryAction, secondaryAction;
    int actionArg;
    int clickX, clickZ, tileX, tileZ;
} Mouse;

void updateButtonState(ButtonState *b){
    if(b->current){
            if(!b->previous){
                b->isNew = true;
                b->previous = true;
            }
            else
                b->isNew = false;
        }
        else
            b->previous = false;
}

/// Instances
Mouse m;
InputStates input;

/// Interface Functions
void updateInput(){
    // Update Mouse
    updateButtonState(&m.m1);
    updateButtonState(&m.m2);

    // Update InputStates
    updateButtonState(&input.Select);
    updateButtonState(&input.Back);
    updateButtonState(&input.Up);
    updateButtonState(&input.Down);
    updateButtonState(&input.Left);
    updateButtonState(&input.Right);
    updateButtonState(&input.W);
    updateButtonState(&input.A);
    updateButtonState(&input.S);
    updateButtonState(&input.D);
    updateButtonState(&input.Q);
    updateButtonState(&input.E);
    updateButtonState(&input.R);
    updateButtonState(&input.Inventory);
}

void pressButton(ButtonState *b){
    b->current = true;
}
void releaseButton(ButtonState *b){
    b->current = false;
}

void setCursor(int x, int y){
    m.x = x;
    m.y = y;
}


#endif

