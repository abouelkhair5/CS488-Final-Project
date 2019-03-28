// Winter 2019

#include <glm/ext.hpp>

#include "A4.hpp"
#include "PhongMaterial.hpp"
#include <cmath>
#include <thread>
#include <list>
#include <chrono>
#include <ctime>
#include <random>

#define MULTI_THREAD
#define REFLECTION
#define REFRACTION
#define SHOW_PROGRESS

std::default_random_engine generator;
std::uniform_real_distribution<double> distribution(0.0, 1.0);

glm::mat4 generate_dcs_to_world_mat(
	uint width, uint height,
	int d, double fov,
	glm::vec3 eye,
	glm::vec3 view,
	glm::vec3 up
) {
	// 1. Translate the origin from top left to origin
	// ie translate by width/2 and -height/2
	glm::mat4 T1 = glm::translate(glm::vec3(-((double)width/2.0), -((double)height)/2.0, d));

	// 2. Scale from the screen the world 
	double world_height = 2 * d * glm::tan(glm::radians(fov/2));
	double scale_factor = world_height/height;
	glm::mat4 S2 = glm::scale(glm::vec3(-scale_factor, -scale_factor, 1));

	// 3. Rotate to align the axes
	glm::vec3 w = glm::normalize(view);
	glm::vec3 u = glm::cross(glm::normalize(up), w);
	glm::vec3 v = glm::cross(w, u);

	glm::mat4 R3 = glm::mat4();
	R3[0] = glm::vec4(u, 0.0);
	R3[1] = glm::vec4(v, 0.0);
	R3[2] = glm::vec4(w, 0.0);

	// 4. Translate to look from ie eye
	glm::mat4 T4 = glm::translate(eye);

	glm::mat4 world = T4 * R3 * S2 * T1;
	return world;
}

void perturb(glm::vec3 &v, glm::vec3 &n, glm::vec3 & p){
  double x1 = distribution(generator);
  double x2 = distribution(generator);

  double alpha = glm::acos(sqrt(1 - x1));
  double beta = 2 * M_PI * x2;

  p = glm::normalize(glm::vec3(glm::rotate(float(beta), v) * glm::rotate(float(alpha), n) * glm::vec4(v, 0)));
}

