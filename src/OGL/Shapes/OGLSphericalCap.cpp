/*
 * OGLSphericalCap.cpp
 *
 *  Created on: 01/mar/2021
 *      Author: lorenzo
 */

#include "OGLSphericalCap.h"
#include "../utils.h"
#include "../../Utils/MathHelper.h"

int OGLSphericalCap::_resolution = -1;

std::shared_ptr<Mesh> OGLSphericalCap::get_mesh(float cosmax, int res) {
	float radius = 1.f;
	float const t_max = 1.f / (float) (res - 1);
	float const s_max = 1.f / (float) (res - 1);

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	for(int curr_t = 0; curr_t < res; curr_t++) {
		float const phi = glm::pi<float>() * curr_t * t_max;
		for(int curr_s = 0; curr_s < res; curr_s++) {
			float const theta = 2.f * glm::pi<float>() * curr_s * s_max;
			float const x = cosf(theta) * sinf(phi);
			float const y = sinf(theta) * sinf(phi);
			float const z = cosf(phi);

			Vertex v;
			v.position[0] = x * radius;
			v.position[1] = y * radius;
			v.position[2] = z * radius;

			v.normal[0] = x;
			v.normal[1] = y;
			v.normal[2] = z;

			vertices.push_back(v);
		}
	}

	float cut_height = radius * cosmax;
	for(int curr_t = 0; curr_t < res - 1; curr_t++) {
		for(int curr_s = 0; curr_s < res - 1; curr_s++) {
			if(vertices[curr_t * res + curr_s].position.z >= cut_height &&
					vertices[curr_t * res + (curr_s + 1)].position.z >= cut_height &&
					vertices[(curr_t + 1) * res + (curr_s + 1)].position.z >= cut_height) {
				indices.push_back(curr_t * res + curr_s);
				indices.push_back(curr_t * res + (curr_s + 1));
				indices.push_back((curr_t + 1) * res + (curr_s + 1));
			}

			if(vertices[(curr_t + 1) * res + (curr_s + 1)].position.z >= cut_height &&
					vertices[(curr_t + 1) * res + curr_s].position.z >= cut_height &&
					vertices[curr_t * res + curr_s].position.z >= cut_height) {
				indices.push_back((curr_t + 1) * res + (curr_s + 1));
				indices.push_back((curr_t + 1) * res + curr_s);
				indices.push_back(curr_t * res + curr_s);
			}
		}
	}

	return std::make_shared<Mesh>(vertices, indices);
}
