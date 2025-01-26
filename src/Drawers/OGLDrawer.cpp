/*
 * OGLDrawer.cpp
 *
 *  Created on: 20/ott/2013
 *      Author: lorenzo
 */

#include "OGLDrawer.h"

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

#include "../OGL/Shapes/OGLSphere.h"
#include "../OGL/Shapes/OGLSphericalCap.h"
#include "../OGL/Shapes/OGLCylinder.h"
#include "../OGL/Shapes/OGLIcosahedron.h"
#include "../OGL/Shapes/OGLCone.h"
#include "../Utils/MathHelper.h"
#include "../Utils/Logging.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/norm.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;

OGLDrawer::OGLDrawer() {
	_axes_arrows[0] = std::make_shared<Arrow>(Arrow(glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f), 0.2, 0.6, glm::vec4(1.f, 0.f, 0.f, 1.0f)));
	_axes_arrows[1] = std::make_shared<Arrow>(Arrow(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), 0.2, 0.6, glm::vec4(0.f, 1.f, 0.f, 1.0f)));
	_axes_arrows[2] = std::make_shared<Arrow>(Arrow(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f), 0.2, 0.6, glm::vec4(0.f, 0.f, 1.f, 1.0f)));
}

OGLDrawer::~OGLDrawer() {

}

void OGLDrawer::initialise() {
	_meshes.clear();

	_meshes["cylinder"] = OGLCylinder::get_mesh(_resolution);
	_meshes["cone"] = OGLCone::get_mesh(_resolution);
	_meshes["icosahedron"] = OGLIcosahedron::get_mesh();
	// maps are sorted in alphabetical order. We should make sure that the "sphere" entry is last so that
	// spheres get drawn last since they are usually the most abundant shapes and the transparency is 
	// sorted out at the shape level
	_meshes["sphere"] = OGLSphere::get_mesh(_resolution);
}

void OGLDrawer::increase_resolution() {
	_resolution++;
	Logging::log_debug("Increasing the resolution to %d\n", _resolution);
	initialise();
}

void OGLDrawer::decrease_resolution() {
	if(_resolution > 4) {
		_resolution--;
	}
	Logging::log_debug("Decreasing the resolution to %d\n", _resolution);
	initialise();
}

void OGLDrawer::toggle_selection_mode() {
	_selection_mode = !_selection_mode;
	Logging::log_debug("Toggling the selection mode to %d\n", _selection_mode);
}

void OGLDrawer::draw() {
	// the order with which the meshes are drawn is given by the order of the MeshType enum. We want spheres
	// to be drawn last, since these are the ones that are usually transparent
	for(auto pair : _meshes) {
		auto mesh = pair.second;
		if(mesh->has_instance_data()) {
			if(Mesh::manage_transparent_color()) {
				// here we sort the instance data according to the distance of each instance from the camera
				mesh->sort_instance_data(_camera_position);
			}
			mesh->draw_instances();
		}
	}
}

void OGLDrawer::draw_axes_arrows(std::shared_ptr<Shader> shader) {
	for(auto arrow : _axes_arrows) {
		mat4 cyl_model, cone_model;
		std::tie(cyl_model, cone_model) = _get_arrow_model_matrices(*(arrow.get()));
		vec4 color = arrow->color();

		shader->set_vec4("color", color);

		shader->set_mat4("model", cyl_model);
		glm::mat3 normal_matrix = glm::mat3(glm::transpose(glm::inverse(cyl_model)));
		shader->set_mat3("normal_matrix", normal_matrix);
		_meshes["cylinder"]->draw();

		shader->set_mat4("model", cone_model);
		normal_matrix = glm::mat3(glm::transpose(glm::inverse(cone_model)));
		shader->set_mat3("normal_matrix", normal_matrix);
		_meshes["cone"]->draw();
	}
}

void OGLDrawer::_add_instance_data_to_mesh(std::string type, const glm::mat4 &model_matrix, const glm::vec4 &color) {
	glm::mat3 inverse_model_matrix(glm::transpose(glm::inverse(model_matrix)));
	_meshes[type]->add_instance_data(model_matrix, inverse_model_matrix, _get_color(color));
}

