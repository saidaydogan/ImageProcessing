#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <sys/types.h>
#include <dirent.h>


typedef struct ImageDataStruct
{
    int** corrolegram;
} ImageData;

typedef struct ImageStruct
{
    char* fileName;
    int maxVal;
    int width;
    int height;
    ImageData** data;
} Image;

typedef struct
{
    int r;
    int g;
    int b;
} RGB;

RGB getValue(uchar* data, int i, int j,int step)
{
    RGB val;

    val.b = data[i*step+j*3+ 0];
    val.g = data[i*step+j*3+ 1];
    val.r=data[i*step+j*3+ 2];

    return val;

}

int** corrolegram(uchar* data,int height,int width, RGB* means,int step)
{
    int koroTablo[256][3];

    int i,j,k;


    for (k = 0; i < 256; i++)
    {
        for (i = 0; i < height; i++)
        {
            for (j = 0; j < width; j++)
            {

                int b = data[i*step+j*3+ 0];
                int g = data[i*step+j*3+ 1];
                int r = data[i*step+j*3+ 2];

                if(means[k].r==r && means[k].g==g && means[k].b==b)
                {

                    int fark=1;
                    int x=0;

                    while(fark<6)
                    {

                        // yukarý
                        if(i+fark>=0 && i+fark < height)
                        {
                            int n=i+fark;
                            RGB item = getValue(data,i,j,step);

                            if(means[k].r==item.r && means[k].g==item.g && means[k].b==item.b)
                                koroTablo[k][x]++;
                        }

                        // asagi
                        if(i-fark>=0 && i-fark < height)
                        {
                            int n=i-fark;
                            RGB item = getValue(data,i,j,step);

                            if(means[k].r==item.r && means[k].g==item.g && means[k].b==item.b)
                                koroTablo[k][x]++;
                        }

                        // saga
                        if(j+fark>=0 && j+fark < height)
                        {
                            RGB item = getValue(data,i,j+fark,step);

                            if(means[k].r==item.r && means[k].g==item.g && means[k].b==item.b)
                                koroTablo[k][x]++;
                        }

                        // sola
                        if(j-fark>=0 && j-fark < height)
                        {
                            int n=i-fark;
                            RGB item = getValue(data,i,j-fark,step);

                            if(means[k].r==item.r && means[k].g==item.g && means[k].b==item.b)
                                koroTablo[k][x]++;
                        }

                        // diag
                        if(i+fark<height && j+fark < height && j+fark>=0 && i+fark < height)
                        {
                            RGB item = getValue(data,i+fark,j+fark,step);

                            if(means[k].r==item.r && means[k].g==item.g && means[k].b==item.b)
                                koroTablo[k][x]++;
                        }

                        // diag
                        if(i-fark>=0 && j+fark < height)
                        {
                            RGB item = getValue(data,i-fark,j+fark,step);

                            if(means[k].r==item.r && means[k].g==item.g && means[k].b==item.b)
                                koroTablo[k][x]++;
                        }

                        // diag
                        if(i-fark>=0 && j-fark>=0)
                        {
                            int n=i+fark;
                            RGB item = getValue(data,i-fark,j-fark,step);

                            if(means[k].r==item.r && means[k].g==item.g && means[k].b==item.b)
                                koroTablo[k][0]++;
                        }

                        // diag
                        if(i+fark<height && j-fark >=0)
                        {
                            int n=i-fark;
                            RGB item = getValue(data,i+fark,j-fark,step);

                            if(means[k].r==item.r && means[k].g==item.g && means[k].b==item.b)
                                koroTablo[k][0]++;
                        }

                        x++;
                        fark = fark + 2;
                    }

                }

            }
        }
    }

    return koroTablo;
}
//uchar* imageCopy(IplImage* img)
//{
//     for (i = 0; i < img->height; i++)
//        {
//            for (j = 0; j < img->width; j++)
//            {
//                int uzakliklar[256];
//
//                // opencv b-g-r olarak okuyor
//                int b = data[i*step+j*3+ 0];
//                int g = data[i*step+j*3+ 1];
//                int r = data[i*step+j*3+ 2];
//
//            }
//        }
//
//}


