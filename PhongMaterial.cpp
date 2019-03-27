// Winter 2019

#include "PhongMaterial.hpp"

PhongMaterial::PhongMaterial(
	const glm::vec3& kd, const glm::vec3& ks, double shininess )
	: m_kd(kd)
	, m_ks(ks)
	, m_shininess(shininess)
	, m_transparent(false)
	, m_ior(0)
{}

PhongMaterial::PhongMaterial(
				const glm::vec3& kd, const glm::vec3& ks, double shininess, double ior )
				: m_kd(kd)
				, m_ks(ks)
				, m_shininess(shininess)
				, m_transparent(true)
				, m_ior(ior)
{}

PhongMaterial::~PhongMaterial()
{}
