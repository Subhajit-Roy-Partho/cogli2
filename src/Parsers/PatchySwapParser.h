/*
 * PatchySwapParser.h
 *
 *  Created on: 26/Sep/2016
 *      Author: Lorenzo
 */

#ifndef PATCHYSWAPPARSER_H_
#define PATCHYSWAPPARSER_H_

#include <map>
#include <vector>

#include "Parser.h"
#include "IoxDNAParser.h"

class PatchySwapParser: public Parser, public IoxDNAParser {
protected:
	int _N = 0;
	std::vector<int> _N_per_species;
	std::vector<int> _N_patches;
	std::vector<std::vector<glm::vec3>> _base_patches;
	std::vector<glm::vec4> _species_colors;

	float _delta;
	float _cosmax, _theta;

	std::vector<glm::vec3> _parse_base_patches(std::string filename, int N_patches);
	std::vector<glm::vec3> _get_base_patches(int);
	void _read_topology(std::string);
public:
	PatchySwapParser(std::string topology, const std::queue<std::string> &input_files, std::string);
	virtual ~PatchySwapParser();

	virtual Scene *next_scene();
};

#endif /* PATCHYSWAPPARSER_H_ */

