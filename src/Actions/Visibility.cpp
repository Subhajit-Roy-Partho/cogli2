/*
 * VisibilityAction.cpp
 *
 *  Created on: Jul 7, 2020
 *      Author: lorenzo
 */

#include "Visibility.h"

#include "../Utils/Logging.h"
#include "../Drawables/Scene.h"


Visibility::Visibility(Camera &camera, std::string command, std::vector<int> ids) :
				BaseAction(camera),
				_ids(ids) {
	if(command == "hide") {
		_command = HIDE;
	}
	else if(command == "show") {
		_command = SHOW;
	}
	else {
		Logging::log_critical("Invalid visibility command %s", command.c_str());
		exit(1);
	}
}

Visibility::~Visibility() {

}

void Visibility::do_action(Scene *scene) {
	bool visible = (_command == SHOW);
	_set_visible(scene, visible);
}

void Visibility::undo_action(Scene *scene) {
	bool visible = (_command != SHOW);
	_set_visible(scene, visible);
}

void Visibility::_set_visible(Scene *scene, bool visible) {
	auto shapes = scene->get_shapes();

	for(auto id : _ids) {
		if(id < (int) shapes.size()) {
			shapes[id]->set_visible(visible);
		}
	}
}
