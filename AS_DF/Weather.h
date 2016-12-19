#ifndef WEATHER_H
#define WEATHER_H
/*
Weather:
Weather chooses a random variation from it's options, updates and draws it.
The UpdateWeather() function checks to see if current option is finished, and if so
waits a random time until selecting the next one.
*/

enum WEATHER{WEATHER_NONE, WEATHER_RAIN, WEATHER_ACID_RAIN, NUM_WEATHER_TYPES};
int currentWeather = WEATHER_NONE;
int weatherWaitTime=0;
int weatherWaitCount=0;


/// ------------ Weather Types

#define MAX_RAIN_DROPLETS 30
struct RainDroplet{
    int x, z;
};
typedef struct RainDroplet RainDroplet;

RainDroplet rainDroplets[MAX_RAIN_DROPLETS];
int rainStart = 0;
const int rainIncrement = 10;

void updateWeather_RAIN(){

    rainStart += rainIncrement;
    if(rainStart >= MAX_RAIN_DROPLETS) rainStart = 0;

    for(int i=0; i < rainStart+rainIncrement; ++i){
        rainDroplets[i].x = rand() % WORLD_SIZE_X;
        rainDroplets[i].z = rand() % WORLD_SIZE_Z;
    }
}

void drawWeather_RAIN(){
    //  /// Note: rainDroplets are already set to draw coordinates for speed.
    //  ///       any world process with them was already done in updateWeather_RAIN().
    int image;
    if(currentWeather == WEATHER_RAIN)
        image = 13;
    else
        image = 14;
    int sourceX = (image)*TILE_SIZE;
    int sourceY = 0;
    int x, z;
    for( int i=0; i<MAX_RAIN_DROPLETS; ++i){
        x = rainDroplets[i].x;
        z = rainDroplets[i].z;
        tileXZ_to_screenXZ(&x, &z);
        custom_draw_scaled_rotated_bitmap(IMG_TileSheet,
                                  sourceX, sourceY, TILE_SIZE, TILE_SIZE,
                                  x , z,
                                  ADJUSTED_TILE_SIZE, ADJUSTED_TILE_SIZE, 0);
    }
}

/// ------------ Weather Init, Update, Draw

void InitWeather(){
    /// init rain droplets
    for(int i=0; i<MAX_RAIN_DROPLETS; ++i){
        rainDroplets[i].x = 0;
        rainDroplets[i].z = 0;
    }
}

void UpdateWeather(){
    // Get new weather if none is happening.
    if(currentWeather == WEATHER_NONE){
        if(weatherWaitCount++ == 0)
            weatherWaitTime = rand()%100; // 100 frames. Not much.

        if( weatherWaitCount >= weatherWaitTime ) { /// Get next weather
            weatherWaitCount = 0;
            currentWeather = rand()%NUM_WEATHER_TYPES;
        }
    }

    // Update current weather if available.
    switch( currentWeather ) {
        case WEATHER_NONE:
            break;
        case WEATHER_RAIN:
            updateWeather_RAIN();
            break;
        case WEATHER_ACID_RAIN:
            updateWeather_RAIN();
            break;
    }
}

void DrawWeather(){
    switch( currentWeather ) {
        case WEATHER_NONE:
            break;
        case WEATHER_RAIN:
            drawWeather_RAIN();
            break;
        case WEATHER_ACID_RAIN:
            drawWeather_RAIN();
            break;
    }
}



#endif // WEATHER_H
