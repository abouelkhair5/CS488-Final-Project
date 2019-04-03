#ifndef FINAL_PROJECT_BUMPMAP_H
#define FINAL_PROJECT_BUMPMAP_H


#include <glm/glm.hpp>
#include "Material.hpp"
#include "Image.hpp"

#define RESOLUTION 2048
#define DELTA 0.005f

class BumpMap : public Material{
public:
    BumpMap(const std::string &filename, const glm::vec3& kd, const glm::vec3 &ks, double shininess);
    void get_derivatives(const float &u, const float &v, glm::vec2 &derivatives);

    Image *m_image;
    glm::vec3 m_kd;
    glm::vec3 m_ks;
    double m_shininess;
};


#endif //FINAL_PROJECT_BUMPMAP_H
