#ifndef FINAL_PROJECT_RANDOMTEXTURE_HPP
#define FINAL_PROJECT_RANDOMTEXTURE_HPP


#include <glm/glm.hpp>
#include "PerlinNoise.hpp"
#include "Material.hpp"

class RandomTexture : public Material{
public:
  RandomTexture();
  void getIntensity(glm::vec2 &uv, float &i);
private:
  PerlinNoise *pn;
};


#endif //FINAL_PROJECT_RANDOMTEXTURE_HPP
