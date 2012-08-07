#include "ray.hpp"


Intersection_t::Intersection_t()
{
}

Intersection_t::~Intersection_t()
{
}

Point3D   Intersection_t::get_pt()
{
	return m_pt;
}

Vector3D  Intersection_t::get_normal()
{
	return m_normal;
}

Material* Intersection_t::get_mat()
{
	return m_mat;
}

double    Intersection_t::get_distance()
{
	return m_distance;
}

bool      Intersection_t::get_hit()
{
	return m_hit;
}

void      Intersection_t::set_pt      ( Point3D   pt )
{
	m_pt = pt;
}

void      Intersection_t::set_normal  ( Vector3D  normal )
{
	m_normal = normal;
}

void      Intersection_t::set_mat     ( Material* mat )
{
	m_mat = mat;
}

void      Intersection_t::set_distance( double    distance )
{
	m_distance = distance;
}

void      Intersection_t::set_hit     ( bool      hit )
{
	m_hit = hit;
}

void      Intersection_t::normal_normalize()
{
	m_normal.normalize();
}


Ray_t::Ray_t()
{
}

Ray_t::~Ray_t()
{
}

Vector3D Ray_t::get_direction()
{
	return m_direction;
}

Point3D  Ray_t::get_position()
{
	return m_position;
}

void     Ray_t::set_direction( Vector3D direction )
{
	m_direction = direction;
}
void     Ray_t::set_position ( Point3D  position  )
{
	m_position = position;
}
