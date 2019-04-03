//
// Created by Amr Abouelkhair on 2019-03-30.
//

#include "Texture.hpp"

Texture::Texture(const std::string &filename) {
  m_image = new Image(filename);
}

void Texture::getColor(const float &u, const float &v, glm::vec3 &color) {
  m_image->getColor(u, v, color);
}
