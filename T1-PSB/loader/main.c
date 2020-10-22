#include <stdio.h>
#include <stdlib.h>
#include <string.h>		// Para usar strings
#include <stdbool.h>
// SOIL é a biblioteca para leitura das imagens
#include "SOIL.h"
#include <math.h>

typedef struct {
    int width, height, totalSize;
} Dimension; 
// Um pixel RGB
typedef struct {
    unsigned char r, g, b;
} RGB;

// Uma imagem em RGB
typedef struct {
    Dimension dimension;
    //int width, height;
    RGB* img;
} Img;

void reduceToFrom(Img* reducedPic, Img* realPic);

void reduceToFrom(Img* reducedPic, Img* realPic) {
    printf("Reduzindo img\n");
    reducedPic->img[reducedPic->dimension.totalSize - 1] = realPic->img[realPic->dimension.totalSize - 1];
    int realPicLinePosition = 0;
    int realPicColumnPosition = 0;
    RGB (* reducedImage)[reducedPic->dimension.width] = (RGB(*)[reducedPic->dimension.width]) reducedPic->img;
    RGB (* realImage)[realPic->dimension.width] = (RGB(*)[realPic->dimension.width]) realPic->img;
    float widthRelation =  (realPic->dimension.width / (float) reducedPic->dimension.width);
    float heightRelation =  (realPic->dimension.height / (float) reducedPic->dimension.height);
    for(int line = 0; line < reducedPic->dimension.height - 1; line++) {
        realPicLinePosition = (int) round(heightRelation * line);
        for(int column = 0; column < reducedPic->dimension.width; column++) {
            realPicColumnPosition = (int) round(widthRelation * column);
            reducedImage[line][column] = realImage[realPicLinePosition][realPicColumnPosition];
        }
    }
}

// Protótipos
void load(char* name, Img* pic);

// Carrega uma imagem para a struct Img
void load(char* name, Img* pic)
{
    int chan;
    pic->img = (unsigned char*) SOIL_load_image(name, &pic->dimension.width, &pic->dimension.height, &chan, SOIL_LOAD_RGB);
    if(!pic->img)
    {
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
        exit(1);
    }
    printf("Load: %d x %d x %d\n", pic->dimension.width, pic->dimension.height, chan);
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
    for(int i=0; i<pic.dimension.width*pic.dimension.height; i++) {
        int pix =(pic.img[i].r*0.3 +pic.img[i].g*0.59 + pic.img[i].b*0.11);
        pic.img[i].r = 255 - pix;
        pic.img[i].g = 255 - pix;
        pic.img[i].b = 255 - pix;  
    }

     //regra de 3
    int tOriWidth= pic.dimension.width;
    int tOriHeight = pic.dimension.height;
    
    
    
    Img pic2;  
    pic2.dimension.width= ((tOriWidth/1.0)*0.5);
    pic2.dimension.height= ((tOriHeight/1.0)*0.5);
    
    load(argv[1], &pic2);
    reduceToFrom(&pic2, &pic);


    bool trigger = false;
    int j = 0;
    int diff = pic.dimension.width - pic2.dimension.width;
    int resize = pic.dimension.width/pic2.dimension.width;

    // for(int i=0;i<pic2.width*pic2.height;i++,j++){
    //      //if(i%pic.width !=0){
    //         pic2.img[i].r = pic.img[j].r;
    //         pic2.img[i].g = pic.img[j].g;
    //         pic2.img[i].b = pic.img[j].b;
    //     //} 
    // }
    // for(int i = 0, l = 0; i < pic.width * pic.height; i++, l++){
    //     if( l >= pic.width ){
    //         l = 0;
    //         trigger = true;
    //     } else if (l > pic2.width){
    //         trigger = false;
    //     }

    //     if(trigger && j < pic2.width * pic2.height){
    //         pic2.img[i].r = pic.img[j].r;
    //         pic2.img[i].g = pic.img[j].g;
    //         pic2.img[i].b = pic.img[j].b;
    //         j++;
    //     }
    // }

    // Inverte as cores
    // for(int i=0; i<pic.width*pic.height; i++) {
    //     pic.img[i].r = 255 - pic.img[i].r;
    //     pic.img[i].g = 255 - pic.img[i].g;
    //     pic.img[i].b = 255 - pic.img[i].b;
    // }

    // Exemplo: gravando um arquivo de saída com a imagem (não é necessário para o trabalho, apenas
    // para ver o resultado intermediário, por ex, da conversão para tons de cinza)
    SOIL_save_image("out.bmp", SOIL_SAVE_TYPE_BMP, pic2.dimension.width, pic2.dimension.height,
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
      for(int i=0; i<pic2.dimension.width* pic2.dimension.height; i++) {
        if(i == pic2.dimension.width){
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
