#include <cstdlib>
#include <cmath>
#include <ctime>
#include "PerlinNoise.h"

PerlinNoise::PerlinNoise() {

    srand(time(NULL));

    _p = new int[256];

    _gX = new float[256];
    _gY = new float[256];
    _gZ = new float[256];

    for(int i = 0; i < 256; i++){
        _p[i] = i;

        _gX[i] = (float(rand()) / (RAND_MAX/2)) - 1.0f;
        _gY[i] = (float(rand()) / (RAND_MAX/2)) - 1.0f;
        _gZ[i] = (float(rand()) / (RAND_MAX/2)) - 1.0f;
    }

    int j;
    int swp;

    for(int i = 0; i < 256; i++){
        j = rand() & 255;
        swp = _p[i];
        _p[i] = _p[j];
        _p[j] = swp;
    }
}

PerlinNoise::~PerlinNoise() {
    delete _p;
    delete _gX;
    delete _gY;
    delete _gZ;
}

float PerlinNoise::noise(float x) {
    int x0 = (int)floorf(x);
    int x1 = x0 + 1;

    float px0 = x - (float)x0;
    float px1 = px0 - 1.0f;

    int gIndex = _p[x0 & 255];
    float dot1 = _gX[gIndex]*px0;
    gIndex = _p[x1 & 255];
    float dot2 = _gX[gIndex]*px1;

    float val = lerp(dot1, dot2, fade(px0));

    return (val + 1.0f)/2.0f;
}

float PerlinNoise::noise(float x, float y) {
    int x0 = (int)floorf(x);
    int x1 = x0 + 1;
    int y0 = (int)floorf(y);
    int y1 = y0 + 1;

    float px0 = x - (float)x0;
    float px1 = px0 - 1.0f;
    float py0 = y - (float)y0;
    float py1 = py0 - 1.0f;

    int gIndex = _p[(x0 + _p[y0 & 255]) & 255];
    float dot00 = _gX[gIndex]*px0 + _gY[gIndex]*py0;
    gIndex = _p[(x0 + _p[y1 & 255]) & 255];
    float dot01 = _gX[gIndex]*px0 + _gY[gIndex]*py1;

    gIndex = _p[(x1 + _p[y0 & 255]) & 255];
    float dot10 = _gX[gIndex]*px1 + _gY[gIndex]*py0;
    gIndex = _p[(x1 + _p[y1 & 255]) & 255];
    float dot11 = _gX[gIndex]*px1 + _gY[gIndex]*py1;

    float u = fade(px0);
    float v = fade(py0);

    float xI1 = lerp(dot00, dot10, u);
    float xI2 = lerp(dot01, dot11, u);
    float val = lerp(xI1, xI2, v);

    return (val + 1.0f)/2.0f;
}

float PerlinNoise::noise(float x, float y, float z) {
    int x0 = (int)floorf(x);
    int x1 = x0 + 1;
    int y0 = (int)floorf(y);
    int y1 = y0 + 1;
    int z0 = (int)floorf(z);
    int z1 = z0 + 1;

    float px0 = x - (float)x0;
    float px1 = px0 - 1.0f;
    float py0 = y - (float)y0;
    float py1 = py0 - 1.0f;
    float pz0 = z - (float)z0;
    float pz1 = pz0 -1.0f;

    int gIndex = _p[(x0 + _p[(y0 + _p[z0 & 255]) & 255]) & 255];
    float dot000 = _gX[gIndex]*px0 + _gY[gIndex]*py0 + _gZ[gIndex]*pz0;

    gIndex = _p[(x0 + _p[(y0 + _p[z1 & 255]) & 255]) & 255];
    float dot001 = _gX[gIndex]*px0 + _gY[gIndex]*py0 + _gZ[gIndex]*pz1;

    gIndex = _p[(x0 + _p[(y1 + _p[z0 & 255]) & 255]) & 255];
    float dot010 = _gX[gIndex]*px0 + _gY[gIndex]*py1 + _gZ[gIndex]*pz0;

    gIndex = _p[(x0 + _p[(y1 + _p[z1 & 255]) & 255]) & 255];
    float dot011 = _gX[gIndex]*px0 + _gY[gIndex]*py1 + _gZ[gIndex]*pz1;

    gIndex = _p[(x1 + _p[(y0 + _p[z0 & 255]) & 255]) & 255];
    float dot100 = _gX[gIndex]*px1 + _gY[gIndex]*py0 + _gZ[gIndex]*pz0;

    gIndex = _p[(x1 + _p[(y0 + _p[z1 & 255]) & 255]) & 255];
    float dot101 = _gX[gIndex]*px1 + _gY[gIndex]*py0 + _gZ[gIndex]*pz1;

    gIndex = _p[(x1 + _p[(y1 + _p[z0 & 255]) & 255]) & 255];
    float dot110 = _gX[gIndex]*px1 + _gY[gIndex]*py1 + _gZ[gIndex]*pz0;

    gIndex = _p[(x1 + _p[(y1 + _p[z1 & 255]) & 255]) & 255];
    float dot111 = _gX[gIndex]*px1 + _gY[gIndex]*py1 + _gZ[gIndex]*pz1;

    float u = fade(px0);
    float v = fade(py0);
    float w = fade(pz0);

    float xI1 = lerp(dot000, dot100, u);
    float xI2 = lerp(dot010, dot110, u);
    float xI3 = lerp(dot001, dot101, u);
    float xI4 = lerp(dot011, dot111, u);

    float yI1 = lerp(xI1, xI2, v);
    float yI2 = lerp(xI3, xI4, v);

    float val = lerp(yI1, yI2, w);

    return (val + 1.0f)/2.0f;
}

float PerlinNoise::octaveNoise(float x, int octaves, float persistence) {
    float total = 0;
    float frequency = 1;
    float amplitude = 1;
    float maxValue = 0;  // Used for normalizing result to 0.0 - 1.0
    for(int i=0;i<octaves;i++) {
        total += noise(x * frequency) * amplitude;

        maxValue += amplitude;

        amplitude *= persistence;
        frequency *= 2;
    }

    return total/maxValue;
}

float PerlinNoise::octaveNoise(float x, float y, int octaves, float persistence) {
    float total = 0;
    float frequency = 1;
    float amplitude = 1;
    float maxValue = 0;  // Used for normalizing result to 0.0 - 1.0
    for(int i=0;i<octaves;i++) {
        total += noise(x * frequency, y * frequency) * amplitude;

        maxValue += amplitude;

        amplitude *= persistence;
        frequency *= 2;
    }

    return total/maxValue;
}

float PerlinNoise::octaveNoise(float x, float y, float z, int octaves, float persistence) {
    float total = 0;
    float frequency = 1;
    float amplitude = 1;
    float maxValue = 0;  // Used for normalizing result to 0.0 - 1.0
    for(int i=0;i<octaves;i++) {
        total += noise(x * frequency, y * frequency, z * frequency) * amplitude;

        maxValue += amplitude;

        amplitude *= persistence;
        frequency *= 2;
    }

    return total/maxValue;
}

// Linear interpolation
float PerlinNoise::lerp(float point1, float point2, float alpha) {
    return point1 + alpha * (point2 - point1);
}

// Fade function as defined by Ken Perlin.  This eases coordinate values
// so that they will "ease" towards integral values.  This ends up smoothing
// the final output.
float PerlinNoise::fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);// 6t^5 - 15t^4 + 10t^3
}
