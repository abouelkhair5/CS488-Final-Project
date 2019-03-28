// Winter 2019

#pragma once

#include <glm/glm.hpp>

#include "Material.hpp"

class PhongMaterial : public Material {
public:
  PhongMaterial(const glm::vec3& kd, const glm::vec3& ks, double shininess, bool glossy);
  PhongMaterial(const glm::vec3& kd, const glm::vec3& ks, const glm::vec3& kt, double shininess, double ior, bool glossy);
  virtual ~PhongMaterial();

  glm::vec3 m_kd;
  glm::vec3 m_ks;
  glm::vec3 m_kt;

  double m_shininess;

  bool m_transparent;
  bool m_glossy;
  double m_ior; // index of refraction
};
