// Winter 2019

#include "Image.hpp"

#include <iostream>
#include <cstring>
#include <lodepng/lodepng.h>

const uint Image::m_colorComponents = 3; // Red, blue, green

//---------------------------------------------------------------------------------------
Image::Image()
  : m_width(0),
    m_height(0),
    m_data(0)
{
}

//---------------------------------------------------------------------------------------
Image::Image(
		uint width,
		uint height
)
  : m_width(width),
    m_height(height)
{
	size_t numElements = m_width * m_height * m_colorComponents;
	m_data = new double[numElements];
	memset(m_data, 0, numElements*sizeof(double));
}

//---------------------------------------------------------------------------------------
Image::Image(const Image & other)
  : m_width(other.m_width),
    m_height(other.m_height),
    m_data(other.m_data ? new double[m_width * m_height * m_colorComponents] : 0)
{
  if (m_data) {
    std::memcpy(m_data, other.m_data,
                m_width * m_height * m_colorComponents * sizeof(double));
  }
}

//---------------------------------------------------------------------------------------
Image::Image(const std::string & filename)
{
	std::vector<unsigned char> color_values;
	unsigned width, height, error;
	error = lodepng::decode(color_values, width, height, filename, LCT_RGB);

	if(error){
		std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
	}

	m_height = height;
	m_width = width;

	size_t num_el = width * height * m_colorComponents;
	m_data = new double[num_el];

	for(int i = 0; i < num_el; i++){
		m_data[i] = double(color_values[i]) / 255.0;
	}
}

//---------------------------------------------------------------------------------------
Image::Image(const std::string & filename, uint width, uint height)
{
	std::vector<unsigned char> color_values;
	unsigned image_width, image_height, error;
	error = lodepng::decode(color_values, image_width, image_height, filename, LCT_RGB);

	if(error){
		std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
	}

	m_height = height;
	m_width = width;

	size_t num_el = width * height * m_colorComponents;
	m_data = new double[num_el];

	for(uint y = 0; y < m_height; y++){
		for(uint x = 0; x < m_width; x++){
			for(uint i = 0; i < m_colorComponents; i++){
				uint iy = y % image_height;
				uint ix = x % image_width;
				uint idx = ((y * m_width + x) * m_colorComponents) + i;
				uint iidx = ((iy * image_width + ix) * m_colorComponents) + i;
				m_data[idx] = double(color_values[iidx]) / 255.0;
			}
		}
	}
}

//---------------------------------------------------------------------------------------
Image::~Image()
{
  delete [] m_data;
}

//---------------------------------------------------------------------------------------
Image & Image::operator=(const Image& other)
{
  delete [] m_data;
  
  m_width = other.m_width;
  m_height = other.m_height;
  m_data = (other.m_data ? new double[m_width * m_height * m_colorComponents] : 0);

  if (m_data) {
    std::memcpy(m_data,
                other.m_data,
                m_width * m_height * m_colorComponents * sizeof(double)
    );
  }
  
  return *this;
}

//---------------------------------------------------------------------------------------
uint Image::width() const
{
  return m_width;
}

//---------------------------------------------------------------------------------------
uint Image::height() const
{
  return m_height;
}

//---------------------------------------------------------------------------------------
double Image::operator()(uint x, uint y, uint i) const
{
  return m_data[m_colorComponents * (m_width * y + x) + i];
}

//--------------------------------------------------------------------------------------- Fall 2018
double & Image::operator()(uint x, uint y, uint i)
{
  return m_data[m_colorComponents * (m_width * y + x) + i];
}

//---------------------------------------------------------------------------------------
void Image::getColor(const float &u, const float &v, glm::vec3 &color) {
  double di = float(width() - 1) * u;
  double dj = float(height() - 1) * v;

  int i = int(di);
  int j = int(dj);

  double up = di - i;
  double vp = dj - j;

  glm::vec3 c00 = glm::vec3((*this)(i, j, 0), (*this)(i, j, 1), (*this)(i, j, 2));
  glm::vec3 c01 = glm::vec3((*this)(i, j + 1, 0), (*this)(i, j + 1, 1), (*this)(i, j + 1, 2));
  glm::vec3 c10 = glm::vec3((*this)(i + 1, j, 0), (*this)(i + 1, j, 1), (*this)(i + 1, j, 2));
  glm::vec3 c11 = glm::vec3((*this)(i + 1, j + 1, 0), (*this)(i + 1, j + 1, 1), (*this)(i + 1, j + 1, 2));

  color = float(1 - up) * float(1 - vp) * c00 +
          float(1 - up) * float(vp) * c01 +
          float(up) * float(1 - vp) * c10 +
          float(up) * float(vp) * c11;
}

//---------------------------------------------------------------------------------------
static double clamp(double x, double a, double b)
{
	return x < a ? a : (x > b ? b : x);
}

//---------------------------------------------------------------------------------------
bool Image::savePng(const std::string & filename) const
{
	std::vector<unsigned char> image;

	image.resize(m_width * m_height * m_colorComponents);

	double color;
	for (uint y(0); y < m_height; y++) {
		for (uint x(0); x < m_width; x++) {
			for (uint i(0); i < m_colorComponents; ++i) {
				color = m_data[m_colorComponents * (m_width * y + x) + i];
				color = clamp(color, 0.0, 1.0);
				image[m_colorComponents * (m_width * y + x) + i] = (unsigned char)(255 * color);
			}
		}
	}

	// Encode the image
	unsigned error = lodepng::encode(filename, image, m_width, m_height, LCT_RGB);

	if(error) {
		std::cerr << "encoder error " << error << ": " << lodepng_error_text(error)
				<< std::endl;
	}

	return true;
}

//---------------------------------------------------------------------------------------
const double * Image::data() const
{
  return m_data;
}

//---------------------------------------------------------------------------------------
double * Image::data()
{
  return m_data;
}
