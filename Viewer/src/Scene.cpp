#include "Scene.h"
#include "MeshModel.h"
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Scene::Scene() :
	active_camera_index(0),
	active_model_index(0),
	active_light_index(0)
{
	axis = NULL;
	setFeatures();
}


void Scene::AddModel(const std::shared_ptr<MeshModel>& mesh_model)
{
	mesh_models.push_back(mesh_model);

}

void Scene::LoadAxis(const std::shared_ptr<MeshModel>& Axis)
{
	axis = Axis;
}

shared_ptr<MeshModel> Scene::GetAxis() const
{
	return axis;
}

void Scene::DeleteAxis()
{
	axis = NULL;
}

int Scene::GetModelCount() const
{
	return (int)mesh_models.size();
}

std::shared_ptr<MeshModel> Scene::GetModel(int index) const
{
	return mesh_models[index];
}

MeshModel& Scene::GetActiveModel() const
{
	return *mesh_models[active_model_index];
}

void Scene::AddCamera(const std::shared_ptr<Camera>& camera)
{
	cameras.push_back(camera);
}

void Scene::AddLighting(const std::shared_ptr<Lighting>& lighting)
{
	lightSources.push_back(lighting);
}

int Scene::GetLightingCount() const
{
	return lightSources.size();
}

void Scene::setActiveLightingIndex(int index)
{
	active_light_index = index;
}

Lighting& Scene::GetActiveLight() const
{
	return *lightSources[active_light_index];
}

int Scene::GetActiveLightingIndex() const
{
	return active_light_index;
}

int Scene::GetCameraCount() const
{
	return (int)cameras.size();
}

Camera& Scene::GetCamera(int index)
{
	return *cameras[index];
}

Camera& Scene::GetActiveCamera() const
{
	return *cameras[active_camera_index];
}

void Scene::SetActiveCameraIndex(int index)
{
	active_camera_index = index;
}

int Scene::GetActiveCameraIndex() const
{
	return active_camera_index;
}

void Scene::SetActiveModelIndex(int index)
{
	active_model_index = index;
}

int Scene::GetActiveModelIndex() const
{
	return active_model_index;
}

bool Scene::getDrawWorld() const
{
	return drawWorld;
}

bool Scene::getDrawBoundingBox() const
{
	return drawBoundingBox;
}

bool Scene::getDrawVertexNormals() const
{
	return normalMap;
}

bool Scene::getDrawFaceNormals() const
{
	return Texture;
}

bool Scene::getDrawFaceBoundingBox() const
{
	return Plane;
}

bool Scene::getTriangleColor() const
{
	return Sphere;
}

bool Scene::getPaintModel() const
{
	return Lines;
}

bool Scene::getZBufferColor() const
{
	return Cylinder;
}

void Scene::setScreenShot(bool sc)
{
	ScreenShot = sc;
}

bool Scene::getScreenShot() const
{
	return ScreenShot;
}

bool Scene::getToon() const
{
	return Toon;
}

int Scene::getKernelSize() const
{
	return kernelSize;
}

bool Scene::getBloom() const
{
	return Bloom;
}

int Scene::getBlurIntensity() const
{
	return blurIntensity;
}

float Scene::getThreshhold() const
{
	return threshhold;
}

void Scene::setPostProcessing(bool bloom, int kernel, int bI, float TH)
{
	Bloom = bloom;
	kernelSize = kernel;
	blurIntensity = bI;
	threshhold = TH;
}

void Scene::setFeatures(bool dWorld, bool BB, bool dVN, bool dFN, bool dFBB,bool tCol, bool pM, bool ZBC,bool toon)
{
	drawWorld = dWorld;
	drawBoundingBox = BB;
	normalMap = dVN;
	
	Toon = toon;
	Texture = dFN;
	Plane = dFBB;
	Sphere = tCol;
	Cylinder = ZBC;
	Lines = pM;
}

