#ifndef FINAL_PROJECT_TEXTURE_H
#define FINAL_PROJECT_TEXTURE_H


#include "Material.hpp"
#include "Image.hpp"
#include "glm/glm.hpp"

class Texture : public Material{
public:
    Texture(const std::string &filename);
    void getColor(const float &u, const float &v, glm::vec3 &color);

    Image *m_image;
};


#endif
