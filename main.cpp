#include <QtCore/QCoreApplication>
#include <QImage>
#include <String>
#include <iostream>
#include "Filter.h"


void main(int argc, char *argv[])
{
    std::string s;
    QImage img, binaryimage;



    for (int i = 0; i < argc; i++)
    {
        if (!strcmp(argv[i], "-p") && (i + 1 < argc))
        {
            s = argv[i+1];
        }
    }

    //std::cout << s;

    img.load(QString(s.c_str()));
    binaryimage.load(QString("D:\\random\\Pictures\\Binary.PNG"));

    binaryimage.save(QString("Images/BinarySourse.png"));
    img.save("Images/Source.png");

    //Common filters

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

    PerfectReflectorFilter perfectreflector;
    perfectreflector.process(img).save("Images/Perfectreflector.png");

    WaveFilter wave;
    wave.process(img).save("Images/Wave.png");

    CarryoverFilter carryover;
    carryover.process(img).save("Images/Carryover.png");

    StretchingHistogramFilter histogram;
    histogram.process(img).save("Images/Historam.png");

    MedianFilter median;
    median.process(binaryimage).save("Images/Median.png");

    //Matrix filters

    SharpnessFilter sharpness;
    sharpness.process(img).save("Images/Sharpness.png");

    GaussianFilter gaussian;
    gaussian.process(img).save("Images/Gaussian.png");

    NewSharpnessFilter newsharpness;
    newsharpness.process(img).save("Images/Newsharpness.png");

    EmbossingFilter embossing;
    embossing.process(img).save("Images/Embossing.png");

    SobelXFilter sobelx;
    sobelx.process(img).save("Images/Sobelx.png");

    SobelFilter sobel;
    sobel.process(img).save("Images/Sobel.png");

    Erosion erosia;
    erosia.process(binaryimage).save("Images/Erosion.png");
    
    Dilation dilatia;
    dilatia.process(binaryimage).save("Images/Dilation.png");
    
    Opening openpicture;
    openpicture.process(binaryimage).save("Images/Openpicture.png");

    Closing closepicture;
    closepicture.process(binaryimage).save("Images/Closepicture.png");

    Grad gradient;
    gradient.process(binaryimage).save("Images/Gradient.png");

}
