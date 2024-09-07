#version 330 core

struct Material
{
	sampler2D textureMap;
//	// You can add more fields here...
//	// Such as:
//	//		1. diffuse/specular relections constants
//	//		2. specular alpha constant
//	//		3. Anything else you find relevant
};

// We set this field's properties from the C++ code
uniform Material material;

uniform bool texturebool;
uniform bool plane;
uniform bool sphere;
uniform bool cylinder;
uniform vec4 minMax2d;

// Inputs from vertex shader (after interpolation was applied)
in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;
in vec3 orig_fragPos;
// The final color of the fragment (pixel)
out vec4 frag_color;

void main()
{

	float diffCalculate = max(0.f,dot(normalize(light.point - fragPos),normalize(fragNormal)));
	float spectCalculate = (pow(max(0.f, dot((normalize(reflect((-normalize(light.point - fragPos)), normalize(fragNormal)))), normalize(cameraEye-fragPos))), light.shine));
	vec3 finalAmb   =  vec3(0);
	vec3 finalDiff  =  vec3(0);
	vec3 finalSpect =  vec3(0);
	

	if(!toon)
	{
		finalAmb = light.ambient * material.ambient;
		finalDiff = light.diff * diffCalculate* material.diff;
		 finalSpect = light.spect * spectCalculate * material.spect;
		frag_color = vec4(finalAmb+finalDiff+finalSpect,1);
	}
	if(toon){
			for(float i = 0.f ; i <= 1 ; i = i + 0.25f)
			{
				if(diffCalculate>= i && diffCalculate  <= i+0.25f )
				{
					float newBrightness = i;
					finalDiff = light.diff * newBrightness * material.diff;
					
				}
				if(spectCalculate>= i && spectCalculate  <= i+0.25f )
				{
					float newBrightness = i;
					finalSpect = light.spect * newBrightness * material.spect;
				}
				
			}
		
			finalAmb = light.ambient * material.ambient;
			frag_color = vec4(finalAmb+finalDiff+finalSpect,1);

		}
	vec3 textureColor = vec3(0); 
	//Sample the texture-map at the UV coordinates given by 'fragTexCoords'
	if(texturebool){
		if(plane){
			textureColor = vec3(texture(material.textureMap, vec2((minMax2d.y - orig_fragPos.x) / (minMax2d.y - minMax2d.x), (minMax2d.w - orig_fragPos.y) / (minMax2d.w - minMax2d.z))));
		}
		else if(sphere){
			float r = sqrt(pow(orig_fragPos.x, 2) + pow(orig_fragPos.y, 2) + pow(orig_fragPos.z, 2));
			float cosTeta = orig_fragPos.z / r;
			float teta = acos(cosTeta);
			float fi = atan(orig_fragPos.y / orig_fragPos.x);
			textureColor = vec3(texture(material.textureMap, vec2(fi, teta)));
		}
		else if(cylinder){
			textureColor = vec3(texture(material.textureMap, vec2(atan(orig_fragPos.z / orig_fragPos.x), orig_fragPos.y)));
		}
		else if(true){
			textureColor =  vec3(texture(material.textureMap,fragTexCoords));
		}
	frag_color = vec4(textureColor, 1);
	}
	else{
		frag_color = vec4(0,0,0,1);
	}

	

	
}