#include "RandomTexture.hpp"

RandomTexture::RandomTexture() {
  pn = new PerlinNoise();
}

RandomTexture::RandomTexture(glm::vec3 col, double freq): m_col(col), m_freq(freq){}

void RandomTexture::getIntensity(glm::vec2 &uv, float &i) {
  i = float(pn->noise(m_freq *  uv.x, m_freq * uv.y, 0.8));
}

void RandomTexture::getColor(glm::vec2 &uv, glm::vec3 &color) {
  double n = pn->noise(m_freq *  uv.x, m_freq * uv.y, 0.8);
  color = float(n) * m_col;
}