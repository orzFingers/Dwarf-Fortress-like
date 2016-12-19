#ifndef MENU_H
#define MENU_H

/// Structs, enums, Data Containers
enum MOUSE_ACTIONS{ ACTION_NONE, DRAW_LINE, DRAW_FILL, ERASE_FILL };

struct Button{
    int x, y, width, height; // Drawn from center.
    ALLEGRO_BITMAP* image;
    bool isImageFromTileSet;
    void (*onRelease)(struct Button*); // takes itself as argument
    void *parent; // The Parent Menu.
    int onReleaseArg;
};
typedef struct Button Button;

#define MAX_MENU_BUTTONS 10
#define MAX_MENU_SUBMENUS 10
struct Menu{
    Button* buttons[MAX_MENU_BUTTONS];
    int numButtons;
    struct Menu* menus[MAX_MENU_SUBMENUS];
    int numMenus, activeMenu;
};
typedef struct Menu Menu;

/// Mouse Actions
void drawLine(ButtonState* actionState){
    if(actionState->isNew){ // first frame.
        m.clickX = m.tileX;
        m.clickZ = m.tileZ;
    }
    if(!actionState->current){ // released. Draw Line.
        m.action = ACTION_NONE;
        int xDiff = abs(m.tileX - m.clickX);
        int zDiff = abs(m.tileZ - m.clickZ);
        int increment;
        if(xDiff >= zDiff){
            if(m.tileX > m.clickX)
                increment = 1;
            else
                increment = -1;
            for(int i=0; i<=xDiff; i++){
                setCell(m.clickX + i*increment, ScreenY, m.clickZ, m.actionArg);
            }
        }
        else{
            if(m.tileZ > m.clickZ)
                increment = 1;
            else
                increment = -1;
            for(int i=0; i<=zDiff; i++){
                setCell(m.clickX, ScreenY, m.clickZ + i*increment, m.actionArg);
            }
        }
    }
}

void drawFill(ButtonState* actionState){
    if(actionState->isNew){ // first frame.
        m.clickX = m.tileX;
        m.clickZ = m.tileZ;
    }
    if(!actionState->current){ // released. Draw.
        m.action = ACTION_NONE;
        int xDiff = abs(m.tileX - m.clickX);
        int zDiff = abs(m.tileZ - m.clickZ);

        //setCell(m.clickX + i*increment, ScreenY, m.clickZ, m.actionArg);
        int incrementX, incrementZ;
        if(m.tileX > m.clickX)
            incrementX = 1;
        else
            incrementX = -1;
        if(m.tileZ > m.clickZ)
            incrementZ = 1;
        else
            incrementZ = -1;
        for(int x=0; x<=xDiff; x++){
            for(int z=0; z<=zDiff; z++){
                setCell(m.clickX + x*incrementX, ScreenY, m.clickZ + z*incrementZ, m.actionArg);
            }
        }

        updateSoundMapDensity();
    }
}
void eraseFill(ButtonState* actionState){
    if(actionState->isNew){ // first frame.
        m.clickX = m.tileX;
        m.clickZ = m.tileZ;
    }
    if(!actionState->current){ // released. Draw.
        m.action = ACTION_NONE;
        int xDiff = abs(m.tileX - m.clickX);
        int zDiff = abs(m.tileZ - m.clickZ);

        //setCell(m.clickX + i*increment, ScreenY, m.clickZ, m.actionArg);
        int incrementX, incrementZ;
        if(m.tileX > m.clickX)
            incrementX = 1;
        else
            incrementX = -1;
        if(m.tileZ > m.clickZ)
            incrementZ = 1;
        else
            incrementZ = -1;
        for(int x=0; x<=xDiff; x++){
            for(int z=0; z<=zDiff; z++){
                setCell(m.clickX + x*incrementX, ScreenY, m.clickZ + z*incrementZ, TILE_NONE);
            }
        }

        updateSoundMapDensity();
    }
}


void drawMouseAction(){
    int x1 = m.clickX;
    int z1 = m.clickZ;
    tileXZ_to_screenXZ(&x1, &z1);

    int x2 = m.tileX;
    int z2 = m.tileZ;
    tileXZ_to_screenXZ(&x2, &z2);

    int temp;
    if(x1 > x2){
        temp = x1;
        x1 = x2;
        x2 = temp;
    }
    if (z1 > z2){
        temp = z1;
        z1 = z2;
        z2 = temp;
    }

    x2 += ADJUSTED_TILE_SIZE;
    z2 += ADJUSTED_TILE_SIZE;

    if(m.action == DRAW_FILL || m.action == ERASE_FILL){
        al_draw_filled_rectangle(x1, z1,
                                 x2, z2,
                                 al_map_rgba(125,125,125, 125));
    }
    if(m.primaryAction == DRAW_FILL || m.secondaryAction == DRAW_FILL){
        al_draw_filled_rectangle(m.x-ADJUSTED_TILE_SIZE/4, m.y-ADJUSTED_TILE_SIZE/4,
                                 m.x+ADJUSTED_TILE_SIZE/4, m.y+ADJUSTED_TILE_SIZE/4,
                                 al_map_rgba(255,255,0, 225));
    }
}

