#include "PerlinNoise.hpp"
#include <cmath>

PerlinNoise::PerlinNoise() {
  // Populate the permutations using the reference vector used by Perlin in his original code
  permutations = {151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
                  8,99,37,240,21,10,23,190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
                  35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,
                  134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
                  55,46,245,40,244,102,143,54, 65,25,63,161,1,216,80,73,209,76,132,187,208, 89,
                  18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,
                  250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
                  189,28,42,223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167,
                  43,172,9,129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,
                  97,228,251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,
                  107,49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
                  138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180 };

  // Duplicate the permutations
  permutations.insert(permutations.end(), permutations.begin(), permutations.end());
}

double PerlinNoise::noise(double x, double y, double z) {
  // Finding the unit cube containing the point by safely flooring x,y,z
  int X = (int) floor(x) & 255;
  int Y = (int) floor(y) & 255;
  int Z = (int) floor(z) & 255;

  // Getting only the decimal part of the point to find it's relative position
  // with respect to the unit cube
  x -= floor(x);
  y -= floor(y);
  z -= floor(z);

  double u = fade(x);
  double v = fade(y);
  double w = fade(z);

  // Hashing the coordinates of the cube corners
  int A = permutations[X] + Y;
  int AA = permutations[A] + Z;
  int AB = permutations[A + 1] + Z;
  int B = permutations[X + 1] + Y;
  int BA = permutations[B] + Z;
  int BB = permutations[B + 1] + Z;

  // Add blended results from the corners
  double res = lerp(
          w,
          lerp(v,
                  lerp(u, grad(permutations[AA], x, y, z), grad(permutations[AB], x - 1, y, z)),
                  lerp(u, grad(permutations[BA], x, y - 1, z), grad(permutations[BB], x - 1, y - 1, z))),
          lerp(v,
                  lerp(u, grad(permutations[AA + 1], x, y, z - 1), grad(permutations[AB + 1], x - 1, y, z - 1)),
                  lerp(u, grad(permutations[BA + 1], x, y - 1, z - 1), grad(permutations[BB + 1], x - 1, y - 1, z - 1))));

  return (res + 1.0) / 2.0;
}

double PerlinNoise::turbulence(double x, double y, double z, int octaves, double frequency, double amp) {
  double total_turbulence = 0.0f;
  double scale = 1;
  double total_amp = 1;

  for(int i = 0; i < octaves; i++){
    total_turbulence += total_amp * noise(scale * x, scale * y, scale * z);
    scale *= frequency;
    total_amp *= amp;
  }

  return fabs(total_turbulence);
}

double PerlinNoise::fade(double t) {
  // Using the fading equation provided in the improved Perlin paper
  return t * t * t * (t * (t * 6 - 15) + 10);
}

double PerlinNoise::lerp(double t, double a, double b) {
  return a + t * (b - a);
}

double PerlinNoise::grad(int hash, double x, double y, double z) {
  int h = hash & 15; // The lowest 4 bits of hash
  // If the largest bit is off set u to x other wise to y
  double u = (h < 8) ? x : y;
  // If the largest 2 bits are off set v to y other wise
  // If the smallest bit is on set v to x otherwise set it to z
  double v = (h < 4) ? y : (h == 12 || h == 14) ? x : z;

  return ((((h & 1) == 0) ? u : -u) + (((h & 2) == 0)? v : -v));
}
