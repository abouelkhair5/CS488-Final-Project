#ifndef FINAL_PROJECT_RANDOMTEXTURE_HPP
#define FINAL_PROJECT_RANDOMTEXTURE_HPP


#include <glm/glm.hpp>
#include "PerlinNoise.hpp"
#include "Material.hpp"

class RandomTexture : public Material{
public:
  RandomTexture();
  RandomTexture(glm::vec3 col, double freq);
  virtual void getIntensity(glm::vec2 &uv, float &i);
  virtual void getColor(glm::vec2 &uv, glm::vec3 &color);
  virtual void getColor(glm::vec3 &p, glm::vec3 &color);
  virtual void getColor(glm::vec3 &p, glm::vec2 &uv, glm::vec3 &color);

protected:
  PerlinNoise *pn;
  glm::vec3 m_col;
  double m_freq;
};


#endif //FINAL_PROJECT_RANDOMTEXTURE_HPP