void updateMouseAction(int action, ButtonState* actionState){
    switch (action){
        case DRAW_LINE:
            drawLine(actionState);
            break;
        case DRAW_FILL:
            drawFill(actionState);
            break;
        case ERASE_FILL:
            eraseFill(actionState);
            break;
        default:
            break;
    }
}

/// Button *onClick() Function Prototypes
void setMouseAction_DRAW_LINE(Button* button){
    m.primaryAction = DRAW_LINE;
    m.secondaryAction = ACTION_NONE;
    m.actionArg = button->onReleaseArg;
}
void setMouseAction_DRAW_FILL(Button* button){
    if(m.primaryAction == DRAW_FILL &&
       m.actionArg == button->onReleaseArg){
        m.primaryAction = ACTION_NONE;
        m.secondaryAction = ACTION_NONE;
        return;
    }


    m.primaryAction = DRAW_FILL;
    m.secondaryAction = ERASE_FILL;
    m.actionArg = button->onReleaseArg;
}
Menu *MainMenu;
void openCloseParentSubmenu(Button* button){
    Menu *menu = (Menu*) button->parent;

    if(menu->activeMenu == button->onReleaseArg)
        menu->activeMenu = -1;
    else
        menu->activeMenu = button->onReleaseArg;
}
void saveGame(Button* button){
    Save_Game();
}
void loadGame(Button* button){
    Load_Game();
}

/// Menu and Button Functions
//Menu *MainMenu;
Button *hoveringButton=NULL, *clickedButton=NULL;
/*
Button* createButton(ALLEGRO_BITMAP *image,
                     int x, int y, Menu *parent,
                     void (*onRelease)(struct Button*), int onReleaseArg){

    Button* button = malloc( sizeof(Button) );
    button->image = image;
    button->x = x;
    button->y = y;
    button->width = al_get_bitmap_width(image)/2;
    button->height = al_get_bitmap_height(image)/2;
    button->parent = parent;
    button->onRelease = onRelease;
    button->onReleaseArg = onReleaseArg;

    return button;
}

*/

Menu* createMenu(){
    Menu* menu = malloc( sizeof(Menu) );
    menu->numButtons = 0;
    menu->numMenus = 0;
    menu->activeMenu = -1;
    for(int i=0; i<MAX_MENU_SUBMENUS; ++i){
        menu->menus[i] = NULL;
    }
    for(int i=0; i<MAX_MENU_BUTTONS; ++i){
        menu->buttons[i] = NULL;
    }

    return menu;
}
/*
void addButtonToMenu(Menu* menu, Button* button){
    menu->buttons[ menu->numButtons++ ] = button;
}
struct Button{
    int x, y, width, height; // Drawn from center.
    ALLEGRO_BITMAP* image;
    bool isImageFromTileSet;
    void (*onRelease)(struct Button*); // takes itself as argument
    void *parent; // The Parent Menu.
    int onReleaseArg;
};
*/
void addButtonToMenu(Menu* parent,
                     ALLEGRO_BITMAP *image,
                     int x, int y,
                     void (*onRelease)(struct Button*), int onReleaseArg){

    Button* button = malloc( sizeof(Button) );
    button->image = image;
    button->x = x;
    button->y = y;
    if(image != NULL){
        button->width = al_get_bitmap_width(image)/2;
        button->height = al_get_bitmap_height(image)/2;
    }
    else{
        button->width = ADJUSTED_TILE_SIZE;///2;
        button->height = ADJUSTED_TILE_SIZE;///2;
    }
    button->parent = parent;
    button->onRelease = onRelease;
    button->onReleaseArg = onReleaseArg;
    button->isImageFromTileSet = (image == NULL);

    parent->buttons[ parent->numButtons++ ] = button;
}

void addSubmenuToMenu(Menu* menu){
    menu->menus[ menu->numMenus++ ] = createMenu();
}

