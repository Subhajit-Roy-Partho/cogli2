/*
 * OGLSphere.cpp
 *
 *  Created on: 03/nov/2013
 *      Author: lorenzo
 */

#include "OGLSphere.h"
#include "../utils.h"
#include "../../Utils/MathHelper.h"

int OGLSphere::_resolution = -1;

std::shared_ptr<Mesh> OGLSphere::get_mesh(int res) {
	float radius = 1.f;
	float const t_max = 1.f / (float) (res - 1);
	float const s_max = 1.f / (float) (res - 1);

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	vertices.resize(res * res);
	auto v = vertices.begin();
	for(int curr_t = 0; curr_t < res; curr_t++) {
		float const phi = glm::pi<float>() * curr_t * t_max;
		for(int curr_s = 0; curr_s < res; curr_s++) {
			float const theta = 2.f * glm::pi<float>() * curr_s * s_max;
			float const x = cosf(theta) * sinf(phi);
			float const y = sinf(theta) * sinf(phi);
			float const z = cosf(phi);

			v->position[0] = x * radius;
			v->position[1] = y * radius;
			v->position[2] = z * radius;

			v->normal[0] = x;
			v->normal[1] = y;
			v->normal[2] = z;

			v++;
		}
	}

	indices.resize(res * res * 6);
	auto i = indices.begin();
	for(int curr_t = 0; curr_t < res - 1; curr_t++) {
		for(int curr_s = 0; curr_s < res - 1; curr_s++) {
			*i++ = curr_t * res + curr_s;
			*i++ = curr_t * res + (curr_s + 1);
			*i++ = (curr_t + 1) * res + (curr_s + 1);

			*i++ = (curr_t + 1) * res + (curr_s + 1);
			*i++ = (curr_t + 1) * res + curr_s;
			*i++ = curr_t * res + curr_s;
		}
	}

	return std::make_shared<Mesh>(vertices, indices);
}
