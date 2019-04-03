// Winter 2019

#include <iostream>
#include <fstream>

#include <glm/ext.hpp>

// #include "cs488-framework/ObjFileDecoder.hpp"
#include "Mesh.hpp"

Mesh::Mesh( const std::string& fname )
	: m_vertices()
	, m_faces()
{
	std::string code;
	double vx, vy, vz;
	size_t s1, s2, s3;

	std::ifstream ifs( fname.c_str() );
	while( ifs >> code ) {
		if( code == "v" ) {
			ifs >> vx >> vy >> vz;
			m_vertices.push_back( glm::vec3( vx, vy, vz ) );
			if(vx < min_point[0]){
				min_point[0] = vx;
			}
			if(vy < min_point[1]){
				min_point[1] = vy;
			}
			if(vz < min_point[2]){
				min_point[2] = vz;
			}

			if(vx - min_point[0] > max_size){
				max_size = vx - min_point[0];
			}
			if(vy - min_point[1] > max_size){
				max_size = vy - min_point[1];
			}
			if(vz - min_point[2] > max_size){
				max_size = vz - min_point[2];
			}

		} else if( code == "f" ) {
			ifs >> s1 >> s2 >> s3;
			m_faces.push_back( Triangle( s1 - 1, s2 - 1, s3 - 1 ) );
		}
	}
}

std::ostream& operator<<(std::ostream& out, const Mesh& mesh)
{
  out << "mesh {";
  /*
  
  for( size_t idx = 0; idx < mesh.m_verts.size(); ++idx ) {
  	const MeshVertex& v = mesh.m_verts[idx];
  	out << glm::to_string( v.m_position );
	if( mesh.m_have_norm ) {
  	  out << " / " << glm::to_string( v.m_normal );
	}
	if( mesh.m_have_uv ) {
  	  out << " / " << glm::to_string( v.m_uv );
	}
  }

*/
  out << "}";
  return out;
}

bool Mesh::intersect(
	const glm::vec3 eye,
	const glm::vec3 direction,
	double &t,
	glm::vec3 &normal,
	glm::vec2 &uv
){
	#ifdef RVOLUME_BOUNDS
	NonhierBox b = NonhierBox(min_point, max_size);
	return b.intersect(eye, direction, t, normal, uv);
	#elif defined VOLUME_BOUNDS
	NonhierBox b = NonhierBox(min_point, max_size);
	if(!b.intersect(eye, direction, t, normal, uv)){
		return false;
	}
	#endif
	t = 0;
	bool intersection = false;
	for(Triangle tri: m_faces){
		glm::vec3 e1 = glm::normalize(m_vertices[tri.v1] - m_vertices[tri.v2]);
		glm::vec3 e2 = glm::normalize(m_vertices[tri.v1] - m_vertices[tri.v3]);
		glm::vec3 n = glm::normalize(glm::cross(e1, e2));

		glm::vec3 p0 = eye - m_vertices[tri.v1];
		double current_t = glm::dot(-n, p0) / glm::dot(n, direction);

		if(current_t > EPSILON && (!intersection || current_t < t)){
			glm::vec3 poi = eye + (float(current_t) * direction);

			glm::vec3 d1 = glm::normalize(m_vertices[tri.v1] - poi);
			glm::vec3 d2 = glm::normalize(m_vertices[tri.v2] - poi);
			glm::vec3 d3 = glm::normalize(m_vertices[tri.v3] - poi);

			double a1 = glm::angle(d1, d2);
			double a2 = glm::angle(d2, d3);
			double a3 = glm::angle(d3, d1);
			double angle_sum = (a1 + a2 + a3);
            double angle_dif = (angle_sum > (2 * M_PI))? angle_sum - (2 * M_PI) : (2 * M_PI) - angle_sum;

			if(angle_dif < EPSILON){
				t = current_t;
				normal = n;
				intersection = true;
			}
		}
	}
	return intersection;
}
