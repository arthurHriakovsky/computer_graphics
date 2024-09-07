#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Utils.h"


glm::vec3 Utils::Vec3fFromStream(std::istream& issLine)
{
	float x, y, z;
	issLine >> x >> std::ws >> y >> std::ws >> z;
	return glm::vec3(x, y, z);
}

glm::vec4 Utils::HomVec4FromVec3(glm::vec3 v)
{
	glm::vec4 output(v[0], v[1], v[2], 1);
	return output;
}

glm::vec3 Utils::Vec3FromHomVec4(glm::vec4 v)
{
	glm::vec3 output(v[0]/v[3], v[1]/v[3], v[2]/v[3]);
	return output;
}

glm::vec2 Utils::Vec2fFromStream(std::istream& issLine)
{
	float x, y;
	issLine >> x >> std::ws >> y;
	return glm::vec2(x, y);
}

std::shared_ptr<MeshModel> Utils::LoadMeshModel(const std::string& filePath)
{
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textureCoords;
	std::ifstream ifile(filePath.c_str());

	// while not end of file
	while (!ifile.eof())
	{
		// get line
		std::string curLine;
		std::getline(ifile, curLine);

		// read the type of the line
		std::istringstream issLine(curLine);
		std::string lineType;

		issLine >> std::ws >> lineType;

		// based on the type parse data
		if (lineType == "v")
		{
			vertices.push_back(Utils::Vec3fFromStream(issLine));
		}
		else if (lineType == "vn")
		{
			normals.push_back(Utils::Vec3fFromStream(issLine));
		}
		else if (lineType == "vt")
		{
			textureCoords.push_back(Utils::Vec2fFromStream(issLine));
		}
		else if (lineType == "f")
		{
			faces.push_back(Face(issLine));
		}
		else if (lineType == "#" || lineType == "")
		{
			// comment / empty line
		}
		else
		{
			std::cout << "Found unknown line Type \"" << lineType << "\"";
		}
	}

	return std::make_shared<MeshModel>(faces, vertices, normals, textureCoords, Utils::GetFileName(filePath));
}


//(model.GetNormalIndex(model.GetFace(i).Face::GetNormalIndex(j)));

std::string Utils::GetFileName(const std::string& filePath)
{
	if (filePath.empty()) {
		return {};
	}

	auto len = filePath.length();
	auto index = filePath.find_last_of("/\\");

	if (index == std::string::npos) {
		return filePath;
	}

	if (index + 1 >= len) {

		len--;
		index = filePath.substr(0, len).find_last_of("/\\");

		if (len == 0) {
			return filePath;
		}

		if (index == 0) {
			return filePath.substr(1, len - 1);
		}

		if (index == std::string::npos) {
			return filePath.substr(0, len);
		}

		return filePath.substr(index + 1, len - index - 1);
	}

	return filePath.substr(index + 1, len - index);
}

float Utils::interpolate(glm::vec3 p1, glm::vec3 p2, glm::vec2 p3)
{
	float totalLength;
	float LengthToP3;
	float alpha;
	if (p1[1] > p2[1]) // make the bigger y between the points be p2
	{
		std::swap(p1, p2);
	}

	int deltaP = (p2[0] - p1[0]), deltaQ = (p2[1] - p1[1]);
	float a = 10;
	if (deltaP != 0)a = (float)deltaQ / deltaP; // y = *a*x + c

	if (p1[0] == p2[0] && p1[1] == p2[1]) // the line acts as a dot take the bigger z
	{
		return (p1[2] > p2[2]) ? p1[2] : p2[2];
	}
	else if (a<1 && a>-1)
	{
		if (p1[0] > p2[0]) // make the bigger x between the points be p2
		{
			std::swap(p1, p2);
		}
		totalLength = p2[0] - p1[0];
		LengthToP3 = p2[0] - p3[0];

	}
	else {
		totalLength = p2[1] - p1[1];
		LengthToP3 = p2[1] - p3[1];
	}
	alpha = LengthToP3 / totalLength;	
	float toReturn = (1 - alpha) * p2[2] + alpha * p1[2];
	return toReturn;
}

glm::vec3 Utils::interpolateAlpha(glm::vec3 p1, glm::vec3 p2, glm::vec2 p3, glm::vec3 value1, glm::vec3 value2)
{
	float totalLength;
	float LengthToP3;
	float alpha;
	if (p1[1] > p2[1]) // make the bigger y between the points be p2
	{
		std::swap(p1, p2);
		std::swap(value1, value2);
	}

	int deltaP = (p2[0] - p1[0]), deltaQ = (p2[1] - p1[1]);
	float a = 10;
	if (deltaP != 0)a = (float)deltaQ / deltaP; // y = *a*x + c

	if (p1[0] == p2[0] && p1[1] == p2[1]) // the line acts as a dot take the bigger z
	{
		return (p1[2] > p2[2]) ? value1 : value2;
	}
	else if (a<1 && a>-1)
	{
		if (p1[0] > p2[0]) // make the bigger x between the points be p2
		{
			std::swap(p1, p2);
			std::swap(value1, value2);
		}
		totalLength = p2[0] - p1[0];
		LengthToP3 = p2[0] - p3[0];

	}
	else {
		totalLength = p2[1] - p1[1];
		LengthToP3 = p2[1] - p3[1];
	}
	alpha = LengthToP3 / totalLength;
	glm::vec3 toReturn = (1.f - alpha) * value2 + alpha * value1;
	return toReturn;
}

bool Utils::kernelCheck(glm::vec2 point, int width, int height)
{
	if (point[0] < 0 || point[1] < 0)
		return false;
	if (point[0] >= width || point[1] >= height)
		return false;
	return true;
} 
