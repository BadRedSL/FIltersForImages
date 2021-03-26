#include <QtCore/QCoreApplication>
#include <QImage>
#include <String>
#include <iostream>
#include "Filter.h"


void main(int argc, char *argv[])
{
    std::string s;
    QImage img;



    for (int i = 0; i < argc; i++)
    {
        if (!strcmp(argv[i], "-p") && (i + 1 < argc))
        {
            s = argv[i+1];
        }
    }

    //std::cout << s;

    img.load(QString(s.c_str()));

    img.save("Images/Source.png");

    InvertFilter invert;
    invert.process(img).save("Images/Invert.png");
    
    BlurFilter blur;
    blur.process(img).save("Images/Blur.png");

    GrayScaleFilter gray;
    gray.process(img).save("Images/Gray.png");

    SepiaFilter sepia;
    sepia.process(img).save("Images/Sepia.png");

    BrightFilter bright;
    bright.process(img).save("Images/Bright.png");

    PerfectReflectorFilter perfect;
    perfect.process(img).save("Images/Perfect.png");

    SharpnessFilter sharpness;
    sharpness.process(img).save("Images/Sharpness.png");

    SobelXFilter sobelx;
    sobelx.process(img).save("Images/Sobelx.png");

    GaussianFilter gaussian;
    gaussian.process(img).save("Images/Gaussian.png");



}
