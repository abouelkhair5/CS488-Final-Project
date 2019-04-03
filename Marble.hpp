#ifndef FINAL_PROJECT_MARBLE_HPP
#define FINAL_PROJECT_MARBLE_HPP


#include "RandomTexture.hpp"

class Marble : public RandomTexture{
public:
    Marble(glm::vec3 color, double freq, unsigned int octaves, double gain);
    void getColor(glm::vec3 &p, glm::vec3 &color) override;
    void getColor(glm::vec3 &p, glm::vec2 &uv, glm::vec3 &color);
private:
    unsigned int m_octaves;
    double m_gain;
};


#endif //FINAL_PROJECT_MARBLE_HPP
