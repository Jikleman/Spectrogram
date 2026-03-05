#include "color.h"
#include <cmath>

#define Rmax    31  //Maximum R Value for RGB565
#define Gmax    63  //Maximum G Value for RGB565
#define Bmax    31  //Maximum B Value for RGB565

//D65 values for sRGB
#define Xn      0.95047f
#define Yn      1.0f
#define Zn      1.08883f

#define MATH_PI 3.14159265358979323846264338327950288419716939937510f

//Function to convert sRGB component to linear RGB
float sRGBComp2LinComp(float x){
    if (x <= 0.04045f)
        return x / 12.92f;
    return powf((x+0.055f)/1.055f,2.4f);
}

//Converts sRGB (rgb565) to linear RGB
linRGB invsRGBCompanding(rgb565 sRGB){
    linRGB linRGB;
    linRGB.R = sRGBComp2LinComp(sRGB.rgb.r / (float) Rmax);
    linRGB.G = sRGBComp2LinComp(sRGB.rgb.g / (float) Gmax);
    linRGB.B = sRGBComp2LinComp(sRGB.rgb.b / (float) Bmax);
    return linRGB;
}

//Function to convert linear RGB component to sRGB
float linComp2sRGBComp(float x){
    if (x <= 0.0031308f)
        return 12.92f * x;
    return 1.055f * powf(x,1/2.4f) - 0.055f;
}

//Clamps the value of x between 0.0 and 1.0
float clamp(float low, float high, float x){
    return fmaxf(low, fminf(high, x));
}

//Converts linear RGB to sRGB (RGB565)
rgb565 sRGBCompanding(linRGB RGB){

    RGB.R = linComp2sRGBComp(clamp(0.0f, 1.0f, RGB.R)) * Rmax;
    RGB.G = linComp2sRGBComp(clamp(0.0f, 1.0f, RGB.G)) * Gmax;
    RGB.B = linComp2sRGBComp(clamp(0.0f, 1.0f, RGB.B)) * Bmax;

    rgb565 sRGB((uint16_t) (RGB.R + 0.5f), (uint16_t) (RGB.G + 0.5f), (uint16_t) (RGB.B + 0.5f));

    return sRGB;
}

//Helper function to get the necessary values to convert from XYZ to LAB.
float XYZ2Lab_nonlinearFunc(float x){
    if (x > 0.008856f)
        return cbrtf(x);
    return 7.787f*x + 16.0f/116.0f;
}

//Converts from Linear RGB to Msh.
Msh RGB2Msh(linRGB RGB){
    float X = 0.4124564f*RGB.R + 0.3575761f*RGB.G + 0.1804375f*RGB.B;
    float Y = 0.2126729f*RGB.R + 0.7151522f*RGB.G + 0.0721750f*RGB.B;
    float Z = 0.0193339f*RGB.R + 0.1191920f*RGB.G + 0.9503041f*RGB.B;

    float fx = XYZ2Lab_nonlinearFunc(X/Xn);
    float fy = XYZ2Lab_nonlinearFunc(Y/Yn);
    float fz = XYZ2Lab_nonlinearFunc(Z/Zn);

    float L = 116*(fy - 16/116.0f);
    float a = 500*(fx - fy);
    float b = 200*(fy - fz);

    float M = sqrtf(L*L + a*a + b*b);
    float s = acosf(L/M);
    float h = atan2f(b, a);

    return Msh{M,s,h};
}

//Helper function to get the necesary values to convert from LAB to XYZ
float LAB2XYZ_nonlinearFunc(float x){
    float res = powf(x,3);
    if (res > 0.008856f)
        return res;
    return (116*x - 16) / 903.3f;
}

linRGB Msh2RGB(Msh msh){
    float L = msh.M*cosf(msh.s);
    float a = msh.M*sinf(msh.s)*cosf(msh.h);
    float b = msh.M*sinf(msh.s)*sinf(msh.h);

    float fy = (L + 16) / 116.0f;
    float fx = a/500.0f + fy;
    float fz = fy - b/200.0f;

    float X = LAB2XYZ_nonlinearFunc(fx) * Xn;
    float Y = LAB2XYZ_nonlinearFunc(fy) * Yn;
    float Z = LAB2XYZ_nonlinearFunc(fz) * Zn;

    float R =  (3.2404542f*X - 1.5371385f*Y - 0.4985314f*Z);
    float G = (-0.9692660f*X + 1.8760108f*Y + 0.0415560f*Z);
    float B =  (0.0556434f*X - 0.2040259f*Y + 1.0572252f*Z);

    return linRGB{R,G,B};
}

float radDiff(float thetaX, float thetaY){
    return fabsf(thetaX-thetaY);
}

//Adjusts the hue to be away from 0 except for purple
float adjustHue(float unsatM, Msh sat){
    if (sat.M >= unsatM)
        return sat.h;

    float h_spin = (sat.s * sqrtf(unsatM*unsatM - sat.M*sat.M)) / (sat.M * sinf(sat.s));
    if (sat.h > (-MATH_PI/3))
        return sat.h + h_spin;
    return sat.h - h_spin;
}

//Does not have the placing white in the middle, just interpolates the colors.
Msh interpolateColor(Msh low, Msh high, float x){

    if (low.s < 0.05f && high.s > 0.05f){
        low.h = adjustHue(low.M, high);
    } else if (high.s < 0.05f && low.s > 0.05f){
        high.h = adjustHue(high.M, low);
    }

    Msh res;
    res.M = (1.0f-x)*low.M + x*high.M;
    res.s = (1.0f-x)*low.s + x*high.s;
    res.h = (1.0f-x)*low.h + x*high.h;
    return res;
}
