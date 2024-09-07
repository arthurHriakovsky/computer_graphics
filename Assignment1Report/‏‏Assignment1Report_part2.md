
## Assignment 1 Report part b

a.
vertex print:
![vertex print](https://user-images.githubusercontent.com/92512783/141661053-c34637d7-62ad-47f8-9573-0272c37ed26a.png)


b. To make the transformations asked we made functions that get set of changes (for scale we take the wanted scale after the transformation, for translate x y and z to add to the original vertex)
and they change the private transformation matrix of the model. then with the getters of the private we multiply each vertex by its own when we draw.
this matrixs start in the center with a decent scale by default inorder they the model will be in the center and in the right size.

c. Model screenshot (Beethoven):
<img width="293" alt="beethoven screenshot" src="https://user-images.githubusercontent.com/92512783/141661083-4360a679-65bc-481a-bf86-6632e69d902c.png">


d. We made gui that you can choose between world and local transformations. each one has scale, rotate and translate which you can change the x, y and z (scale has uniform too, rotate acts as rotating around one of the axis).
then we before each face draw we call the getTransformation of the model and multiply each vertex. getTransformation is World Transformation * Local Transformation each one built by 
translate * rotate * scale.
GUI preview:
<img width="329" alt="gui preview" src="https://user-images.githubusercontent.com/92512783/141661108-3fbc2952-045d-4986-a0d4-42c9df550b03.png">


e. starting point:
<img width="306" alt="beforechange" src="https://user-images.githubusercontent.com/92512783/141661114-9320d64c-0ae2-43f7-ba70-ea197a6ed631.png">

rotate local around y then translate by x:
<img width="312" alt="rotatelocaltranslateworld" src="https://user-images.githubusercontent.com/92512783/141661119-9ca04e2e-92f1-48a2-8610-21429da84b6a.png">

translate local by x then rotate world by y:
<img width="340" alt="translatelocalrotateworld" src="https://user-images.githubusercontent.com/92512783/141661123-e5a8bcd8-27ea-464d-92d5-3152f00a4ba7.png">


f. gif showing how you add model and switch between them: 
[https://gyazo.com/51e92ad136175cfd109f42bd1d703e1e](im a gif link)

g. we added a few controls you can transform with.
for the keyboard we added ASWD for rotating and the arrows for translating.
with the mouse if you press control and move the mouse the model rotates around z axis. and if you press left and right clicks the model scales.