bool ray_color(
	SceneNode* scene,
	const glm::vec3 &eye,
	const glm::vec3 &ray_direction,
	const glm::vec3 &ambient,
	const std::list<Light *> &lights,
	glm::vec3 &col,
	uint remaining_bounces
){

  glm::vec3 normal;
  double t;

  glm::vec3 kd;
  glm::vec3 ks;
  double shininess;

  bool transparency;
  double ior;

	glm::mat4 world_to_model = glm::mat4();
	bool ray_intersection = false;
	hit(scene, eye, ray_direction, world_to_model, t, normal, kd, ks, shininess, transparency, ior, ray_intersection);

	if(ray_intersection) {
    col[0] = kd[0] * ambient[0];
    col[1] = kd[1] * ambient[1];
    col[2] = kd[2] * ambient[2];

    glm::vec3 point_of_intersection = eye + (float(t) * ray_direction);
    glm::vec3 dummy_normal, dummy_kd, dummy_ks;
    double dummy_t, dummy_shininess, dummy_ior;
    bool dummy_transparency;

    // loop over light sources to check for shadows
    for (Light *light: lights) {
      glm::vec3 l = light->position - point_of_intersection;
      double dist_to_light = glm::length(l);
      double attenuation = light->falloff[0];
      attenuation += (light->falloff[1] * dist_to_light);
      attenuation += (light->falloff[2] * dist_to_light * dist_to_light);
      glm::vec3 I_in = float(1 / attenuation) * light->colour;
      l = glm::normalize(l);

      // dummy values to pass to hit, however their values aren't needed
      ray_intersection = false;
      bool light_blocked = hit(
              scene,
              point_of_intersection,
              l,
              world_to_model,
              dummy_t,
              dummy_normal,
              dummy_kd,
              dummy_ks,
              dummy_shininess,
              dummy_transparency,
              dummy_ior,
              ray_intersection);

      if (!light_blocked) {
        // shoot a ray from POI to light source, if that ray intersects with an object
        // on the way then that light source is blocked and shadow will be casted

        glm::vec3 v = normalize(eye - point_of_intersection);
        glm::vec3 r = glm::normalize(-l + (2 * glm::dot(l, normal)) * normal);
        double l_dot_n = glm::dot(l, normal);
        double r_dot_v_to_p = std::pow(glm::dot(r, v), shininess);

        col[0] += std::max(0.0, std::min(1.0, (kd[0] * l_dot_n * I_in[0]) + (ks[0] * r_dot_v_to_p * I_in[0])));
        col[1] += std::max(0.0, std::min(1.0, (kd[1] * l_dot_n * I_in[1]) + (ks[1] * r_dot_v_to_p * I_in[1])));
        col[2] += std::max(0.0, std::min(1.0, (kd[2] * l_dot_n * I_in[2]) + (ks[2] * r_dot_v_to_p * I_in[2])));
      }
    }


    #ifdef REFLECTION
    // if reflection option is enabled and we haven't done all our recursive rays yet then we send another one
		glm::vec3 reflected_color = glm::vec3(0.0);
    if(remaining_bounces > 0)
    {
    	int glossy_rays = 4;
    	glm::vec3 normal2 = cross(ray_direction, normal);
			glm::vec3 perturbed_normal;
			glm::vec3 reflected;
			glm::vec3 color_part;
			bool reflect_ray_intersection;

			for(int i = 0; i < glossy_rays; i++) {
				double x1 = distribution(generator);
				double x2 = distribution(generator);

				double alpha = glm::acos(sqrt(1 - x1));
				double beta = 2 * M_PI * x2;

				perturbed_normal = glm::normalize(glm::vec3(glm::rotate(float(beta), normal) * glm::rotate(float(alpha), normal2) * glm::vec4(normal, 0)));
//				perturb(normal, normal2, perturbed_normal);
				reflected = reflect(ray_direction, perturbed_normal);
				reflect_ray_intersection = false;
				color_part = glm::vec3(0.0f);

				reflect_ray_intersection = ray_color
								(
												scene,
												point_of_intersection, reflected,
												ambient, lights,
												color_part,
												remaining_bounces - 1
								);

				reflected_color += float(1 / glossy_rays) * color_part;
			}
    }

    #ifdef REFRACTION
    if (transparency) {
      // * we should send a reflective ray here as well but I am not going to worry about this right now*

      // the end goal we want to find the ray coming out of our object and get the color of that
      // theta is the angle between the incident ray the normal
      // cos phi is the angle between the normal and the refracted ray
      double cos_theta = glm::dot(ray_direction, normal);
      if (cos_theta < 0) {
        // ray entering the object
        double cos_phi_sq = 1 - ((1 - cos_theta * cos_theta) / ior);

        if (cos_phi_sq >= 0) {
          // not a complete internal refraction
          // refracted = sin_phi * b - normal * cos_phi where b along with normal form an
          // orthonormal basis to the plane containing the the incident ray and the normal
          glm::vec3 transmitted = ((ray_direction - (normal * float(cos_theta))) / float(ior)) -
                                  (normal * float(sqrt(cos_phi_sq)));

          // the ratio to blend reflection and refraction
          double r0 = ((ior - 1) / (ior + 1)) * ((ior - 1) / (ior + 1));
          double r_theta = r0 + ((1 - r0) * pow((1 - cos_theta), 5));

          glm::mat4 trans_model;
          glm::vec3 trans_norm;
          double trans_t = 0;
          glm::vec3 trans_kd;
          glm::vec3 trans_ks;
          double trans_shininess;
          bool trans_transparency;
          double trans_ior;
          bool trans_ray_intersection = false;
          glm::vec3 trans_color = glm::vec3(0.0);

          // we need to find where this refracted ray exits the object from which point we will send another ray to
          // get us the colour refracted through our object
          // so we cast the refracted ray from the point of intersection to find it's point of exit
          bool reflected_ray_exits = hit(
                  scene,
                  point_of_intersection,
                  transmitted,
                  trans_model,
                  trans_t,
                  trans_norm,
                  trans_kd,
                  trans_ks,
                  trans_shininess,
                  trans_transparency,
                  trans_ior,
                  trans_ray_intersection);

          if (reflected_ray_exits) {
            // this should always be true
            glm::vec3 exit_point = point_of_intersection + float(trans_t) * transmitted;

            // the exiting ray has the same direction as the entering ray just a different origin
            // we want the colour of the exiting ray
            ray_color(
                    scene,
                    exit_point, transmitted,
                    ambient, lights,
                    trans_color,
                    remaining_bounces - 1
            );

						glm::vec3 total_color = 0.5f * ((float(r_theta) + reflected_color) + (float(1 - r_theta) + trans_color));
            col[0] += std::max(0.0f, std::min(1.0f, ks[0] * total_color[0]));
            col[1] += std::max(0.0f, std::min(1.0f, ks[1] * total_color[1]));
            col[2] += std::max(0.0f, std::min(1.0f, ks[2] * total_color[2]));

          }
        }
      }
    }
    else {
			reflected_color = 0.5f * reflected_color;
			col[0] += std::max(0.0f, std::min(1.0f, ks[0] * reflected_color[0]));
			col[1] += std::max(0.0f, std::min(1.0f, ks[1] * reflected_color[1]));
			col[2] += std::max(0.0f, std::min(1.0f, ks[2] * reflected_color[2]));
		}
    #endif
		#endif
  }
	else {
		//background colour
		// background color is a function of y position on the screen
		// background is moving from sky blue to orange
		// red is start from 120 and going to 250
		col[0] = 0;//(100.0 + ((float) y * 155.0 / (float) h)) / 255.0;
		// green is going from 170 to 180
		col[1] = 0;//150.0 / 255.0;
		//col[1] = (ray_direction.y < 0)? -ray_direction.y : ray_direction.y;
		//blue is going form 250 to 0
		col[2] = 0.4 + (2 * (ray_direction.y < 0)? -ray_direction.y : ray_direction.y);
	}

	return ray_intersection;
}

