#pragma once
#include "Scene.h"
#include "ShaderProgram.h"
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <memory>
#include <Texture2D.h>


class Renderer
{
public:
	Renderer();
	virtual ~Renderer();
	void Render(const Scene& scene);
	/*
	void DrawModelAxis(const Scene& scene);
	void DrawFaceNormals(const Scene& scene);
	void DrawVertexNormals(const Scene& scene);
	void DrawWorld(const Scene& scene);
	void DrawBoundingBox(const Scene& scene);
	const glm::vec3 GetRandomColor();
	void DrawModel(const Scene& scene);
	void PaintFace(const Scene& scene, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,const glm::vec3& color,const int& FaceNum);
	void DrawFaceBoundingBox(const Scene& scene, const int FaceIndex);
	glm::vec3 GetViewportTrans(glm::vec4 vertex);
	void SetNewViewport(int newWidth, int newHeight);
	void SwapBuffers();
	void ClearColorBuffer(const glm::vec3& color);
	int GetViewportWidth() const;
	int GetViewportHeight() const;
	void SetZbufferColor(bool setter);
	bool GetZbufferColor() const;
	float DiffuseLighting(const Scene& scene, std::pair<glm::vec3, glm::vec3> faceNormal, glm::vec3 lightPosition);
	std::pair<glm::vec3, glm::vec3> GetFaceNormal(const Scene& scene,const int& faceIndex);
	void PaintModel(const Scene& scene);
	float SpecularLighting(const Scene& scene, std::pair<glm::vec3, glm::vec3> faceNormal, glm::vec3 lightPosition, glm::vec3 eyePosition, float shineCoef);
	void DrawDirectionsOfSpectralReflection(const Scene& scene);
	std::vector<std::pair<glm::vec3, glm::vec3>> GetVertexNormalsForFace(const Scene& scene, const int& faceIndex);
	glm::vec3 updColor(const Scene& scene, const glm::vec3& color,const int& FaceNum,const glm::vec3& lightPosition);
	std::vector<glm::vec3> updColorGauraud(const Scene& scene, const glm::vec3& color, const int& FaceNum, const glm::vec3& lightPosition);
	void ScreenShotColorBuffer(float* buffer, float* ScreenShotBuffer);
	void BlurImage(const Scene& scene, float* buffer, float* ScreenShotBuffer);
	void Bloom(const Scene& scene);
	*/
	void LoadShaders();
	void LoadTextures();

	std::vector<std::pair<glm::vec3, glm::vec3>> GetVertexNormalsForFacePhong(const Scene& scene, const int& faceIndex);
private:
	/*
	void PutPixel(const int i, const int j, const glm::vec3& color);
	void DrawLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& color);
	void DrawLineOpp(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& color);

	void CreateBuffers(int w, int h);
	void CreateOpenglBuffer();
	void InitOpenglRendering();
	*/
	ShaderProgram lightShader;
	ShaderProgram colorShader;
	Texture2D texture1;
	//GLuint gl_screen_tex;
	//GLuint gl_screen_vtc;
};


