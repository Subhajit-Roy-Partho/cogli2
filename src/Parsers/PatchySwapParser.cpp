/*
 * PatchySwapParser.cpp
 *
 *  Created on: 20/ott/2013
 *      Author: lorenzo
 */

#include <glm/gtc/matrix_access.hpp>

#include "PatchySwapParser.h"
#include "../Utils/StringHelper.h"
#include "../Utils/Logging.h"
#include "../Drawables/PatchyParticle.h"
#include "../Drawables/Sphere.h"
#include "../Drawables/Cylinder.h"

using std::string;
using std::vector;
using glm::vec3;
using glm::vec4;

PatchySwapParser::PatchySwapParser(string topology, const std::queue<string> &input_files, string opts) : Parser(input_files) {
	_species_colors.push_back(glm::vec4(1, 0, 0, 1));
	_species_colors.push_back(glm::vec4(0, 1, 0, 1));
	_species_colors.push_back(glm::vec4(0, 0, 1, 1));

	_species_colors.push_back(glm::vec4(1, 1, 0, 1));
	_species_colors.push_back(glm::vec4(0, 1, 1, 1));
	_species_colors.push_back(glm::vec4(1, 0, 1, 1));

	_species_colors.push_back(glm::vec4(1, 0.6, 0, 1));

	_species_colors.push_back(glm::vec4(0.56f, 0.f, 1.f, 1.f));
	_species_colors.push_back(glm::vec4(0.647f, 0.165f, 0.165f, 1.f));
	_species_colors.push_back(glm::vec4(0.98f, 0.855f, 0.867f, 1.f));

	_read_topology(topology);

	vector<string> spl = StringHelper::split(opts, ',');
	if(spl.size() < 2) {
		Logging::log_critical("The --patchy-swap option (%s) requires a comma-separated value containing two fields (delta and cosmax).\n", opts.c_str());
		exit(1);
	}
	StringHelper::from_string<float>(spl[0], _delta);
	StringHelper::from_string<float>(spl[1], _cosmax);
	_theta = acos(_cosmax);
}

PatchySwapParser::~PatchySwapParser() {

}

Scene *PatchySwapParser::next_scene() {
	Scene *my_scene = new Scene();

	try {
		_open_input_if_required();

		_parse_headers(_current_input, my_scene);

		// set scene title
		std::stringstream title;
		title << _current_input_name << ": " << _conf_index << ", time: " << _time;
		my_scene->set_base_title(title.str());

		int curr_line = 3;
		bool done = false;
		int index = 0;
		int curr_limit = _N_per_species[0];
		int curr_species = 0;
		while(!done) {
			string buff;
			int current = _current_input.tellg();
			getline(_current_input, buff);
			// if the first character is a 't' and the string contains a has 3
			// entries we have reached the end of the configuration. If this is
			// the case, we have to be sure that the next time we read from this
			// file, we get the first line.
			vector<string> spl = StringHelper::split(buff);
			if(buff[0] == 't' && spl.size() == 3) {
				_current_input.seekg(current);
				done = true;
			}
			else if(spl.size() != 0){
				spl = StringHelper::split(buff);
				vec3 pos, v1, v3;

				StringHelper::get_vec3(spl, 0, pos);
				StringHelper::get_vec3(spl, 3, v1);
				StringHelper::get_vec3(spl, 6, v3);
				vec3 v2 = glm::cross(v3, v1);

				glm::mat3x3 orientation;
				orientation[0] = v1;
				orientation[1] = v2;
				orientation[2] = v3;

				if(index == curr_limit) {
					curr_species++;
					curr_limit += _N_per_species[curr_species];
				}

				Shape *p;

				if(_N_patches[curr_species] > 0) {
					p = new PatchyParticle(pos, 0.5f, _species_colors[curr_species]);
					PatchyParticle *pp = static_cast<PatchyParticle *>(p);
					for(auto patch : _base_patches[curr_species]) {
						glm::vec3 patch_pos = (0.5f + _delta)*(orientation * patch);
						pp->add_patch(patch_pos, _theta, glm::vec4(0.5f, 0.5f, 0.5f, 1.f));
					}
				}
				// the topology file can contain also patch-less particles, which we render as simple spheres
				else {
					p = new Sphere(pos, 0.5f, _species_colors[curr_species]);
				}
				p->set_id(index);
				my_scene->add_shape(p);

				index++;
			}
			curr_line++;
			if(_current_input.eof()) done = true;
		}

		if(_N != index) {
			Logging::log_critical("The number of nucleotides found in the trajectory (%d) is different from the one found in the topology (%d)\n", index, _N);
			exit(1);
		}

		_close_current_input_if_eof();
	}
	catch (string &error) {
		Logging::log_critical(error);
		exit(1);
	}

	return my_scene;
}

