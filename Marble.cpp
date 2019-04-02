//
// Created by Amr Abouelkhair on 2019-04-01.
//

#include "Marble.hpp"

Marble::Marble(glm::vec3 color, double freq, unsigned int octaves, double gain):
RandomTexture(color, freq), m_octaves(octaves), m_gain(gain) {}

void Marble::getColor(glm::vec3 &p, glm::vec3 &color) {
  auto turbulence = float(pn->turbulence(p.x, p.y, p.z, m_octaves, m_freq, m_gain));
  float intensity = sin(float(m_gain * (p.x + m_octaves * turbulence)));
  color = intensity * m_col;
}

void Marble::getColor(glm::vec3 &p, glm::vec2 &uv, glm::vec3 &color) {
  getColor(p, color);
}