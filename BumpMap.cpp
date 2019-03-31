#include "BumpMap.h"

BumpMap::BumpMap(const std::string &filename) {
  m_image = new Image(filename);
}

void BumpMap::get_derivatives(const float &u, const float &v, glm::vec2 &derivatives) {
  double di = float(m_image->width() - 1) * u;
  double dj = float(m_image->height() - 1) * v;

  int i = int(di);
  int j = int(dj);

  double up = di - i;
  double vp = dj - j;

  glm::vec3 c00 = glm::vec3((*m_image)(i, j, 0), (*m_image)(i, j, 1), (*m_image)(i, j, 2));
  glm::vec3 c01 = glm::vec3((*m_image)(i, j + 1, 0), (*m_image)(i, j + 1, 1), (*m_image)(i, j + 1, 2));
  glm::vec3 c10 = glm::vec3((*m_image)(i + 1, j, 0), (*m_image)(i + 1, j, 1), (*m_image)(i + 1, j, 2));

  float h00 = glm::dot(c00, c00);
  float h01 = glm::dot(c01, c01);
  float h10 = glm::dot(c10, c10);

  derivatives.x = h01 - h00;
  derivatives.y = h10 - h00;
}