vector<vec3> PatchySwapParser::_get_base_patches(int N_patches) {
	vector<vec3> v(N_patches);

	switch(N_patches) {
	case 0:
		break;
	case 1: {
		v[0] = vec3(1.f, 0.f, 0.f);
		break;
	}
	case 2: {
		v[0] = vec3(0.f, 1.f, 0.f);
		v[1] = vec3(0.f, -1.f, 0.f);
		break;
	}
	case 3: {
		float cos30 = cos(M_PI/6.);
		float sin30 = sin(M_PI/6.);

		v[0] = vec3(0.f, 1.f, 0.f);
		v[1] = vec3(cos30, -sin30, 0.f);
		v[2] = vec3(-cos30, -sin30, 0.f);
		break;
	}
	case 4: {
		float half_isqrt3 =  0.5f/sqrt(3.f);

		v[0] = vec3(-half_isqrt3, -half_isqrt3,  half_isqrt3);
		v[1] = vec3( half_isqrt3, -half_isqrt3, -half_isqrt3);
		v[2] = vec3( half_isqrt3,  half_isqrt3,  half_isqrt3);
		v[3] = vec3(-half_isqrt3,  half_isqrt3, -half_isqrt3);
		break;
	}
	default:
		Logging::log_critical("Unsupported number of patches %d\n", N_patches);
		exit(1);
	}

	for(int i = 0; i < N_patches; i++) {
		v[i] = glm::normalize(v[i]);
	}

	return v;
}

std::vector<glm::vec3> PatchySwapParser::_parse_base_patches(std::string filename, int N_patches) {
	std::ifstream patch_file(filename);
	if(!patch_file.good()) {
		Logging::log_critical("Can't read patch file '%s'. Aborting\n", filename.c_str());
		exit(1);
	}

	std::vector<glm::vec3> base_patches(N_patches);
	string line;
	for(int i = 0; i < N_patches; i++) {
		if(!patch_file.good()) {
			Logging::log_critical("The patch file '%s' does not seem to contain enough lines (%d found, should be %d)\n", filename.c_str(), i, N_patches);
			exit(1);
		}
		std::getline(patch_file, line);
		auto spl = StringHelper::split(line);
		if(spl.size() != 3) {
			Logging::log_critical("Patch file '%s': invalid line '%s'\n", filename.c_str(), line.c_str());
			exit(1);
		}
		glm::vec3 v;
		StringHelper::get_vec3(spl, 0, v);
		v = glm::normalize(v);

		base_patches[i] = v;
	}

	patch_file.close();

	return base_patches;
}

void PatchySwapParser::_read_topology(std::string filename) {
	std::ifstream topology(filename.c_str());

	string buff;
	getline(topology, buff);
	std::vector<std::string> spl = StringHelper::split(buff);
	if(spl.size() != 2) {
		Logging::log_critical("Invalid topology found in '%s'\n", filename.c_str());
		exit(1);
	}
	int N_species;
	StringHelper::from_string<int>(spl[0], _N);
	StringHelper::from_string<int>(spl[1], N_species);

	if(N_species < 1) {
		Logging::log_critical("The number of species should be larger than 0");
		exit(1);
	}

	_N_per_species.resize(N_species);
	_N_patches.resize(N_species);
	_base_patches.resize(N_species);

	int N_tot = 0;
	for(int i = 0; i < N_species; i++) {
		std::string line;
		std::getline(topology, line);
		auto spl = StringHelper::split(line);
		if(spl.size() < 2) {
			Logging::log_critical("The topology line '%s' is malformed, since it should contain at least two integer numbers (number of particles and number of patches\n", line.c_str());
			exit(1);
		}
		int N_s = atoi(spl[0].c_str());
		_N_per_species[i] = N_s;
		N_tot += N_s;
		_N_patches[i] = atoi(spl[1].c_str());

		// the last field might be used to specify the color
		bool is_color = false;
		std::string last_token = spl[spl.size() - 1];
		auto last_token_spl = StringHelper::split(last_token, ':');
		if(last_token_spl.size() == 2 && last_token_spl[0] == "color") {
			try {
				auto color = _get_color(last_token_spl[1]);
				_species_colors[i] = color;
				is_color = true;
			}
			catch(string &s) {
				Logging::log_critical("Invalid color '%s' specified for species '%d' (error: %s)\n", last_token_spl[1].c_str(), i, s.c_str());
				exit(1);
			}
		}

		// the third argument might either be a color or a file that specifies the patches' positions
		if(spl.size() > 2 && (spl.size() == 3 && !is_color)) {
			_base_patches[i] = _parse_base_patches(spl[2], _N_patches[i]);
		}
		else {
			_base_patches[i] = _get_base_patches(_N_patches[i]);
		}
	}

	if(N_tot != _N) {
		Logging::log_critical("The sum of the particles belonging to each species (%d) is different from the number of particles found in the first line of the topology file (%d)\n", N_tot, _N);
		exit(1);
	}

	topology.close();
}
