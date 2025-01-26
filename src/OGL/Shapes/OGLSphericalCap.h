/*
 * OGLSphericalCap.h
 *
 *  Created on: 01/mar/2021
 *      Author: lorenzo
 */

#ifndef OGLSPHERICALCAP_H_
#define OGLSPHERICALCAP_H_

#include "Mesh.h"

#include <glm/glm.hpp>
#include <glad/gl.h>

#include <vector>
#include <memory>

class OGLSphericalCap {
protected:
	static int _resolution;
public:
	OGLSphericalCap() = delete;
	virtual ~OGLSphericalCap() = delete;

	static std::shared_ptr<Mesh> get_mesh(float cosmax, int res);
};

#endif /* OGLSPHERICALCAP_H_ */
