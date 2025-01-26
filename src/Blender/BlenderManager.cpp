/*
 * BlenderManager.cpp
 *
 *  Created on: 26/apr/2024
 *      Author: lorenzo
 */

#include <algorithm>
#include <cstdlib>
#include <vector>

#include "BlenderManager.h"
#include "../Utils/StringHelper.h"
#include "../Utils/MathHelper.h"
#include "../Utils/Logging.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "../cl_arguments.h"

using std::string;
using glm::vec3;
using glm::dvec3;
using glm::vec4;
using glm::mat4;
using glm::dmat4;

BlenderManager::BlenderManager(option::Option *options) : _options(options) {
	if(options[PATCHY_KF]) _drawer.set_patchy_as_kf(true);
}

BlenderManager::~BlenderManager() {

}

void BlenderManager::print_scene(Scene &scene, Camera &camera, LightingState &lights) {
	// we first take out the : in the scene's title and then we substitute any space with underscores
	string filename = StringHelper::sanitize_filename(scene.get_title());
	filename += ".py";

	Logging::log_info("Generating a Blender python script for scene '%s' on file '%s'\n", scene.get_title().c_str(), filename.c_str());

	FILE *out = fopen(filename.c_str(), "w");

	fprintf(out, R"LIT(
# in blender the UP direction is Z, while we use Y in cogli
# to avoid issues coordinates are printed as (x, -z, y)
import bpy
import mathutils

)LIT");

	vec3 cam_pos = camera.position();
	vec3 look_at = camera.look_at();
	std::string camera_stuff = R"LIT(
def point_at(obj, target, roll=0):
    """
    Rotate obj to look at target

    :arg obj: the object to be rotated. Usually the camera
    :arg target: the location (3-tuple or Vector) to be looked at
    :arg roll: The angle of rotation about the axis from obj to target in radians. 

    Based on: https://blender.stackexchange.com/a/127440
    """
    if not isinstance(target, mathutils.Vector):
        target = mathutils.Vector(target)
    loc = obj.location
    # direction points from the object to the target
    direction = target - loc
    tracker, rotator = (('-Z', 'Y'),'Z') if obj.type=='CAMERA' else (('X', 'Z'),'Y') #because new cameras points down(-Z), usually meshes point (-Y)
    quat = direction.to_track_quat(*tracker)
    
    # /usr/share/blender/scripts/addons/add_advanced_objects_menu/arrange_on_curve.py
    quat = quat.to_matrix().to_4x4()
    rollMatrix = mathutils.Matrix.Rotation(roll, 4, rotator)

    # remember the current location, since assigning to obj.matrix_world changes it
    loc = loc.to_tuple()
    obj.matrix_world = quat @ rollMatrix
    obj.location = loc

cam = bpy.context.scene.camera
)LIT";

	camera_stuff += StringHelper::sformat("cam.location = (%lf, %lf, %lf)\n", cam_pos[0], -cam_pos[2], cam_pos[1]);
	camera_stuff += StringHelper::sformat("point_at(cam, (%lf, %lf, %lf))\n", look_at[0], -look_at[2], look_at[1]);
	camera_stuff += R"LIT(
bpy.context.scene.camera = cam
)LIT";

	fprintf(out, "%s\n", camera_stuff.c_str());

	_drawer.init();
	_drawer.set_out_file(out);
	_drawer.visit(scene);

	fprintf(out, "bpy.ops.object.select_all(action='DESELECT')\n");

	fclose(out);

	Logging::log_info("done\n");
}