glm::vec4 OGLDrawer::_get_color(glm::vec4 color) {
	if(_selection_mode) {
		int id = (_curr_shape == nullptr) ? -1 : _curr_shape->get_id();
		// get a unique color from the id
		int r = (id & 0x000000FF) >> 0;
		int g = (id & 0x0000FF00) >> 8;
		int b = (id & 0x00FF0000) >> 16;
		color = glm::vec4(r / 255.f, g / 255.f, b / 255.f, 1.f);
	}
	// if the current shape is selected then color it yellow
	else if(_curr_shape != nullptr && _curr_shape->is_selected()) {
		color = glm::vec4(1.f, 1.f, 0.f, 1.f);
	}

	return color;
}

void OGLDrawer::visit(Drawable &d) {

}

void OGLDrawer::visit(Scene &s) {
	for(auto pair : _meshes) {
		auto mesh = pair.second;
		mesh->clear_instance_data();
	}

	for(auto shape : s.get_shapes()) {
		if(shape->is_visible()) {
			_curr_shape = shape;
			shape->accept_drawer(*this);
		}
	}

	_curr_shape = nullptr;
	if(s.is_box_visible()) {
		for(auto cyl : s.get_box()) {
			cyl->accept_drawer(*this);
		}
	}

	s.set_dirty(false);
}

void OGLDrawer::visit(Sphere &s) {
	mat4 my_scale = glm::scale(s.r(), s.r(), s.r());
	mat4 my_trans = glm::translate(s.position());
	mat4 model = my_trans * my_scale;

	_add_instance_data_to_mesh("sphere", model, s.color());
}

void OGLDrawer::visit(Cylinder &c) {
	float r = c.r();
	vec3 axis = c.axis();
	vec3 position = c.position();

	mat4 my_scale = glm::scale(r, r, glm::length(axis));

	// the cylinder is drawn oriented towards z
	vec3 z_lab(0.f, 0.f, 1.f);
	mat4 my_rot = MathHelper::get_rotation(z_lab, axis);

	// the cylinder is drawn centred in the origin
	mat4 my_trans = glm::translate(position + 0.5f * axis);
	mat4 model = my_trans * my_rot * my_scale;

	_add_instance_data_to_mesh("cylinder", model, c.color());
}

void OGLDrawer::visit(Ellipsoid &e) {

	float a = e.a();
	float b = e.b();
	float c = e.c();

	vec3 axis1 = e.axis1();
	vec3 axis2 = e.axis2();
	vec3 axis3 = glm::cross(axis1, axis2);
	vec3 position = e.position();
	
	// scale matrix
	//mat4 my_scale = glm::scale(a / float(0.5), float(b / 0.5), float(c / 0.5));
	mat4 my_scale = glm::scale(a, b, c);
	
	// translation matrix
	mat4 my_trans = glm::translate(position);

	// get the rotation matrix
	vec3 x_lab(1.0f, 0.0f, 0.0f);
	vec3 z_lab(0.0f, 0.0f, 1.0f);

	mat4 x_rot = MathHelper::get_rotation(z_lab, axis3);
	vec3 new_x = glm::mat3(x_rot) * x_lab;
	mat4 z_rot = MathHelper::get_rotation(new_x, axis1);
	mat4 my_rot = z_rot * x_rot;
	
	mat4 mv_matrix = my_trans * my_rot * my_scale;
	
	_add_instance_data_to_mesh("sphere", mv_matrix, e.color());
}

void OGLDrawer::visit(Icosahedron &c) {
	float r = c.r();
	vec3 axis = c.axis();
	vec3 axis2 = c.axis2();
	vec3 position = c.position();

	// radius is 0.5 by default
	float default_r = 0.5f;
	mat4 my_scale = glm::scale(r / default_r, r / default_r, r / default_r);

	mat4 my_trans = glm::translate(position);

	vec3 z_lab(0.0f, 0.0f, 1.0f);
	vec3 x_lab(1.0f, 0.0f, 0.0f);
	mat4 x_rot = MathHelper::get_rotation(z_lab, axis);
	vec3 new_x = glm::mat3(x_rot) * x_lab;
	mat4 z_rot = MathHelper::get_rotation(new_x, axis2);

	mat4 my_rot = z_rot * x_rot;

	mat4 mv_matrix = my_trans * my_rot * my_scale;

	_add_instance_data_to_mesh("icosahedron", mv_matrix, c.color());

	std::vector<IcoPatch> patches = c.get_patches();
	for(std::vector<IcoPatch>::iterator it = patches.begin(); it != patches.end(); it++) {
		my_scale = glm::scale(it->radius, it->radius, it->radius);
		my_trans = glm::translate(position + it->pos);

		mv_matrix = my_trans * my_scale;
		vec4 color = it->color;

		_add_instance_data_to_mesh("sphere", mv_matrix, color);
	}
}

