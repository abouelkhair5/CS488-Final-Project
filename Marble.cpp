//
// Created by Amr Abouelkhair on 2019-04-01.
//

#include "Marble.hpp"

Marble::Marble(glm::vec3 color, double freq, unsigned int octaves, double gain):
RandomTexture(color, freq), m_octaves(octaves), m_gain(gain) {}

void Marble::getColor(glm::vec2 &uv, glm::vec3 &color) {
  auto turbulence = float(pn->turbulence(uv.x, uv.y, 0.6, m_octaves, m_freq, m_gain));
  float intensity = sin(float(m_freq * (uv.x + m_gain * turbulence)));
  color = intensity * m_col;
}