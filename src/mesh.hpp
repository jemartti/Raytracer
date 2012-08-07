#ifndef CS488_MESH_HPP
#define CS488_MESH_HPP

#include <vector>
#include <iosfwd>
#include "primitive.hpp"
#include "algebra.hpp"


// A polygonal mesh.
class Mesh : public Primitive {
public:
	Mesh( const std::vector<Point3D>& verts,
	      const std::vector< std::vector<int> >& faces )
		: m_verts( verts ), m_faces( faces )
	{
	}

	typedef std::vector<int> Face;

	// Checks if Ray_t r intersects with the polygonal mesh
	virtual Intersection_t intersect_check( Ray_t r );

private:
	std::vector<Point3D> m_verts;
	std::vector<Face>    m_faces;

	friend std::ostream& operator<<(std::ostream& out, const Mesh& mesh);
};

#endif
