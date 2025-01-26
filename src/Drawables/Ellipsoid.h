/*
 * Ellipsoid.h
 *
 *  Created on: 16/lug/2018
 *      Author: flavio
 */

#ifndef ELLIPSOID_H_
#define ELLIPSOID_H_

#include "Sphere.h"
#include <vector>

class Ellipsoid: public Sphere {
protected:
	glm::vec3 _axis1, _axis2;
	float _a, _b, _c;
	
public:
	Ellipsoid(glm::vec3 pos, float r, glm::vec3 axis1, glm::vec3 axis2, float a, float b, float c, glm::vec4 color);
	virtual ~Ellipsoid();

	virtual void accept_drawer(Drawer &d);
	virtual glm::vec3 axis1() { return _axis1; }
	virtual glm::vec3 axis2() { return _axis2; }
	float a() { return _a; }
	float b() { return _b; }
	float c() { return _c; }
	virtual void rotate(glm::mat3, glm::vec3);
};

#endif /* ELLIPSOID_H_ */
