#ifndef FINAL_PROJECT_PERLINNOISE_HPP
#define FINAL_PROJECT_PERLINNOISE_HPP

#include <vector>

class PerlinNoise {
  std::vector<int> permutations;

public:
  PerlinNoise();

  double noise(double x, double y, double z);
  double turbulence(double x, double y, double z, int octaves, double frequency, double amp);
private:
  double fade(double t);
  double lerp(double t, double a, double b);
  double grad(int hash, double x, double y, double z);
};


#endif //FINAL_PROJECT_PERLINNOISE_HPP
