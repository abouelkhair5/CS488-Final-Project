#include "RandomTexture.hpp"

RandomTexture::RandomTexture() {
  pn = new PerlinNoise();
}

void RandomTexture::getIntensity(glm::vec2 &uv, float &i) {
  i = float(pn->noise(uv.x, uv.y, 0.0));
}