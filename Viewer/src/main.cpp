#define _USE_MATH_DEFINES
#include <iostream>
#include <map>
#include <cmath>
#include <imgui/imgui.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nfd.h>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Renderer.h"
#include "Scene.h"
#include "Utils.h"
#include "MeshModel.h"


/**
 * Fields
 */
bool screenShot = false;
bool Blur = false;
bool Bloom = false;
bool orthoSet = false;
bool active_camera_settings = false;
bool active_lighting_settings = false;
bool active_feature_settings = false;
bool drawWorld = false;
bool drawBoundingBox = false;
bool drawVertexNormals = false;
bool triangleColor = false;
bool paintModel = false;
bool drawFaceNormals = false;
bool drawFaceBoundingBox = false;
bool show_demo_window = false;
bool show_another_window = false;
bool localButtonPressed = false;
bool worldButtonPressed = false;
bool scaleLButtonPressed = false;
bool scaleWButtonPressed = false;
bool rotationLButtonPressed = false;
bool translateLButtonPressed = false;
bool rotationWButtonPressed = false;
bool translateWButtonPressed = false;
bool scaleUniformButtonPressed = false;
bool scaleSpecifedButtonPressed = false;
bool eyePressed = false;
bool atPressed = false;
bool upPressed = false;
bool LookAtPressed = false;
bool nearFarPressed = false;
bool PerspectivePressed = false;
bool leftRightPressed = false;
bool upDownPressed = false;
bool zColor = false;
bool ambient = false;
bool diffuse = false;
bool specular = false;
bool GShading = false;
bool AmbienLighting = false;
bool DiffuseLighting = false;
int kernelSize = 3;
int blurIntensity = 3;
float bloom_threshhold = 0.7;
bool SpecularLighting = false;
bool PShading = false;
bool PReflection = false;
bool directionsOfSpecular = false;
bool directionalSource;
bool Toon = false;

int ScaleUniformFlag = 3;
glm::vec4 clear_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f); 
// map of the transformations
std::map<int, float>		  LocalScaleX,
LocalScaleY,
LocalScaleZ,
worldScaleX,
worldScaleY,
worldScaleZ,
LocRotX,
LocRotY,
LocRotZ,
WorRotX,
WorRotY,
WorRotZ,
LocalUniformScale,
WorldUniformScale,
LocTransX,
LocTransY,
LocTransZ,
WorTransX,
WorTransY,
WorTransZ,
CameraPosX,
CameraPosY,
CameraPosZ,
CameraLooksAtX,
CameraLooksAtY,
CameraLooksAtZ,
CamerasAboveX,
CamerasAboveY,
CamerasAboveZ,
NearDist,
FarDist,
Left,
Right,
Up, Down, Fovy,
ambLightingR, ambLightingG, ambLightingB,
lightPosX, lightPosY, lightPosZ,
//eyePosX, eyePosY, eyePosZ,
DifLightColorR, DifLightColorB, DifLightColorG,
SpecLightColorR, SpecLightColorB, SpecLightColorG,
modelMaterialR, modelMaterialG, modelMaterialB,
modeldiffMaterialR, modeldiffMaterialG, modeldiffMaterialB,
modelspectMaterialR, modelspectMaterialG, modelspectMaterialB,
shiniesCoefficent;
float Aspect;
int main(int argc, char** argv);

/**
 * Function declarations
 */
static void GlfwErrorCallback(int error, const char* description);
GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name);
ImGuiIO& SetupDearImgui(GLFWwindow* window);
void StartFrame();
void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io);
void Cleanup(GLFWwindow* window);
void DrawImguiMenus(ImGuiIO& io, Scene& scene);
void initCameraParameters(int ActiveCamera);
void initLightingParameters(int ActiveLighting);
void initParamaters(int ActiveModel);

/**
 * Function implementation
 */
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	// TODO: Handle mouse scroll here
}

int main(int argc, char** argv)
{
	int windowWidth = 1920, windowHeight = 1080;
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, "Mesh Viewer");
	if (!window)
		return 1;

	glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
	glEnable(GL_DEPTH_TEST);

	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	Aspect = (float)windowWidth/(float)windowHeight;
	Scene scene = Scene();
	scene.AddCamera(std::make_shared<Camera>());
		scene.SetActiveCameraIndex(0);
		initCameraParameters(0);
	scene.AddLighting(std::make_shared<Lighting>());
		scene.setActiveLightingIndex(0);
		initLightingParameters(0);

	ImGuiIO& io = SetupDearImgui(window); 
	glfwSetScrollCallback(window, ScrollCallback);
	Renderer renderer = Renderer();
	renderer.LoadShaders();
	renderer.LoadTextures();

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		StartFrame();
		DrawImguiMenus(io, scene);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		RenderFrame(window, scene, renderer, io);
	}

	Cleanup(window);
	return 0;
}

