#ifndef FINAL_PROJECT_BUMPMAP_H
#define FINAL_PROJECT_BUMPMAP_H


#include <glm/glm.hpp>
#include "Material.hpp"
#include "Image.hpp"

#define RESOLUTION 2048
#define DELTA 0.05f

class BumpMap : public Material{
public:
    BumpMap(const std::string &filename);
    void get_derivatives(const float &u, const float &v, glm::vec2 &derivatives);

    Image *m_image;
};


#endif //FINAL_PROJECT_BUMPMAP_H
