#include "BumpMap.hpp"

BumpMap::BumpMap(const std::string &filename, const glm::vec3& kd, const glm::vec3 &ks, double shininess)
:m_kd(kd), m_ks(ks), m_shininess(shininess){
  m_image = new Image(filename, RESOLUTION, RESOLUTION);
}

void BumpMap::get_derivatives(const float &u, const float &v, glm::vec2 &derivatives) {

  glm::vec3 c00;
  glm::vec3 c01;
  glm::vec3 c10;

  m_image->getColor(u, v, c00);
  m_image->getColor(u + DELTA, v, c01);
  m_image->getColor(u, v + DELTA, c10);

  float h00 = glm::dot(c00, c00);
  float h01 = glm::dot(c01, c01);
  float h10 = glm::dot(c10, c10);

  derivatives.x = h01 - h00;
  derivatives.y = h10 - h00;
}