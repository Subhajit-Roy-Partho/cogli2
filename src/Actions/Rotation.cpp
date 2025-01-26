/*
 * Rotation.cpp
 *
 *  Created on: 31 gen 2018
 *      Author: lorenzo
 */

#include "Rotation.h"

#include "../Camera.h"

using namespace glm;

Rotation::Rotation(Camera &camera, vec3 axis, float angle) :
				BaseAction(camera),
				_axis(glm::normalize(axis)),
				_angle(angle) {

}

Rotation::~Rotation() {

}

void Rotation::do_action(Scene *) {
	_camera.rotate(_axis, _angle);
}

void Rotation::undo_action(Scene *) {
	_camera.rotate(_axis, -_angle);
}
