/*
 * DNAParser.h
 *
 *  Created on: 11/Nov/2013
 *      Author: Flavio
 */

#ifndef DNAPARSER_H_
#define DNAPARSER_H_

#include <map>
#include <vector>

#include "Parser.h"
#include "IoxDNAParser.h"
#include "../Drawables/DNA.h"

class DNAParser: public Parser, public IoxDNAParser {
protected:
	int _N_nuc, _N_str;
	std::vector<std::string> _base_id;
	void _read_topology(std::string);
	std::map<int, int> _n2s_map;
	std::map<int, Nucleotide::type> _types;
	std::vector<bool> _strand_circularity;
	DNAStrand::direction_handling _dir_handling;
public:
	DNAParser(std::string topology, const std::queue<std::string> &input_files, std::string end_handling);
	virtual ~DNAParser();

	virtual Scene *next_scene();
};

#endif /* DNAPARSER_H_ */

