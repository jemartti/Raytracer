#ifndef CS488_RAY_HPP
#define CS488_RAY_HPP

#include "algebra.hpp"


class Intersection_t {
public:
	Intersection_t();
	~Intersection_t();

	Point3D   get_pt();
	Vector3D  get_normal();
	Material* get_mat();
	double    get_distance();
	bool      get_hit();

	void      set_pt      ( Point3D   pt );
	void      set_normal  ( Vector3D  normal );
	void      set_mat     ( Material* mat );
	void      set_distance( double    distance );
	void      set_hit     ( bool      hit );

	void      normal_normalize();

private:
	// The intersection point
	Point3D   m_pt;

	// The normal at the intersection point
	Vector3D  m_normal;

	// The material at the intersection point
	Material *m_mat;

	// The distance from the intersection point to the ray origin
	double    m_distance;

	// Whether or not the ray actually intersects with the intersection point
	bool      m_hit;
};

class Ray_t {
public:
	Ray_t();
	~Ray_t();

	Vector3D get_direction();
	Point3D  get_position();

	void     set_direction( Vector3D direction );
	void     set_position ( Point3D  position  );

private:
	// The direction of the ray
	Vector3D m_direction;

	// The origin position of the ray
	Point3D  m_position;
};

#endif // CS488_RAY_HPP
