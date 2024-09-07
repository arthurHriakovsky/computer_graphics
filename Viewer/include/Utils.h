#pragma once
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "MeshModel.h"
#include <glm/gtx/compatibility.hpp>

class Utils
{
public:
	static glm::vec3 Vec3fFromStream(std::istream& issLine);
	static glm::vec4 HomVec4FromVec3(glm::vec3 v);
	static glm::vec3 Vec3FromHomVec4(glm::vec4 v);
	static glm::vec2 Vec2fFromStream(std::istream& issLine);
	static std::shared_ptr<MeshModel> LoadMeshModel(const std::string& filePath);
	static std::string GetFileName(const std::string& filePath);
	static float interpolate(glm::vec3 p1, glm::vec3 p2, glm::vec2 p3);
	static glm::vec3 interpolateAlpha(glm::vec3 p1, glm::vec3 p2, glm::vec2 p3,glm::vec3 value1, glm::vec3 value2);
	static bool kernelCheck(glm::vec2 point, int width, int height);
};