static void GlfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name)
{
	glfwSetErrorCallback(GlfwErrorCallback);
	if (!glfwInit())
		return NULL;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	

	GLFWwindow* window = glfwCreateWindow(w, h, window_name, NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync
						 // very importent!! initialization of glad
						 // https://stackoverflow.com/questions/48582444/imgui-with-the-glad-opengl-loader-throws-segmentation-fault-core-dumped

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	return window;
}

ImGuiIO& SetupDearImgui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();
	return io;
}

void StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io)
{
	ImGui::Render();
	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	int activeModel=0;
	if(scene.GetModelCount()>=1)
		activeModel = scene.GetActiveModelIndex();
	int activeCamera = scene.GetActiveCameraIndex();
	int activeLight = scene.GetActiveLightingIndex();

		Aspect = (float)frameBufferWidth/(float)frameBufferHeight;
	

	//if (frameBufferWidth != renderer.GetViewportWidth() || frameBufferHeight != renderer.GetViewportHeight())
	//{
	//	renderer.SetNewViewport(frameBufferWidth, frameBufferHeight); 
	//}

	if (!io.WantCaptureKeyboard)
	{
		if (scene.GetModelCount() >= 1) {// WASD(wasd) control of local rotations and arrows for local translate (not supported for all keyboards)
			
				if ((io.KeysDown[119] || io.KeysDown[87]) && LocRotX[activeModel] > -2.0f)
				{
					LocRotX[activeModel] = LocRotX[activeModel] - 0.05f;
				}
				if ((io.KeysDown[115] || io.KeysDown[83]) && LocRotX[activeModel] < 2.0f)
				{
					LocRotX[activeModel] = LocRotX[activeModel] + 0.05f;
				}
				if (LocRotY[activeModel] < 2.0f && (io.KeysDown[100] || io.KeysDown[68]))
				{
					LocRotY[activeModel] = LocRotY[activeModel] + 0.05f;
				}
				if (LocRotY[activeModel] > -2.0f  && (io.KeysDown[97] || io.KeysDown[65]))
				{
					LocRotY[activeModel] = LocRotY[activeModel] - 0.05f;
				}
			
			
			
				if (io.KeysDown[VK_UP])
				{
					WorTransY[activeModel] = WorTransY[activeModel] + 5;
				}
				if (io.KeysDown[VK_DOWN])
				{
					WorTransY[activeModel] = WorTransY[activeModel] - 5;
				}
				if (io.KeysDown[VK_LEFT])
				{
					WorTransX[activeModel] = WorTransX[activeModel] - 5;
				}
				if (io.KeysDown[VK_RIGHT])
				{
					WorTransX[activeModel] = WorTransX[activeModel] + 5;
				}
		}
	}
	if (!io.WantCaptureMouse)
	{

		// TODO: Handle mouse events here
		if (scene.GetModelCount() >= 1 && io.MouseDown[0]) { 
		    {
					if (CameraLooksAtX[activeCamera] < 10.0f && io.MouseDelta[0] > 0)
						CameraLooksAtX[activeCamera] += 0.05f;
					if (CameraLooksAtX[activeCamera] > -10.0f && io.MouseDelta[0] < 0)
						CameraLooksAtX[activeCamera] -= 0.05f;
					if (CameraLooksAtY[activeCamera] < 10.0f && io.MouseDelta[1] < 0)
						CameraLooksAtY[activeCamera] += 0.05f;
					if (CameraLooksAtY[activeCamera] > -10.0f && io.MouseDelta[1] > 0)
						CameraLooksAtY[activeCamera] -= 0.05f;
					if (CameraLooksAtZ[activeCamera] < 10.0f && io.MouseWheel > 0)
						CameraLooksAtZ[activeCamera] += 0.05f;
					if (CameraLooksAtZ[activeCamera] > -10.0f && io.MouseWheel < 0)
						CameraLooksAtZ[activeCamera] -= 0.05f;
				
			}
			if(io.KeyShift)
			{
				if (io.MouseDown[0]&& LocalUniformScale[activeModel] <= 10.f)
					LocalUniformScale[activeModel] = LocalUniformScale[activeModel] + 5;// left mouse button for zoom in
				if (io.MouseDown[1]&& LocalUniformScale[activeModel] >= 0.f)
					LocalUniformScale[activeModel] = LocalUniformScale[activeModel] - 5;// right mouse button for zoom in
			}
		
		}
		}
	//if (!screenShot)
	//{
	//	renderer.ClearColorBuffer(clear_color);
	//}
	int a = glGetError();
	renderer.Render(scene);
	////renderer.SwapBuffers();
	//
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwMakeContextCurrent(window);
	glfwSwapBuffers(window);
}

void Cleanup(GLFWwindow* window)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