void OGLDrawer::visit(Group &g) {

}

void OGLDrawer::visit(DNAStrand &strand) {
	std::map<int, Nucleotide*> nucleotides = strand.nucleotides();
	std::map<int, Nucleotide*>::iterator it = nucleotides.begin();

	Nucleotide *last = nullptr;
	for(; it != nucleotides.end(); it++) {
		Nucleotide *current = it->second;
		// we have to design the backbone-backbone links
		if(it != nucleotides.begin()) {
			vec3 curr_back_pos = current->position() + current->backbone();

			vec3 last_back_pos = last->position() + last->backbone();

			vec3 cyl_axis = last_back_pos - curr_back_pos;

			Cylinder cyl_shape(curr_back_pos, cyl_axis, 0.13f, current->color());
			cyl_shape.accept_drawer(*this);
		}
		// to speed up things, we only show the nucleotides for resolution > 5
		if(_resolution > 5) {
			it->second->accept_drawer(*this);
		}
		last = it->second;
	}

	// if the strand is circular we also add a cylinder between the first and last nucleotides
	if(strand.is_circular()) {
		Nucleotide *first = nucleotides.begin()->second;
		vec3 curr_back_pos = first->position() + first->backbone();
		vec3 last_back_pos = last->position() + last->backbone();

		vec3 cyl_axis = last_back_pos - curr_back_pos;

		Cylinder cyl_shape(curr_back_pos, cyl_axis, 0.13f, first->color());
		cyl_shape.accept_drawer(*this);
	}
}

void OGLDrawer::visit(Nucleotide &n) {
	vec3 x = n.principal_axis();
	vec3 z = n.stacking_axis();
	vec3 pos = n.position();

	// the backbone position depends whether mm grooving is enabled or not
	vec3 back_pos = pos + n.backbone();

	Sphere back_sphere(back_pos, n.backbone_r(), n.color());
	back_sphere.accept_drawer(*this);

	// base
	vec3 base_pos = pos + x * 0.3f;
	float base_r = 0.1f;
	vec3 z_lab(0.f, 0.f, 1.f);
	mat4 x_rot = MathHelper::get_rotation(z_lab, x);
	vec3 x_lab(1.f, 0.f, 0.f);
	vec3 new_x = glm::mat3(x_rot) * x_lab;
	mat4 z_rot = MathHelper::get_rotation(new_x, z);
	mat4 my_trans = glm::translate(base_pos);

	vec4 color = n.base_color();

	// we draw ellipsoids by taking a sphere and deforming it along the 3 axes by this much
	vec3 ratios = vec3(0.8f * base_r, 2.f * base_r, 3.f * base_r);
	glm::mat4 my_scale = glm::scale(ratios);

	mat4 model = my_trans * z_rot * x_rot * my_scale;
	_add_instance_data_to_mesh("sphere", model, color);

	// com --> base
	float cyl_r = 0.1f;
	Cylinder cyl_com_base(pos, x * 0.3f * 0.8f, cyl_r, n.color());
	cyl_com_base.accept_drawer(*this);

	// backbone -- com
	Cylinder cyl_back_base(pos, n.backbone(), cyl_r, n.color());
	cyl_back_base.accept_drawer(*this);

	// sfere at com
	Sphere base_sphere(pos, cyl_r, n.color());
	base_sphere.accept_drawer(*this);
}

void OGLDrawer::visit(DHS &s) {
	vec3 dipole = s.dipole() * DIP_FACTOR;
	float cyl_r = s.dipole_cylinder_radius();
	float cyl_length = s.dipole_cylinder_length() * DIP_FACTOR;
	vec3 arrow_position = s.position() - 0.5f * dipole;

	Sphere sphere(s.position(), s.r(), s.color());
	Arrow arrow(arrow_position, dipole, cyl_r, cyl_length, glm::vec4(0.f, 0.f, 0.f, 1.f));

	sphere.accept_drawer(*this);
	arrow.accept_drawer(*this);
}

