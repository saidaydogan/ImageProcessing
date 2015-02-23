#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* 11011052 - Said Aydogan - Image Processing Hw1*/

typedef struct ImageStruct
{
    char* fileName;
    int maxVal;
    int width;
    int height;
    unsigned char** data;
} PGMImage;



void loadImage (char fileName[], PGMImage *img);
void save(PGMImage *img);
void sobelFilter(PGMImage* img);
void histogramEqualization(unsigned char** input,int width,int height,char* fileName,int grayLevel);


// PGM dosyasi acar
void loadImage (char fileName[], PGMImage *img)
{
    FILE* fp;
    char ch;
    int x, y;

    fp = fopen(strcat(fileName,".pgm"), "r");
    if (fp == NULL)
    {
        printf("Dosya acilamadi!");
        exit(1);
    }

    while(getc(fp) != '\n');

    (*img).fileName=fileName;

    fscanf(fp,"%d", &((*img).width));
    fscanf(fp,"%d", &((*img).height));
    fscanf(fp,"%d", &((*img).maxVal));

    printf("\n width  = %d",(*img).width);
    printf("\n height = %d",(*img).height);
    printf("\n maxVal = %d",(*img).maxVal);
    printf("\n");

    img->data = (unsigned char**)malloc(sizeof(unsigned char*) * (*img).height);
    int k;
    for(k=0; k<(*img).width; k++)
    {
        img->data[k] = (unsigned char*) malloc(sizeof(unsigned char) * (*img).width);
    }


    while(getc(fp) != '\n');

    for (x=0; x<(*img).height; x++)
    {
        for (y=0; y< (*img).width; y++)
        {
            ch = getc(fp);
            (*img).data[x][y]=ch;
        }
    }


    fclose(fp);
    printf("\nDosya okuma tamamlandi\n");
}

// PGM dosyasi olusturur
void save(PGMImage *img)
{
    int i, j, x, y, pixel;

    FILE *fp;

    x = img->height;
    y = img->width;

    fp = fopen(img->fileName, "w");
    fprintf(fp, "P5 ");
    fprintf(fp, "%d %d ", y, x);
    fprintf(fp, "%d ",255);

    for(i =0; i<x; i++)
    {
        for(j = 0; j <  y; j++)
        {
            pixel = img->data[i][j];
            putc(pixel, fp);
        }
    }
    fprintf(fp, "\n");
    fclose(fp);

    printf("Dosya kaydedildi - %s\n\n",img->fileName);
}

// sobel filtresini uygular
void sobelFilter(PGMImage* img)
{
    int width = img->width;
    int height = img->height;

    double gradient_x,gradient_y;
    int i,j,x,y;

    PGMImage* image;
    image =(PGMImage*) malloc(sizeof(PGMImage));
    image->height=height;
    image->width=width;
    image->maxVal=255;
    image->fileName= strcat(img->fileName," - sobel");

    image->data = (unsigned char**)malloc(sizeof(unsigned char*) * image->height);
    for(i=0; i<image->width; i++)
    {
        image->data[i] = (unsigned char*) malloc(sizeof(unsigned char) * image->width);
    }

    double operator_x[3][3] = {{-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    double operator_y[3][3] = {{-1, -2, -1},
        {0,  0,  0},
        {1,  2,  1}
    };


    // yeni resimde 0 satir ve 0 sutundaki verileri aynı olsun
    for(i=0; i<height; i++)
    {
        (*image).data[i][0] = img->data[i][0];
        (*image).data[0][i] = img->data[0][i];

    }
    // sobel filtresi uygulanarak pixel ler olusturuluyor
    for(i=1; i<height-2; i++)
    {
        for(j=1; j<width-2; j++)
        {
            gradient_x=0.0;
            gradient_y=0.0;
            for(x=0; x<3; x++)
            {
                for(y=0; y<3; y++)
                {
                    gradient_x+=img->data[i+x-1][j+y-1]*operator_x[x][y];
                    gradient_y+=img->data[i+x-1][j+y-1]*operator_y[x][y];
                }
            }
            double filter = sqrt(pow(gradient_x,2)+pow(gradient_y,2));

            (*image).data[i][j]=(unsigned char) (filter);
        }
    }
    printf("Sobel filtresi uygulandi\n");
    save(image);

}

// Histogram esleme yapar
void histogramEqualization(unsigned char** input,int width,int height,char* fileName,int grayLevel)
{
    int i,j;

    unsigned char k;

    int* hist = (int*)calloc(sizeof(int),256);
    double* histeq =  (double*)calloc(sizeof(double),256);
    double* histeq_sum = (double*)calloc(sizeof(double),256);

    PGMImage* image;
    image =(PGMImage*) malloc(sizeof(PGMImage));
    image->height=height;
    image->width=width;
    image->maxVal=255;
    image->fileName= strcat(fileName," - histogramEQ");
    image->data = (unsigned char**)malloc(sizeof(unsigned char*) * image->height);
    for(i=0; i<image->width; i++)
    {
        image->data[i] = (unsigned char*) malloc(sizeof(unsigned char) * image->width);
    }

    // histogram bulunuyor
    for (i=0; i<height; i++)
    {
        for (j=0; j<width; j++)
        {
            hist[input[i][j]]++;
        }
    }

    // intensity bulunuyor
    for (i=0; i<256; i++)
    {
        histeq[i] =(double) hist[i]/(width * height);
    }

    // kumulatif histogram hesaplaniyor
    histeq_sum[0]=histeq[0];

    for(i=1; i<256; i++)
    {
        histeq_sum[i]=histeq_sum[i-1]+histeq[i];
    }

    // normalize edilip histogram esitleme yapiliyor
    for(i=0; i<height; i++)
    {
        for(j=0; j<width; j++)
        {
            k = input[i][j];
            if(grayLevel==2)
            {
                if(round(histeq_sum[k] * 255.0) > 127.0)
                    k=255;
                else
                    k=0;

                image->data[i][j] = k;
            }
            else
                image->data[i][j] = (unsigned char)round(histeq_sum[k] * 255.0);


        }
    }

    printf("Histogram esitleme uygulandi\n");
    save(image);

}

int main()
{

    while(1)
    {
        char tmpfileName[20];
        char* fileName;

        PGMImage* image;
        image =(PGMImage*) malloc(sizeof(PGMImage));

        printf("Resim adi girin\n");
        scanf("%s", tmpfileName);

        fileName = strtok(tmpfileName, "\n");
        loadImage(fileName,image);

        sobelFilter(image);
        histogramEqualization(image->data,image->width,image->height,fileName,2);

        getchar();
    }
    return 0;
}
