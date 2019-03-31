// Winter 2019

#include <iostream>
#include <math.h>
#include <glm/ext.hpp>
#include "Primitive.hpp"
#include "polyroots.hpp"

Primitive::~Primitive()
{
}

bool Primitive::intersect(const glm::vec3 ray_origin, const glm::vec3 ray_direction, double &t, glm::vec3 &normal, glm::vec2 &uv)
{
	return false;
}

Sphere::~Sphere()
{
}

bool Sphere::intersect(const glm::vec3 ray_origin, const glm::vec3 ray_direction, double &t, glm::vec3 &normal, glm::vec2 &uv)
{
	NonhierSphere s = NonhierSphere(glm::vec3(0.0, 0.0, 0.0), 1.0);
	return s.intersect(ray_origin, ray_direction, t, normal, uv);
}

Cube::~Cube()
{
}

bool Cube::intersect(const glm::vec3 ray_origin, const glm::vec3 ray_direction, double &t, glm::vec3 &normal, glm::vec2 &uv)
{
	NonhierBox b = NonhierBox(glm::vec3(0.0, 0.0, 0.0), 1.0);
	return b.intersect(ray_origin, ray_direction, t, normal, uv);
}

NonhierSphere::~NonhierSphere()
{
}

bool NonhierSphere::intersect(const glm::vec3 ray_origin, const glm::vec3 ray_direction, double &t, glm::vec3 &normal, glm::vec2 &uv)
{
	// std::cout << "intersect" <<std::endl;
	double roots[2];
	glm::vec3 a_minus_c = ray_origin - m_pos;

	// calculating the quadratic func parameters using notes in class
	double a = glm::dot(ray_direction, ray_direction);
	double b = 2 * glm::dot(ray_direction, a_minus_c);
	double c = glm::dot(a_minus_c, a_minus_c) - m_radius*m_radius;

	// now we solve the quadratic equation
	size_t nroots = quadraticRoots(a, b, c, roots);
	double min_root = std::min(roots[0], roots[1]);
	double max_root = std::max(roots[0], roots[1]);

	if(nroots <= 0 || max_root < EPSILON)
	{
		// no roots so no intesection
		// both roots negative then sphere behind eye and we don't show it
		// std::cout << "no roots" << std::endl;
		return false;
	}
	// there's at least one positive root
	// if the closer intersection is in front of the ray pick that
	// other wise pick the other one
	t = (min_root > EPSILON)? min_root : max_root;

	glm::vec3 point_of_intersection = ray_origin + (float)t * ray_direction;
	normal = glm::normalize(point_of_intersection - m_pos);
	double theta = glm::atan(-(point_of_intersection.z - m_pos.z), point_of_intersection.x - m_pos.x);
	double phi = glm::acos(-(point_of_intersection.y - m_pos.y) / float(m_radius));
	uv.x = (theta + M_PI) / (2 * M_PI);
  uv.y = (M_PI - phi) / M_PI;

	return true;

}

NonhierBox::~NonhierBox()
{
}

