#pragma once
#define _USE_MATH_DEFINES
#include "Utils.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


struct Light
{
	glm::vec3 point;
	glm::vec3 ambient;
	glm::vec3 diff;
	glm::vec3 spect;
	float shine;
};

class Lighting
{
public:
	
	Lighting();
	virtual ~Lighting();
	void setLightingPosition(int X = 0.f, int Y = 0.f, int Z = 10.f);
	//void setEyePosition(int X = 0.f, int Y = 0.f, int Z = 10.f);
	void setLightingsColor(glm::vec3 ambV = { 0,0,0 }, glm::vec3 difV = { 1,1,1 }, glm::vec3 specV = { 1,1,1 });
	glm::vec3 getAmbientLightingVector() const;
	glm::vec3 getDiffuseLightingVector() const;
	glm::vec3 getSpecularLightingVector() const;
	glm::vec3 getLightingPosition() const;
	//glm::vec3 getEyePosition() const;
	void setGouraudShading(bool set);
	bool getGouraudShading() const;
	void setAmbientLighting(bool set);
	bool getAmbientLighting() const;
	void setDiffuseLighting(bool set);
	bool getDiffuseLighting() const;
	void setSpecularLighting(bool set);
	bool getSpecularLighting() const;
	void setPhongReflection(const bool& toSet);
	bool getPhongReflection() const;
	void setDirectionsOfSpecular(const bool& toSet);
	bool getDirectionsOfSpecular() const;
	void setPhongShading(const bool& toSet);
	bool getPhongShading() const;
	void setDirectionalSource(const bool& toSet);
	bool getDirectionalSource() const;
	float getShiniesCoefficent() const;
	void setShiniesCoefficent(const float& toSet);
	Light getLight() const;
private:
	float shiniesCoefficent;
	bool directionalSource;
	bool directionsOfSpecular;
	bool PhongReflection;
	bool PhongShading;
	bool GouraudShading;
	bool SpecularLighting;
	bool AmbientLighting;
	bool DiffuseLighting;
	glm::vec3 lightPosition; //incase of point lighting
	// float angle; // incase of directional lighting
	//glm::vec3 eyePosition;
	glm::vec3 ambientLightingVector;
	glm::vec3 diffuseLightingVector;
	glm::vec3 specularLightingVector;

	Light light;
	

};
