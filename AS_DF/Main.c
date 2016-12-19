#define _USE_MATH_DEFINES
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
//#include <iostream>
//#include <fstream>
//#include <sstream>
//using namespace std;
// Order is very important. Constants/Images, Classes, Encompassing Classes, World.
#include <allegro5/allegro.h>
 #include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
//#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <math.h>
#include <time.h>


#define M_PI 3.14159265358979323846

#include "constants.h"
#include "assets.h"
#include "Input.h"

#include "Tile.h"
#include "Sound.h"
#include "Odor.h"
#include "Vision.h"
#include "Weather.h"
#include "Map_Generator.h"
#include "Objects.h"

#include "Player.h"
#include "File_Handler.h"
#include "Menu.h"
#include "world.h"

enum STATE{TITLE, PLAYING, LOST};
//enum KEYS{UP, DOWN, LEFT, RIGHT, SPACE, KEY_Q, KEY_W, KEY_A, KEY_S, KEY_D, KEY_ENTER, KEY_BACK, NUM_KEYS}; // these are all the keys we're checking.
//bool keys[NUM_KEYS] = {0};

void Game_Logic (){
    Update_World();
}

void Draw_Game (){
   Draw_World();
}


int main(void)
{
	//primitive variables
	bool done = false;
	bool redraw = true;
	const int FPS = 60;
	int state = PLAYING;
	// Values for Game Logic

	//Allegro variables
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;

	//Initialization Functions
	if(!al_init())										//initialize Allegro
		return -1;
    int flags=0;
    //flags += ALLEGRO_OPENGL;
    //flags += ALLEGRO_FULLSCREEN;
    al_set_new_display_flags(flags);

	display = al_create_display(Screen_W, Screen_H);			//create our display object


	if(!display)									//test display object
		return -1;

	al_init_primitives_addon();
	al_install_keyboard();
	al_init_font_addon();
	al_init_image_addon();
	al_install_mouse();

	al_install_audio();
	al_init_acodec_addon();
	al_reserve_samples(1);
	al_install_joystick();



	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);

	// ---------------------------- Initialize
	initConstants();
	Init_Images();
	Init_Sounds();
	Init_World();

	// ============================End Initialize



	srand(time(NULL));

	// Call Init Functions.

    al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_joystick_event_source());

	al_start_timer(timer);
	while(!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if(ev.type == ALLEGRO_EVENT_TIMER)
		{
			redraw = true;




			if(state == TITLE)
			{}
			else if(state == PLAYING)
			{
                Game_Logic();
			}
			else if(state == LOST)
			{}

		}
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch(ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
                pressButton(&input.Up);
				break;
			case ALLEGRO_KEY_DOWN:
                pressButton(&input.Down);
				break;
			case ALLEGRO_KEY_LEFT:
                pressButton(&input.Left);
				break;
			case ALLEGRO_KEY_RIGHT:
                pressButton(&input.Right);
				break;
			case ALLEGRO_KEY_SPACE:
                break;
            case ALLEGRO_KEY_Q:
                pressButton(&input.Q);
                break;
            case ALLEGRO_KEY_E:
                pressButton(&input.E);
                break;
            case ALLEGRO_KEY_R:
                pressButton(&input.R);
                break;
            case ALLEGRO_KEY_W:
                pressButton(&input.W);
                break;
            case ALLEGRO_KEY_A:
                pressButton(&input.A);
                break;
            case ALLEGRO_KEY_S:
                pressButton(&input.S);
                break;
            case ALLEGRO_KEY_D:
                pressButton(&input.D);
                break;
            case ALLEGRO_KEY_ENTER:
                pressButton(&input.Select);
                break;
            case ALLEGRO_KEY_BACKSPACE:
                pressButton(&input.Back);
                break;
			}
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch(ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
                releaseButton(&input.Up);
				break;
			case ALLEGRO_KEY_DOWN:
                releaseButton(&input.Down);
				break;
			case ALLEGRO_KEY_LEFT:
                releaseButton(&input.Left);
				break;
			case ALLEGRO_KEY_RIGHT:
                releaseButton(&input.Right);
				break;
			case ALLEGRO_KEY_SPACE:
				break;
            case ALLEGRO_KEY_Q:
                releaseButton(&input.Q);
                break;
            case ALLEGRO_KEY_E:
                releaseButton(&input.E);
                break;
            case ALLEGRO_KEY_R:
                releaseButton(&input.R);
                break;
            case ALLEGRO_KEY_W:
                releaseButton(&input.W);
                break;
            case ALLEGRO_KEY_A:
                releaseButton(&input.A);
                break;
            case ALLEGRO_KEY_S:
                releaseButton(&input.S);
                break;
            case ALLEGRO_KEY_D:
                releaseButton(&input.D);
                break;
            case ALLEGRO_KEY_ENTER:
                releaseButton(&input.Select);
                break;
            case ALLEGRO_KEY_BACKSPACE:
                releaseButton(&input.Back);
                break;
			}
		}
		else if(ev.type == ALLEGRO_EVENT_JOYSTICK_AXIS)
		{
		}

		else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{

		    if(ev.mouse.button & 1)
                pressButton(&m.m1);
            if(ev.mouse.button & 2)
                pressButton(&m.m2);

		}
		else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
		{

		    if(ev.mouse.button & 1)
                releaseButton(&m.m1);
            if(ev.mouse.button & 2)
                releaseButton(&m.m2);

		}
		else if(ev.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
		    setCursor(ev.mouse.x, ev.mouse.y);
		    //World_Input.setScroll(ev.mouse.dz);
		    //World_Input.setAxisChange(ev.mouse.dx, ev.mouse.dy);
		}

		if(redraw && al_is_event_queue_empty(event_queue))
		{
			redraw = false;

			if(state == TITLE)
			{
			}
			else if(state == PLAYING)
			{
				// Draw Stuff
                Draw_Game();
			}
			else if(state == LOST)
			{
                // Lost. Do Stuff.
			}
			float colorScale = 0.0;//0.5;

			al_flip_display();
			al_clear_to_color(al_map_rgb(26*colorScale,120*colorScale,248*colorScale));//(26,120,248));

		}
	}

	Cleanup_World();
	return 0;
}


