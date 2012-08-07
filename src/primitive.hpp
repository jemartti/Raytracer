#ifndef CS488_PRIMITIVE_HPP
#define CS488_PRIMITIVE_HPP

#include "algebra.hpp"
#include "ray.hpp"


class Primitive {
public:
	virtual ~Primitive();

	// Check if the ray intersects with the primitive
	virtual Intersection_t intersect_check( Ray_t r ) = 0;
};

class NonhierSphere : public Primitive {
public:
	NonhierSphere( const Point3D& pos, double radius )
		: m_pos( pos ), m_radius( radius )
	{
	}
	virtual ~NonhierSphere();

	// Check if the ray intersects with the sphere
	virtual Intersection_t intersect_check( Ray_t r );

private:
	Point3D m_pos;
	double  m_radius;
};

class NonhierBox : public Primitive {
public:
	NonhierBox( const Point3D& pos, double size )
		: m_pos( pos ), m_size( size )
	{
	}
	virtual ~NonhierBox();

	// Check if the ray intersects with the box
	virtual Intersection_t intersect_check( Ray_t r );

private:
	Point3D m_pos;
	double  m_size;
};

class Sphere : public Primitive {
public:
	Sphere();
	virtual ~Sphere();

	// Check if the ray intersects with the sphere
	virtual Intersection_t intersect_check( Ray_t r );

private:
	NonhierSphere* m_sphere;
};

class Cube : public Primitive {
public:
	Cube();
	virtual ~Cube();

	// Check if the ray intersects with the cube
	virtual Intersection_t intersect_check( Ray_t r );

private:
	NonhierBox* m_box;
};

#endif
