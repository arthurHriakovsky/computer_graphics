#define _USE_MATH_DEFINES
#include <cmath>
#include <random>
#include <algorithm>
#include <iostream>
#include "Utils.h"
#include "Renderer.h"
#include "InitShader.h"
#include <vector>
#include <memory>
#include <imgui/imgui.h>
#include "ShaderProgram.h"
#include "MeshModel.h"

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
#define Z_INDEX(width,x,y) ((x)+(y)*(width))

void Renderer::LoadShaders()
{
	colorShader.loadShaders("vshader.glsl", "fshader.glsl");

	//GLuint program = InitShader("vshader.glsl", "fshader.glsl");
	//// Make this program the current one.
	//glUseProgram(program);
}

void Renderer::LoadTextures()
{
	if (!texture1.loadTexture("bin\\Debug\\NormalMap2.png", true))
	{
		texture1.loadTexture("bin\\Release\\NormalMap2.png", true);
	}
}

Renderer::Renderer()
{
}
Renderer::~Renderer()
{
}


//##############################
//##OpenGL stuff. Don't touch.##
//##############################

// Basic tutorial on how opengl works:
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
// don't linger here for now, we will have a few tutorials about opengl later.
//void Renderer::InitOpenglRendering()
//{
//	// Creates a unique identifier for an opengl texture.
//	glGenTextures(1, &gl_screen_tex);
//
//	// Same for vertex array object (VAO). VAO is a set of buffers that describe a renderable object.
//	glGenVertexArrays(1, &gl_screen_vtc);
//
//	GLuint buffer;
//
//	// Makes this VAO the current one.
//	glBindVertexArray(gl_screen_vtc);
//
//	// Creates a unique identifier for a buffer.
//	glGenBuffers(1, &buffer);
//
//	// (-1, 1)____(1, 1)
//	//	     |\  |
//	//	     | \ | <--- The exture is drawn over two triangles that stretch over the screen.
//	//	     |__\|
//	// (-1,-1)    (1,-1)
//	const GLfloat vtc[] = {
//		-1, -1,
//		 1, -1,
//		-1,  1,
//		-1,  1,
//		 1, -1,
//		 1,  1
//	};
//
//	const GLfloat tex[] = {
//		0,0,
//		1,0,
//		0,1,
//		0,1,
//		1,0,
//		1,1 };
//
//	// Makes this buffer the current one.
//	glBindBuffer(GL_ARRAY_BUFFER, buffer);
//
//	// This is the opengl way for doing malloc on the gpu. 
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc) + sizeof(tex), NULL, GL_STATIC_DRAW);
//
//	// memcopy vtc to buffer[0,sizeof(vtc)-1]
//	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);
//
//	// memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
//	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);
//
//	// Loads and compiles a sheder.
//
//
//	// Tells the shader where to look for the vertex position data, and the data dimensions.
//	GLint  vPosition = glGetAttribLocation(program, "vPosition");
//	glEnableVertexAttribArray(vPosition);
//	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);
//
//	// Same for texture coordinates data.
//	GLint  vTexCoord = glGetAttribLocation(program, "vTexCoord");
//	glEnableVertexAttribArray(vTexCoord);
//	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)sizeof(vtc));
//
//	//glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );
//
//	// Tells the shader to use GL_TEXTURE0 as the texture id.
//	glUniform1i(glGetUniformLocation(program, "texture"), 0);
//}




void Renderer::Render(const Scene& scene)
{
	int cameraCount = scene.GetCameraCount();
	if (cameraCount >= 0)
	{
		int modelCount = scene.GetModelCount();
		const Camera& camera = scene.GetActiveCamera();
		
		const Lighting light = scene.GetActiveLight();

		for (int currentModelIndex = 0; currentModelIndex < modelCount; currentModelIndex++)
		{
			std::shared_ptr<MeshModel> currentModel = scene.GetModel(currentModelIndex);

			// Activate the 'colorShader' program (vertex and fragment shaders)
			colorShader.use();
			// Set the uniform variables
			colorShader.setUniform("model", currentModel->GetTrans());
			colorShader.setUniform("view", camera.GetViewTransformation());
			colorShader.setUniform("projection", camera.GetProjectionTransformation());
			
			
			colorShader.setUniform("material.ambient", currentModel->getModelMaterial().ambient);
			colorShader.setUniform("material.diff", currentModel->getModelMaterial().diff);
			colorShader.setUniform("material.spect", currentModel->getModelMaterial().spect);
			colorShader.setUniform("light.point", light.getLight().point);
			colorShader.setUniform("light.ambient", light.getLight().ambient);
			colorShader.setUniform("light.diff", light.getLight().diff);
			colorShader.setUniform("light.spect", light.getLight().spect);
			colorShader.setUniform("light.shine", light.getLight().shine);
			colorShader.setUniform("toon", scene.getToon());
			colorShader.setUniform("cameraEye", camera.getCameraPos());
			//colorShader.setUniform("material.textureMap", 0);
			
			colorShader.setUniform("material.textureMap", 0);
			colorShader.setUniform("texturebool", scene.getDrawFaceNormals()); // put on texture
			colorShader.setUniform("normalMap", scene.getDrawVertexNormals()); // put on normalmap
			colorShader.setUniform("plane", scene.getDrawFaceBoundingBox()); // plane texture mapping
			colorShader.setUniform("sphere", scene.getTriangleColor()); // sphere texture mapping
			colorShader.setUniform("cylinder", scene.getZBufferColor()); // cylinder texture mapping
			colorShader.setUniform("minMax2d", currentModel->getMinMax());


			//glm::vec4 test = camera.GetProjectionTransformation() *camera.GetViewTransformation() *currentModel->GetTrans()*glm::vec4(1, 1, 1, 1);

			// Set 'texture1' as the active texture at slot #0
			texture1.bind(0);

			// Drag our model's faces (triangles) in fill mode
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBindVertexArray(currentModel->GetVAO());
			glDrawArrays(GL_TRIANGLES, 0, currentModel->GetModelVertices().size());
			glBindVertexArray(0);

		
			// Unset 'texture1' as the active texture at slot #0
			texture1.unbind(0);

			colorShader.setUniform("color", glm::vec3(0, 0, 0));
		
		//	// Drag our model's faces (triangles) in line mode (wireframe)
			if(scene.getPaintModel()) // wireframe
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glBindVertexArray(currentModel->GetVAO());
			glDrawArrays(GL_TRIANGLES, 0, currentModel->GetModelVertices().size());
			glBindVertexArray(0);
		}
	}
	//static int timesToBlur = 0;
	//static int BloomDid = 0;
	//if(!scene.getScreenShot())
	//{ 
	//	timesToBlur = 0;
	//	BloomDid = 0;
	//	if(scene.getDrawWorld())
	//	DrawWorld(scene);
	//	if (scene.GetModelCount() >= 1)
	//		DrawModel(scene);
	//	if (scene.getPaintModel())
	//		PaintModel(scene);
	//		if (scene.getDrawBoundingBox())
	//			DrawBoundingBox(scene);
	//		if (scene.getDrawVertexNormals())
	//			DrawVertexNormals(scene);
	//		if (scene.getDrawFaceNormals())
	//			DrawFaceNormals(scene);
	//		SetZbufferColor(scene.getZBufferColor());
	//		//if (scene.GetActiveLight().getDirectionsOfSpecular())
	//		//	DrawDirectionsOfSpectralReflection(scene);
	//}
	//else
	//{
	//	
	//	if(timesToBlur < scene.getBlurIntensity())
	//	{ 
	//		ScreenShotColorBuffer(color_buffer, color_ScreenShotBuffer);
	//		timesToBlur++;
	//		BlurImage(scene, color_buffer, color_ScreenShotBuffer);
	//	}
	//	if (scene.getBloom()&&BloomDid == 0)
	//	{
	//		ScreenShotColorBuffer(color_buffer, color_ScreenShotBuffer);
	//		BloomDid++;
	//		Bloom(scene);
	//	}
	//}
}



