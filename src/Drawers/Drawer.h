/*
 * Drawer.h
 *
 *  Created on: 20/ott/2013
 *      Author: lorenzo
 */

#ifndef DRAWER_H_
#define DRAWER_H_

class Drawable;
class Scene;
class Cylinder;
class Icosahedron;
class Ellipsoid;
class Sphere;
class DNAStrand;
class Nucleotide;
class DHS;
class Arrow;
class PatchyParticle;
class Triangle;
class Group;

class Drawer {
public:
	Drawer();
	virtual ~Drawer();

	virtual void visit(Drawable &) = 0;
	virtual void visit(Scene &) = 0;
	virtual void visit(Cylinder &) = 0;
	virtual void visit(Icosahedron &) = 0;
	virtual void visit(Ellipsoid &) = 0;
	virtual void visit(Sphere &) = 0;
	virtual void visit(DNAStrand &) = 0;
	virtual void visit(Nucleotide &) = 0;
	virtual void visit(DHS &) = 0;
	virtual void visit(Arrow &) = 0;
	virtual void visit(PatchyParticle &) = 0;
	virtual void visit(Triangle &) = 0;
	virtual void visit(Group &) = 0;
};

#endif /* DRAWER_H_ */
