/*
 * BlenderManager.h
 *
 *  Created on: 07/nov/2013
 *      Author: lorenzo
 */

#ifndef BLENDERMANAGER_H_
#define BLENDERMANAGER_H_

#include <string>

#include "../Drawers/BlenderDrawer.h"
#include "../Drawables/Scene.h"
#include "../optionparser.h"
#include "../Camera.h"
#include "../Lighting.h"

class BlenderManager {
protected:
	option::Option *_options;
	BlenderDrawer _drawer;

public:
	BlenderManager(option::Option *options);
	virtual ~BlenderManager();

	void print_scene(Scene &scene, Camera &camera, LightingState &lighs);
};

#endif /* BLENDERMANAGER_H_ */
