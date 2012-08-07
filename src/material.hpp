#ifndef CS488_MATERIAL_HPP
#define CS488_MATERIAL_HPP

#include "algebra.hpp"


class Material {
public:
	virtual ~Material();

	virtual void apply_gl() const  = 0;

	virtual Colour get_diffuse()   = 0;
	virtual Colour get_specular()  = 0;
	virtual double get_shininess() = 0;

protected:
	Material()
	{
	}
};

class PhongMaterial : public Material {
public:
	PhongMaterial( const Colour& kd, const Colour& ks, double shininess );
	virtual ~PhongMaterial();

	virtual void apply_gl() const;

	virtual Colour get_diffuse();
	virtual Colour get_specular();
	virtual double get_shininess();

private:
	Colour m_kd;
	Colour m_ks;

	double m_shininess;
};

#endif
