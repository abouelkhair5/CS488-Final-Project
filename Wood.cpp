//
// Created by Amr Abouelkhair on 2019-04-01.
//

#include "Wood.hpp"

Wood::Wood(glm::vec3 color, double grain):RandomTexture(color, grain) {}

void Wood::getColor(glm::vec2 &uv, glm::vec3 &color) {
  auto intensity = float(m_freq * pn->noise(uv.x, uv.y, 0.8));
  intensity = intensity - floor(intensity);
  color = intensity * m_col;
}

void Wood::getColor(glm::vec3 &p, glm::vec3 &color) {
  glm::vec3 p_normalized = glm::normalize(p);
  auto intensity = float(m_freq * pn->noise(p_normalized.x, p_normalized.y, p_normalized.z));
  intensity = intensity - floor(intensity);
  color = intensity * m_col;
}