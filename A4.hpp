// Winter 2019

#pragma once

#include <glm/glm.hpp>

#include "SceneNode.hpp"
#include "GeometryNode.hpp"
#include "Light.hpp"
#include "Image.hpp"
#include "PhongMaterial.hpp"

void A4_Render(
		// What to render
		SceneNode * root,

		// Image to write to, set to a given width and height
		Image & image,

		// Viewing parameters
		const glm::vec3 & eye,
		const glm::vec3 & view,
		const glm::vec3 & up,
		double fovy,

		// Lighting parameters
		const glm::vec3 & ambient,
		const std::list<Light *> & lights
);

void perturb(
		glm::vec3 &v,
		glm::vec3 &p
);

void perturb(
		glm::vec3 &v,
		const float &du,
		const float &dv
);

glm::mat4 generate_dcs_to_world_mat(
		uint width, uint height,
		int d, double fov,
		glm::vec3 eye,
		glm::vec3 view,
		glm::vec3 up
);

bool hit(
	SceneNode* scene,
	const glm::vec3 &ray_origin,
	const glm::vec3 &ray_direction,
	const glm::mat4 &world_to_model,
	double &ray_closest_t,
	glm::vec3 &intersection_normal,
	PhongMaterial &mat,
	bool& ray_intersection
);

bool ray_color(
	SceneNode* scene,
	const glm::vec3 &eye,
	const glm::vec3 &ray_direction,
	const glm::vec3 &ambient,
	const std::list<Light *> &lights,
	glm::vec3 &col,
	uint remaining_bounces
);

void set_pixel(
	SceneNode* root,
	uint x, uint y,
	size_t h, size_t w,
	glm::vec3 &color,
	const glm::mat4 dcs_to_world,
	const glm::vec3 eye,
	const glm::vec3 ambient,
	const std::list<Light *> lights
);

void set_segment(
	SceneNode* root,
	uint ystart,uint yend,
	uint xstart,uint xend,
	size_t h, size_t w,
	glm::vec3 *image_array,
	const glm::mat4 dcs_to_world,
	const glm::vec3 eye,
	const glm::vec3 ambient,
	const std::list<Light *> lights
);