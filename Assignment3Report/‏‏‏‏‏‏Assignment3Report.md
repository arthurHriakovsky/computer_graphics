a. the InitOpenGLRendering() loads two 2d triangles into the gpu and displays them on the screen. we used it before soo we could show our models through the cpu. the texture displayed on the two triangles
with loading the data into the gpu.
b.done.
c.+#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

out vec2 texCoord;
// The model/view/projection matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	gl_Position = projection * view *  model * vec4(pos, 1.0f);
}
d.out vec4 frag_color;

void main()
{
	frag_color = vec4(1,1,0,1);
	
}
e.std::shared_ptr<MeshModel> currentModel = scene.GetModel(currentModelIndex);

			// Activate the 'colorShader' program (vertex and fragment shaders)
			colorShader.use();
			// Set the uniform variables
			colorShader.setUniform("model", currentModel->GetTrans());
			colorShader.setUniform("view", camera.GetViewTransformation());
			colorShader.setUniform("projection", camera.GetProjectionTransformation());

			// Drag our model's faces (triangles) in fill mode
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBindVertexArray(currentModel->GetVAO());
			glDrawArrays(GL_TRIANGLES, 0, currentModel->GetModelVertices().size());
			glBindVertexArray(0);
}



![e](https://user-images.githubusercontent.com/92512783/154870182-352dca77-ea95-40c2-833e-d027d2f71a1d.png)
![e2](https://user-images.githubusercontent.com/92512783/154870185-d44e95e9-f721-470e-bac1-3c58b61e4c67.png)


f. links for gifs
	https://gyazo.com/50a835249a269c092276c7313db9a29b
	
	https://gyazo.com/c2c56f84db18673ea7858a504ef881e6
	
	https://gyazo.com/288bcbafb55c5cc13f3ac5e9ffd98cc0
	
	https://gyazo.com/eedf906a11b39e6d341564e1f518e80f
	
g. 
	canon:
	
![plane](https://user-images.githubusercontent.com/92512783/154872075-61915fb3-7073-4b31-9c0a-f52fda2d8e00.png)
![sphere](https://user-images.githubusercontent.com/92512783/154872076-d9dc74ed-4991-4005-b007-7dd68669fbf3.png)
![cylinder](https://user-images.githubusercontent.com/92512783/154872074-e51aaf23-db54-45ef-9094-8844c0ef5f4d.png)
	
	texture cordinates:
(for some reason i couldnt find models that looked good on the internet, they had some missing faces)
	
![texture1](https://user-images.githubusercontent.com/92512783/154872079-01453318-13ad-458d-b211-faf43faba421.png)
![texture2](https://user-images.githubusercontent.com/92512783/154872080-9721e9d2-21a8-4713-9a30-4a8dbbfecd6e.png)
![texture3](https://user-images.githubusercontent.com/92512783/154872082-2e0aabae-3201-4e06-b9eb-dc448a5d0a69.png)
![texture4](https://user-images.githubusercontent.com/92512783/154872083-ad6fd3eb-9ac2-43b5-b356-3593bc01eab0.png)
![texture5](https://user-images.githubusercontent.com/92512783/154872085-a46fa4f9-f3b5-4c1f-ad4e-cbea8435db65.png)
	
h.
	toon shading:
	![toon1](https://user-images.githubusercontent.com/92512783/154872148-12abfd43-a919-481c-b588-9d9584dd16a3.png)
	![toon2](https://user-images.githubusercontent.com/92512783/154872150-830ecdeb-d137-4d09-a895-3048e19abd06.png)
	https://gyazo.com/e51bb3552ecc85f1108773b85241a995
	
	normal mapping:
	
	https://gyazo.com/0462a81d3a7472011c50eb21adab40c1
	
	https://gyazo.com/cc2bca5170e0d77ff8b247fd44fa1be4

	we didnt manage to do the enviorment mapping sorry :( 

