/*
 * Ellipsoid.cpp
 *
 *  Created on: 16/luh/2018
 *      Author: flavio
 */

#include "Ellipsoid.h"
#include "../Drawers/Drawer.h"

using glm::vec3;
using glm::vec4;

Ellipsoid::Ellipsoid(vec3 pos, float r, glm::vec3 axis1, glm::vec3 axis2, float a, float b, float c, vec4 color) : Sphere(pos, r, color), _axis1(axis1), _axis2(axis2), _a(a), _b(b), _c(c) {

}

Ellipsoid::~Ellipsoid() {

}

void Ellipsoid::accept_drawer(Drawer &d) {
	d.visit(*this);
}

void Ellipsoid::rotate(glm::mat3 R, glm::vec3 wrt) {
	Shape::rotate(R, wrt);
	_axis1 = R * _axis1;
	_axis2 = R * _axis2;
}
