/*
 * VisibilityAction.h
 *
 *  Created on: Jul 7, 2020
 *      Author: lorenzo
 */

#ifndef ACTIONS_VISIBILITY_H_
#define ACTIONS_VISIBILITY_H_

#include "BaseAction.h"

#include <string>
#include <vector>

class Visibility: public BaseAction {
	enum VisibilityState {
		SHOW,
		HIDE
	};
public:
	Visibility(Camera &camera, std::string command, std::vector<int> ids);
	virtual ~Visibility();

	void do_action(Scene *) override;
	void undo_action(Scene *) override;

private:
	VisibilityState _command;
	std::vector<int> _ids;

	void _set_visible(Scene *, bool visible);
};

#endif /* ACTIONS_VISIBILITY_H_ */
