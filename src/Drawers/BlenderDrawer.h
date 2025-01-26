/*
 * BlenderDrawer.h
 *
 *  Created on: 26/apr/2024
 *      Author: lorenzo
 */

#ifndef BLENDERDRAWER_H_
#define BLENDERDRAWER_H_

#include <cstdlib>

#include "Drawer.h"

class BlenderDrawer: public Drawer {
protected:
	FILE *_out;
	bool _patchy_as_kf;

	std::string _patches_as_textures(PatchyParticle &, const std::string &);
	std::string _patches_as_kf(PatchyParticle &, const std::string &);

public:
	BlenderDrawer();
	virtual ~BlenderDrawer();

	void init();
	void set_out_file(FILE *out) { _out = out; }

	void set_patchy_as_kf(bool v) { _patchy_as_kf = v; }

	virtual void visit(Drawable &);
	virtual void visit(Scene &);
	virtual void visit(Cylinder &);
	virtual void visit(Icosahedron &);
	virtual void visit(Ellipsoid &);
	virtual void visit(Sphere &);
	virtual void visit(DNAStrand &);
	virtual void visit(Nucleotide &);
	virtual void visit(DHS &);
	virtual void visit(PatchyParticle &);
	virtual void visit(Arrow &);
	virtual void visit(Triangle &);
	virtual void visit(Group &);
};

#endif /* BLENDERDRAWER_H_ */
