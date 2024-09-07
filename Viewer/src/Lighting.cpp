#include "Lighting.h"

Lighting::Lighting()
{
	setLightingsColor();
	setLightingPosition();
	//setEyePosition();
}

Lighting::~Lighting()
{
}
/*
void Lighting::setEyePosition(int X, int Y, int Z)
{
	eyePosition = { X , Y, Z };
}
*/

void Lighting::setLightingPosition(int X, int Y, int Z)
{
	light.point = { X, Y, Z };
	lightPosition = { X, Y, Z };
}


void Lighting::setLightingsColor(glm::vec3 ambV, glm::vec3 difV, glm::vec3 specV)
{
	ambientLightingVector = ambV;
	light.ambient = ambV;
	diffuseLightingVector = difV;
	light.diff = difV;
	specularLightingVector = specV;
	light.spect = specV;
}

bool Lighting::getSpecularLighting() const
{
	return SpecularLighting;
}

void Lighting::setSpecularLighting(bool set)
{
	SpecularLighting = set;
}

bool Lighting::getGouraudShading() const
{
	return GouraudShading;
}

void Lighting::setGouraudShading(bool set)
{
	GouraudShading = set;
}
void Lighting::setAmbientLighting(bool set)
{
	AmbientLighting = set;
}
bool Lighting::getAmbientLighting() const
{
	return AmbientLighting;
}
void Lighting::setDiffuseLighting(bool set)
{
	DiffuseLighting = set;
}
bool Lighting::getDiffuseLighting() const
{
	return DiffuseLighting;
}

glm::vec3 Lighting::getAmbientLightingVector() const
{
	return ambientLightingVector;
}

glm::vec3 Lighting::getDiffuseLightingVector() const
{
	return diffuseLightingVector;
}

glm::vec3 Lighting::getSpecularLightingVector() const
{
	return specularLightingVector;
}
/*
glm::vec3 Lighting::getEyePosition() const
{
	return eyePosition;
}
*/
glm::vec3 Lighting::getLightingPosition() const
{
	return lightPosition;
}

void Lighting::setPhongReflection(const bool& toSet) 
{
	PhongReflection = toSet;
}

bool Lighting::getPhongReflection() const
{
	return PhongReflection;
}

void Lighting::setDirectionsOfSpecular(const bool& toSet)
{
	directionsOfSpecular = toSet;
}

bool Lighting::getDirectionsOfSpecular() const
{
	return directionsOfSpecular;
}

void Lighting::setPhongShading(const bool& toSet)
{
	PhongShading = toSet;
}

bool Lighting::getPhongShading() const
{
	return PhongShading;
}

void Lighting::setShiniesCoefficent(const float& toSet)
{
	shiniesCoefficent = toSet;
	
	light.shine = toSet;
}

float Lighting::getShiniesCoefficent() const
{
	return shiniesCoefficent;
}
void Lighting::setDirectionalSource(const bool& toSet)
{
	directionalSource = toSet;
}
bool Lighting::getDirectionalSource() const
{
	return directionalSource;
}
Light Lighting::getLight() const
{
	return light;
}
