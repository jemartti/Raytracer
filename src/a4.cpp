#include "a4.hpp"
#include "image.hpp"
#include "ray.hpp"

void a4_render( // What to render
                SceneNode*               root,
                // Where to output the image
                const std::string&       filename,
                // Image size
                int                      width,
                int                      height,
                // Viewing parameters
                const Point3D&           eye,
                const Vector3D&          view,
                const Vector3D&          up,
                double                   fov,
                // Lighting parameters
                const Colour&            ambient,
                const std::list<Light*>& lights )
{
	// Normalize our viewing vectors
	Vector3D m_view      = view;
	Vector3D m_up        = up;
	Vector3D m_side      = view.cross(up);
	m_side.normalize();
	m_view.normalize();
	m_up.normalize();

	// Our actual image
	Image    img( width, height, 3 );

	// The firing ray direction
	Vector3D dir;

	// Run ray tracer on each y row
	for ( int y = 0; y < width; y++ )
	{
		// Run ray tracer on each x pixel in the current y row
		for ( int x = 0; x < height; x++ )
		{
			// Get our firing ray direction
			dir = ( x / ((double)width) * 2 - 1 ) *
					tan( fov * M_PI / 360.0 ) *
					( (double)width / (double)height ) *
					m_side + ( y / (double)height * 2 - 1 ) *
					tan( fov * M_PI / 360.0 ) *
					-m_up + m_view;
			dir.normalize();

			// Fire the primary: Do the actual intersection check
			Ray_t r;
			r.set_position ( eye );
			r.set_direction( dir );
			Intersection_t intersection = root->intersect_check( r );

			// Do background colouring (blue->red fade from BL->TR)
			Colour p_colour( (double)x / width, 0, (double)y / height );

			// If we intersect, overwrite the background with the object colour
			if ( intersection.get_hit() )
			{
				// Set up vectors used in lighting
				Vector3D viewer_v = -r.get_direction();
				Vector3D normal_v =  intersection.get_normal();
				Vector3D light_v;
				Vector3D reflected_v;

				// Normalize intersection vectors
				viewer_v.normalize();
				normal_v.normalize();

				// Set default colour values
				Colour diffuse (0, 0, 0);
				Colour specular(0, 0, 0);

				// Calculate the shading for each light source
				std::list<Light*>::const_iterator I;
				for ( I = lights.begin(); I != lights.end(); ++I )
				{
					// Calculate the light and reflected vectors for this light
					// source
					light_v = ( *I )->position - intersection.get_pt();
					reflected_v = 2 * ( light_v.dot(normal_v) ) *
							normal_v - light_v;

					// Normalize light and reflected vectors
					light_v.normalize();
					reflected_v.normalize();

					// Cast secondary ray to check for shadows
					Ray_t secondary;
					secondary.set_direction( light_v );
					secondary.set_position ( intersection.get_pt() );
					Intersection_t intersection_l =
							root->intersect_check( secondary );

					// If we're not in a shadow zone, calculate our diffuse and
					// specular shading
					if( !intersection_l.get_hit() )
					{
						// Calculate the shininess of the material
						double shininess = pow( (reflected_v.dot( viewer_v )),
								(intersection.get_mat())->get_shininess() );

						// Calculate the specular lighting for this light source
						if ( reflected_v.dot(viewer_v) >= 0 )
						{
							specular = specular +
									( (intersection.get_mat())->get_specular()
									* shininess
									* (*I)->colour );
						}

						// Calculate the diffuse lighting for this light source
						if ( (light_v.dot( normal_v )) >= 0 )
						{
							diffuse = diffuse +
									( (intersection.get_mat())->get_diffuse()
									* (light_v.dot( normal_v ))
									* (( *I )->colour) );
						}
					}
				}
				// Sum the shading for each light source at this intersection
				// point
				p_colour = specular + diffuse +
						ambient * ( intersection.get_mat() )->get_diffuse();
			}

			// Set the final pixel colour on the image
			img(x, y, 0) = p_colour.R();
			img(x, y, 1) = p_colour.G();
			img(x, y, 2) = p_colour.B();
		}
	}

	// Save the final file
	img.savePng( filename );

	// Give a completion notification
	std::cerr << "Done rendering." << std::endl;
}
