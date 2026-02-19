#include <stdint.h>

union rgb565 {
	struct rgb {
		uint16_t r : 5;
		uint16_t g : 6;
		uint16_t b : 5;
	} rgb;
	uint16_t data;

    rgb565(){
        data = 0;
    }
    rgb565(const uint16_t r, const uint16_t g, const uint16_t b){
        rgb.r = r; rgb.g = g; rgb.b = b;
    }
};

struct linRGB {
    float R;
    float G;
    float B;
};

struct Msh {
    float M;
    float s;
    float h;
};

linRGB invsRGBCompanding(rgb565 sRGB);
rgb565 sRGBCompanding(linRGB RGB);
Msh RGB2Msh(linRGB RGB);
linRGB Msh2RGB(Msh msh);
Msh interpolateColor(Msh low, Msh high, float x);