void initLightingParameters(int ActiveLighting)
{
	shiniesCoefficent.insert(std::pair<int, int>(ActiveLighting, 1));
	ambLightingR.insert(std::pair<int, float>(ActiveLighting, 0.f));
	ambLightingG.insert(std::pair<int, float>(ActiveLighting, 0.f));
	ambLightingB.insert(std::pair<int, float>(ActiveLighting, 0.f));
	DifLightColorR.insert(std::pair<int, float>(ActiveLighting, 0.5f));
	DifLightColorG.insert(std::pair<int, float>(ActiveLighting, 0.5f));
	DifLightColorB.insert(std::pair<int, float>(ActiveLighting, 0.5f));
	SpecLightColorR.insert(std::pair<int, float>(ActiveLighting, 0.3f));
	SpecLightColorG.insert(std::pair<int, float>(ActiveLighting, 0.3f));
	SpecLightColorB.insert(std::pair<int, float>(ActiveLighting, 0.3f));
	lightPosX.insert(std::pair<int, int>(ActiveLighting, 0));
	lightPosY.insert(std::pair<int, int>(ActiveLighting, 0));
	lightPosZ.insert(std::pair<int, int>(ActiveLighting, 10));
	//eyePosX.insert(std::pair<int, int>(ActiveLighting, 0));
	//eyePosY.insert(std::pair<int, int>(ActiveLighting, 0));
	//eyePosZ.insert(std::pair<int, int>(ActiveLighting, 10));
}

void initCameraParameters(int ActiveCamera)
{
	CameraPosX.insert(std::pair<int, int>(ActiveCamera, 0));
	CameraPosY.insert(std::pair<int, int>(ActiveCamera, 0));
	CameraPosZ.insert(std::pair<int, int>(ActiveCamera, 10));
	CameraLooksAtX.insert(std::pair<int, int>(ActiveCamera, 0));
	CameraLooksAtY.insert(std::pair<int, int>(ActiveCamera, 0));
	CameraLooksAtZ.insert(std::pair<int, int>(ActiveCamera, 0));
	CamerasAboveX.insert(std::pair<int, int>(ActiveCamera, 0));
	CamerasAboveY.insert(std::pair<int, int>(ActiveCamera, 1));
	CamerasAboveZ.insert(std::pair<int, int>(ActiveCamera, 0));
	NearDist.insert(std::pair<int, float>(ActiveCamera,1));
	FarDist.insert(std::pair<int, float>(ActiveCamera, 1000));
	Left.insert(std::pair<int, float>(ActiveCamera, -5));
	Right.insert(std::pair<int, float>(ActiveCamera, 5));
	Up.insert(std::pair<int, float>(ActiveCamera, 5));
	Down.insert(std::pair<int, float>(ActiveCamera, -5));
	Fovy.insert(std::pair<int, float>(ActiveCamera, 0.8));
}

void initParamaters(int ActiveModel)
{
	
	          LocRotX.insert(std::pair<int,float> (ActiveModel,0));
	          LocRotY.insert(std::pair<int,float> (ActiveModel,0));
	          LocRotZ.insert(std::pair<int,float> (ActiveModel,0));
	        LocTransX.insert(std::pair<int,int> (ActiveModel,0));
	        LocTransY.insert(std::pair<int,int> (ActiveModel,0));
	        LocTransZ.insert(std::pair<int,int> (ActiveModel,0));
	      worldScaleX.insert(std::pair<int,float> (ActiveModel,0));
		  worldScaleY.insert(std::pair<int,float> (ActiveModel,0));
	      worldScaleZ.insert(std::pair<int,float> (ActiveModel,0));
	WorldUniformScale.insert(std::pair<int,float> (ActiveModel,1));
	LocalUniformScale.insert(std::pair<int,float> (ActiveModel,1));
	      LocalScaleX.insert(std::pair<int,float> (ActiveModel,0));
	      LocalScaleY.insert(std::pair<int,float> (ActiveModel,0));
	      LocalScaleZ.insert(std::pair<int,float> (ActiveModel,0));
	          WorRotX.insert(std::pair<int,float> (ActiveModel,0));
	          WorRotY.insert(std::pair<int,float> (ActiveModel,0));
	          WorRotZ.insert(std::pair<int,float> (ActiveModel,0));
	        WorTransX.insert(std::pair<int,int> (ActiveModel,0));
	        WorTransY.insert(std::pair<int,int> (ActiveModel,0));
	        WorTransZ.insert(std::pair<int,int> (ActiveModel,0));	
			modelMaterialR.insert(std::pair<int, float>(ActiveModel, 0.5));
			modelMaterialG.insert(std::pair<int, float>(ActiveModel, 0.5));
			modelMaterialB.insert(std::pair<int, float>(ActiveModel, 0.5));
			modeldiffMaterialR.insert(std::pair<int, float>(ActiveModel, 0.5));
			modeldiffMaterialG.insert(std::pair<int, float>(ActiveModel, 0.5));
			modeldiffMaterialB.insert(std::pair<int, float>(ActiveModel, 0.5));
			modelspectMaterialR.insert(std::pair<int, float>(ActiveModel, 0.5));
			modelspectMaterialG.insert(std::pair<int, float>(ActiveModel, 0.5));
			modelspectMaterialB.insert(std::pair<int, float>(ActiveModel, 0.5));
}

