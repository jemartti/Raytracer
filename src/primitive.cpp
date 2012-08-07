#include <algorithm>
#include "primitive.hpp"
#include "polyroots.hpp"


Primitive::~Primitive()
{
}


NonhierSphere::~NonhierSphere()
{
}

Intersection_t NonhierSphere::intersect_check( Ray_t r )
{
	// Default intersection point
	Intersection_t intersect;
	intersect.set_distance( -1 );
	intersect.set_hit( false );

	// Standard quadratic form for calculating ray/sphere intersection
	double A = pow( r.get_direction()[0], 2 ) +
			   pow( r.get_direction()[1], 2 ) +
			   pow( r.get_direction()[2], 2 );
	double B = 2 * ( r.get_direction()[0] * (r.get_position()[0] - m_pos[0]) +
					 r.get_direction()[1] * (r.get_position()[1] - m_pos[1]) +
					 r.get_direction()[2] * (r.get_position()[2] - m_pos[2]) );
	double C = pow( r.get_position()[0] - m_pos[0], 2 ) +
			   pow( r.get_position()[1] - m_pos[1], 2 ) +
			   pow( r.get_position()[2] - m_pos[2], 2 ) -
			   pow( m_radius, 2 );

	// Calculate roots for sphere intersection
	double roots[2];
	int    numRoots = quadraticRoots( A, B, C, roots );
	double sect     = 0;

	// Calculate intersection points, if any
	if ( numRoots != 0 )
	{
		// Ray goes through the sphere (two intersections)
		if ( numRoots == 2 )
		{
			// Check which of the roots is the closest, the one to be drawn
			double sect_t;
			if ( roots[0] < roots[1] )
			{
				sect_t = roots[0];
			}
			else
			{
				sect_t = roots[1];
			}
			if ( sect_t > 1e-10 )
			{
				sect = roots[1];
				intersect.set_hit( true );
			}
	    }
		// Ray is tangent to the sphere
		else if ( roots[0] > 1e-10 )
		{
	    	sect = roots[0];
	    	intersect.set_hit( true );
	    }
		// No intersection after all
		else
		{
	    	intersect.set_hit( false );
	    }
	}
	else
	{
		// No intersection
		return intersect;
	}

	// Set the intersection point accordingly
	Point3D p = r.get_position() + sect * r.get_direction();
	intersect.set_pt( p );
	intersect.set_normal( p - m_pos );
	intersect.set_distance( sect );
	intersect.normal_normalize();

	return intersect;
}


NonhierBox::~NonhierBox()
{
}

Intersection_t NonhierBox::intersect_check( Ray_t r )
{
	Intersection_t intersect;
	intersect.set_hit( false );
	intersect.set_distance( -1 );

	// Near and far intersection bounds
	double sect_far  =  10000000.0;
	double sect_near = -10000000.0;

	// Do our standard intersection check for each of the three planes of the
	// box, for both front and back for each
	for ( int i = 0; i < 3; i += 1 )
	{
		// Calculate the intersection point in the current plane
		double sect_t1 = ( m_pos[i] - r.get_position()[i] ) /
				(double)r.get_direction()[i];
		double sect_t2 = ( m_pos[i] + m_size - r.get_position()[i] ) /
				(double)r.get_direction()[i];

		// Ensures sect_t1/sect_t2 are the intersections with the
		// front/back respectively
		if ( sect_t1 > sect_t2 )
		{
			double sect_t;
			sect_t  = sect_t1;
			sect_t1 = sect_t2;
			sect_t2 = sect_t;
		}

		// Check if our intersections are within our intersection bounds
		if ( sect_t2 < sect_far )
		{
			// We have a new closest back intersection
			sect_far = sect_t2;
		}
		if ( sect_t1 > sect_near )
		{
			// Set the normal at the intersection point
			if      ( i == 0 )
			{
				intersect.set_normal(
						Vector3D(-r.get_direction()[0], 0, 0) );
			}
			else if ( i == 1 )
			{
				intersect.set_normal(
						Vector3D(0, -r.get_direction()[1], 0) );
			}
			else if ( i == 2 )
			{
				intersect.set_normal(
						Vector3D(0, 0, -r.get_direction()[2]) );
			}
			intersect.normal_normalize();

			// We have a new closest intersection
			sect_near    = sect_t1;
		}

		// Now if our near is greater than our far, we actually have no
		// intersection, as the bounds have crossed
		if ( (sect_near > sect_far) || (sect_far < 0) )
		{
			intersect.set_hit( false );
			return intersect;
		}
	}

	// Calculate the intersection point on the object
	double xi = r.get_direction()[0] *
			( sect_near - 1e-10 ) + r.get_position()[0];
	double yi = r.get_direction()[1] *
			( sect_near - 1e-10 ) + r.get_position()[1];
	double zi = r.get_direction()[2] *
			( sect_near - 1e-10 ) + r.get_position()[2];

	// Set intersection parameters
	intersect.set_pt( Point3D(xi, yi, zi) );
	intersect.set_distance( sect_near - 1e-10 );
	intersect.set_hit( true );

	return intersect;
}


Sphere::Sphere()
{
	m_sphere = new NonhierSphere( Point3D(0, 0, 0), 1 );
}

Sphere::~Sphere()
{
}

Intersection_t Sphere::intersect_check( Ray_t r )
{
	return m_sphere->intersect_check( r );
}


Cube::Cube()
{
	m_box = new NonhierBox( Point3D(0, 0, 0), 1 );
}

Cube::~Cube()
{
}

Intersection_t Cube::intersect_check( Ray_t r )
{
	return m_box->intersect_check( r );
}
