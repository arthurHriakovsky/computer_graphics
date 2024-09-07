#pragma once

#include <vector>
#include <memory>

#include "Camera.h"
#include "MeshModel.h"
#include "Lighting.h"

using namespace std;

class Scene {
public:
	Scene();

	void AddModel(const std::shared_ptr<MeshModel>& mesh_model);
	void LoadAxis(const std::shared_ptr<MeshModel>& Axis);
	void AddLighting(const std::shared_ptr<Lighting>& lighting);
	shared_ptr<MeshModel> GetAxis() const;
	void DeleteAxis();
	int GetModelCount() const;
	std::shared_ptr<MeshModel> GetModel(int index) const;
	MeshModel& GetActiveModel() const;
	void AddCamera(const shared_ptr<Camera>& camera);
	int GetCameraCount() const;
	Camera& GetCamera(int index);
	Camera& GetActiveCamera() const;
	
	void setActiveLightingIndex(int index);
	int GetActiveLightingIndex() const;
	Lighting& GetActiveLight() const;
	int GetLightingCount() const;

	void SetActiveCameraIndex(int index);
	int GetActiveCameraIndex() const;

	void SetActiveModelIndex(int index);
	int GetActiveModelIndex() const;

	bool getDrawWorld() const;
	bool getDrawBoundingBox() const;
	bool getDrawVertexNormals() const;
	bool getDrawFaceNormals() const;
	bool getDrawFaceBoundingBox() const;
	bool getTriangleColor() const;
	bool getPaintModel() const;
	bool getZBufferColor() const;
	void setScreenShot(bool sc);
	bool getScreenShot() const;
	 bool getToon() const;
	int getKernelSize() const;
	bool getBloom() const;
	int getBlurIntensity() const;
	float getThreshhold() const;
	void setPostProcessing(bool bloom = 0, int kernel = 3, int blurIntensity = 3, float TH = 0.7);
	void setFeatures(bool dWorld=0, bool BB=0, bool dVN=0, bool dFN=0, bool dFBB=0, bool tCol = 0, bool pM = 0, bool ZBC=0,bool Toon = 0);
private:
	vector<shared_ptr<MeshModel>> mesh_models;
	vector<shared_ptr<Camera>> cameras;
	vector<shared_ptr<Lighting>> lightSources;
	shared_ptr<MeshModel> axis;

	bool ScreenShot;
	bool Bloom;
	int kernelSize;
	int blurIntensity;
	float threshhold;
	bool drawWorld;
	bool Lines;
	bool drawBoundingBox;
	bool normalMap;
	 bool Toon;
	bool Texture;  
	bool Plane;
	bool Sphere;
	bool Cylinder;
	int active_light_index;
	int active_camera_index;
	int active_model_index;
};