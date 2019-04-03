#include "RandomTexture.hpp"

RandomTexture::RandomTexture() {
  pn = new PerlinNoise();
}

RandomTexture::RandomTexture(glm::vec3 col, double freq): m_col(col), m_freq(freq){
  pn = new PerlinNoise();
}

void RandomTexture::getIntensity(glm::vec2 &uv, float &i) {
  i = float(pn->noise(m_freq *  uv.x, m_freq * uv.y, 0.8));
}

void RandomTexture::getColor(glm::vec2 &uv, glm::vec3 &color) {
  double n = pn->noise(m_freq *  uv.x, m_freq * uv.y, 0.8);
  color = float(n) * m_col;
}

void RandomTexture::getColor(glm::vec3 &p, glm::vec3 &color) {
  glm::vec3 p_normalize = glm::normalize(p);
  double n = pn->noise(m_freq *  p_normalize.x, m_freq * p_normalize.y, m_freq * p_normalize.z);
  color = float(n) * m_col;
}

void RandomTexture::getColor(glm::vec3 &p, glm::vec2 &uv, glm::vec3 &color){
  getColor(uv, color);
}