bool NonhierBox::intersect(const glm::vec3 ray_origin, const glm::vec3 ray_direction, double &t, glm::vec3 &normal, glm::vec2 &uv)
{

	// we have an interval for each of the 3 coordinates for that coordinate
	// to fit within the box
	// we can use all of the 3 intervals to creat intervals for t
	// we make sure all 3 intervals overal, otherwise there's no t that 
	// meets these requirements
	// if they all overlap we pick the intersection between them and pick
	// the smallest positive value of the two values for t

	glm::vec3 closest_corner = m_pos;
	glm::vec3 furthest_corner = m_pos + glm::vec3(m_size);

	// solving for t using x 
	// we know x = ray_origin.x + (t * ray_direction.x)
	// and m_pos.x <= x <= m_pos.x + m_size 
	// therefore (m_pos.x - ray_origin.x) / ray_direction.x <= t
	// and t <= (m_pos.x + m_size.x - ray_origin.x) / ray_direction.x

	double lower_t = (closest_corner[0] - ray_origin[0]) / ray_direction[0];
	double upper_t = (furthest_corner[0] - ray_origin[0]) / ray_direction[0];

	// if the intersection does occur at either of these bounds for t
	// then it occurs at one of the bounds for the x coordiante
	// then the ray intersects with one of the box faces that are normal
	// to the x axis

	glm::vec3 close_normal = glm::vec3(-1.0, 0.0, 0.0);
	glm::vec3 far_normal = glm::vec3(1.0, 0.0, 0.0);

	// if the box is built backwards (or we are looking at it from the back)
	// then the bounds may be inversed so we swap them and their corresponding normals

	if(upper_t < lower_t){
		std::swap(upper_t, lower_t);
		std::swap(close_normal, far_normal);
	}


	// we repeat the same process with y 

	double lower_ty = (closest_corner[1] - ray_origin[1]) / ray_direction[1];
	double upper_ty = (furthest_corner[1] - ray_origin[1]) / ray_direction[1];

	// faces corresponding to these bounds have the following normals

	glm::vec3 close_normal_y = glm::vec3(0.0, -1.0, 0.0);
	glm::vec3 far_normal_y = glm::vec3(0.0, 1.0, 0.0);

	// we makes sure that the bounds are ordered correctly and so are their faces

	if(upper_ty < lower_ty){
		std::swap(upper_ty, lower_ty);
		std::swap(close_normal_y, far_normal_y);
	}

	// if the two intervals generated don't overlap then there are no solution
	// because t can't be in two exclusive intervals

	if(upper_ty < lower_t or upper_t < lower_ty){
		return false;
	}

	// we pick the lower bound as the max of both lower bounds found
	// and the upper bound as the min of both upper bounds found
	// ie we pick the intersection between both intervals because
	// that's the real interval including our t

	if(std::isnan(lower_t) || lower_t < lower_ty){
		lower_t = lower_ty;
		close_normal = close_normal_y;
	}

	if(std::isnan(upper_t) || upper_ty < upper_t){
		upper_t = upper_ty;
		far_normal = far_normal_y;
	}

	// we repeat the same process for z and pick the intersection
	// of the inrtervals again

	double lower_tz = (closest_corner[2] - ray_origin[2]) / ray_direction[2];
	double upper_tz = (furthest_corner[2] - ray_origin[2]) / ray_direction[2];

	glm::vec3 close_normal_z = glm::vec3(0.0, 0.0, -1.0);
	glm::vec3 far_normal_z = glm::vec3(0.0, 0.0, 1.0);

	if(upper_tz < lower_tz){
		std::swap(upper_tz, lower_tz);
		std::swap(close_normal_z, far_normal_z);
	}

	// make sure intervals overlap otherwise no intersection

	if(upper_tz < lower_t or upper_t < lower_tz){
		return false;
	}

	// there's overlap so we pick the intersection
	// and the normal to the face corresponding to the bounds of that intersection
	if(std::isnan(lower_t) || lower_t < lower_tz){
		lower_t = lower_tz;
		close_normal = close_normal_z;
	}

	if(std::isnan(upper_t) || upper_tz < upper_t){
		upper_t = upper_tz;
		far_normal = far_normal_z;
	}

	// if the larger of the bounds is negative then both of them are and 
	// the box is behind our eye and the intersection with it does not concern us
	if(upper_t < EPSILON){
		return false;
	}

	// if only the lower bound is negative then the eye is in the middle of the box
	// and we pick the larger bound as the t for our point of intersection
	// we pick one of the bounds because these are the values of t corresponding
	// to an intersection between the ray and one of the box faces
	// all the other values are for points on the ray and inside the box but not
	// necessarily on one of its faces

	if(lower_t < EPSILON){
		t = upper_t;
		normal = far_normal;
	}
	else {
		// else the lower bound of t represents an intersection between the ray
		// and the face with the corresponding normal and it's closer to the 
		// ray origin (eye)
		t = lower_t;
		normal = close_normal;
	}

	glm::vec3 poi = float(1.0 / m_size) * (ray_origin + (float(t) * ray_direction) - m_pos);
    //std::cout << poi.x << " " << poi.y << " " << poi.z << std::endl;
	if (normal.z != 0.0){
	  uv = glm::vec2(poi.x, 1 - poi.y);
	}else if(normal.x != 0.0){
	  uv = glm::vec2(poi.z, 1 - poi.y);
	}
    else if (normal.y != 0.0){
	  uv = glm::vec2(poi.x, poi.z);
	}

	return true;
}

Cylinder::~Cylinder() = default;

