#define _USE_MATH_DEFINES
#include "MeshModel.h"
#include "Utils.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals,std::vector<glm::vec2> textureCoords, const std::string& model_name) :
	faces(faces),
	vertices(vertices),
	normals(normals),
	localTranslateMat(1.f),
	localRotateMat(1.f),
	localScaleMat(1.f),
	worldScaleMat(1.f),
	worldTranslateMat(1.f),
	worldRotationMat(1.f),
	modelMaterial(0.5f)
{
	int minX = vertices[0].x, maxX = vertices[0].x, minY = vertices[0].y, maxY = vertices[0].y;
	for (int i = 0; i < vertices.size(); i++)
	{
		if (vertices[i].x < minX)
			minX = vertices[i].x;
		if (vertices[i].x > maxX)
			maxX = vertices[i].x;
		if (vertices[i].y < minY)
			minY = vertices[i].y;
		if (vertices[i].y > maxY)
			maxY = vertices[i].y;
	}
	minMax2d = { minX, maxX, minY, maxY };

	modelVertices.reserve(3 * faces.size());
	for (int i = 0; i < faces.size(); i++)
	{
		Face currentFace = faces.at(i);
		for (int j = 0; j < 3; j++)
		{
			int vertexIndex = currentFace.GetVertexIndex(j) - 1;
			int normalIndex = currentFace.GetNormalIndex(j) - 1;

			Vertex vertex;
			vertex.position = vertices[vertexIndex];
			if(normals.size()>0) // for data2.0 models
			vertex.normal = normals[normalIndex];
			if (textureCoords.size() > 0)
			{
				int textureCoordsIndex = currentFace.GetTextureIndex(j) - 1;
				vertex.textureCoords = textureCoords[textureCoordsIndex];
			}

			
		modelVertices.push_back(vertex);
		}
	}


	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, modelVertices.size() * sizeof(Vertex), &modelVertices[0], GL_STATIC_DRAW);


	// Vertex Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Vertex Texture Coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// unbind to make sure other code does not change it somewhere else
	glBindVertexArray(0);
	//normOfVer = std::vector<glm::vec3>(GetVertexCount());
	//InitBounding();
	//InitNormOfVer();
}
MeshModel::~MeshModel()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

const std::vector<Vertex>& MeshModel::GetModelVertices()
{
	return modelVertices;
}

GLuint MeshModel::GetVAO() const
{
	return vao;
}

std::vector<glm::vec2> MeshModel::getBounding()
{
	return minMaxVec;
}

std::vector<glm::vec2> MeshModel::getFaceBounding(int FaceIndex)
{
	std::vector<glm::vec2> minMaxFaceVec = { {1000,-1000},{1000,-1000},{1000,-1000} };
	Face face = this->GetFace(FaceIndex);
	for (int i = 0; i < 3; i++)
	{
		glm::vec3 vertex = this->getVertexIndex(face.GetVertexIndex(i)-1);
		for (int j = 0; j < 3; j++)
		{
			if (vertex[j] < minMaxFaceVec[j][0])
				minMaxFaceVec[j][0] = vertex[j];
			if (vertex[j] > minMaxFaceVec[j][1])
				minMaxFaceVec[j][1] = vertex[j];
		}
	}
	return minMaxFaceVec;
}

void MeshModel::InitBounding()
{
	minMaxVec = { {1000,-1000},{1000,-1000},{1000,-1000} };
	for (int i = 0; i < GetVertexCount(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (getVertexIndex(i)[j] < minMaxVec[j][0])
				minMaxVec[j][0] = getVertexIndex(i)[j];
			if (getVertexIndex(i)[j] > minMaxVec[j][1])
				minMaxVec[j][1] = getVertexIndex(i)[j];
		}
	}

}

glm::mat4 MeshModel::GetTrans()
{
	return this->GetWorldTransformation() * this->getLocalTransformation();
}

glm::mat4 MeshModel::GetWorldTransformation()
{
	return worldTranslateMat  * worldRotationMat * worldScaleMat;
}

void MeshModel::RotateWorld(float rotateX, float rotateY, float rotateZ)
{
	rotateX = rotateX * M_PI, rotateY = rotateY * M_PI, rotateZ = rotateZ * M_PI;
	glm::mat4 TransformX(1.f), TransformY(1.f), TransformZ(1.f);
	TransformX = glm::rotate(TransformX, rotateX, { 1.0f,0.0f,0.0f });
	TransformY = glm::rotate(TransformY, rotateY, { 0.0f,1.0f,0.0f });
	TransformZ = glm::rotate(TransformZ, rotateZ, { 0.0f,0.0f,1.0f });
	worldRotationMat = TransformX * TransformY * TransformZ;
}