bool hit(
	SceneNode* scene,
	const glm::vec3 &ray_origin,
	const glm::vec3 &ray_direction,
	const glm::mat4 &world_to_model,
	double &ray_closest_t,
	glm::vec3 &intersection_normal,
	glm::vec3 &kd,
	glm::vec3 &ks,
	double &shininess,
	bool &transparency,
	double &ior,
	bool &ray_intersection
){
	glm::mat4 world_to_new_model = scene->get_inverse() * world_to_model;
	glm::vec3 ray_origin_in_model = glm::vec3(scene->get_inverse() * glm::vec4(ray_origin, 1.0));
	glm::vec3 ray_direction_in_model = glm::vec3(scene->get_inverse() * glm::vec4(ray_direction, 0.0));

	// use the intersection with the minimum t (closest intersection)
	// decide the color of the pixel
	// get ray color generated from eye going through pixel
	if(scene->m_nodeType == NodeType::GeometryNode){
		GeometryNode *gn = static_cast<GeometryNode*>(scene);
		double t;
		glm::vec3 normal;
		bool intersection = gn->m_primitive->intersect(ray_origin_in_model, ray_direction_in_model, t, normal);
		if(intersection){
			// std::cout << "GeometryChild" << std::endl;
			// std::cout << child->m_name << std::endl;
			if(!ray_intersection or t < ray_closest_t){
				ray_intersection = true;
				ray_closest_t = t;
				intersection_normal = glm::normalize(glm::vec3(glm::transpose(world_to_new_model) * glm::vec4(normal, 0.0)));
				PhongMaterial* phong_mat = static_cast<PhongMaterial *>(gn->m_material);
				kd = phong_mat->m_kd;
				ks = phong_mat->m_ks;
				shininess = phong_mat->m_shininess;
				transparency = phong_mat->m_transparent;
				ior = phong_mat->m_ior;
			}
			//set pixel to color of object
		}
	}
	else {
		// recurse on other children 
		for(SceneNode *child: scene->children){
			hit(child, ray_origin_in_model, ray_direction_in_model, world_to_new_model,
				ray_closest_t, intersection_normal, kd, ks, shininess, transparency, ior, ray_intersection);
		}
	}
	return ray_intersection;
}

