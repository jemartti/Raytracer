#include "scene.hpp"
#include <iostream>

#include "light.hpp"

SceneNode::SceneNode( const std::string& name )
	: m_name( name )
{
}

SceneNode::~SceneNode()
{
}

// Pulled straight from A3
void SceneNode::rotate( char axis, double angle )
{
	Vector4D row1, row2, row3, row4;

	if ( (axis == 'x') || (axis == 'y') || (axis == 'z') )
	{
		angle = ( angle / 180.0 ) * M_PI;

		double   co = cos( angle );
		double   si = sin( angle );

		if ( axis == 'x' )
		{
			row1 = Vector4D( 1, 0,   0,  0 );
			row2 = Vector4D( 0, co, -si, 0 );
			row3 = Vector4D( 0, si,  co, 0 );
			row4 = Vector4D( 0, 0,   0,  1 );
		}
		else if ( axis == 'y' )
		{
			row1 = Vector4D(  co, 0, si, 0 );
			row2 = Vector4D(  0,  1, 0,  0 );
			row3 = Vector4D( -si, 0, co, 0 );
			row4 = Vector4D(  0,  0, 0,  1 );
		}
		else if ( axis == 'z' )
		{
			row1 = Vector4D( co, -si, 0, 0 );
			row2 = Vector4D( si,  co, 0, 0 );
			row3 = Vector4D( 0,   0,  1, 0 );
			row4 = Vector4D( 0,   0,  0, 1 );
		}
	}

	this->set_transform( m_trans * Matrix4x4(row1, row2, row3, row4) );
}

// Pulled straight from A3
void SceneNode::scale( const Vector3D& amount )
{
	Vector4D  row1, row2, row3, row4;
	double    x = amount[0];
	double    y = amount[1];
	double    z = amount[2];

	row1 = Vector4D( x, 0, 0, 0 );
	row2 = Vector4D( 0, y, 0, 0 );
	row3 = Vector4D( 0, 0, z, 0 );
	row4 = Vector4D( 0, 0, 0, 1 );

	this->set_transform( m_trans * Matrix4x4(row1, row2, row3, row4) );
}

// Pulled straight from A3
void SceneNode::translate( const Vector3D& amount )
{
	Vector4D  row1, row2, row3, row4;
	double    x = amount[0];
	double    y = amount[1];
	double    z = amount[2];

	row1 = Vector4D( 1, 0, 0, x );
	row2 = Vector4D( 0, 1, 0, y );
	row3 = Vector4D( 0, 0, 1, z );
	row4 = Vector4D( 0, 0, 0, 1 );

	this->set_transform( m_trans * Matrix4x4(row1, row2, row3, row4) );
}

bool SceneNode::is_joint() const
{
	return false;
}

// Similar to A3, but we must reconfigure our ray prior to doing the
// intersection check, as well as resetting the intersection point to align with
// the original firing ray afterwards
Intersection_t SceneNode::intersect_check( Ray_t r ) {
	Intersection_t intersect;
	intersect.set_hit( false );

	// Our closest intersection point
	double         closest_dist = 10000000.0;

	// Reconfigure our firing ray
	r.set_direction( m_invtrans * r.get_direction() );
	r.set_position ( m_invtrans * r.get_position()  );

	// Iterate through all children of the object to see if one has a closer
	// intersection
	ChildList::const_iterator i;
	for ( i = m_children.begin(); i != m_children.end(); ++i )
	{
		// Check if we intersect
		Intersection_t child_intersect = (*i)->intersect_check( r );

		// If intersect and we are the closest, update the return
		if ( child_intersect.get_hit() &&
			 child_intersect.get_distance() <= closest_dist )
		{
			intersect    = child_intersect;
			closest_dist = child_intersect.get_distance();
		}
	}

	// Align our intersection point with what it really should be
	intersect.set_pt( m_trans * intersect.get_pt() );
	intersect.set_normal( (m_invtrans.transpose()) * intersect.get_normal() );

	return intersect;
}


JointNode::JointNode( const std::string& name )
  : SceneNode(name)
{
}

JointNode::~JointNode()
{
}

bool JointNode::is_joint() const
{
	return true;
}

void JointNode::set_joint_x( double min, double init, double max )
{
	m_joint_x.min  = min;
	m_joint_x.init = init;
	m_joint_x.max  = max;
}

void JointNode::set_joint_y( double min, double init, double max )
{
	m_joint_y.min  = min;
	m_joint_y.init = init;
	m_joint_y.max  = max;
}


GeometryNode::GeometryNode( const std::string& name, Primitive* primitive )
  : SceneNode( name ), m_primitive( primitive )
{
}

GeometryNode::~GeometryNode()
{
}

// Similar to A3, but we must reconfigure our ray prior to doing the
// intersection check, as well as resetting the intersection point to align with
// the original firing ray afterwards
Intersection_t GeometryNode::intersect_check( Ray_t r ) {
	Intersection_t intersect;
	intersect.set_hit( false );

	// Our closest intersection point
	double closest_dist = 10000000.0;

	// Reconfigure our firing ray
	r.set_direction( m_invtrans * r.get_direction() );
	r.set_position ( m_invtrans * r.get_position()  );

	// Do the actual intersection check on the geometry object
	intersect = m_primitive->intersect_check( r );

	// Iterate through all children of the object to see if one has a closer
	// intersection
	ChildList::const_iterator i;
	for ( i = m_children.begin(); i != m_children.end(); ++i )
	{
		// Check if we intersect
		Intersection_t child_intersect = (*i)->intersect_check( r );

		// If intersect and we are the closest, update the return
		if ( child_intersect.get_hit() &&
			 child_intersect.get_distance() <= closest_dist )
		{
			intersect    = child_intersect;
			closest_dist = child_intersect.get_distance();
		}
	}

	// Set the material of the intersected object
	if ( intersect.get_hit() )
	{
		intersect.set_mat( m_material );
	}

	// Align our intersection point with what it really should be
	intersect.set_pt( m_trans * intersect.get_pt() );
	intersect.set_normal( (m_invtrans.transpose()) * intersect.get_normal() );

	return intersect;
}
