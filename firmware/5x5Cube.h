#ifndef _L3D_H
#define _L3D_H

#include "neopixel/neopixel.h"
#include "application.h"

#define PIXEL_COUNT 125
#define PIXEL_PIN D0
#define PIXEL_TYPE WS2812B

// An RGB color.
typedef struct // struct Color
{
    unsigned char red, green, blue;
    // Color(int r, int g, int b) : red(r), green(g), blue(b) {}
    // Color() : red(0), green(0), blue(0) {}
} color;

// A point in 3D space.
typedef struct // struct Point
{
    int x;
    int y;
    int z;
    // Point() : x(0), y(0), z(0) {}
    // Point(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
} point;

// Overloaded != operator.
// bool operator!= (const Color& a, const Color& b);

// Overloaded == operator.
// bool operator== (const Color& a, const Color& b);

class Cube
{
    private:
        Adafruit_NeoPixel strip;

    public:
        int size;
        int maxBrightness;
        point center;
        float theta, phi;
        Cube(unsigned int s, unsigned int mb);
        Cube(void);

        void setVoxel(int x, int y, int z, color* col);
        void setVoxel(point p, color* col);
        void setPixel(uint16_t p, color* col);
        color getVoxel(int x, int y, int z);
        // color getVoxel(point p);
        // void line(int x1, int y1, int z1, int x2, int y2, int z2, color* col);
        // void line(point p1, point p2, color* col);
        void background(color col);

        color colorMap(float val, float min, float max);
        color lerpColor(color* a, color* b, int val, int min, int max);

        void begin(void);
        void show(void);
};

// common colors
const color black     = {0x00, 0x00, 0x00};
const color grey      = {0x92, 0x95, 0x91};
const color yellow    = {0xff, 0xff, 0x14};
const color magenta   = {0xc2, 0x00, 0x78};
const color orange    = {0xf9, 0x73, 0x06};
const color teal      = {0x02, 0x93, 0x86};
const color red       = {0xe5, 0x00, 0x00};
const color brown     = {0x65, 0x37, 0x00};
const color pink      = {0xff, 0x81, 0xc0};
const color blue      = {0x03, 0x43, 0xdf};
const color green     = {0x15, 0xb0, 0x1a};
const color purple    = {0x7e, 0x1e, 0x9c};
const color white     = {0xff, 0xff, 0xff};

#endif
