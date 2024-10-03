#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "./headers/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./headers/stb_image_write.h"

unsigned char *uc_arrayNew_1d(int _size)
{
    return (unsigned char *)calloc(_size, sizeof(unsigned char));
}

unsigned char *cvtGray(unsigned char *img, int wImg, int hImg, int cImg)
{
    unsigned char *grayImg = uc_arrayNew_1d(wImg * hImg);
    float red, green, blue, sum;

    for (int i = 0; i < hImg; i++)
    {
        for (int j = 0; j < wImg; j++)
        {
            sum = 0;
            red = img[i * wImg * cImg + j * cImg];
            green = img[i * wImg * cImg + j * cImg + 1];
            blue = img[i * wImg * cImg + j * cImg + 2];
            grayImg[i * wImg + j] = 0.28 * red + 0.32 * green + 0.40 * blue;
        }
    }
    return grayImg;
}

void saveTemplate2Gray()
{
    // for template image
    int wTpl, hTpl, cTpl;

    char path_template[] = "template.jpg";
    unsigned char *template = stbi_load(path_template, &wTpl, &hTpl, &cTpl, 0);

    if (template == NULL)
    {
        printf("\nError in loading the image\n");
        exit(1);
    }
    printf("%d %d\n", wTpl, hTpl);

    unsigned char *template1D = cvtGray(template, wTpl, hTpl, cTpl);
    // save image
    char save_template_path[] = "template_gray/template_gray0.png";
    stbi_write_png(save_template_path, wTpl, hTpl, 1, template1D, wTpl);
}
//convert all images into gray
void convertAll2Gray()
{
    int wImg, hImg, cImg;

    char path_img[] = "template.jpg";
    unsigned char *imgTest;

    char fileName[1000];
    char fileNameSave[1000];

    for (int i = 0; i < 63; i++)
    {
        sprintf(fileName, "./images/img%d.jpg", i);
        unsigned char *img = stbi_load(fileName, &wImg, &hImg, &cImg, 0);

        if (img == NULL)
        {
            printf("\nError in loading the image\n");
            exit(1);
        }
        unsigned char *imgGray = cvtGray(img, wImg, hImg, cImg);

        sprintf(fileNameSave, "./images_gray/img%d.png", i);
        stbi_write_png(fileNameSave, wImg, hImg, 1, imgGray, wImg);
    }
}

int main()
{
    saveTemplate2Gray();
    convertAll2Gray();
}