/*
 * DNAStrand.h
 *
 *  Created on: 14/nov/2013
 *      Author: lorenzo
 */

#ifndef DNA_H_
#define DNA_H_

#include <map>
#include <vector>
#include <string>

#include "Shape.h"

class Nucleotide: public Shape {
public:
	enum {
		DEFAULT,
		MM_GROOVING,
		RNA
	};

	enum {
		DEFAULT_COLOUR,
		BACKBONE_COLOUR,
		DRUMS_COLOUR,
		FULL_DRUMS_COLOUR
	};

	enum type {
		END_3,
		END_5,
		NO_END
	};

	static constexpr float DEFAULT_BACKBONE_R = 0.25f;
	static int mode;
	static int base_color_mode;

	Nucleotide(glm::vec3 pos, glm::vec3 principal, glm::vec3 stacking, glm::vec4 color, std::string base_id="");
	virtual ~Nucleotide();

	virtual void accept_drawer(Drawer &d);

	virtual glm::vec3 principal_axis() { return _principal_axis; }
	virtual glm::vec3 stacking_axis() { return _stacking_axis; }
	virtual glm::vec3 third_axis() { return _third_axis; }
	virtual glm::vec3 backbone();
	glm::vec4 color() { return _color; }
	glm::vec4 base_color() { return _base_color; }
	void set_color(glm::vec4 c);
	glm::vec4 drums_base_color();

	void set_backbone_r(float r) { _backbone_r = r; }
	float backbone_r() { return _backbone_r; }

	void set_nid(int nid) { _nid = nid; }
	int get_nid() { return _nid; }

	void set_type(type t) { _type = t; }
	type get_type() { return _type; }

	virtual void set_orientation(glm::mat3 &m);

	virtual void set_opacity(float n_op);
	virtual void rotate(glm::mat3, glm::vec3);

protected:
	glm::vec3 _principal_axis;
	glm::vec3 _stacking_axis;
	glm::vec3 _third_axis;
	glm::vec4 _color;
	glm::vec4 _base_color;
	std::string _base_id;
	float _backbone_r = DEFAULT_BACKBONE_R;

	type _type = NO_END;
	int _nid;
	static std::vector<glm::vec4> _drums_colors;
};

class DNAStrand: public Shape {
public:
	enum direction_handling {
		NORMAL,
		SIZE,
		OPACITY
	};

	DNAStrand();
	virtual ~DNAStrand();

	virtual void accept_drawer(Drawer &d);
	bool is_circular() { return _is_circular; }
	void set_as_circular() { _is_circular = true; }

	void add_nucleotide(Nucleotide *n, int nid);
	void set_color(glm::vec4 color);
	void set_automatic_color();
	virtual void set_opacity(float n_op);
	void finalise();
	virtual glm::vec3 position() const;
	std::map<int, Nucleotide *> nucleotides() { return _nucleotides; }

	virtual glm::mat3 inertia_tensor(glm::vec3);
	virtual void shift(glm::vec3 &amount);
	virtual void rotate(glm::mat3, glm::vec3);

	void set_direction_handling(direction_handling dh) { _dir_handling = dh; }
	void next_direction_handling();

protected:
	glm::vec3 _com;
	std::map<int, Nucleotide *> _nucleotides;
	bool _is_circular;
	float _current_opacity = 1.0f;
	direction_handling _dir_handling = direction_handling::NORMAL;
	Nucleotide *_end_3 = nullptr;
	Nucleotide *_end_5 = nullptr;

	static std::vector<glm::vec4> _automatic_colors;

	void _apply_direction_handling();
};

#endif /* DNA_H_ */
