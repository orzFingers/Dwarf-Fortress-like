#ifndef IMAGES_H
#define IMAGES_H

ALLEGRO_BITMAP *IMG_TileSheet;
ALLEGRO_BITMAP *IMG_Button;
ALLEGRO_BITMAP *IMG_Button_Save;
ALLEGRO_BITMAP *IMG_Button_Load;

ALLEGRO_SAMPLE *SND_Door_Open;
ALLEGRO_SAMPLE *SND_Door_Close;
ALLEGRO_SAMPLE *SND_Shower;
ALLEGRO_SAMPLE *SND_RainDrop;


void Init_Images(){
    IMG_TileSheet = al_load_bitmap("Images/retrodaystiles10_2.png");
    IMG_Button = al_load_bitmap("Images/Button_2.png");
    IMG_Button_Save = al_load_bitmap("Images/Button_Save.png");
    IMG_Button_Load = al_load_bitmap("Images/Button_Load.png");
}

void Init_Sounds(){
    SND_Door_Open = al_load_sample( "Samples/open_interior_wood_door.wav" );
    SND_Door_Close = al_load_sample( "Samples/close_interior_wood_door.wav" );
    SND_Shower = al_load_sample( "Samples/washing_and_rinsing_hair.wav" );
    SND_RainDrop = al_load_sample( "Samples/Rain0.wav" );
}

void playSound(int REF){
}


#endif // IMAGES_H