void set_pixel(
	SceneNode* root,
	uint x, uint y,
	size_t h, size_t w,
	glm::vec3 &color,
	const glm::mat4 dcs_to_world,
	const glm::vec3 eye,
	const glm::vec3 ambient,
	const std::list<Light *> lights
){
	// convert from the pixel to the world coordinates
	// use world coordinates of the pixel along with the eye position
	// to generate the ray
	glm::vec4 p_world = dcs_to_world * glm::vec4(x, y, 0, 1);
	// std::cout << glm::to_string(p_world) << std::endl;
	glm::vec3 ray_direction = glm::normalize(glm::vec3(p_world) - eye);
	glm::mat4 world_to_model = glm::mat4();
	glm::vec3 col;
	ray_color(root, eye, ray_direction, ambient, lights, col, 4);
	color = col;
}

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
){
	for (uint y = ystart ; y < yend; ++y) {
		for (uint x = xstart; x < xend; ++x) {
			set_pixel
			( 
			root, 
			x, y, h, w, 
			std::ref(image_array[y*h + x]),
			dcs_to_world,
			eye,
			ambient,
			lights
			);
		}
	}
}

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
) {

  // Fill in raytracing code here...  

  std::cout << "Calling A4_Render(\n" <<
		  "\t" << *root <<
          "\t" << "Image(width:" << image.width() << ", height:" << image.height() << ")\n"
          "\t" << "eye:  " << glm::to_string(eye) << std::endl <<
		  "\t" << "view: " << glm::to_string(view) << std::endl <<
		  "\t" << "up:   " << glm::to_string(up) << std::endl <<
		  "\t" << "fovy: " << fovy << std::endl <<
          "\t" << "ambient: " << glm::to_string(ambient) << std::endl <<
		  "\t" << "lights{" << std::endl;

	for(const Light * light : lights) {
		std::cout << "\t\t" <<  *light << std::endl;
	}
	std::cout << "\t}" << std::endl;
	std:: cout <<")" << std::endl;

	std::chrono::system_clock::time_point start_time = std::chrono::system_clock::now();

	size_t h = image.height();
	size_t w = image.width();
	int d = glm::length(view);

	glm::mat4 dcs_to_world = generate_dcs_to_world_mat(w, h, d, fovy, eye, view, up);

	int y_threads = 64;
	int x_threads = 1;
	std::thread my_threads[y_threads][x_threads];
	glm::vec3 *image_array = new glm::vec3[h*w];
	#ifdef SHOW_PROGRESS
	std::cout << "Creating threads" << std::endl;
	#endif

	for (uint i = 0; i < y_threads ; ++i) {
		uint ystart = i * h / y_threads;
		uint yend = (i + 1) * h / y_threads;
		for(uint j = 0; j < x_threads; ++j) {
			uint xstart = j * w / x_threads;
			uint xend = (j+1) * w / x_threads;
			#ifdef MULTI_THREAD
			my_threads[i][j] = std::thread(set_segment,
										root, 
										ystart, yend,
										xstart, xend,
										h, w, 
										image_array,
										dcs_to_world,
										eye,
										ambient,
										lights
										);
			#else
			set_segment
			( 
			root, 
			ystart, yend,
			xstart, xend,
			h, w, 
			image_array,
			dcs_to_world,
			eye,
			ambient,
			lights
			);
			#endif
		}
		#ifdef SHOW_PROGRESS
		std::cout << "\r" << i * x_threads << " threads spun";
		#endif
	}
#ifdef SHOW_PROGRESS
	std::cout << std::endl << "All threads spinning" << std::endl;
#endif

#ifdef MULTI_THREAD
	for(int i = 0; i < y_threads; ++i){
		for(int j = 0; j < x_threads; ++j) {
			if(my_threads[i][j].joinable()){
				my_threads[i][j].join();
			}
		}
		#ifdef SHOW_PROGRESS
			std::cout << "\r" << i * x_threads << " Threads done";
		#endif
	}
	std::cout << std::endl << "Scene was rendered using multi-threading" << std::endl;
#endif
#ifdef REFLECTION
	std::cout << "Reflection: ENABLED" << std::endl;
#else
	std::cout << "Reflection: DISABLED" << std::endl;
#endif

#ifdef VOLUME_BOUNDS
	std::cout << "Scene was rendered using mesh volume bounding" << std::endl;
#else
	std::cout << "Scene was rendered without mesh volume bounding" << std::endl;
#endif

	for (uint y = 0; y < h; ++y) {
		for (uint x = 0; x < w; ++x) {
			image(x, y, 0) = image_array[y*h + x][0];
			image(x, y, 1) = image_array[y*h + x][1];
			image(x, y, 2) = image_array[y*h + x][2];
		}
	}

	std::chrono::system_clock::time_point end_time = std::chrono::system_clock::now();
	std::chrono::duration<double> time_elapsed = end_time - start_time;

	std::time_t start_time_t = std::chrono::system_clock::to_time_t(start_time);
	std::time_t end_time_t = std::chrono::system_clock::to_time_t(end_time);
	std::cout << "Start time: " << std::ctime(&start_time_t) << std::endl;
	std::cout << "End time: " << std::ctime(&end_time_t) << std::endl;
	std::cout << "Time elapsed: " << time_elapsed.count() << std::endl;
}