void generateTileMenuButtons(Menu* menu, int menuX, int menuY){
    // initialize from bottom up, left to right
    int i=0;
    for(int x=0; x<16; ++x){
        for(int y=0; y < Screen_H/16; ++y){
            addButtonToMenu(menu, NULL, menuX + x*ADJUSTED_TILE_SIZE*2, menuY - y*ADJUSTED_TILE_SIZE*2,
                            setMouseAction_DRAW_FILL, i);
            if(++i >= NUM_TILE_TYPES)
                return;
        }
    }
}

void initMenus(){
    int mMenuX=30, mMenuY=Screen_H-20;

    // Create Main Menu
    MainMenu = createMenu();

    // Save/Load Buttons
    addButtonToMenu(MainMenu, IMG_Button_Save, Screen_W-30,20,
                          saveGame, 0);
    addButtonToMenu(MainMenu, IMG_Button_Load, Screen_W-90,20,
                          loadGame, 0);

    // Create and add Menu and Button.
    addSubmenuToMenu(MainMenu);
    addButtonToMenu(MainMenu, IMG_Button, mMenuX, mMenuY,
                          openCloseParentSubmenu, 0);

    generateTileMenuButtons(MainMenu->menus[0], ADJUSTED_TILE_SIZE, mMenuY-100);

}


void drawMenu(Menu* menu){
    // Draw this menu first.
    Button* button;
    static int imgSourceX, imgSourceY, imgNum;
    for(int i=0; i < menu->numButtons; ++i){
        button = menu->buttons[i];
        if(!button->isImageFromTileSet)
            al_draw_rotated_bitmap( button->image, button->width, button->height,
                                button->x, button->y, 0, 0);
        else{
                if(TileTypes[button->onReleaseArg].isWall)
                    imgNum = TileTypes[button->onReleaseArg].imageNum+2;
                else
                    imgNum = TileTypes[button->onReleaseArg].imageNum;
                imgSourceX = (imgNum%16)*TILE_SIZE;
                imgSourceY = (imgNum/16)*TILE_SIZE;

                al_draw_scaled_bitmap(IMG_TileSheet,
                            imgSourceX, imgSourceY, TILE_SIZE, TILE_SIZE,
                            button->x - button->width, button->y - button->height,
                            ADJUSTED_TILE_SIZE*2, ADJUSTED_TILE_SIZE*2, 0);

        }
        // Draw hovering rectangle
        if(button == hoveringButton){
            al_draw_filled_rectangle(button->x - button->width, button->y - button->height,
                                     button->x + button->width, button->y + button->height,
                                     al_map_rgba(125,125,125, 125));
        }
    }
    // Draw submenu.
    if(menu->activeMenu != -1)
        drawMenu( menu->menus[ menu->activeMenu ] );
}

void drawMenus(){
    drawMenu(MainMenu);
    drawMouseAction();
}

Button* collideButtons(Menu* menu){
    Button* button = NULL;
    // open Active submenu if available.
    if(menu->activeMenu != -1)
        button = collideButtons( menu->menus[ menu->activeMenu ] );
    if(button != NULL)
        return button;

    for(int i=0; i < menu->numButtons; ++i){
        button = menu->buttons[i];
        if( abs(m.x - button->x) <= button->width &&
            abs(m.y - button->y) <= button->height)
                return button;
    }
    return NULL;
}



void updateMenus(){
    // Update mouse values
    updateMouseTileValues();

    // Update Menu
    hoveringButton = collideButtons(MainMenu);
    if(hoveringButton != NULL){
        if(m.m1.isNew){ // CLICK
            clickedButton = hoveringButton;
            return;
        }
        else if(m.m1.previous && !m.m1.current) { // RELEASE. Do action.
            //openCloseParentSubmenu(NULL);
            //((Object*)NULL)->getImage((Object*)NULL); // lol
            if(clickedButton == hoveringButton)
                clickedButton->onRelease( clickedButton );

        }
    }

    // Break action if both mouse buttons pressed
    if(m.m1.current && m.m2.current){
        m.action = ACTION_NONE;
        return;
    }

    // Check new input for actions.
    if(m.m1.isNew && m.primaryAction != ACTION_NONE)
        m.action = m.primaryAction;
    else if(m.m2.isNew && m.secondaryAction != ACTION_NONE)
        m.action = m.secondaryAction;

    // update action.
    if(m.action == m.primaryAction) // update Primary Action
        updateMouseAction(m.primaryAction, &m.m1);
    else if(m.action == m.secondaryAction)// update Secondary Action
        updateMouseAction(m.secondaryAction, &m.m2);
}





#endif // MENU_H
