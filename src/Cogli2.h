/*
 * Cogli1.h
 *
 *  Created on: 20/ott/2013
 *      Author: lorenzo
 */

#ifndef COGLI2_H_
#define COGLI2_H_

#include <vector>
#include <string>
#include <map>
#include <memory>

#include "Drawables/Scene.h"
#include "Parsers/Parser.h"
#include "optionparser.h"
#include "Camera.h"
#include "Lighting.h"
#include "Blender/BlenderManager.h"
#include "povray/PovManager.h"

class Cogli1 {
	enum VisibilityState {
		SHOW,
		HIDE
	};
protected:
	std::vector<std::shared_ptr<Scene>> _scenes;
	option::Option *_options;
	std::shared_ptr<Parser> _parser;
	Camera _camera;
	LightingState _lighting;
	BlenderManager _blender_manager;
	PovManager _pov_manager;
	CentreOptions _centre;

	VisibilityState _default_visibility_state;
	std::map<int, VisibilityState> _invisibile_particles;

	void _get_scenes();
	void _parse_visibility_file(const char *v_file);

public:
	Cogli1(option::Parser &parser, option::Option *options);
	virtual ~Cogli1();

	virtual void start_ogl();
	virtual void print_all_pov();
	virtual void print_all_blender();
};

#endif /* COGLI2_H_ */