std::tuple<glm::mat4, glm::mat4> OGLDrawer::_get_arrow_model_matrices(Arrow &arrow) {
	vec3 axis = arrow.axis();
	float cyl_r = arrow.r_cyl();
	float cyl_length = arrow.cyl_length();
	float cone_length = arrow.cone_length();
	float tot_length = cyl_length + cone_length;
	vec3 position = arrow.position();

	// the arrow is drawn centred in the origin
	mat4 model = glm::translate(position);

	// the arrow is drawn oriented towards z
	vec3 z_lab(0.f, 0.f, 1.f);
	model *= MathHelper::get_rotation(z_lab, axis);
	model *= glm::scale(cyl_r, cyl_r, tot_length);

	float head_ratio = cone_length / tot_length;
	float base_to_origin = (1.f - head_ratio) * 0.5f;

	mat4 cyl_model = model;
	cyl_model *= glm::translate(0.f, 0.f, base_to_origin);
	cyl_model *= glm::scale(0.4f, 0.4f, 1.f - head_ratio);

	float cone_base_to_cyl_end = 0.5f * head_ratio + 2.f * base_to_origin;
	// the cone is drawn with its centre in the origin: we move it so that its base coincides with the end of the cylinder
	mat4 cone_model = model;
	cone_model *= glm::translate(0.f, 0.f, cone_base_to_cyl_end);
	cone_model *= glm::scale(0.8f, 0.8f, head_ratio);

	return std::make_tuple(cyl_model, cone_model);
}

void OGLDrawer::visit(Arrow &s) {
	mat4 cyl_model, cone_model;
	std::tie(cyl_model, cone_model) = _get_arrow_model_matrices(s);
	vec4 color = s.color();

	_add_instance_data_to_mesh("cylinder", cyl_model, color);
	_add_instance_data_to_mesh("cone", cone_model, color);
}

void OGLDrawer::visit(PatchyParticle &p) {
	Sphere sphere(p.position(), p.r(), p.color());
	sphere.accept_drawer(*this);

	std::vector<Patch> patches = p.get_patches();

	vec3 apex = p.position();
	for(std::vector<Patch>::iterator it = patches.begin(); it != patches.end(); it++) {
		vec3 base = it->pos;
		float r = glm::length(it->pos);

		// draw the cone
		float height = r * it->cosmax;
		// this is because the base radius (NOT THE DIAMETER) of the cone is 1
		float width = r * sinf(it->width);

		mat4 my_scale = glm::scale(width, width, height);

		// the cone has its base in the -z direction
		vec3 z_lab(0.f, 0.f, -1.f);
		mat4 my_rot = MathHelper::get_rotation(z_lab, base);

		// this moves the apex in 0, 0, 0 and the base in 0, 0, -1
		mat4 apex_trans = glm::translate(vec3(0.f, 0.f, -0.5f));

		// and this moves the whole thing so that the apex overlaps with
		// the particle's centre
		mat4 my_trans = glm::translate(apex);

		mat4 mv_matrix = my_trans * my_rot * my_scale * apex_trans;
		_add_instance_data_to_mesh("cone", mv_matrix, it->color);

		// draw the spherical cap
		my_scale = glm::scale(r, r, r);

		// the cap points towards z
		z_lab = vec3(0.f, 0.f, 1.f);
		my_rot = MathHelper::get_rotation(z_lab, base);

		mv_matrix = my_trans * my_rot * my_scale;
		std::string key = StringHelper::sformat("cap%lf", it->cosmax);
		if(_meshes.find(key) == _meshes.end()) {
			_meshes[key] = OGLSphericalCap::get_mesh(it->cosmax, _resolution);
		}
		_add_instance_data_to_mesh(key, mv_matrix, it->color);
	}

}

void OGLDrawer::visit(Triangle &t) {
	std::vector<Vertex> vertices;
	for(auto v: t.vertices()) {
		Vertex new_v;
		new_v.position = v;
		new_v.normal = glm::vec3(1, 1, 1);
	}
	static const std::vector<unsigned int> indices({0, 1, 2});
	Mesh m_t(vertices, indices);
	m_t.draw();

//	glBegin(GL_TRIANGLES);
////	set_color(t.color());
//	for(int i = 0; i < 3; i++) {
//		glVertex3f(vs[i][0], vs[i][1], vs[i][2]);
//	}
//	glEnd();
//
//	glEnable(GL_POLYGON_OFFSET_LINE);
//	glPolygonOffset(-1, -1);
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//	glLineWidth(5);
//
//	glBegin(GL_TRIANGLES);
////	set_color(vec4(0, 0, 0, 1));
//	for(int i = 0; i < 3; i++) {
//		glVertex3f(vs[i][0], vs[i][1], vs[i][2]);
//	}
//	glDisable(GL_POLYGON_OFFSET_LINE);
//	glEnd();
//
//	glLineWidth(1);
//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