void DrawImguiMenus(ImGuiIO& io, Scene& scene)
{
	/**
	 * MeshViewer menu
	 */
	ImGui::Begin("MeshViewer Menu");

	// Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open", "CTRL+O"))
			{
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("obj;", NULL, &outPath);
				if (result == NFD_OKAY)
				{
					scene.AddModel(Utils::LoadMeshModel(outPath));
					free(outPath);
					if(scene.GetModelCount()==1){
					scene.SetActiveModelIndex(0);
					initParamaters(0);
					}
					
				}
				else if (result == NFD_CANCEL)
				{
				}
				else
				{
				}

			}
			if (ImGui::MenuItem("Add Axis"))
			{
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("obj;", NULL, &outPath);
				if (result == NFD_OKAY)
				{
					scene.LoadAxis(Utils::LoadMeshModel(outPath));
					free(outPath);

				}
				else if (result == NFD_CANCEL)
				{
				}
				else
				{
				}

			}
			if (ImGui::MenuItem("delete Axis"))
			{
				scene.DeleteAxis();

			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Camera"))
		{
			if (ImGui::MenuItem("Add Camera"))
			{
				scene.AddCamera(std::make_shared<Camera>());
			}
			
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Light"))
		{
			if (ImGui::MenuItem("Add Light"))
			{
				scene.AddLighting(std::make_shared<Lighting>());
			}

			ImGui::EndMenu();
		}

		// TODO: Add more menubar items (if you want to)
		ImGui::EndMainMenuBar();
	}

	// Controls
	ImGui::ColorEdit3("Clear Color", (float*)&clear_color);
	// TODO: Add more controls as needed

	ImGui::End();

	/**
	 * Imgui demo - you can remove it once you are familiar with imgui
	 */

	 // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);
		int activeLighting = scene.GetActiveLightingIndex();
		int activeCamera = scene.GetActiveCameraIndex();
		Camera& camera = scene.GetActiveCamera();
	if (active_camera_settings)
	{
		ImGui::Begin("Camera Menu");
		if (ImGui::Button("Look At", ImVec2((ImGui::GetWindowWidth() / 2), 20)) || LookAtPressed) {
			LookAtPressed = true; ImGui::SameLine();
			if (ImGui::Button("X"))
				LookAtPressed = false;
			else {
				if (ImGui::Button("Eye", ImVec2((ImGui::GetWindowWidth() / 3) - 20, 20)) || eyePressed) {
					eyePressed = true; ImGui::SameLine();
					if (ImGui::Button("x"))
						eyePressed = false;
					else {
						ImGui::SliderFloat("Eye X: ", &CameraPosX[activeCamera], -30.f, 30.f);
						ImGui::SliderFloat("Eye Y: ", &CameraPosY[activeCamera], -30.f, 30.f);
						ImGui::SliderFloat("Eye Z: ", &CameraPosZ[activeCamera], -30.f, 30.f);
					}															 
				}
					ImGui::SameLine();
					if (ImGui::Button("At", ImVec2((ImGui::GetWindowWidth() / 3) - 20, 20)) || atPressed) {
						atPressed = true; ImGui::SameLine();
						if (ImGui::Button("x"))
							atPressed = false;
						else {
							ImGui::SliderFloat("At X: ", &CameraLooksAtX[activeCamera], -30.f, 30.f);
							ImGui::SliderFloat("At Y: ", &CameraLooksAtY[activeCamera], -30.f, 30.f);
							ImGui::SliderFloat("At Z: ", &CameraLooksAtZ[activeCamera], -30.f, 30.f);
						}
					}
						ImGui::SameLine();
						if (ImGui::Button("Up", ImVec2((ImGui::GetWindowWidth() / 3)-20, 20)) || upPressed) {
							upPressed = true; ImGui::SameLine();
							if (ImGui::Button("x"))
								upPressed = false;
							else {
								ImGui::SliderFloat("Up X: ", &CamerasAboveX[activeCamera], -10.f, 10.f);
								ImGui::SliderFloat("Up Y: ", &CamerasAboveY[activeCamera], -10.f, 10.f);
								ImGui::SliderFloat("Up Z: ", &CamerasAboveZ[activeCamera], -10.f, 10.f);
							}
						}
					}
			}
		if (ImGui::Button("Perspective Settings", ImVec2((ImGui::GetWindowWidth() / 2) - 20, 20)) || PerspectivePressed) {
			PerspectivePressed = true; ImGui::SameLine();
			if (ImGui::Button("X"))
				PerspectivePressed = false;
			else {
				if(ImGui::Button("Fovy", ImVec2(ImGui::GetWindowWidth(), 20)) || PerspectivePressed)
					ImGui::SliderFloat("Fovy: ", &Fovy[activeCamera], 0.f, 2.f);
			}

		}
		if (ImGui::Button("near/far", ImVec2((ImGui::GetWindowWidth() / 3) - 20, 20)) || nearFarPressed) {
			nearFarPressed = true; ImGui::SameLine();
			if (ImGui::Button("x"))
				nearFarPressed = false;
			else {
				ImGui::SliderFloat("Near distance: ", &NearDist[activeCamera],0.f, 20.f);
				ImGui::SliderFloat("Far distance: ", &FarDist[activeCamera], 0.f, 20.f);
			}
		}
		ImGui::SliderFloat("View unit : ", &Up[activeCamera], 0.f, 10.f);
		ImGui::Checkbox("ortho", &orthoSet);
		
			ImGui::End();

	}
	if (active_lighting_settings)
	{
		ImGui::Checkbox("Set Directional Lighting", &directionalSource);
		ImGui::Checkbox("Show Directions Of Specular Light", &directionsOfSpecular);
		ImGui::SliderFloat("Model Ambient Material Red", &modelMaterialR[scene.GetActiveModelIndex()], 0.f,1.f);
		ImGui::SliderFloat("Model Ambient Material Green", &modelMaterialG[scene.GetActiveModelIndex()], 0.f, 1.f);
		ImGui::SliderFloat("Model Ambient Material Blue", &modelMaterialB[scene.GetActiveModelIndex()], 0.f, 1.f);
		ImGui::SliderFloat("Model Diffuse Material Red", &modeldiffMaterialR[scene.GetActiveModelIndex()], 0.f, 1.f);
		ImGui::SliderFloat("Model Diffuse Material Green", &modeldiffMaterialG[scene.GetActiveModelIndex()], 0.f, 1.f);
		ImGui::SliderFloat("Model Diffuse Material Blue", &modeldiffMaterialB[scene.GetActiveModelIndex()], 0.f, 1.f);
		ImGui::SliderFloat("Model Specular Material Red", &modelspectMaterialR[scene.GetActiveModelIndex()], 0.f, 1.f);
		ImGui::SliderFloat("Model Specular Material Green", &modelspectMaterialG[scene.GetActiveModelIndex()], 0.f, 1.f);
		ImGui::SliderFloat("Model Specular Material Blue", &modelspectMaterialB[scene.GetActiveModelIndex()], 0.f, 1.f);
		
		ImGui::Checkbox("Toon", &Toon);
		
		ImGui::Checkbox("Ambient Lighting", &AmbienLighting);
		ImGui::Checkbox("Diffuse Lighting", &DiffuseLighting);
		ImGui::Checkbox("Specular Lighting", &SpecularLighting);
		ImGui::Checkbox("Gouraud Shading", &GShading);
		ImGui::Checkbox("Phong Reflection", &PReflection);
		ImGui::Checkbox("Phong Shading", &PShading);
		ImGui::SliderFloat("Lighting Position X ", &lightPosX[activeLighting], -30.f, 30.f);
		ImGui::SliderFloat("Lighting Position Y ", &lightPosY[activeLighting], -30.f, 30.f);
		ImGui::SliderFloat("Lighting Position Z ", &lightPosZ[activeLighting], -30.f, 30.f);
		if (ImGui::Button("Ambient", ImVec2((ImGui::GetWindowWidth() / 3), 20)) || ambient) {
			ambient = true; ImGui::SameLine();
			if (ImGui::Button("X"))
				ambient = false;
			else {
				ImGui::SliderFloat("Ambient lighting red value ", &ambLightingR[activeLighting], 0.0f, 1.f);
				ImGui::SliderFloat("Ambient lighting green value ", &ambLightingG[activeLighting], 0.0f, 1.f);
				ImGui::SliderFloat("Ambient lighting blue value ", &ambLightingB[activeLighting], 0.0f, 1.f);
			}
		}
			ImGui::SameLine();
			if (ImGui::Button("Diffuse", ImVec2((ImGui::GetWindowWidth() / 3), 20)) || diffuse) {
				diffuse = true; ImGui::SameLine();
				if (ImGui::Button("X"))
					diffuse = false;
				else {
					ImGui::SliderFloat("Diffuse lighting red value ", &DifLightColorR[activeLighting], 0.0f, 1.f);
					ImGui::SliderFloat("Diffuse lighting green value ", &DifLightColorG[activeLighting], 0.0f, 1.f);
					ImGui::SliderFloat("Diffuse lighting blue value ", &DifLightColorB[activeLighting], 0.0f, 1.f);
				}}
				ImGui::SameLine();
				if (ImGui::Button("Specular", ImVec2((ImGui::GetWindowWidth() / 3), 20)) || specular) {
					specular = true; ImGui::SameLine();
					if (ImGui::Button("X"))
						specular = false;
					else {
						ImGui::SliderFloat("Shining Coefficient", &shiniesCoefficent[activeLighting], 0, 50);
						//ImGui::SliderFloat("Eye Position X ", &eyePosX[activeLighting], -10.f, 10.f);
						//ImGui::SliderFloat("Eye Position Y ", &eyePosY[activeLighting], -10.f, 10.f);
						//ImGui::SliderFloat("Eye Position Z ", &eyePosZ[activeLighting], -10.f, 10.f);
						ImGui::SliderFloat("Specular lighting red value ", &SpecLightColorR[activeLighting], 0.0f, 1.f);
						ImGui::SliderFloat("Specular lighting green value ", &SpecLightColorG[activeLighting], 0.0f, 1.f);
						ImGui::SliderFloat("Specular lighting blue value ", &SpecLightColorB[activeLighting], 0.0f, 1.f);
					}
				}
	}
	if (screenShot)
	{
		if (ImGui::Button("Blur"))
		{
			blurIntensity++;
		}
		ImGui::Checkbox("Bloom", &Bloom);
		ImGui::SliderInt("Kernel Size: ", &kernelSize, 0, 9);
		ImGui::SliderFloat("Bloom Threshhold: ", &bloom_threshhold, 0.f, 1.f);
	}
	else
	{
		blurIntensity = 0;
		Bloom = 0;
	}

	if (active_feature_settings)
	{
		//ImGui::Checkbox("Draw World Axis", &drawWorld);
		//ImGui::Checkbox("Draw Bounding Box", &drawBoundingBox);
		
		ImGui::Checkbox("Texture On", &drawFaceNormals);
		ImGui::Checkbox("Normal Map", &drawVertexNormals);
		ImGui::Checkbox("Plane", &drawFaceBoundingBox);
		ImGui::Checkbox("Sphere", &triangleColor);
		ImGui::Checkbox("Cylinder", &zColor);
		ImGui::Checkbox("Lines", &paintModel);
		
	}
	


	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		ImGui::Begin("GUI: "); 
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color// Create a window called "Hello, world!" and append into it.
		//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("ScreenShot", &screenShot);
		if(!screenShot)
		{ 
		ImGui::Checkbox("Camera Settings", &active_camera_settings);
		ImGui::Checkbox("Lighting Settings", &active_lighting_settings);
		
		static int previousModelCount = 0;
		static int previousCameraCount = 0;
		static int previousLightingCount = 0;
		if (scene.GetModelCount() >= 1) 
		{
			ImGui::Checkbox("Feature Settings", &active_feature_settings);
			int activeModel = scene.GetActiveModelIndex();
			if (scene.GetModelCount() > previousModelCount)
			{
				previousModelCount++;
				initParamaters(previousModelCount);
			}
			if (scene.GetLightingCount() > previousLightingCount)
			{
				previousLightingCount++;
				initParamaters(previousLightingCount);
			}
			if (scene.GetCameraCount() > previousCameraCount)
			{
				previousCameraCount++;
				initCameraParameters(previousCameraCount);
			}

			Lighting& light = scene.GetActiveLight();
			MeshModel& model = scene.GetActiveModel();

			ImGui::Text("ActiveModel: %d / %d", scene.GetActiveModelIndex() + 1, scene.GetModelCount());
			if ((ImGui::Button("<")))
			{
				if (scene.GetActiveModelIndex() >= 1)
					scene.SetActiveModelIndex(scene.GetActiveModelIndex() - 1);
			}
			ImGui::SameLine();
			if (ImGui::Button(">"))
			{
				if (scene.GetActiveModelIndex() < scene.GetModelCount() - 1)
					scene.SetActiveModelIndex(scene.GetActiveModelIndex() + 1);
			}

			ImGui::Text("ActiveCamera: %d / %d", scene.GetActiveCameraIndex() + 1, scene.GetCameraCount());
			if ((ImGui::Button("-")))
			{
				if (scene.GetActiveCameraIndex() >= 1)
					scene.SetActiveCameraIndex(scene.GetActiveCameraIndex() - 1);
			}
			ImGui::SameLine();
			if (ImGui::Button("+"))
			{
				if (scene.GetActiveCameraIndex() < scene.GetCameraCount() - 1)
					scene.SetActiveCameraIndex(scene.GetActiveCameraIndex() + 1);
			}

			ImGui::Text("ActiveLight: %d / %d", scene.GetActiveLightingIndex() + 1, scene.GetLightingCount());
			if ((ImGui::Button("-")))
			{
				if (scene.GetActiveLightingIndex() >= 1)
					scene.setActiveLightingIndex(scene.GetActiveLightingIndex() - 1);
			}
			ImGui::SameLine();
			if (ImGui::Button("+"))
			{
				if (scene.GetActiveLightingIndex() < scene.GetLightingCount() - 1)
					scene.setActiveLightingIndex(scene.GetActiveLightingIndex() + 1);
			}

			if (ImGui::Button("Local", ImVec2((ImGui::GetWindowWidth() / 2 - 20), 20)) || localButtonPressed) {
				localButtonPressed = true; ImGui::SameLine();
				if (ImGui::Button("X"))
					localButtonPressed = false;
				else {
					if (ImGui::Button("ScaleL", ImVec2((ImGui::GetWindowWidth() / 3 - 20), 20)) || scaleLButtonPressed) {
						scaleLButtonPressed = true; ImGui::SameLine();
						if (ImGui::Button("x"))
							scaleLButtonPressed = false;
						else
						{
							ImGui::SliderFloat("Uniform Local Scale", &LocalUniformScale[activeModel], 0.0f, 10.0f);
							ImGui::SliderFloat("ScaleL X: ", &LocalScaleX[activeModel], 0.0f, 10.0f);
							ImGui::SliderFloat("ScaleL Y: ",                &LocalScaleY[activeModel], 0.0f, 10.0f);
							ImGui::SliderFloat("ScaleL Z: ",                &LocalScaleZ[activeModel], 0.0f, 10.0f);
						}
					}
					if (ImGui::Button("TranslateL", ImVec2((ImGui::GetWindowWidth() / 3 - 10), 20)) || rotationLButtonPressed) {
						rotationLButtonPressed = true; ImGui::SameLine();
						if (ImGui::Button("x"))
							rotationLButtonPressed = false;
						else {
							ImGui::SliderFloat("TranslateL X: ", &LocTransX[activeModel], -10.f, 10.f);
							ImGui::SliderFloat("TranslateL Y: ", &LocTransY[activeModel], -10.f, 10.f);
							ImGui::SliderFloat("TranslateL Z: ", &LocTransZ[activeModel], -10.f, 10.f);
						}
					}
					
					if (ImGui::Button("RotationL", ImVec2((ImGui::GetWindowWidth() / 3 - 20), 20)) || translateWButtonPressed) {
						translateWButtonPressed = true; ImGui::SameLine();
						if (ImGui::Button("x"))
							translateWButtonPressed = false;
						else {
							ImGui::SliderFloat("RotateL X: ", &LocRotX[activeModel], -2.0f, 2.0f);
							ImGui::SliderFloat("RotateL Y: ", &LocRotY[activeModel], -2.0f, 2.0f);
							ImGui::SliderFloat("RotateL Z: ", &LocRotZ[activeModel], -2.0f, 2.0f);
						}
					}
				}
			}
			
			if (ImGui::Button("World", ImVec2((ImGui::GetWindowWidth() / 2) - 20, 20)) || worldButtonPressed) {
				worldButtonPressed = true; ImGui::SameLine();
				if (ImGui::Button("X"))
					worldButtonPressed = false;
				else {
					if (ImGui::Button("ScaleW", ImVec2((ImGui::GetWindowWidth() / 3 - 20), 20)) || scaleWButtonPressed) {
						scaleWButtonPressed = true; ImGui::SameLine();
						if (ImGui::Button("x"))
							scaleWButtonPressed = false;
						else
						{
							ImGui::SliderFloat("Uniform World Scale", &WorldUniformScale[activeModel], 0.0f, 100000.0f);
							ImGui::SliderFloat("ScaleW X: ", &worldScaleX[activeModel], 0.0f, 10.0f);
							ImGui::SliderFloat("ScaleW Y: ", &worldScaleY[activeModel], 0.0f, 10.0f);
							ImGui::SliderFloat("ScaleW Z: ", &worldScaleZ[activeModel], 0.0f, 10.0f);
						}
					}
				
					if (ImGui::Button("TranslateW", ImVec2((ImGui::GetWindowWidth() / 3 - 10), 20)) || translateWButtonPressed) {
						translateWButtonPressed = true; ImGui::SameLine();
						if (ImGui::Button("x"))
							translateWButtonPressed = false;
						else {
							ImGui::SliderFloat("TranslateW X: ", &WorTransX[activeModel], -10.f, 10.f);
							ImGui::SliderFloat("TranslateW Y: ", &WorTransY[activeModel], -10.f, 10.f);
							ImGui::SliderFloat("TranslateW Z: ", &WorTransZ[activeModel], -10.f, 10.f);
						}
					}
				
					if (ImGui::Button("RotationW", ImVec2((ImGui::GetWindowWidth() / 3 - 20), 20)) || rotationWButtonPressed) {
						rotationWButtonPressed = true; ImGui::SameLine();
						if (ImGui::Button("x"))
							rotationWButtonPressed = false;
						else {
							ImGui::SliderFloat("RotateW X: ", &WorRotX[activeModel], -2.0f, 2.0f);
							ImGui::SliderFloat("RotateW Y: ", &WorRotY[activeModel], -2.0f, 2.0f);
							ImGui::SliderFloat("RotateW Z: ", &WorRotZ[activeModel], -2.0f, 2.0f);
						}
					}
				}
			}
			// transformations

			model.localScale(LocalScaleX[activeModel],
				LocalScaleY[activeModel],
				LocalScaleZ[activeModel],
				LocalUniformScale[activeModel]);
			model.worldScale(worldScaleX[activeModel]
				, worldScaleY[activeModel]
				, worldScaleZ[activeModel]
				, WorldUniformScale[activeModel]);
		  model.TranslateLocal(LocTransX[activeModel],
			  LocTransY[activeModel],
			  LocTransZ[activeModel]);
		  model.RotateLocal(LocRotX[activeModel],
	   			                 LocRotY[activeModel],
			   				     LocRotZ[activeModel]
			);							
		  model.TranslateWorld(WorTransX[activeModel],
			  WorTransY[activeModel],
			  WorTransZ[activeModel]);
		  model.RotateWorld(WorRotX[activeModel],
							WorRotY[activeModel],
							WorRotZ[activeModel]);
		  model.setModelMaterial({ modelMaterialR[activeModel], modelMaterialG[activeModel], modelMaterialB[activeModel] }, { modeldiffMaterialR[activeModel], modeldiffMaterialG[activeModel], modeldiffMaterialB[activeModel] }
		  , { modelspectMaterialR[activeModel], modelspectMaterialG[activeModel], modelspectMaterialB[activeModel] });

		  scene.setFeatures(drawWorld, drawBoundingBox, drawVertexNormals, drawFaceNormals, drawFaceBoundingBox, triangleColor, paintModel, zColor,Toon);
		  
		  light.setDirectionalSource(directionalSource);
		  light.setDirectionsOfSpecular(directionsOfSpecular);
		  light.setGouraudShading(GShading);
		  light.setAmbientLighting(AmbienLighting);
		  light.setDiffuseLighting(DiffuseLighting);
		  light.setSpecularLighting(SpecularLighting);
		  light.setPhongReflection(PReflection);
		  light.setPhongShading(PShading);
		  light.setLightingsColor({ ambLightingR[activeLighting], ambLightingG[activeLighting], ambLightingB[activeLighting] },
			  { DifLightColorR[activeLighting] ,DifLightColorG[activeLighting] ,DifLightColorB[activeLighting]},
			  { SpecLightColorR[activeLighting], SpecLightColorG[activeLighting], SpecLightColorB[activeLighting]});
		  light.setLightingPosition(lightPosX[activeLighting], lightPosY[activeLighting], lightPosZ[activeLighting]);
		  //light.setEyePosition(CameraPosX[activeCamera], CameraPosY[activeCamera], CameraPosZ[activeCamera]);
		  //light.setLightingPosition(scene.GetActiveCamera().getCameraPos()[0], scene.GetActiveCamera().getCameraPos()[1], scene.GetActiveCamera().getCameraPos()[2]);
		  light.setShiniesCoefficent(shiniesCoefficent[activeLighting]);

		  if (orthoSet) {
			  Left[activeCamera] = Up[activeCamera] * Aspect * -1;
			  Right[activeCamera] = Up[activeCamera] * Aspect;
			  Down[activeCamera] = Up[activeCamera] * -1;
			  camera.GetOrtho(Left[activeCamera], Right[activeCamera], Down[activeCamera], Up[activeCamera], NearDist[activeCamera], FarDist[activeCamera]);
		  }
		  else {
			  camera.Prespective(Fovy[activeCamera], Aspect, NearDist[activeCamera], FarDist[activeCamera]);
		  }
		  camera.SetCameraLookAt({ CameraPosX[activeCamera], CameraPosY[activeCamera], CameraPosZ[activeCamera] },
			  { CameraLooksAtX[activeCamera],CameraLooksAtY[activeCamera],CameraLooksAtZ[activeCamera] },
			  { CamerasAboveX[activeCamera],CamerasAboveY[activeCamera],CamerasAboveZ[activeCamera] });

}
		}
		scene.setScreenShot(screenShot);
		scene.setPostProcessing(Bloom,kernelSize,blurIntensity);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	
}