glm::mat4 MeshModel::localScale(float x, float y, float z, float U) {
	glm::mat4 Transform(1.f);
	glm::vec3 scaleVec(1.f);
	scaleVec[0] = x +U, scaleVec[1] = y + U , scaleVec[2] = z + U ;
	Transform = glm::scale(Transform, scaleVec);
	localScaleMat = Transform;
	return localScaleMat;
}

void MeshModel::TranslateWorld(float xT, float yT, float zT)
{
	glm::mat4 Transform(1.f);
	glm::vec3 translateVec(1.f);
	translateVec[0] = xT;
	translateVec[1] = yT;
	translateVec[2] = zT;
	Transform = glm::translate(Transform, translateVec);
	worldTranslateMat = Transform;
	Vertex vertcies;
}

glm::mat4 MeshModel::TranslateLocal(float xT, float yT, float zT) // cord is from 0 - 2 and scales only x y or z
{
	glm::mat4 Transform(1.f);
	glm::vec3 translateVec(1.f);
	translateVec[0] = xT;
	translateVec[1] = yT;
	translateVec[2] = zT;
	Transform = glm::translate(Transform, translateVec);
	localTranslateMat = Transform;
	return localTranslateMat;
}

glm::mat4 MeshModel::RotateLocal(float rotateX, float rotateY, float rotateZ)
{
	glm::mat4 TransformX(1.f), TransformY(1.f), TransformZ(1.f);
	rotateX = rotateX * M_PI, rotateY = rotateY * M_PI, rotateZ = rotateZ * M_PI;
	TransformX = glm::rotate(TransformX, rotateX, { 1.0f,0.0f,0.0f });
	TransformY = glm::rotate(TransformY, rotateY, { 0.0f,1.0f,0.0f });
	TransformZ = glm::rotate(TransformZ, rotateZ, { 0.0f,0.0f,1.0f });
	localRotateMat = TransformX * TransformY * TransformZ;
	return localRotateMat;
}


void MeshModel::worldScale(float x, float y, float z, float U) {
	glm::mat4 Transform(1.f);
	glm::vec3 scaleVec(1.f);
	scaleVec[0] = x + U, scaleVec[1] = y + U , scaleVec[2] = z + U;
	Transform = glm::scale(Transform, scaleVec);
	worldScaleMat = Transform;
}

glm::mat4 MeshModel::getLocalTransformation()
{
	return localTranslateMat * localRotateMat * localScaleMat;
}

glm::vec4 MeshModel::getMinMax()
{
	return minMax2d;
}

const Face& MeshModel::GetFace(int index) const
{
	return faces[index];
}

int MeshModel::GetFacesCount() const
{
	return faces.size();
}

const std::string& MeshModel::GetModelName() const
{
	return model_name;
}

const int MeshModel::GetVertexCount()
{
	return vertices.size();
}

glm::vec3 MeshModel::getVertexIndex(int index)
{
	return vertices[index];
}

glm::vec3 MeshModel::GetNormalIndex(int index)
{
	return normals[index];
}

void MeshModel::setModelMaterial(const glm::vec3& toSet, const glm::vec3& diff, const glm::vec3& spect)
{
	modelMaterial = toSet;
	modelMaterialStruct.ambient = toSet;
	modelMaterialStruct.diff = diff;
	modelMaterialStruct.spect = spect;
	modeldiffuseMaterial = diff;
	modelspectMaterial = spect;
}

Material MeshModel::getModelMaterial() const
{
	return modelMaterialStruct;
}

glm::vec3 MeshModel::getModelMaterial(int index) const
{
	if (index == 0)
		return modelMaterial;
	else if (index == 1)
		return modeldiffuseMaterial;
	else
		return modelspectMaterial;
}


void MeshModel::InitNormOfVer() 
{
	std::vector<std::vector<glm::vec3>> temp (GetVertexCount());
	int vertNum;
	glm::vec3 sum;
	for (int i = 0; i < GetFacesCount(); i++)
	{
		for (int j = 0; j < 3; j++) {
			vertNum = GetFace(i).GetVertexIndex(j) - 1;
			temp[vertNum].push_back(GetNormalIndex(GetFace(i).GetNormalIndex(j) - 1));
		}
	}
	for (int i = 0; i < temp.size(); i++)
	{
		sum = { 0,0,0 };
		for (int j = 0; j < temp[i].size(); j++)
			sum += temp[i][j];
		normOfVer[i] = sum / (glm::vec3)(temp[i].size()-1);
	}
}

const std::vector<glm::vec3>& MeshModel::GetNormOfVer()
{
	return normOfVer;
}



