#include <stdio.h>
#include <stdlib.h>
#include <string.h>		// Para usar strings
#include <stdbool.h>
// SOIL é a biblioteca para leitura das imagens
#include "SOIL.h"
#include <math.h>

 
// Um pixel RGB
typedef struct {
    unsigned char r, g, b;
} RGB;

// Uma imagem em RGB
typedef struct {
   
    int width, height, totalSize;
    RGB* img;
} Img;

// Protótipos
void load(char* name, Img* pic);
void resizeImg(Img* resized , Img* oriPic);
char* asciiSelector(int escalaGray);

// Carrega uma imagem para a struct Img
void load(char* name, Img* pic)
{
    int chan;
    pic->img = (unsigned char*) SOIL_load_image(name, &pic->width, &pic->height, &chan, SOIL_LOAD_RGB);
    //pic->totalSize= pic->height* pic->width;
    if(!pic->img)
    {
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
        exit(1);
    }
    printf("Load: %d x %d x %d\n", pic->width, pic->height, chan);
}


void resizeImg(Img* resized , Img* oriPic){
    resized->img[resized->totalSize - 1] = oriPic->img[oriPic->totalSize - 1];
    RGB (* resizedImage)[resized->width] = (RGB(*)[resized->width]) resized->img;
    RGB (* oriImage)[oriPic->width] = (RGB(*)[oriPic->width]) oriPic->img;


    double widthRelation = (oriPic->width / (double) resized->width);
    double heightRelation = (oriPic->height / (double) resized->height);

    int originalPicLine = 0;
    int originalPicColum = 0;


    for(int i = 0; i < resized->height; i++){
        originalPicLine = round(i * heightRelation);
        for(int j = 0; j < resized->width; j++){
            originalPicColum = round(j * widthRelation);
            resizedImage[i][j] = oriImage[originalPicLine][originalPicColum];
        }
    }

}
char* asciiSelector(int escalaGray){
    if( escalaGray >= 0 && escalaGray < 32){
        return ".";
    } else if(escalaGray < 64){
        return ":";
    } else if(escalaGray < 96){
        return "c";
    } else if(escalaGray < 128){
        return "o";
    } else if(escalaGray < 160){
        return "C";
    } else if(escalaGray < 192){
        return "0";
    } else if(escalaGray < 224){
        return "8";
    } 
    return "@";
}


int main(int argc, char** argv)
{
    Img pic;
    // if(argc == 1) {
    //     printf("loader [img]\n");
    //     exit(1);
    // }

    char nomeImagem[100];

    printf(" Digite o nome da imagem seguido do tipo de arquivo. por exemplo: teste.jpg\n");
    printf("Digite: \n");
    scanf("%s", nomeImagem);

    load(nomeImagem, &pic);

    printf("Primeiros 10 pixels da imagem:\n");
     for(int i=0; i<10; i++) {
         printf("[%d %d %d] ", pic.img[i].r, pic.img[i].g, pic.img[i].b);
     }
    printf("\n");

    printf("Digite o valor de redimensionamento, por exemplo: para obter 50'%' da imagem, digite 0.5, para obter 80'%' da imagem, digite 0.8 e assim por diante\n");
    double porcentagem;
    scanf("%lf",&porcentagem);

    //Transforma as cores para Preto e Branco
    int pix;
    for(int i=0; i<pic.width*pic.height; i++) {
        pix =(pic.img[i].r*0.3 +pic.img[i].g*0.59 + pic.img[i].b*0.11);
        pic.img[i].r = pix;
        pic.img[i].g = pix;
        pic.img[i].b = pix;  
    }

     //regra de 3
    Img pic2;// imagem reduzida  
    load(argv[1], &pic2);

    pic2.width= ((porcentagem*pic.width)/1.0);
    pic2.height= ((porcentagem*pic.height)/1.0);
    pic2.totalSize= pic2.height*pic2.width;


    RGB resizedImage[pic.width * pic.height];
    pic2.img = resizedImage;
    
    resizeImg(&pic2 , &pic);

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
    fprintf(arq,"<style> pre  {color: white;font-family: Courier;font-size: 8px;}</style>\n");
    fprintf(arq,"<body style='background: black;' leftmargin=0 topmargin=0>\n");
    fprintf(arq,"<pre>");
    RGB (* resizedPixels)[pic2.width] = (RGB(*)[pic2.width]) pic2.img;  
     int add= 0;
    for(int line = 0; line + 5 <= pic2.height; line += 5) {
        for (int column = 0; column + 4 <= pic2.width; column += 4) {
            for (int setorLinha = line; setorLinha < line + 5; setorLinha++) {
                for (int setorColuna = column; setorColuna < column + 4; setorColuna++) {
                    add += resizedPixels[setorLinha][setorColuna].r;
                }
            }
            fprintf(arq,asciiSelector((int) round(add / 20.0)));
            add = 0;
        }
        fprintf(arq,"\n");
    }
    fprintf(arq,"</pre>\n");                
    fprintf(arq,"</body>\n");
    fclose(arq);
    free(pic.img);
    free(pic2.img);
}
