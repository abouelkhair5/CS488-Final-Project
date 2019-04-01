#ifndef FINAL_PROJECT_WOOD_HPP
#define FINAL_PROJECT_WOOD_HPP


#include "RandomTexture.hpp"

class Wood : public RandomTexture{
public:
  Wood(glm::vec3 color, double grain);
  void getColor(glm::vec2 &uv, glm::vec3 &color);
};


#endif //FINAL_PROJECT_WOOD_HPP
