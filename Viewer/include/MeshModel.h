#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "MeshModel.h"
#include "Face.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoords;
};


struct Material
{
	glm::vec3 ambient;
	glm::vec3 diff;
	glm::vec3 spect;
};

class MeshModel
{
public:
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> textureCoords,const std::string& model_name);
	virtual ~MeshModel();

	const std::vector<Vertex>& GetModelVertices();

	GLuint GetVAO() const;

	
	std::vector<glm::vec2> getBounding();

	std::vector<glm::vec2> getFaceBounding(int faceIndex);
	glm::vec3 getModelMaterial(int index) const;
	void setModelMaterial(const glm::vec3& toSet, const glm::vec3& diff, const glm::vec3& spect);
	const Face& GetFace(int index) const;
	int GetFacesCount() const;
	const std::string& GetModelName() const;
	const int GetVertexCount();
	glm::mat4 GetTrans();
	glm::mat4 GetWorldTransformation();
	glm::mat4 TranslateLocal(float xT, float yT, float zT);
	glm::mat4 RotateLocal(float rotateX, float rotateY, float rotateZ);
	glm::mat4 localScale(float x, float y, float z, float U);
	glm::vec3 getVertexIndex(int index);
	glm::vec3 GetNormalIndex(int index);
	glm::mat4 getLocalTransformation();
	glm::vec4 getMinMax();
	void TranslateWorld(float xT, float yT, float zT);
	void RotateWorld(float rotateX, float rotateY, float rotateZ);
	void worldScale(float x, float y, float z, float U);
	void InitNormOfVer();
	const std::vector<glm::vec3>& GetNormOfVer();
	Material getModelMaterial() const;
private:
	std::vector<glm::vec3> normOfVer;
	std::vector<Face>			faces;
	std::vector<glm::vec3>		vertices;
	std::vector<glm::vec3>		normals;
	std::string					model_name;
	glm::mat4					localTranslateMat;
	glm::mat4					localRotateMat;
	glm::mat4					localScaleMat;
	glm::mat4					worldScaleMat;
	glm::mat4					worldTranslateMat;
	glm::mat4					worldRotationMat;
	std::vector<glm::vec2>		minMaxVec;
	void InitBounding();
	
	glm::vec3						modelMaterial; // ambient
	glm::vec3						modeldiffuseMaterial;
	glm::vec3						modelspectMaterial;
	glm::vec4 minMax2d;

	std::vector<Vertex> modelVertices;

	
	Material modelMaterialStruct;

	std::vector<glm::vec3> textureCoords;

	GLuint vbo;
	GLuint vao;

};