bool Cylinder::intersect(const glm::vec3 eye, const glm::vec3 direction, double &t, glm::vec3 &normal, glm::vec2 &uv)
{
	double radius = 1.0;
	double height = 1.0;
	glm::vec3 m_pos = glm::vec3(0.0);
	bool intersection = false;
	double current_t;
	glm::vec3 current_normal;

  double bottom_t = (m_pos.y - eye.y) / direction.y;
  glm::vec3 point_of_intersection = eye + float(bottom_t) * direction;
  double distance_to_center = sqrt((point_of_intersection.x * point_of_intersection.x) + (point_of_intersection.z * point_of_intersection.z));
  if( bottom_t > EPSILON && distance_to_center < radius )
  {
		current_t = bottom_t;
		current_normal = glm::vec3(0, -1, 0);
		intersection = true;
  }

  double top_t = (m_pos.y + height - eye.y) / direction.y;
  point_of_intersection = eye + float(top_t) * direction;
  distance_to_center = sqrt((point_of_intersection.x * point_of_intersection.x) + (point_of_intersection.z * point_of_intersection.z));
  if(
		top_t > EPSILON
		&& distance_to_center < radius
		&& (!intersection || top_t < current_t)
		)
	{
  	current_t = top_t;
  	current_normal = glm::vec3(0, 1, 0);
  	intersection = true;
	}


  // using the equation x^2/p^2 + y^2/q^2 = 1 and eye + t * direction = point
	// we get the following a b and c for solving a quadratic equation
	double mx = eye.x - m_pos.x;
	double mz = eye.z - m_pos.z;
	double a = (direction.x * direction.x) + (direction.z * direction.z);
	double b = 2 * ((mx * direction.x) + (mz * direction.z));
	double c = (mx * mx) + (mz * mz) - radius;

  // solving the quadratic equation
  double roots[2];
  size_t num_roots = quadraticRoots(a, b, c, roots);


	if(num_roots > 0) {
		double min_root = std::min(roots[0], roots[1]);
		double max_root = std::max(roots[0], roots[1]);

		// finding a bound on t using y
		double t_min_y = std::min(top_t, bottom_t);
		double t_max_y = std::max(top_t, bottom_t);

		double cylinder_intersection;
		bool eligible_point = false;

		if (min_root > std::max(EPSILON, t_min_y) && min_root < t_max_y) {
			cylinder_intersection = min_root;
			eligible_point = true;
		} else if (max_root > std::max(EPSILON, t_min_y) && max_root < t_max_y) {
			cylinder_intersection = max_root;
			eligible_point = true;
		}

		if(eligible_point && (!intersection || cylinder_intersection < current_t))
		{
			current_t = cylinder_intersection;
			point_of_intersection = eye + float(t) * direction;
			current_normal = glm::normalize(point_of_intersection - m_pos);
			current_normal[1] = 0;
			intersection = true;
		}
	}

	if(intersection){
		t = current_t;
		normal = current_normal;
		uv.x = glm::angle(normal, glm::vec3(0, 0, 1)) / (2 * M_PI);
		uv.y = float((height - ((eye.z + (t * direction.z)) - m_pos.z)) / height);
		return true;
	}

  return false;
}

Cone::~Cone() = default;

bool Cone::intersect(const glm::vec3 eye, const glm::vec3 direction, double &t, glm::vec3 &normal, glm::vec2 &uv)
{
	glm::vec3 m_pos = glm::vec3(0.0);
	double radius = 1.0;
	double height = 1.0;
	bool intersection = false;
	double current_t;
	glm::vec3 current_normal;

	double top_t = (m_pos.y + height - eye.y) / direction.y;
	double bottom_t = (m_pos.y - eye.y) / direction.y;
	glm::vec3 point_of_intersection = eye + float(top_t) * direction;
	double distance_to_center = sqrt((point_of_intersection.x * point_of_intersection.x) + (point_of_intersection.z * point_of_intersection.z));
	if( top_t > EPSILON && distance_to_center < radius )
	{
		current_t = top_t;
		current_normal = glm::vec3(0, 1, 0);
		intersection = true;
	}

	// using the equation x^2/p^2 + z^2/q^2 = y^2/h^2 and eye + t * direction = point
	// we get the following a b and c for solving a quadratic equation
	double tan_theta_sq = (radius / height) * (radius / height);

	double mx = eye.x - m_pos.x;
	double my = eye.y - m_pos.y;
	double mz = eye.z - m_pos.z;

	double a = ((direction.x * direction.x) + (direction.z * direction.z) - (direction.y * direction.y * tan_theta_sq));

	double b = 2 * ((mx * direction.x) + (mz * direction.z) - (my * direction.y * tan_theta_sq));

	double c = (mx * mx) + (mz * mz) - (my * my * tan_theta_sq);

	double roots[2];

	size_t num_roots = quadraticRoots(a, b, c, roots);

	if(num_roots > 0) {
		double min_root = std::min(roots[0], roots[1]);
		double max_root = std::max(roots[0], roots[1]);

		// finding a bound on t using y
		double t_min_y = std::min(top_t, bottom_t);
		double t_max_y = std::max(top_t, bottom_t);

		double cone_intersection;
		bool eligible_point = false;

		if (min_root > std::max(EPSILON, t_min_y) && min_root < t_max_y) {
			cone_intersection = min_root;
			eligible_point = true;
		} else if (max_root > std::max(EPSILON, t_min_y) && max_root < t_max_y) {
			cone_intersection = max_root;
			eligible_point = true;
		}

		if(eligible_point && (!intersection || cone_intersection < current_t))
		{
			current_t = cone_intersection;
			point_of_intersection = eye + float(t) * direction;
			glm::vec3 v = point_of_intersection - m_pos;
			double r = sqrt((v.x * v.x) + (v.z * v.z));
			current_normal[0] = v.x;
			current_normal[1] = r * r / v.y;
			current_normal[2] = v.z;
			intersection = true;
		}
	}

	if(intersection){
		t = current_t;
		normal = current_normal;
		return true;
	}

	return false;
}
