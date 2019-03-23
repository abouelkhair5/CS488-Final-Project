// Fall 2018

#pragma once

#include <glm/glm.hpp>
#include <algorithm>

#define EPSILON 0.05
// #define RVOLUME_BOUNDS
// #define VOLUME_BOUNDS

class Primitive {
public:
  virtual ~Primitive();
  virtual bool intersect(const glm::vec3 eye, const glm::vec3 direction, double &t, glm::vec3 &normal);

  // static constexpr float EPSILON = 0.05;
};

class Sphere : public Primitive {
public:
  virtual bool intersect(const glm::vec3 eye, const glm::vec3 direction, double &t, glm::vec3 &normal);
  virtual ~Sphere();
};

class Cube : public Primitive {
public:
  virtual bool intersect(const glm::vec3 eye, const glm::vec3 direction, double &t, glm::vec3 &normal);
  virtual ~Cube();
};

class NonhierSphere : public Primitive {
public:
  NonhierSphere(const glm::vec3& pos, double radius)
    : m_pos(pos), m_radius(radius)
  {
  }
  virtual bool intersect(const glm::vec3 eye, const glm::vec3 direction, double &t, glm::vec3 &normal);
  virtual ~NonhierSphere();

private:
  glm::vec3 m_pos;
  double m_radius;
};

class NonhierBox : public Primitive {
public:
  NonhierBox(const glm::vec3& pos, double size)
    : m_pos(pos), m_size(size)
  {
  }
  virtual bool intersect(const glm::vec3 eye, const glm::vec3 direction, double &t, glm::vec3 &normal);
  virtual ~NonhierBox();

private:
  glm::vec3 m_pos;
  double m_size;
};

class NonhierCylinder : public Primitive {
public:
    NonhierCylinder(const glm::vec3& pos, double xradius, double yradius, double height)
      : m_pos(pos), m_xradius(xradius), m_yradius(yradius), m_height(height)
    {
    }
    virtual bool intersect(const glm::vec3 eye, const glm::vec3 direction, double &t, glm::vec3 &normal);
    virtual ~NonhierCylinder();

private:
    glm::vec3 m_pos;
    double m_xradius;
    double m_yradius;
    double m_height;
};

class NonhierCone : public Primitive {
public:
    NonhierCone(const glm::vec3& pos, double xradius, double zradius, double height)
            : m_pos(pos), m_xradius(xradius), m_zradius(zradius), m_height(height)
    {
    }
    virtual bool intersect(const glm::vec3 eye, const glm::vec3 direction, double &t, glm::vec3 &normal);
    virtual ~NonhierCone();

private:
    glm::vec3 m_pos;
    double m_xradius;
    double m_zradius;
    double m_height;
};