int** kMeans(int N, int K,  char* filename)
{

    int** koroTablo;

    srand((unsigned)time(0));
    RGB* eskiMeans = (RGB*)malloc(sizeof(RGB) * 256);
    RGB* yeniMeans = (RGB*)malloc(sizeof(RGB) * 256);

    int i,j,k;
    int meanOncesiR[256][256];
    int meanOncesiG[256][256];
    int meanOncesiB[256][256];

    for (i = 0; i < N; i++)
    {
        int r  = rand();
        int g  = rand();
        int b  = rand();

        yeniMeans[i].r = r % 256;
        yeniMeans[i].g = g % 256;
        yeniMeans[i].b = b % 256;


//        printf("Mean%d  -> %d %d %d\n",i , yeniMeans[i].r,yeniMeans[i].g,  yeniMeans[i].b);
    }
    printf("--RANDOM BITTI--");

    IplImage* img = cvLoadImage(filename,CV_LOAD_IMAGE_COLOR);
    uchar* data = (uchar *)img->imageData;
    int step= img->widthStep;

    int uzunluklar[256];

    int degisimCok = 1;
    int m =0;
    int donguSayisi=0;

    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 1; j++)
        {
            int uzakliklar[256];


            int b = data[i*step+j*3+ 0];
            int g = data[i*step+j*3+ 1];
            int r = data[i*step+j*3+ 2];
            printf("%d %d %d\n",b,g,r);

        }
    }

    printf("--RESIM BITTI--");


    do
    {
        donguSayisi++;
        for (i = 0; i < 256; i++)
        {
            meanOncesiR[i][0]=yeniMeans[i].r;
            meanOncesiG[i][0]=yeniMeans[i].g;
            meanOncesiB[i][0]=yeniMeans[i].b;

            uzunluklar[i] = 1;

        }

        for (i = 0; i < N; i++)
        {
            eskiMeans[i].r = yeniMeans[i].r;
            eskiMeans[i].g = yeniMeans[i].g;
            eskiMeans[i].b = yeniMeans[i].b;
//            printf("Mean%d  -> %d %d %d\n",i , yeniMeans[i].r,yeniMeans[i].g,  yeniMeans[i].b);
        }






        for (i = 0; i < img->height; i++)
        {
            for (j = 0; j < img->width; j++)
            {
                int uzakliklar[256];

                // opencv b-g-r olarak okuyor
                int b = data[i*step+j*3+ 0];
                int g = data[i*step+j*3+ 1];
                int r = data[i*step+j*3+ 2];



                int min = 1000;
                int minIndex;
                RGB minRGB;

                for (k = 0; k < 256; k++)
                {
                    int uzaklik = abs(yeniMeans[k].r - r) + abs(yeniMeans[k].g - g) + abs(yeniMeans[k].b - b);

                    if (uzaklik < min)
                    {
                        minIndex = k;
                        min=uzaklik;
                        minRGB.r = r;
                        minRGB.g = g;
                        minRGB.b = b;
                        data[i*step+j*3+ 0]=yeniMeans[k].b;
                        data[i*step+j*3+ 1]=yeniMeans[k].g;
                        data[i*step+j*3+ 2]=yeniMeans[k].r;

                    }

                }
                meanOncesiR[minIndex][uzunluklar[minIndex]] = minRGB.r;
                meanOncesiG[minIndex][uzunluklar[minIndex]] = minRGB.g;
                meanOncesiB[minIndex][uzunluklar[minIndex]] = minRGB.b;
                uzunluklar[minIndex]++;

            }
        }


        for (i = 0; i < 256; i++)
        {
            int rToplam = 0;
            int gToplam = 0;
            int bToplam = 0;

            if(uzunluklar[i]<1)
                continue;

            for (j = 0; j < uzunluklar[i]; j++)
            {
                rToplam += meanOncesiR[i][j];
                gToplam += meanOncesiG[i][j];
                bToplam += meanOncesiB[i][j];
            }


//            printf("%d %d %d\n",rToplam,gToplam,bToplam);

            yeniMeans[i].r = rToplam / uzunluklar[i];
            yeniMeans[i].g = gToplam / uzunluklar[i];
            yeniMeans[i].b = bToplam / uzunluklar[i];
        }


        m = 0;
        int hassaslik = 10;

        while (m<256)
        {
            int fark = abs(yeniMeans[m].r - eskiMeans[m].r) + abs(yeniMeans[m].g - eskiMeans[m].g) + abs(yeniMeans[m].b - eskiMeans[m].b);

            if (fark < hassaslik)
            {
                degisimCok = 0;
            }
            else
            {
                degisimCok = 1;
                break;
            }
            m++;
        }

    }
    while (m<256 || degisimCok==1);
