#ifndef SHADER_HANDLER_H
#define SHADER_HANDLER_H

char *textFileRead(char *fn) {
    FILE *fp;
    char *content = NULL;

    int count=0;

    if (fn != NULL) {
        fp = fopen(fn,"rt");

        if (fp != NULL) {

        fseek(fp, 0, SEEK_END);
        count = ftell(fp);
        rewind(fp);

            if (count > 0) {
                content = (char *)malloc(sizeof(char) * (count+1));
                count = fread(content,sizeof(char),count,fp);
                content[count] = '\0';
            }
            fclose(fp);
        }
    }
    return content;
}

ALLEGRO_SHADER* defaultShader;

int setupShaders(){
    char* vs = textFileRead("Shaders/ex_shader_vertex.txt");
    char* ps = textFileRead("Shaders/sSheetShader.txt");//ex_shader_pixel.txt");
    int e[5];

    //const char* vs = al_get_default_shader_source(ALLEGRO_SHADER_AUTO, ALLEGRO_VERTEX_SHADER);
    s = al_create_shader(ALLEGRO_SHADER_GLSL);
    e[0] = bool(defaultShader);
    e[1] = al_attach_shader_source(s, ALLEGRO_VERTEX_SHADER, vs);
    e[2] = al_attach_shader_source(s, ALLEGRO_PIXEL_SHADER, ps);
    e[3] = al_build_shader(defaultShader);
    e[4] = al_use_shader(defaultShader);

    free(vs);
    free(ps);
    printf("shader success: 0 =[%i], 1 =[%i], 2 =[%i], 3 =[%i], 4 =[%i]\n %s\n",
                e[0], e[1], e[2], e[3], e[4], al_get_shader_log(s));
}



#endif
