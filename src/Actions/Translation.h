/*
 * Translation.h
 *
 *  Created on: 31 gen 2018
 *      Author: lorenzo
 */

#ifndef ACTIONS_TRANSLATION_H_
#define ACTIONS_TRANSLATION_H_

#include "BaseAction.h"

#include <glm/glm.hpp>

class Translation: public BaseAction {
protected:
	glm::vec3 _direction;
	float _extent;
public:
	Translation(Camera &camera, glm::vec3 direction, float extent);
	virtual ~Translation();

	virtual void do_action(Scene *);
	virtual void undo_action(Scene *);
};

#endif /* ACTIONS_TRANSLATION_H_ */
