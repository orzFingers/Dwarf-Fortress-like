#ifndef CONSTANTS_H
#define CONSTANTS_H




const int Screen_W = 400;
const int Screen_H = 400;

// ! - ! - ! MAKE SURE TO CHANGE SIZE DEFINES IN SOUND.H !-  ! - ! - ! -

#define WORLD_SIZE_X  100
#define WORLD_SIZE_Y  20
#define WORLD_SIZE_Z  100

int ScreenX=0, ScreenY=0, ScreenZ=0;
int characterX=0, characterY=0, characterZ=0;
int TILE_SIZE=10;
int ZOOM=1;
int ADJUSTED_TILE_SIZE;
int ADJUSTED_S_WIDTH;
int ADJUSTED_S_HEIGHT;
int frameNum=0;
float SFXVolume=1.0;

FILE* file;

ALLEGRO_FONT *fontDefault = NULL;
ALLEGRO_COLOR BACKGROUND_COLOR, COLOR_CLEAR;
ALLEGRO_COLOR COLOR_MENU_SELECTION, COLOR_MENU_SELECTED;

void initConstants(){
    ADJUSTED_TILE_SIZE = TILE_SIZE*ZOOM;
    ADJUSTED_S_WIDTH = Screen_W / (ADJUSTED_TILE_SIZE);
    ADJUSTED_S_HEIGHT = Screen_H / (ADJUSTED_TILE_SIZE);



    fontDefault = al_load_font("pirulen.ttf", 12, 0);
	BACKGROUND_COLOR = al_map_rgb(26,120,248);
	COLOR_CLEAR = al_map_rgba(255,255,255,255);
	COLOR_MENU_SELECTION = al_map_rgb(0,0,0);
	COLOR_MENU_SELECTED = al_map_rgb(200,200,0);
}


int getDigit(int num, int digit){
    //if(num < pow(10,digit) ) // if num does not have that location
       //return 0;
    int step1 = num/pow(10,(digit-1)); // step1 holds the value in the ones location.
    int step2 = (step1/10)*10;
    int step3 = step1-step2;
    return step3;
}
int getPN0(int num){
    if(num<0)
        return -1;
    if(num==0)
        return 0;
    return 1;

}

enum ORIENTATION{NORTH, SOUTH, EAST, WEST};
enum IMAGES {IMAGE_NONE, IMAGE_CHARACTER=193, IMAGE_DOOR=66, IMAGE_DOORWAY=67, IMAGE_SHOWER=70};
enum OBJECT_TYPES {OBJECT_NONE, OBJECT_CHARACTER, OBJECT_DOOR, OBJECT_SHOWER, NUM_OBJECT_TYPES};//,  NUM_OBJECT_TYPES};
//enum CELL_SOLIDS {SOLID_NONE, SOLID_DIRT, NUM_SOLID_TYPES};
//enum CELL_LIQUIDS {LIQUID_NONE, LIQUID_WATER, LIQUID_LAVA, NUM_LIQUID_TYPES};

void custom_draw_scaled_rotated_bitmap(ALLEGRO_BITMAP* bitmap,
                                       float sx, float sy, float sw, float sh,
                                       float dx, float dy, float dw, float dh,
                                       float angle){
    al_draw_tinted_scaled_rotated_bitmap_region(bitmap,
                                                sx, sy, sw, sh,
                                                COLOR_CLEAR,
                                                sw/2, sh/2, dx+dw/2, dy+dh/2, dw/sw, dh/sh,
                                                angle, 0);
}



void* my_malloc(size_t size, const char *file, int line, const char *func)
{

    void *p = malloc(size);
//    printf ("Frame: %i, Allocated = %s, %i, %s, %p[%li]\n", frameNum, file, line, func, p, size);

    //Link List functionality goes in here

    return p;
}
#define malloc(X) my_malloc( X, __FILE__, __LINE__, __FUNCTION__)

#endif