//    printf("%d",donguSayisi);

//    for (i = 0; i < img->height; i++)
//    {
//        for (j = 0; j < img->width; j++)
//        {
//            int uzakliklar[256];
//
//
//            int b = data[i*step+j*3+ 0];
//            int g = data[i*step+j*3+ 1];
//            int r = data[i*step+j*3+ 2];
//            printf("%d %d %d\n",b,g,r);
//
//        }
//    }

    koroTablo = corrolegram(data,img->height,img->width,yeniMeans,step);

    return koroTablo;

}

int loadTrainData()
{
    int i,j,k;

    char fileNames[59][100];

    DIR *dp;
    struct dirent *ep;
    dp = opendir ("./TRAIN");
    i=0;
    if (dp != NULL)
    {
        while (ep = readdir (dp))
        {
//            char* string = ep->d_name;
            if (strstr(ep->d_name, ".bmp") != NULL)
            {

                memset(fileNames[i], '\0', sizeof(ep->d_name));
                strcpy(fileNames[i], ep->d_name);
                i++;
            }
        }
        (void) closedir (dp);
    }

    int imageCount = i;

    IplImage* img = 0;
    int height,width,step,channels;
    uchar *data;


    Image** images = (Image**) malloc(sizeof(Image*) * 56);

    ImageData** imageKrlgm = (ImageData**) malloc(sizeof(ImageData*) * 56);
    char folder[100];
    strcpy(folder,"TRAIN\\");



    for(i=0; i<imageCount; i++)
    {
        images[i]= (Image*) malloc(sizeof(Image));
        imageKrlgm[i]= (ImageData*) malloc(sizeof(imageKrlgm));



        strcat(folder,fileNames[i]);
        printf("%s - %d\n",fileNames[i], i);

        img=cvLoadImage(folder,CV_LOAD_IMAGE_COLOR);
        if(!img)
        {
            printf("Could not load image file:\n");
            exit(0);
        }


        // 256 - 3
        imageKrlgm[i]->corrolegram=kMeans(256,256,folder);




    }
}


int main()
{

//    loadTrainData();
//    getch();
//    return 0;

//    kMeans(256,256,"TRAIN\\000.bmp");

    IplImage* img = 0;
    int height,width,step,channels;
    uchar *data;
    int i,j,k;


    // load an image
    img=cvLoadImage("TRAIN\\000.bmp",CV_LOAD_IMAGE_COLOR);
    if(!img)
    {
        printf("Could not load image file:\n");
        exit(0);
    }
    //

    // get the image data
    height    = img->height;
    width     = img->width;
    step      = img->widthStep;
    channels  = img->nChannels;
    data      = (uchar *)img->imageData;
    printf("Processing a %dx%d image with %d channels\n",height,width,channels);

    // create a window
    cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE);
    cvMoveWindow("mainWin", 100, 100);

    // invert the image
    for (i = 0; i < img->height; i++)
    {
        for (j = 0; j < img->width; j++)
        {
            int uzakliklar[256];


            int b = data[i*step+j*3+ 0];
            int g = data[i*step+j*3+ 1];
            int r = data[i*step+j*3+ 2];
            printf("%d %d %d\n",b,g,r);

        }
    }

    // show the image
    cvShowImage("mainWin", img );

    // wait for a key
    cvWaitKey(0);

    // release the image
    cvReleaseImage(&img );
    return 0;
}
