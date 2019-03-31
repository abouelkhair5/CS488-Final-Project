//
// Created by Amr Abouelkhair on 2019-03-30.
//

#include "Texture.hpp"

Texture::Texture(const std::string &filename) {
  m_image = Image(filename);
}

void Texture::getColor(float &u, float &v, glm::vec3 color) {
  double di = (m_image.width() - 1) * u;
  double dj = (m_image.height() - 1) * v;

  uint i = uint(di);
  uint j = uint(dj);

  double up = di - i;
  double vp = dj - j;

  glm::vec3 c00 = glm::vec3(m_image(i, j, 0), m_image(i, j, 2), m_image(i, j, 2));
  glm::vec3 c01 = glm::vec3(m_image(i, j + 1, 0), m_image(i, j + 1, 2), m_image(i, j + 1, 2));
  glm::vec3 c10 = glm::vec3(m_image(i + 1, j, 0), m_image(i + 1, j, 2), m_image(i + 1, j, 2));
  glm::vec3 c11 = glm::vec3(m_image(i + 1, j + 1, 0), m_image(i + 1, j + 1, 2), m_image(i + 1, j + 1, 2));

  color = float(1 - up) * float(1 - vp) * c00 +
          float(1 - up) * float(vp) * c01 +
          float(up) * float(1 - vp) * c10 +
          float(up) * float(vp) * c11;
}