//void Renderer::BlurImage(const Scene& scene, float* buffer, float* ScreenShotBuffer)
//{
//	int kernelSize = scene.getKernelSize();
//	float* tempBuffer = new float[3 * viewport_width * viewport_height];
//	ScreenShotColorBuffer(ScreenShotBuffer, tempBuffer);
//
//	
//	for (int i = 0; i < viewport_width; i++) 
//	{
//		for (int j = 0; j < viewport_height; j++)
//		{
//			glm::vec3 sum(0);
//			int pointsAdded = 0;
//			for (int x = -1*(kernelSize) / 2; x <= kernelSize / 2; x++)
//			{
//				for (int y = -1 * (kernelSize) / 2; y <= kernelSize / 2; y++)
//				{
//					glm::vec2 point(x+i, y+j);
//					if (Utils::kernelCheck(point, viewport_width, viewport_height) == true)
//					{
//						sum[0] = sum[0]+tempBuffer[INDEX(viewport_width, x + i, y + j, 0)];
//						sum[1] = sum[1]+tempBuffer[INDEX(viewport_width, x + i, y + j, 1)];
//						sum[2] = sum[2]+tempBuffer[INDEX(viewport_width, x + i, y + j, 2)];
//						pointsAdded++;
//					}
//				}
//			}
//			ScreenShotBuffer[INDEX(viewport_width, i, j, 0)] = { sum[0] / pointsAdded };
//			ScreenShotBuffer[INDEX(viewport_width, i, j, 1)] = sum[1] / pointsAdded;
//			ScreenShotBuffer[INDEX(viewport_width, i, j, 2)] = sum[2] / pointsAdded;
//		}
//		for (int i = 0; i < viewport_width; i++)
//		{
//			for (int j = 0; j < viewport_height; j++)
//			{
//				buffer[INDEX(viewport_width, i, j, 0)] = ScreenShotBuffer[INDEX(viewport_width, i, j, 0)];
//				buffer[INDEX(viewport_width, i, j, 1)] = ScreenShotBuffer[INDEX(viewport_width, i, j, 1)];
//				buffer[INDEX(viewport_width, i, j, 2)] = ScreenShotBuffer[INDEX(viewport_width, i, j, 2)];
//			}
//		}
//	}
//	delete[] tempBuffer;
//}
//
//void Renderer::Bloom(const Scene& scene)
//{
//	float* temp_buffer = new float[3 * viewport_width * viewport_height];
//	for (int i = 0; i < viewport_width; i++)
//	{
//		for (int j = 0; j < viewport_height; j++)
//		{
//			temp_buffer[INDEX(viewport_width, i, j, 0)] = 0;
//			temp_buffer[INDEX(viewport_width, i, j, 1)] = 0;
//			temp_buffer[INDEX(viewport_width, i, j, 2)] = 0;
//		}
//	}
//	for (int i = 0; i < viewport_width; i++)
//	{
//		for (int j = 0; j < viewport_height; j++)
//		{
//			if (color_ScreenShotBuffer[INDEX(viewport_width, i, j, 0)]>=scene.getThreshhold() && 
//				color_ScreenShotBuffer[INDEX(viewport_width, i, j, 1)] >= scene.getThreshhold() && 
//				color_ScreenShotBuffer[INDEX(viewport_width, i, j, 2)] >= scene.getThreshhold() )
//			{
//				temp_buffer[INDEX(viewport_width, i, j, 0)] = color_ScreenShotBuffer[INDEX(viewport_width, i, j, 0)];
//				temp_buffer[INDEX(viewport_width, i, j, 1)] = color_ScreenShotBuffer[INDEX(viewport_width, i, j, 1)];
//				temp_buffer[INDEX(viewport_width, i, j, 2)] = color_ScreenShotBuffer[INDEX(viewport_width, i, j, 2)];
//			}
//			
//		}
//	}
//
//	ScreenShotColorBuffer(temp_buffer, color_ScreenShotBuffer);
//
//	for (int i = 0; i < 5; i++)
//	{
//		BlurImage(scene, temp_buffer, color_ScreenShotBuffer);
//		cout <<"pass: " << i << endl;
//	}
//
//	for (int i = 0; i < viewport_width; i++)
//	{
//		for (int j = 0; j < viewport_height; j++)
//		{
//			color_buffer[INDEX(viewport_width, i, j, 0)] = color_buffer[INDEX(viewport_width, i, j, 0)]+temp_buffer[INDEX(viewport_width, i, j, 0)];
//			color_buffer[INDEX(viewport_width, i, j, 1)] = color_buffer[INDEX(viewport_width, i, j, 1)]+temp_buffer[INDEX(viewport_width, i, j, 1)];
//			color_buffer[INDEX(viewport_width, i, j, 2)] = color_buffer[INDEX(viewport_width, i, j, 2)]+temp_buffer[INDEX(viewport_width, i, j, 2)];
//		}
//	}
//	delete[] temp_buffer;
//
//}
//
//void Renderer::ScreenShotColorBuffer(float* buffer, float* ScreenShotBuffer)
//{
//	for (int i = 0; i < viewport_width; i++)
//	{
//		for (int j = 0; j < viewport_height; j++)
//		{
//			ScreenShotBuffer[INDEX(viewport_width, i, j, 0)]= buffer[INDEX(viewport_width, i, j, 0)];
//			ScreenShotBuffer[INDEX(viewport_width, i, j, 1)]= buffer[INDEX(viewport_width, i, j, 1)];
//			ScreenShotBuffer[INDEX(viewport_width, i, j, 2)]= buffer[INDEX(viewport_width, i, j, 2)];
//		}
//	}
//}
//
//const glm::vec3 Renderer::GetRandomColor()
//{
//	glm::vec3 color;
//	color[0] = (float)1 / 255 * (rand() % 255);
//	color[1] = (float)1 / 255 * (rand() % 255);
//	color[2] = (float)1 / 255 * (rand() % 255);
//	return color;
//}
//
//float Renderer::DiffuseLighting(const Scene& scene, std::pair<glm::vec3, glm::vec3> faceNormal, glm::vec3 lightPosition)
//{
//	//glm::vec3 normalOfLight = glm::normalize(lightPosition - faceNormal.second);
//	//glm::vec3 normalOfFace = glm::normalize(faceNormal.first - faceNormal.second);
//	if (scene.GetActiveLight().getDirectionalSource())
//		return (max(0.f, dot(glm::normalize(lightPosition), glm::normalize(faceNormal.second - faceNormal.first))));
//	else
//		return (max(0.f, dot(glm::normalize(lightPosition - faceNormal.first), glm::normalize(faceNormal.second - faceNormal.first))));
//}
//
//float Renderer::SpecularLighting(const Scene& scene, std::pair<glm::vec3, glm::vec3> faceNormal, glm::vec3 lightPosition, glm::vec3 eyePosition, float shineCoef)
//{
//
//	if (scene.GetActiveLight().getDirectionalSource()) {
//		glm::vec3 temp = glm::normalize(glm::reflect((glm::normalize(lightPosition)), glm::normalize(faceNormal.second - faceNormal.first)));
//		return (powf(max(0.f, dot(temp, glm::normalize(eyePosition - faceNormal.first))), shineCoef));
//	}
//	
//	glm::vec3 temp = glm::normalize(glm::reflect((glm::normalize(lightPosition - faceNormal.first)), glm::normalize(faceNormal.second - faceNormal.first)));
//	return (powf(max(0.f, dot(temp, glm::normalize(eyePosition - faceNormal.first))), shineCoef));
//
//}
//
//void Renderer::PaintModel(const Scene& scene)
//{
//	for (int j = 0; j < scene.GetModelCount(); j++)
//	{
//		MeshModel demo = scene.GetModel(j);
//		Camera ActiveCam = scene.GetActiveCamera();
//		int faceNum = demo.GetFacesCount();
//		bool randomColorFlag = scene.getTriangleColor();
//		for (int i = 0; i < faceNum; i++)
//		{
//			glm::vec3 color = demo.getModelMaterial(0);
//			if (randomColorFlag)
//				color = GetRandomColor();
//			glm::vec4 p1 = Utils::HomVec4FromVec3(demo.getVertexIndex((demo.GetFace(i).GetVertexIndex(0)) - 1)),
//				p2 = Utils::HomVec4FromVec3(demo.getVertexIndex((demo.GetFace(i).GetVertexIndex(1)) - 1)),
//				p3 = Utils::HomVec4FromVec3(demo.getVertexIndex((demo.GetFace(i).GetVertexIndex(2)) - 1));
//			p1 = ActiveCam.GetProjectionTransformation() * ActiveCam.GetViewTransformation() * demo.GetTrans() * p1;
//			p2 = ActiveCam.GetProjectionTransformation() * ActiveCam.GetViewTransformation() * demo.GetTrans() * p2;
//			p3 = ActiveCam.GetProjectionTransformation() * ActiveCam.GetViewTransformation() * demo.GetTrans() * p3;
//			glm::vec3 realp1 = GetViewportTrans(p1), realp2 = GetViewportTrans(p2), realp3 = GetViewportTrans(p3);
//			if (((realp1[0] < GetViewportWidth() && GetViewportHeight() > realp1[1]) || (realp2[0] < GetViewportWidth() && GetViewportHeight() > realp2[1])) &&
//				((realp2[0] < GetViewportWidth() && GetViewportHeight() > realp2[1]) || (realp3[0] < GetViewportWidth() && GetViewportHeight() > realp3[1])) &&
//				((realp3[0] < GetViewportWidth() && GetViewportHeight() > realp3[1]) || (realp1[0] < GetViewportWidth() && GetViewportHeight() > realp1[1])))
//			{
//				PaintFace(scene, realp1, realp2, realp3, color, i);
//			}
//		}
//	}
//}
//void Renderer::DrawModel(const Scene& scene)
//{
//	if (scene.getPaintModel())
//		return;
//	for (int j = 0; j < scene.GetModelCount(); j++)
//	{
//		MeshModel demo = scene.GetModel(j);
//		Camera ActiveCam = scene.GetActiveCamera();
//		int faceNum = demo.GetFacesCount();
//		glm::vec3 color = { 0,0,0 };
//		for (int i = 0; i < faceNum; i++)
//		{ 
//			glm::vec4 p1 = Utils::HomVec4FromVec3(demo.getVertexIndex((demo.GetFace(i).GetVertexIndex(0)) - 1)),
//				p2 = Utils::HomVec4FromVec3(demo.getVertexIndex((demo.GetFace(i).GetVertexIndex(1)) - 1)),
//				p3 = Utils::HomVec4FromVec3(demo.getVertexIndex((demo.GetFace(i).GetVertexIndex(2)) - 1));
//			p1 = ActiveCam.GetProjectionTransformation() * ActiveCam.GetViewTransformation() * demo.GetTrans() * p1;
//			p2 = ActiveCam.GetProjectionTransformation() * ActiveCam.GetViewTransformation() * demo.GetTrans() * p2;
//			p3 = ActiveCam.GetProjectionTransformation() * ActiveCam.GetViewTransformation() * demo.GetTrans() * p3;
//			glm::vec3 realp1 = GetViewportTrans(p1), realp2 = GetViewportTrans(p2), realp3 = GetViewportTrans(p3);
//				if ((realp1[0] < GetViewportWidth() && GetViewportHeight() > realp1[1]) || (realp2[0] < GetViewportWidth() && GetViewportHeight() > realp2[1]))
//					DrawLine({ realp1 }, { realp2 }, { color });
//				if ((realp2[0] < GetViewportWidth() && GetViewportHeight() > realp2[1]) || (realp3[0] < GetViewportWidth() && GetViewportHeight() > realp3[1]))
//					DrawLine({ realp2 }, { realp3 }, { color });
//				if ((realp3[0] < GetViewportWidth() && GetViewportHeight() > realp3[1]) || (realp1[0] < GetViewportWidth() && GetViewportHeight() > realp1[1]))
//					DrawLine({ realp3 }, { realp1 }, { color });
//			if (scene.getDrawFaceBoundingBox() && i !=0)
//				DrawFaceBoundingBox(scene, i);
//		}
//		DrawModelAxis(scene);
//	}
//}
//
//glm::vec3 Renderer::updColor(const Scene& scene,const glm::vec3& color,const int& FaceNum, const glm::vec3& lightPosition)
//{
//	glm::vec3 updatedColor = { 0,0,0 };
//	Lighting activeLight = scene.GetActiveLight();
//	std::pair<glm::vec3, glm::vec3> normal = GetFaceNormal(scene, FaceNum);
//	MeshModel model = scene.GetActiveModel();
//
//	if (activeLight.getAmbientLighting()|| activeLight.getPhongReflection())
//		updatedColor += color * activeLight.getAmbientLightingVector(); //Ambient Lighting
//
//	if (activeLight.getDiffuseLighting()|| activeLight.getPhongReflection())
//		updatedColor += model.getModelMaterial(1) * DiffuseLighting(scene, normal, lightPosition) * activeLight.getDiffuseLightingVector(); //Diffuse Lighting
//	if (activeLight.getSpecularLighting()|| activeLight.getPhongReflection())
//	{
//		updatedColor += model.getModelMaterial(2) * SpecularLighting(scene, normal, lightPosition, scene.GetActiveCamera().getCameraPos(), activeLight.getShiniesCoefficent()) * activeLight.getSpecularLightingVector();
//	}
//	return updatedColor;
//}
//
//std::vector<glm::vec3> Renderer::updColorGauraud(const Scene& scene, const glm::vec3& color, const int& FaceNum, const glm::vec3& lightPosition)
//{
//	std::vector<glm::vec3> updatedColor;
//	glm::vec3 temp;
//	std::vector<std::pair<glm::vec3, glm::vec3>> normal = GetVertexNormalsForFace(scene, FaceNum);
//	MeshModel model = scene.GetActiveModel();
//
//	Lighting activeLight = scene.GetActiveLight();
//	for (int i = 0; i < 3; i++) {
//		temp = { 0,0,0 };
//		if (activeLight.getAmbientLighting())
//			temp += color * activeLight.getAmbientLightingVector(); //Ambient Lighting
//
//		if (activeLight.getDiffuseLighting())
//			temp += model.getModelMaterial(1) *DiffuseLighting(scene, normal[i], lightPosition) * activeLight.getDiffuseLightingVector(); //Diffuse Lighting
//		if (activeLight.getSpecularLighting())
//		{
//			temp += model.getModelMaterial(2) * SpecularLighting(scene, normal[i], lightPosition, scene.GetActiveCamera().getCameraPos(), activeLight.getShiniesCoefficent()) * activeLight.getSpecularLightingVector();
//
//		}
//		updatedColor.push_back(temp);
//		glm::vec3 temp = { 0,0,0 };
//	}
//
//	return updatedColor;
//}
//
//
//void Renderer::PaintFace(const Scene& scene, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& color, const int& FaceNum)
//{
//	glm::vec3 colorp1{}, colorp2{}, colorp3{}, vertex1,vertex2,vertex3;
//	glm::vec3 colorx1{}, colorx2{}, colorz1{}, colorz2{};
//	bool GS = false;
//	bool PS = false;
//	MeshModel model = scene.GetActiveModel();
//	Lighting activeLight = scene.GetActiveLight();
//	glm::vec3 updatedColor;
//	glm::vec3 lightPosition = Utils::Vec3FromHomVec4(scene.GetActiveCamera().GetProjectionTransformation()*scene.GetActiveCamera().GetViewTransformation() * Utils::HomVec4FromVec3(activeLight.getLightingPosition()));
//
//	if (activeLight.getGouraudShading())
//	{
//		vector<glm::vec3> temp = updColorGauraud(scene,color, FaceNum, lightPosition);
//		GS = true;
//			colorp1 = temp[0],colorp2 = temp[1],colorp3 = temp[2];
//	}
//	else {
//		if (activeLight.getPhongShading())
//		{
//			std::vector<std::pair<glm::vec3, glm::vec3>> temp = GetVertexNormalsForFacePhong(scene, FaceNum);
//			PS = true;
//			colorp1 = temp[0].second, colorp2 = temp[1].second, colorp3 = temp[2].second;
//			vertex1 = temp[0].first, vertex2 = temp[1].first, vertex3 = temp[2].first;
//		}
//		else {
//			if (activeLight.getAmbientLighting() || activeLight.getDiffuseLighting() || activeLight.getSpecularLighting()|| activeLight.getPhongReflection())
//				updatedColor = updColor(scene, color, FaceNum, lightPosition);
//			else
//				updatedColor = color;
//		}
//	}
//	glm::vec3 realp1 = (p1), realp2 = (p2), realp3 = (p3);
//	float x1{}, x2{}, z1{}, z2{};
//	bool vert31 = true;
//	bool vert32 = true;
//	bool vert21 = true;
//	float a31{}, b31{}, a21{}, b21{}, a32{}, b32{};
//	if (realp2[1] < realp1[1])
//	{
//		swap(realp1, realp2);
//		if (GS||PS)
//			swap(colorp1, colorp2);
//		if (PS)
//		{
//			swap(vertex1, vertex2);
//		}
//	}
//	if (realp3[1] < realp1[1]) 
//	{
//	swap(realp1, realp3);
//	if (GS||PS)
//		swap(colorp1, colorp3);
//	if (PS)
//	{
//		swap(vertex1, vertex3);
//	}
//	}
//	if (realp3[1] < realp2[1])
//	{
//		swap(realp2, realp3);
//		if (GS||PS)
//			swap(colorp2, colorp3);
//		if (PS)
//		{
//			swap(vertex2, vertex3);
//		}
//	}
//
//	if (((realp3[0] == realp1[0]) && (realp3[0] == realp2[0])) || ((realp3[0] == realp1[0]) && (realp1[0] == realp2[0])) || ((realp2[0] == realp1[0]) && (realp3[0] == realp2[0])))
//		return;
//	if (realp3[0] != realp1[0])
//	{
//		vert31 = false;
//		a31 = (realp3[1] - realp1[1]) / (realp3[0] - realp1[0]);
//		b31 = -1 * a31 * realp3[0] + realp3[1];
//	}
//	if (realp3[0] != realp2[0])
//	{
//		vert32 = false;
//		a32 = (realp3[1] - realp2[1]) / (realp3[0] - realp2[0]);
//		b32 = -1 * a32 * realp3[0] + realp3[1];
//	}
//	if (realp1[0] != realp2[0])
//	{
//		vert21 = false;
//		a21 = (realp2[1] - realp1[1]) / (realp2[0] - realp1[0]);
//		b21 = -1 * a21 * realp2[0] + realp2[1];
//	}
//
//	for (int j = realp3[1]; j >= realp1[1]; j--)
//	{
//		if (j >= realp2[1] && realp3[1]!=realp2[1]) // we didnt pass p2 y
//		{
//
//			if (vert31)
//			{
//				x1 = (j - b32) / a32, x2 = realp3[0];
//				// point 3 and 1 above eachother
//			}
//			else if (vert32)
//			{
//				x1 = realp3[0], x2 = (j - b31) / a31;
//				// point 3 and 2 above eachother
//			}
//			else
//			{
//				x1 = (j - b32) / a32, x2 = (j - b31) / a31;
//	
//			}
//			if (GS||PS)
//			{
//				 
//				colorx1 = Utils::interpolateAlpha(realp3, realp2, { x1,j }, colorp3,colorp2);
//				colorx2 = Utils::interpolateAlpha(realp3, realp1, { x2,j }, colorp3, colorp1);
//			}
//			if (PS)
//			{
//				colorz1 = Utils::interpolateAlpha(realp3, realp2, { x1,j }, vertex3, vertex2);
//				colorz2 = Utils::interpolateAlpha(realp3, realp1, { x2,j }, vertex3, vertex1);
//			}
//			z1 = Utils::interpolate(realp3, realp2, { x1,j });
//			z2 = Utils::interpolate(realp3, realp1, { x2,j });
//		}
//		if (j < realp2[1] && realp2[1]!=realp1[1])
//		{
//			if (vert31)
//			{
//				x1 = (j - b21) / a21, x2 = realp3[0];
//				// 3 above 1 verticly
//			}
//			else if (vert21)
//			{
//				x1 = realp2[0], x2 = (j - b31) / a31;
//			}
//			else
//			{
//				x1 = (j - b21) / a21, x2 = (j - b31) / a31;
//			}
//			if (GS||PS)
//			{
//				colorx1 = Utils::interpolateAlpha(realp2, realp1, { x1,j }, colorp2, colorp1);
//				colorx2 = Utils::interpolateAlpha(realp3, realp1, { x2,j }, colorp3, colorp1);
//			}
//			if (PS)
//			{
//				colorz1 = Utils::interpolateAlpha(realp2, realp1, { x1,j }, vertex2, vertex1);
//				colorz2 = Utils::interpolateAlpha(realp3, realp1, { x2,j }, vertex3, vertex1);
//			}
//			z1 = Utils::interpolate(realp2, realp1, { x1,j });
//			z2 = Utils::interpolate(realp3, realp1, { x2,j });
//		}
//		if (x1 > x2)
//		{
//			swap(x1, x2);
//			swap(z1, z2);
//			swap(colorx1, colorx2);
//			if (PS)
//			{
//				swap(colorz1, colorz2);
//			}
//		}
//		for (int x = x1; x < x2; x++)
//		{
//			if (GS||PS)
//			{
//				updatedColor = Utils::interpolateAlpha({ x1,j,z1 }, { x2,j,z2 }, { x,j }, colorx1, colorx2);
//			}
//			float z = Utils::interpolate({ x1,j,z1 }, { x2,j,z2 }, { x,j });
//			if (PS)
//			{
//				glm::vec3 col = { 0,0,0 };
//				glm::vec3 point = Utils::interpolateAlpha({ x1,j,z1 }, { x2,j,z2 }, { x,j }, colorz1, colorz2);
//				if (activeLight.getAmbientLighting())
//					col += color * activeLight.getAmbientLightingVector();
//
//				if (activeLight.getDiffuseLighting())
//					col += model.getModelMaterial(1) * DiffuseLighting(scene, { point,updatedColor }, lightPosition) * activeLight.getDiffuseLightingVector(); //Diffuse Lighting
//
//				if (activeLight.getSpecularLighting())
//					col += model.getModelMaterial(2) * SpecularLighting(scene, { point,updatedColor },lightPosition, scene.GetActiveCamera().getCameraPos(), activeLight.getShiniesCoefficent()) * activeLight.getSpecularLightingVector();
//
//				updatedColor = col;
//			}
//			if (z > 0.f && j * viewport_width + x < viewport_height * viewport_width && j * viewport_width + x > 0 && depth_buffer[j * viewport_width + x] > z)
//			{
//				depth_buffer[j * viewport_width + x] = z;
//				if (GetZbufferColor())
//				{
//					updatedColor = glm::vec3(1.f - z);
//				}
//				PutPixel(x, j,  updatedColor);
//			}
//		}
//	}
//}
//
//std::pair<glm::vec3, glm::vec3> Renderer::GetFaceNormal(const Scene& scene, const int& faceIndex)
//{
//	MeshModel Active = scene.GetActiveModel();
//	Camera ActiveCam = scene.GetActiveCamera();
//	vector<glm::vec2> minMax = Active.getBounding();
//
//	float max = -1000;
//	for (int i = 0; i < 3; i++)
//	{
//		if (minMax[i][1] > max)
//			max = minMax[i][1];
//	}
//		glm::vec3 p1 = Active.getVertexIndex((Active.GetFace(faceIndex).GetVertexIndex(0)) - 1),
//			p2 = Active.getVertexIndex((Active.GetFace(faceIndex).GetVertexIndex(1)) - 1),
//			p3 = Active.getVertexIndex((Active.GetFace(faceIndex).GetVertexIndex(2)) - 1);
//		glm::vec3 normal = glm::normalize(glm::cross(p2 - p1, p3 - p1));
//		glm::vec3 middleVertex = (p1 + p2 + p3) / 3.f;
//		normal *= 1 / max;
//		normal = normal + middleVertex;
//		glm::vec3 realp1 = Utils::Vec3FromHomVec4(ActiveCam.GetProjectionTransformation() * ActiveCam.GetViewTransformation() * Active.GetTrans() * Utils::HomVec4FromVec3(normal))
//			, realp2 = Utils::Vec3FromHomVec4(ActiveCam.GetProjectionTransformation() * ActiveCam.GetViewTransformation() * Active.GetTrans() * Utils::HomVec4FromVec3(middleVertex));
//		return { realp2,realp1 };
//}
//
//void Renderer::DrawModelAxis(const Scene& scene)
//{
//	MeshModel Active = scene.GetActiveModel();
//	Camera ActiveCam = scene.GetActiveCamera();
//	shared_ptr<MeshModel> Axis = scene.GetAxis();
//	vector<glm::vec2> minMax = Active.getBounding();
//	if (Axis != NULL)
//	{
//		MeshModel demo = *Axis;
//		int faceNum = demo.GetFacesCount();
//		demo.localScale((minMax[0][1]*1.5f), (minMax[1][1] * 1.5f), (minMax[1][1] * 1.5f), 1);
//		for (int i = 0; i < faceNum; i++)
//		{
//			glm::vec4 p1 = Utils::HomVec4FromVec3(demo.getVertexIndex((demo.GetFace(i).GetVertexIndex(0)) - 1)),
//				p2 = Utils::HomVec4FromVec3(demo.getVertexIndex((demo.GetFace(i).GetVertexIndex(1)) - 1)),
//				p3 = Utils::HomVec4FromVec3(demo.getVertexIndex((demo.GetFace(i).GetVertexIndex(2)) - 1));
//			p1 = ActiveCam.GetProjectionTransformation() * ActiveCam.GetViewTransformation() *Active.GetWorldTransformation()* demo.getLocalTransformation() * p1;
//			p2 = ActiveCam.GetProjectionTransformation() * ActiveCam.GetViewTransformation() *Active.GetWorldTransformation() * demo.getLocalTransformation() * p2;
//			p3 = ActiveCam.GetProjectionTransformation() * ActiveCam.GetViewTransformation() *Active.GetWorldTransformation() * demo.getLocalTransformation() * p3;
//			glm::vec3 realp1 = GetViewportTrans(p1), realp2 = GetViewportTrans(p2), realp3 = GetViewportTrans(p3);
//			DrawLine({ realp1 }, { realp2 }, { 1,0,0 });
//			DrawLine({ realp2 }, { realp3 }, { 1,0,0 });
//			DrawLine({ realp3 }, { realp1 }, { 1,0,0 });
//		}
//	}
//}
//
//void Renderer::DrawFaceNormals(const Scene& scene)
//{
//	MeshModel Active = scene.GetActiveModel();
//	Camera ActiveCam = scene.GetActiveCamera();
//	vector<glm::vec2> minMax = Active.getBounding();
//	float max = -1000;
//	for (int i = 0; i < 3; i++)
//	{
//		if (minMax[i][1] > max)
//			max = minMax[i][1];
//	}
//
//	int faceNum = Active.GetFacesCount();
//	for (int i = 0; i < faceNum; i++)
//	{	
//			glm::vec3 p1 = Active.getVertexIndex((Active.GetFace(i).GetVertexIndex(0)) - 1),
//				p2 = Active.getVertexIndex((Active.GetFace(i).GetVertexIndex(1)) - 1),
//				p3 = Active.getVertexIndex((Active.GetFace(i).GetVertexIndex(2)) - 1);
//			glm::vec3 normal = glm::normalize(glm::cross(p2 - p1, p3 - p1));
//			glm::vec3 middleVertex = (p1 + p2 + p3) / 3.f;
//			normal *= 1 / max;
//			normal = normal + middleVertex;
//
//			glm::vec3 realp1 = GetViewportTrans(ActiveCam.GetProjectionTransformation() * ActiveCam.GetViewTransformation() * Active.GetTrans() * Utils::HomVec4FromVec3(normal))
//				, realp2 = GetViewportTrans(ActiveCam.GetProjectionTransformation() * ActiveCam.GetViewTransformation() * Active.GetTrans() * Utils::HomVec4FromVec3(middleVertex));
//			DrawLine({ realp1 }, { realp2 }, { 1,0,1 });
//	}
//}
//
//
//void Renderer::DrawDirectionsOfSpectralReflection(const Scene& scene)
//{
//	MeshModel Active = scene.GetActiveModel();
//	Camera ActiveCam = scene.GetActiveCamera();
//	Lighting ActiveLight = scene.GetActiveLight();
//	vector<glm::vec2> minMax = Active.getBounding();
//	float max = -1000;
//	for (int i = 0; i < 3; i++)
//	{
//		if (minMax[i][1] > max)
//			max = minMax[i][1];
//	}
//
//	int faceNum = Active.GetFacesCount();
//	for (int i = 0; i < faceNum; i++)
//	{
//		std::pair<glm::vec3, glm::vec3> faceNormal = GetFaceNormal(scene, i);
//		glm::vec3 p1 = Active.getVertexIndex((Active.GetFace(i).GetVertexIndex(0)) - 1),
//			p2 = Active.getVertexIndex((Active.GetFace(i).GetVertexIndex(1)) - 1),
//			p3 = Active.getVertexIndex((Active.GetFace(i).GetVertexIndex(2)) - 1);
//		glm::vec3 normal = glm::reflect(glm::normalize(ActiveLight.getLightingPosition() - faceNormal.first), glm::normalize(faceNormal.second - faceNormal.first));
//		glm::vec3 middleVertex = (p1 + p2 + p3) / 3.f;
//		normal = glm::normalize(normal);
//		normal = normal + middleVertex;
//
//
//		glm::vec3 realp1 = GetViewportTrans(ActiveCam.GetProjectionTransformation() * ActiveCam.GetViewTransformation() * Active.GetTrans() * Utils::HomVec4FromVec3(normal))
//			, realp2 = GetViewportTrans(ActiveCam.GetProjectionTransformation() * ActiveCam.GetViewTransformation() * Active.GetTrans() * Utils::HomVec4FromVec3(middleVertex))
//			, realp3 = GetViewportTrans(ActiveCam.GetProjectionTransformation() * ActiveCam.GetViewTransformation() * Utils::HomVec4FromVec3(ActiveLight.getLightingPosition()));
//		if (normal[2] < depth_buffer[(int)normal[1] * GetViewportWidth() + (int)normal[0]]) {
//			DrawLine({ realp1 }, { realp2 }, { 1,0,1 });
//			DrawLine({ realp2 }, { realp3 }, { 1,1,1 });
//		}
//	}
//}
//
//std::vector<std::pair<glm::vec3, glm::vec3>> Renderer::GetVertexNormalsForFace(const Scene& scene, const int& faceIndex)
//{
//
//	MeshModel Active = scene.GetActiveModel();
//	Camera ActiveCam = scene.GetActiveCamera();
//	vector<glm::vec2> minMax = Active.getBounding();
//	std::vector<std::pair<glm::vec3, glm::vec3>> toRet;
//	float max = -1000;
//	for (int i = 0; i < 3; i++)
//	{
//		if (minMax[i][1] > max)
//			max = minMax[i][1];
//	}
//		for (int j = 0; j < 3; j++)
//		{
//
//			glm::vec4 p1 = Utils::HomVec4FromVec3(Active.getVertexIndex((Active.GetFace(faceIndex).GetVertexIndex(j)) - 1));
//			glm::vec4 p2 = Utils::HomVec4FromVec3(Active.GetNormOfVer()[(Active.GetFace(faceIndex).GetVertexIndex(j)) - 1]);
//
//			p2 *= 1 / max;
//			p2 = p2 + p1;
//			p2[3] = 1;
//
//			glm::vec3  realp1 = Utils::Vec3FromHomVec4(ActiveCam.GetProjectionTransformation() * ActiveCam.GetViewTransformation() * Active.GetTrans() * p1);
//			glm::vec3 realp2 = Utils::Vec3FromHomVec4(ActiveCam.GetProjectionTransformation() * ActiveCam.GetViewTransformation() * Active.GetTrans() * p2);
//			toRet.push_back({ realp1, realp2 });
//		}
//
//	return toRet;
//}
//
//std::vector<std::pair<glm::vec3, glm::vec3>> Renderer::GetVertexNormalsForFacePhong(const Scene& scene, const int& faceIndex)
//{
//	MeshModel Active = scene.GetActiveModel();
//	Camera ActiveCam = scene.GetActiveCamera();
//	vector<glm::vec2> minMax = Active.getBounding();
//	std::vector<std::pair<glm::vec3, glm::vec3>> toRet;
//	float max = -1000;
//	for (int i = 0; i < 3; i++)
//	{
//		if (minMax[i][1] > max)
//			max = minMax[i][1];
//	}
//	for (int j = 0; j < 3; j++)
//	{
//
//		glm::vec4 p1 = Utils::HomVec4FromVec3(Active.getVertexIndex((Active.GetFace(faceIndex).GetVertexIndex(j)) - 1));
//		glm::vec4 p2 = Utils::HomVec4FromVec3(Active.GetNormOfVer()[(Active.GetFace(faceIndex).GetVertexIndex(j)) - 1]);
//
//
//		p2 *= 1 / max;
//		p2 = p2 + p1;
//		p2[3] = 1;
//
//		 glm::vec3 realp1 = Utils::Vec3FromHomVec4(ActiveCam.GetProjectionTransformation() * ActiveCam.GetViewTransformation() * Active.GetTrans() * p1);
//		 glm::vec3 realp2 = Utils::Vec3FromHomVec4(ActiveCam.GetProjectionTransformation() * ActiveCam.GetViewTransformation() * Active.GetTrans() * p2);
//		toRet.push_back({ realp1, realp2 });
//	}
//
//	return toRet;
//}
//
//
//void Renderer::DrawVertexNormals(const Scene& scene)
//{
//	MeshModel Active = scene.GetActiveModel();
//	Camera ActiveCam = scene.GetActiveCamera();
//	vector<glm::vec2> minMax = Active.getBounding();
//	float max = -1000;
//	for (int i = 0; i < 3; i++)
//	{
//		if (minMax[i][1] > max)
//			max = minMax[i][1];
//	}
//	int faceNum = Active.GetFacesCount();
//	for (int i = 0; i < faceNum; i++)
//	{
//		for (int j = 0; j < 3; j++)
//		{
//			glm::vec4 p1 = Utils::HomVec4FromVec3(Active.getVertexIndex((Active.GetFace(i).GetVertexIndex(j)) - 1)),
//				p2 = Utils::HomVec4FromVec3(Active.GetNormalIndex((Active.GetFace(i).GetNormalIndex(j)) - 1)) ;
//			p2 *= 1/max;
//			p2 = p2 + p1;
//			p2[3] = 1;
//				
//			p1 = ActiveCam.GetProjectionTransformation() * ActiveCam.GetViewTransformation() * Active.GetTrans() * p1;
//			p2 = ActiveCam.GetProjectionTransformation() * ActiveCam.GetViewTransformation() * Active.GetTrans() * p2;
//			glm::vec3 realp1 = GetViewportTrans(p1), realp2 = GetViewportTrans(p2);
//			DrawLine({ realp1 }, { realp2 }, { 0,1,0 });
//		}
//	}
//}
//
//
//void Renderer::DrawWorld(const Scene& scene)
//{
//	Camera ActiveCam = scene.GetActiveCamera();
//	glm::vec4 XaxesFirst = Utils::HomVec4FromVec3(glm::vec3({ -200,0,0 })),
//		XaxesSecond = Utils::HomVec4FromVec3(glm::vec3({ 200,0,0 })),
//		YaxesFirst = Utils::HomVec4FromVec3(glm::vec3({ 0,-200,0 })),
//		YaxesSecond = Utils::HomVec4FromVec3(glm::vec3({ 0,200,0 })),
//		ZaxesFirst = Utils::HomVec4FromVec3(glm::vec3({ 0,0,-200 })),
//		ZaxesSecond = Utils::HomVec4FromVec3(glm::vec3({ 0,0,200 }));
//		glm::vec3 Xfirst = GetViewportTrans(ActiveCam.GetProjectionTransformation() * ActiveCam.GetViewTransformation() * XaxesFirst);
//		glm::vec3 Xsecond = GetViewportTrans(ActiveCam.GetProjectionTransformation() * ActiveCam.GetViewTransformation() * XaxesSecond);
//		glm::vec3 Yfirst = GetViewportTrans(ActiveCam.GetProjectionTransformation() * ActiveCam.GetViewTransformation() * YaxesFirst);
//		glm::vec3 Ysecond = GetViewportTrans(ActiveCam.GetProjectionTransformation() * ActiveCam.GetViewTransformation() * YaxesSecond);
//		glm::vec3 Zfirst = GetViewportTrans(ActiveCam.GetProjectionTransformation() * ActiveCam.GetViewTransformation() * ZaxesFirst);
//		glm::vec3 Zsecond = GetViewportTrans(ActiveCam.GetProjectionTransformation() * ActiveCam.GetViewTransformation() * ZaxesSecond);
//		 
//		DrawLine({ Xfirst }, { Xsecond }, { 0,0,0 });
//		DrawLine({ Yfirst }, { Ysecond }, { 0,0,0 });
//		DrawLine({ Zfirst }, { Zsecond }, { 0,0,0 });
//}
//
//void Renderer::DrawBoundingBox(const Scene& scene)
//{
//	MeshModel ActiveModel = scene.GetActiveModel();
//	Camera ActiveCam = scene.GetActiveCamera();
//	std::vector<glm::vec2> limits = ActiveModel.getBounding();
//	vector<glm::vec4> points = // the first point in each line is an origin while the others are the points we need to draw lines with
//	{
//		{limits[0][0],limits[1][0],limits[2][1],1},{limits[0][0],limits[1][1],limits[2][1],1},{limits[0][0],limits[1][0],limits[2][0],1},{limits[0][1],limits[1][0],limits[2][1],1},
//		{limits[0][1],limits[1][1],limits[2][1],1},{limits[0][1],limits[1][1],limits[2][0],1},{limits[0][1],limits[1][0],limits[2][1],1},{limits[0][0],limits[1][1],limits[2][1],1},
//		{limits[0][0],limits[1][1],limits[2][0],1},{limits[0][0],limits[1][0],limits[2][0],1},{limits[0][0],limits[1][1],limits[2][1],1},{limits[0][1],limits[1][1],limits[2][0],1},
//		{limits[0][1],limits[1][0],limits[2][0],1},{limits[0][0],limits[1][0],limits[2][0],1},{limits[0][1],limits[1][1],limits[2][0],1},{limits[0][1],limits[1][0],limits[2][1],1}
//	};
//	for (int i = 0; i < 16; i++)
//	{
//		points[i] = ActiveCam.GetProjectionTransformation() * ActiveCam.GetViewTransformation() * ActiveModel.GetTrans() * points[i];
//	}
//	std::vector<glm::vec3> origins = { GetViewportTrans(points[0]),  GetViewportTrans(points[4]) , GetViewportTrans(points[8]) ,GetViewportTrans(points[12]) };
//	for (int i = 0; i < 4; i++)
//	{
//		for (int j = 1; j < 4; j++)
//		{
//			int place = j + (4 * i);
//			glm::vec3 temp = GetViewportTrans(points[place]);
//			DrawLine({ origins[i] }, { temp }, { 1, 1, 1 });
//		}
//	}
//}
//
//void Renderer::DrawFaceBoundingBox(const Scene& scene, const int FaceIndex)
//{
//	MeshModel ActiveModel = scene.GetActiveModel();
//	Camera ActiveCam = scene.GetActiveCamera();
//	std::vector<glm::vec2> limits = ActiveModel.getFaceBounding(FaceIndex);
//	vector<glm::vec4> points = // the first point in each line is an origin while the others are the points we need to draw lines with
//	{
//		{limits[0][0],limits[1][0],limits[2][1],1},{limits[0][0],limits[1][1],limits[2][1],1},{limits[0][0],limits[1][0],limits[2][0],1},{limits[0][1],limits[1][0],limits[2][1],1},
//		{limits[0][1],limits[1][1],limits[2][1],1},{limits[0][1],limits[1][1],limits[2][0],1},{limits[0][1],limits[1][0],limits[2][1],1},{limits[0][0],limits[1][1],limits[2][1],1},
//		{limits[0][0],limits[1][1],limits[2][0],1},{limits[0][0],limits[1][0],limits[2][0],1},{limits[0][0],limits[1][1],limits[2][1],1},{limits[0][1],limits[1][1],limits[2][0],1},
//		{limits[0][1],limits[1][0],limits[2][0],1},{limits[0][0],limits[1][0],limits[2][0],1},{limits[0][1],limits[1][1],limits[2][0],1},{limits[0][1],limits[1][0],limits[2][1],1}
//	};
//	for (int i = 0; i < 16; i++)
//	{
//		points[i] = ActiveCam.GetProjectionTransformation() * ActiveCam.GetViewTransformation() * ActiveModel.GetTrans() * points[i];
//	}
//	std::vector<glm::vec3> origins = { GetViewportTrans(points[0]),  GetViewportTrans(points[4]) , GetViewportTrans(points[8]) ,GetViewportTrans(points[12]) };
//	for (int i = 0; i < 4; i++)
//	{
//		for (int j = 1; j < 4; j++)
//		{
//			int place = j + (4 * i);
//			glm::vec3 temp = GetViewportTrans(points[place]);
//			DrawLine({ origins[i] }, { temp }, { 1, 1, 1 });
//		}
//	}
//}
//
//glm::vec3 Renderer::GetViewportTrans(glm::vec4 inputVertex)
//{
//	glm::vec3 vertex = Utils::Vec3FromHomVec4(inputVertex);
//	vertex = { (vertex[0] + 1.f) * ((float)GetViewportWidth()) / 2.f,(vertex[1] + 1.f) * ((float)GetViewportHeight()) / 2.f,(vertex[2]+1.f)/2.f };
//	return vertex;
//}
//
//void Renderer::SetNewViewport(int newWidth,int newHeight)
//{
//	viewport_width = newWidth;
//	viewport_height = newHeight;
//	delete color_buffer;
//	delete depth_buffer;
//	delete color_ScreenShotBuffer;
//	CreateBuffers(newWidth, newHeight); 
//}
//
//
//int Renderer::GetViewportWidth() const
//{
//	return viewport_width;
//}
//
//int Renderer::GetViewportHeight() const
//{
//	return viewport_height;
//}
//
//void Renderer::SetZbufferColor(bool setter)
//{
//	ZcolorBuffer = setter;
//}
//
//bool Renderer::GetZbufferColor() const
//{
//	return ZcolorBuffer;
//}


