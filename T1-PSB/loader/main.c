#include <stdio.h>
#include <stdlib.h>
#include <string.h>		// Para usar strings
#include <stdbool.h>
// SOIL é a biblioteca para leitura das imagens
#include "SOIL.h"

// Um pixel RGB
typedef struct {
    unsigned char r, g, b;
} RGB;

// Uma imagem em RGB
typedef struct {
    int width, height;
    RGB* img;
} Img;

// Protótipos
void load(char* name, Img* pic);

// Carrega uma imagem para a struct Img
void load(char* name, Img* pic)
{
    int chan;
    pic->img = (unsigned char*) SOIL_load_image(name, &pic->width, &pic->height, &chan, SOIL_LOAD_RGB);
    if(!pic->img)
    {
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
        exit(1);
    }
    printf("Load: %d x %d x %d\n", pic->width, pic->height, chan);
}

int main(int argc, char** argv)
{
    Img pic;
    if(argc == 1) {
        printf("loader [img]\n");
        exit(1);
    }
    load(argv[1], &pic);

    printf("Primeiros 10 pixels da imagem:\n");
     for(int i=0; i<10; i++) {
         printf("[%d %d %d] ", pic.img[i].r, pic.img[i].g, pic.img[i].b);
     }
    printf("\n");

    
//Transforma as cores para Preto e Branco
    for(int i=0; i<pic.width*pic.height; i++) {
        int pix =(pic.img[i].r*0.3 +pic.img[i].g*0.59 + pic.img[i].b*0.11);
        pic.img[i].r = 255 - pix;
        pic.img[i].g = 255 - pix;
        pic.img[i].b = 255 - pix;  
    }

     //regra de 3
    int tOriWidth= pic.width;
    int tOriHeight = pic.height;
    
    int tModWidth = ((0.5*tOriWidth)/1.0);
    int tModHeight = ((0.5*tOriHeight)/1.0);
    Img pic2;  
    load(argv[1], &pic2);
    pic2.width= tModWidth;
    pic2.height= tModHeight;


    bool trigger = false;
    int j = 0;
    int diff = pic.width - pic2.width;

    for(int i=0;i<pic2.width*pic2.height;i++){
        if(i%pic2.width !=0){
            
        }
    }
    for(int i = 0, l = 0; i < pic.width * pic.height; i++, l++){
        if( l >= pic.width ){
            l = 0;
            trigger = true;
        } else if (l > pic2.width){
            trigger = false;
        }

        if(trigger && j < pic2.width * pic2.height){
            pic2.img[i].r = pic.img[j].r;
            pic2.img[i].g = pic.img[j].g;
            pic2.img[i].b = pic.img[j].b;
            j++;
        }
    }

    
    // int resize = pic.width/tModWidth;
    
    // // tentativa de resize
    // int i=0;
    // while(i<(tModWidth*tModHeight)){
    //     pic2.img[i].r = pic.img[i].r;
    //     pic2.img[i].g = pic.img[i].g;
    //     pic2.img[i].b = pic.img[i].b;
    //     i+= resize;
    // }

    // for(int i=0; i<(tModWidth*tModHeight)-1; i++){
    //     pic2.img[i].r = pic.img[i].r;
    //     pic2.img[i].g = pic.img[i].g;
    //     pic2.img[i].b = pic.img[i].b; 
    // }
    


    // Inverte as cores
    // for(int i=0; i<pic.width*pic.height; i++) {
    //     pic.img[i].r = 255 - pic.img[i].r;
    //     pic.img[i].g = 255 - pic.img[i].g;
    //     pic.img[i].b = 255 - pic.img[i].b;
    // }

    // Exemplo: gravando um arquivo de saída com a imagem (não é necessário para o trabalho, apenas
    // para ver o resultado intermediário, por ex, da conversão para tons de cinza)
    SOIL_save_image("out.bmp", SOIL_SAVE_TYPE_BMP, pic2.width, pic2.height,
        3, (const unsigned char*) pic2.img);

    // Exemplo: gravando um arquivo saida.html
    FILE* arq = fopen("saida.html", "w"); // criar o arquivo: w
    if(arq == NULL) // 0, falso
    {
        printf("Erro abrindo arquivo de saída\n");
        exit(1);
    }

    fprintf(arq, "<html>\n");
    fprintf(arq,"<head>\n");
    fprintf(arq,"</head>\n");
    fprintf(arq,"<body style=""background: black;"" leftmargin=0 topmargin=0>\n");
    fprintf(arq,"<style> pre  {color: white;font-family: Courier;font-size: 8px;}</style>\n");
    fprintf(arq,"<pre>");
    printf("Primeiros 10 pixels da imagem:\n");
      for(int i=0; i<tModWidth*tModHeight; i++) {
        if(i == tModWidth){
            fprintf(arq,"[%d %d %d] \n", pic2.img[i].r, pic2.img[i].g, pic2.img[i].b);
        }
        fprintf(arq,"[%d %d %d]", pic2.img[i].r, pic2.img[i].g, pic2.img[i].b);
      }
     printf("\n");
     fprintf(arq,"</pre>\n");                
     fprintf(arq,"</body>\n");
     fclose(arq);
    // printf("\n");
    free(pic.img);
    free(pic2.img);
}
