a. ambient only: i chose a bluyish color for the ambient model material and a dark grey for the ambient lighting color, i got a dark silhouette of a blue bunny because the calculation
of ambient lighting color is ambientModelMaterialColor * ambientLightingColor (if we had a only green model color and only red light color the silhouette was black)
![1a](https://user-images.githubusercontent.com/92512783/147901857-eb60e399-ed32-4f96-abc5-ae8db7133877.png)
b. Added diffuse to the scene:
![1b](https://user-images.githubusercontent.com/92512783/147901868-dde74c13-3dd5-41d4-9b27-55c72d9ace0c.png)
c. Added Gouraud shading to the scene: (no specular lighting yet) 
![1c](https://user-images.githubusercontent.com/92512783/147901870-e158717b-2a6a-46be-b599-82d21fa3a18d.png)
d. white lines is the light going from the point and the pink lines are the reflection of the light
![d](https://user-images.githubusercontent.com/92512783/147901875-f798b8ec-b7fd-400a-b14e-8b5d765f45ba.png)
e. Specular and Phong shading added to the scene:
![e](https://user-images.githubusercontent.com/92512783/150654507-620ee11a-1d09-4c71-9afd-667b897dfc9f.png)

f. Directional Light added: (same position as e)
![f](https://user-images.githubusercontent.com/92512783/150654517-d293e8bf-b958-433e-8a8f-e052b32cdf95.png)

Dircetional from front light, before and after:
![f front no dic](https://user-images.githubusercontent.com/92512783/150654534-269caf8f-491d-466d-9280-bd2a921807c0.png)
![f front dic](https://user-images.githubusercontent.com/92512783/150654533-f79c29b0-2bd6-40fd-a6ca-e80fed276936.png)


g. Examples of models with our shading implememt: (going by flat shading, Gouraud shading and Phong shading)
![fbethovenflat](https://user-images.githubusercontent.com/92512783/150654408-5ee58939-1831-4627-be0e-716ce37130bc.png)
![fbethovengour](https://user-images.githubusercontent.com/92512783/150654411-16a1e384-91a2-4f31-bc7c-34a70e9d8f94.png)
![fbethovenphong](https://user-images.githubusercontent.com/92512783/150654414-ddce8184-f9c3-4a5a-ace4-4cadde629401.png)
![fcamflat](https://user-images.githubusercontent.com/92512783/150654418-a23a923c-9ffd-4e5b-bd58-e921c77e6641.png)
![fcamgour](https://user-images.githubusercontent.com/92512783/150654419-c4630cfd-7c9d-4275-91c5-c50a92e74d8c.png)
![fcamphong](https://user-images.githubusercontent.com/92512783/150654420-f0af81c4-d100-416f-9df1-4408a869f432.png)
![fpawnflat](https://user-images.githubusercontent.com/92512783/150654422-6eba9c0d-497b-42dd-8517-1f03c2b5710c.png)
![fpawngour](https://user-images.githubusercontent.com/92512783/150654423-f8dbbb9a-ff2e-4387-93f1-8ccd496ae934.png)
![fpawnphong](https://user-images.githubusercontent.com/92512783/150654424-e6ad44ff-eca7-4111-aeac-634a087026ae.png)
![fteapotflat](https://user-images.githubusercontent.com/92512783/150654426-800b73c6-f5c1-43e4-8758-0060a8acdbbc.png)
![fteapotgour](https://user-images.githubusercontent.com/92512783/150654427-7e635142-8988-46b4-af34-7f10fd3896f7.png)
![fteapotphong](https://user-images.githubusercontent.com/92512783/150654428-d1ac3eff-6024-4f42-945c-85181d2efb7d.png)

h. teapot before any post-processing (although added a button of screenshot): *please note that the screenshot took before i fixed the phong lighting and thats why it looks this way*
![g0blur](https://user-images.githubusercontent.com/92512783/147902510-9b6030df-2f18-4fd8-8cfa-50dbdf2a503a.png)
1 process of blur: (3x3 kernel)
![g1blur](https://user-images.githubusercontent.com/92512783/147902514-5d35e09b-d6ee-41cd-a1c2-ca6fa3e748a3.png)
2 processes of blur:
![g2blur](https://user-images.githubusercontent.com/92512783/147902515-6f0fac66-087f-4a18-87eb-348b7e992829.png)
3 processes of blur:
![g3blur](https://user-images.githubusercontent.com/92512783/147902516-b6c135dd-23c3-4050-91ad-fa14313f3788.png)
1 processes of blur but with 9x9 kernel:
![g1bigkernel](https://user-images.githubusercontent.com/92512783/147902511-8f260515-8857-4bdc-9211-1826ada86082.png)
bloom affect:
![gbloom](https://user-images.githubusercontent.com/92512783/147902517-1d96841d-fb2a-4749-8aef-522e441f1083.png)

