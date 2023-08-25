#pragma once
#include "Common.h"
#include <iostream>
#include <vector>

using namespace glm;


struct GradientMap {
    ~GradientMap() { delete[] vectors; }
    int width;
    int height;
    vec2* vectors;
};

class Random
{
public:
    static unsigned int seed;

    static unsigned int RandomUInt() {
        seed ^= seed << 13, seed ^= seed >> 17; seed ^= seed << 5;
        return seed;
    }

    static float RangedRandom(int min, int max) {
        return (float)(max - min) * RandomFloat() + (float)min;
    }

    static uint RangedRandomUint(uint min, uint max) {
        return min + RandomUInt() % ((max + 1) - min);
    }

    static vec2 RandomVector() {
        float x = RangedRandom(-1, 1);
        float y = RangedRandom(-1, 1);
        float length = sqrtf(x * x + y * y);

        return vec2{ x / length, y / length };
    }

    static float RandomFloat() { return (float)RandomUInt() * 2.3283064365387e-10f; }

    static std::string GenerateRandomName(std::vector<std::string>& vector) {
        uint sylNumber = RangedRandomUint(2, 5);

        uint syllable = RangedRandomUint(0, vector.size() - 1);
        assert(syllable < vector.size());
        std::string name = vector[syllable];
        for (uint i = 1; i < sylNumber; i++) {
            name += vector[RangedRandomUint(2, vector.size() - 1)];
        }

        return name;
    }

    static float PerlinWithOctaves(float x, float y, float redistribution, float frequency, float persistence, float granularity, int octaves, GradientMap& map) {

        float value = 0;
        float amplitude = 1.f;

        for (int i = 0; i < octaves; i++) {
            value += amplitude * Perlin(x, y, frequency, map);
            amplitude *= persistence;
            frequency *= granularity;
        }

        value = (float)pow(value, redistribution);
        return value;
    }

    //Returns perlin height at given coordinates
    static float Perlin(float x, float y, float whiteFrequency, GradientMap& map) {
        // Determine grid cell coordinates
        float coordX = x * whiteFrequency;
        float coordY = y * whiteFrequency;
        int gridX0 = (int)floor(coordX);
        int gridX1 = gridX0 + 1;
        int gridY0 = (int)floor(coordY);
        int gridY1 = gridY0 + 1;

        // Determine interpolation weights
        // Could also use higher order polynomial/s-curve here
        float wghtX = coordX - (float)gridX0;
        float wghtY = coordY - (float)gridY0;

        // Interpolate between grid point gradients
        float dot0, dot1, dot2, dot3, lerp0, lerp1, value;

        dot0 = DotGridGradient(gridX0, gridY0, map, coordX, coordY);
        dot1 = DotGridGradient(gridX1, gridY0, map, coordX, coordY);
        dot2 = DotGridGradient(gridX0, gridY1, map, coordX, coordY);
        dot3 = DotGridGradient(gridX1, gridY1, map, coordX, coordY);

        lerp0 = Interpolate(dot0, dot1, wghtX);
        lerp1 = Interpolate(dot2, dot3, wghtX);

        value = Interpolate(lerp0, lerp1, wghtY);
        return value; // Will return in range -1 to 1. To make it in range 0 to 1, multiply by 0.5 and add 0.5
    }

    static GradientMap CreateGradientMap(int width, int height) {
        GradientMap newM;

        newM.width = width;
        newM.height = height;

        newM.vectors = new vec2[width * height];

        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {

                newM.vectors[i + j * width] = RandomVector();
            }
        }
        return newM;
    }

private:


    static float DotGridGradient(int gridX, int gridY, GradientMap& map, float x, float y) {
        // Get gradient from integer coordinates
        assert(gridX + gridY * map.width < map.width * map.height);
        vec2 gradient = map.vectors[gridX + gridY * map.width];

        // Compute the distance vector
        float distX = x - (float)gridX;
        float distY = y - (float)gridY;

        // Compute the dot-product
        return (distX * gradient.x + distY * gradient.y);
    }

    static float Interpolate(float a0, float a1, float w) {
        // You may want clamping by inserting:
        if (0.0 > w) return a0;
        if (1.0 < w) return a1;

        return (a1 - a0) * (3.0f - w * 2.0f) * w * w + a0;
    }
};

