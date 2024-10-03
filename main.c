#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "./headers/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./headers/stb_image_write.h"

unsigned char *uc_arrayNew_1d(int _size)
{
    return (unsigned char *)calloc(_size, sizeof(unsigned char));
}

// draw bounding box around the domain image that look similar to template
void draw(float *cosineImg, float max, int hTpl, int wTpl, int wImg, int hImg, int hLen, int wLen, char outputName[1000], char templateName[1000], unsigned char *orginalImg, unsigned char *img)
{
    unsigned char *new_temp = uc_arrayNew_1d(hTpl * wTpl);
    for (int i = 0; i < hLen; i++)
    {
        for (int j = 0; j < wLen; j++)
        {
            if (cosineImg[i * wLen + j] == max)
            {
                for (int k = i; k < i + hTpl; k++)
                {
                    for (int m = j; m < j + wTpl; m++)
                    {
                        new_temp[(k - i) * wTpl + (m - j)] = img[k * wImg + m];
                        if (k - i == 0 || m - j == 0 || k == i + hTpl - 1 || m == j + wTpl - 1)
                        {
                            // for color image
                            orginalImg[k * wImg * 3 + m * 3] = 254;
                            orginalImg[k * wImg * 3 + m * 3 + 1] = 251;
                            orginalImg[k * wImg * 3 + m * 3 + 2] = 45;
                            // for gray image
                            // img[k*wImg+j] = 255;
                        }
                    }
                }
            }
        }
    }
    stbi_write_png(outputName, wImg, hImg, 3, orginalImg, wImg * 3);
    stbi_write_png(templateName, wTpl, hTpl, 1, new_temp, wTpl);
    printf("New image saved to %s\n", outputName);
}
//compute cosine similarity
void cosine_similarity(unsigned char *template, unsigned char *img, int wTpl, int hTpl, int wImg, int hImg, char outputName[1000], char templateName[1000], unsigned char *orginalImg)
{
    // unsigned char* temp;
    int wLen = wImg - wTpl + 1;
    int hLen = hImg - hTpl + 1;
    float cosineImg[hLen * wLen];
    float n, x, y;
    float max = 0.0;

    // slide template over the source images
    // calculate cosine similarity
    // find the maximum value of cosine similarity to find the domain that are exactly the same with template
    // update template to that maximum value after every matching

    for (int i = 0; i < hLen; i+=20)
    {
        for (int j = 0; j < wLen; j+=20)
        {
            n = 0.0;
            x = 0.0;
            y = 0.0;

            for (int k = i; k < i + hTpl; k+=20)
            {
                for (int m = j; m < j + wTpl; m+=20)
                {
                    double imgVal = img[k * wImg + m];
                    double tplVal = template[(k - i) * wTpl + (m - j)];
                    n += (double)imgVal * (double)tplVal;

                    x += template[(k - i) * wTpl + (m - j)]*template[(k - i) * wTpl + (m - j)];
                    y += img[k * wImg + m]*img[k * wImg + m];
                }
            }

            cosineImg[i * wLen + j] = n / (sqrt(x) * sqrt(y));
            if (n / (sqrt(x) * sqrt(y)) > max)
                max = n / (sqrt(x) * sqrt(y));
        }
    }
    draw(cosineImg, max, hTpl, wTpl, wImg, hImg, hLen, wLen, outputName, templateName, orginalImg, img);
}

int main()
{
    // printf("1\n");
    int wTpl, hTpl, cTpl;
    char path_template[] = "./template_gray/template_gray0.png";
    unsigned char *template = stbi_load(path_template, &wTpl, &hTpl, &cTpl, 0);

    int wImg, hImg, cImg;
    char fileName[1000];
    char file[1000];
    char outputName[1000];
    char templateName[1000];

    for (int i = 0; i < 63; i++)
    {
        sprintf(fileName, "./images_gray/img%d.png", i);
        sprintf(templateName, "./template_gray/template_gray%d.png", i + 1);
        sprintf(file, "./images/img%d.jpg", i);

        if (template == NULL)
        {
            printf("\nError in loading the image\n");
            exit(1);
        }
        unsigned char *imgGray = stbi_load(fileName, &wImg, &hImg, &cImg, 0);
        unsigned char *orginalImg = stbi_load(file, &wImg, &hImg, &cImg, 0);

        if (imgGray == NULL)
        {
            printf("\nError in loading the image\n");
            exit(1);
        }

        sprintf(outputName, "./output/img%d.png", i);
        cosine_similarity(template, imgGray, wTpl, hTpl, wImg, hImg, outputName, templateName, orginalImg);
        template = stbi_load(templateName, &wTpl, &hTpl, &cTpl, 0);
        // stbi_write_png(outputName, wImg, hImg,1, img, wImg);
    }
}