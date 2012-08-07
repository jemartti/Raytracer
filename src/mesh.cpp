#include "mesh.hpp"
#include <iostream>


std::ostream& operator<<( std::ostream& out, const Mesh& mesh )
{
	std::cerr << "mesh({";
	std::vector<Point3D>::const_iterator II;
	for ( II = mesh.m_verts.begin(); II != mesh.m_verts.end(); ++II )
	{
		if ( II != mesh.m_verts.begin() )
		{
			std::cerr << ",\n      ";
		}
		std::cerr << *II;
	}
	std::cerr << "},\n\n     {";

	std::vector<Mesh::Face>::const_iterator I;
	for ( I = mesh.m_faces.begin(); I != mesh.m_faces.end(); ++I )
	{
		if ( I != mesh.m_faces.begin() )
		{
			std::cerr << ",\n      ";
		}
		std::cerr << "[";
		Mesh::Face::const_iterator J;
		for ( J = I->begin(); J != I->end(); ++J )
		{
			if ( J != I->begin() )
			{
				std::cerr << ", ";
			}
			std::cerr << *J;
		}
		std::cerr << "]";
	}
	std::cerr << "});" << std::endl;
	return out;
}

Intersection_t Mesh::intersect_check( Ray_t r )
{
	Intersection_t intersect;
	intersect.set_hit( false );

	// The intersection parameters
	Point3D  pt;
	Vector3D normal;
	bool     intersects_cur = false;

	// The working intersection point and boundaries
	double sect_max = 10000000.0;
	double sect_min = 10000000.0;
	double sect_cur;

	// Do the standard intersection check
	// Work through each face of the polygonal mesh
	std::vector<Face>::const_iterator X;
	for ( X = m_faces.begin(); X != m_faces.end(); ++X )
	{
		// We obviously don't intersect with this face yet
		intersects_cur = false;

		// Store all vertices of the face
		Point3D vertices[ (*X).size() ];
		int     count = 0;
		Mesh::Face::const_iterator Y;
		for( Y = (*X).begin(); Y != (*X).end(); ++Y )
		{
			vertices[ count ] = m_verts[ *Y ];
			count            += 1;
		}

		// Calculate the normal of the face using first three points
		normal = ( vertices[2] - vertices[1] ).
				cross( vertices[0] - vertices[1] );
		normal.normalize();

		// Check if our current intersection is within our min bound and is
		// closer than our current closest intersection
		sect_cur = normal.dot( vertices[0] - r.get_position() ) /
				   normal.dot( r.get_direction() );
		if ( (sect_cur < 1e-10) || (sect_cur > sect_min) )
		{
			continue;
		}

		// Get the intersection point
		pt = r.get_position() + sect_cur * r.get_direction();

		// Check if we are within each boundary of the face
		// Intersection-of-halfspaces method
		for ( int i = 0; i < (int)(*X).size(); i++ )
		{
			double ioh = ( vertices[( i + 1 ) % ( *X ).size()] - vertices[i] ).
					cross( pt - vertices[i] ).dot( normal );
			if( ioh < 0 )
			{
				intersects_cur = false;
				break;
			}
			intersects_cur = true;
		}

		// If intersection-of-halfspaces algorithm tells us that we're within
		// all face boundaries, then we have a new closest ray intersection
		if ( intersects_cur )
		{
			intersect.set_pt( pt );
			intersect.set_normal( normal );
			intersect.set_distance( sect_cur );

			// Our new closest intersection is this one!
			sect_min = sect_cur;
		}
	}

	// Check if we have a real hit
	if ( sect_min != sect_max )
	{
		intersect.set_hit( true );
	}

	return intersect;
}
