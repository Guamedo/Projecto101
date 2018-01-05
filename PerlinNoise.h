#ifndef PROJECT101_PERLINNOISE_H
#define PROJECT101_PERLINNOISE_H

#include <array>

class PerlinNoise {
public:

    PerlinNoise();
    ~PerlinNoise();

    float noise(float x);
    float noise(float x, float y);
    float noise(float x, float y, float z);

    float octaveNoise(float x, int octaves, float persistence);
    float octaveNoise(float x, float y, int octaves, float persistence);
    float octaveNoise(float x, float y, float z, int octaves, float persistence);

private:

    float lerp(float point1, float point2, float alpha);
    float fade(float t);

    int *_p;
    float *_gX;
    float *_gY;
    float *_gZ;
};


#endif //PROJECT101_PERLINNOISE_H
