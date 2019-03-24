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
	glm::vec3 &normal
){
	#ifdef RVOLUME_BOUNDS
	NonhierBox b = NonhierBox(min_point, max_size);
	return b.intersect(eye, direction, t, normal);
	#elifdef VOLUME_BOUNDS
	NonhierBox b = NonhierBox(min_point, max_size);
	if(!b.intersect(eye, direction, t, normal)){
		return false;
	}
	#endif
	t = 0;
	bool intersection = false;
	for(Triangle tri: m_faces){
		glm::vec3 p0 = m_vertices[tri.v1];
		glm::vec3 p1 = m_vertices[tri.v2] - p0;
		glm::vec3 p2 = m_vertices[tri.v3] - p0;
		glm::vec3 R = eye - p0;

		glm::mat3 D = glm::mat3(p1, p2, -direction);
		glm::mat3 D1 = glm::mat3(R, p2, -direction);
		glm::mat3 D2 = glm::mat3(p1, R, -direction);
		glm::mat3 D3 = glm::mat3(p1, p2, R);

		double beta = glm::determinant(D1) / glm::determinant(D);
		double gamma = glm::determinant(D2) / glm::determinant(D);
		double current_t = glm::determinant(D3) / glm::determinant(D);

		if(
			beta > 0.0 && gamma > 0.0 &&
			beta + gamma <= 1.0 &&
			current_t > EPSILON &&
			(!intersection || current_t < t)
		){
			// std::cout << current_t << std::endl;
			intersection = true;
			t = current_t;
			normal = glm::normalize(glm::cross(p1, p2));
		}
	}
	return intersection;
}
