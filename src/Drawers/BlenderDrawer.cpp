/*
 * BlenderDrawer.cpp
 *
 *  Created on: 20/ott/2013
 *      Author: lorenzo
 */

#include <vector>
#include <sstream>
#include <string>
#include <iostream>

#include "BlenderDrawer.h"

#include "../Drawables/Drawable.h"
#include "../Drawables/Scene.h"
#include "../Drawables/Cylinder.h"
#include "../Drawables/Icosahedron.h"
#include "../Drawables/Ellipsoid.h"
#include "../Drawables/Arrow.h"
#include "../Drawables/Sphere.h"
#include "../Drawables/DNA.h"
#include "../Drawables/DHS.h"
#include "../Drawables/PatchyParticle.h"
#include "../Drawables/Triangle.h"
#include "../Drawables/Group.h"

#include "../Utils/MathHelper.h"
#include "../Utils/StringHelper.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

using namespace std;
using glm::vec3;
using glm::vec4;
using glm::mat4;

BlenderDrawer::BlenderDrawer() :
		_out(nullptr), _patchy_as_kf(false) {
}

BlenderDrawer::~BlenderDrawer() {

}

void BlenderDrawer::init() {

}

void BlenderDrawer::visit(Drawable &d) {

}

void BlenderDrawer::visit(Scene &s) {
	for(auto shape : s.get_shapes()) {
		if(shape->is_visible()) {
			shape->accept_drawer(*this);
		}
	}

	if(s.is_box_visible()) {
		fprintf(_out, "# box output\n");
		for(auto cyl : s.get_box()) {
			cyl->accept_drawer(*this);
		}
		fprintf(_out, "# end of box output\n");
	}
}

void BlenderDrawer::visit(Cylinder &c) {

}

void BlenderDrawer::visit(Icosahedron &s) {
	printf("Blender for icosahedra not implemented yet\n");
	return;
}

void BlenderDrawer::visit(Ellipsoid &s) {
	printf("Blender for ellipsoids not implemented yet\n");
	return;
}

void BlenderDrawer::visit(Sphere &s) {
	static bool sphere_printed = false;

	vec4 color = s.color();
	vec3 pos = s.position();

	fprintf(_out, "# particle %d\n", s.get_id());
	std::string radius = StringHelper::sformat("radius=%lf", s.r());
	std::string location = StringHelper::sformat("location=(%lf,%lf,%lf)", pos[0], -pos[2], pos[1]);
	if(!sphere_printed) {
		fprintf(_out, "bpy.ops.mesh.primitive_uv_sphere_add(%s,%s)\n", radius.c_str(), location.c_str());
		fprintf(_out, "sphere = bpy.context.object\n");
		sphere_printed = true;
	}
	else {
		fprintf(_out, "ob = sphere.copy()\n");
		fprintf(_out, "ob.data = sphere.data.copy()\n");
		fprintf(_out, "ob.%s\n", location.c_str());
		fprintf(_out, 
		"mat = bpy.data.materials.new(name='mat_%d')\n"
		"mat.diffuse_color = (%lf,%lf,%lf,%lf)\n"
		"ob.active_material = mat\n", s.get_id(), color[0], color[1], color[2], color[3]
		);
		fprintf(_out, "bpy.context.collection.objects.link(ob)\n");
	}
}

void BlenderDrawer::visit(Group &) {

}

void BlenderDrawer::visit(DNAStrand &strand) {

}

void BlenderDrawer::visit(Nucleotide &n) {
	
}

void BlenderDrawer::visit(DHS &s) {
	
}

void BlenderDrawer::visit(Arrow &a) {
	
}

string BlenderDrawer::_patches_as_textures(PatchyParticle &p, const string &texture_name) {
	stringstream ss;
	

	return ss.str();
}

string BlenderDrawer::_patches_as_kf(PatchyParticle &p, const string &texture_name) {
	stringstream ss;


	return ss.str();
}

void BlenderDrawer::visit(PatchyParticle &p) {
	vec3 pos = p.position();
	float r = p.r();

	string texture_name = StringHelper::sformat("my_texture_%d", p.get_id());
	string patches = (_patchy_as_kf) ? _patches_as_kf(p, texture_name) : _patches_as_textures(p, texture_name);
	fprintf(_out, "%s", patches.c_str());

	fprintf(_out, "\n// patchy particle %d\n", p.get_id());
	fprintf(_out, "sphere {\n");
	fprintf(_out, "\t<%f, %f, %f> %f\n", pos[0], pos[1], pos[2], r);
	fprintf(_out, "\ttexture { %s }\n", texture_name.c_str());
	fprintf(_out, "\tinterior { ior 1. }\n");
	fprintf(_out, "\thollow\n");
	fprintf(_out, "}\n");
}

void BlenderDrawer::visit(Triangle &t) {

}
