// Winter 2019

#include "PhongMaterial.hpp"

PhongMaterial::PhongMaterial()
: m_kd(glm::vec3(0.0f))
, m_ks(glm::vec3(0.0f))
, m_kt(glm::vec3(0.0f))
, m_shininess(0)
, m_transparent(false)
, m_glossy(false)
, m_ior(0)
{}

PhongMaterial::PhongMaterial(
	const glm::vec3& kd, const glm::vec3& ks, double shininess, bool glossy)
	: m_kd(kd)
	, m_ks(ks)
	, m_kt(glm::vec3(0.0f))
	, m_shininess(shininess)
	, m_transparent(false)
	, m_glossy(glossy)
	, m_ior(0)
{}

PhongMaterial::PhongMaterial(
				const glm::vec3& kd, const glm::vec3& ks, const glm::vec3& kt, double shininess, double ior , bool glossy)
				: m_kd(kd)
				, m_ks(ks)
				, m_kt(kt)
				, m_shininess(shininess)
				, m_transparent(true)
				, m_ior(ior)
				, m_glossy(glossy)
{}

PhongMaterial::~PhongMaterial()
{}
