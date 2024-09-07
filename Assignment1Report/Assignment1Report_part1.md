
## Assignment 1 Report part a

DrawLine function:
```c++
void Renderer::DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color)
{
	// we want to draw line from p1 to p2.
	// followed the bresenham algorithem shown in class
	glm::ivec2 firP = p1, secP = p2; // local variables for the points
	if (p1[0] > p2[0])               // swap if p1(x) > p2(x) because we want left to right
		swap(firP, secP);
	int deltaP = (secP[0]-firP[0]), 
		deltaQ = (secP[1]-firP[1]),
		sign = 1; // delta p and delta q from bresenham algo, sign for if a < 0
	float a = 10;
	if(deltaP!=0)a = (float)deltaQ/deltaP; // y = *a*x + c
	int x = firP[0], // variables for the cords of p1
		y = firP[1], 
		error = -deltaP; 
	if (a < 0)          // the slope is negative
		sign = -1; 
	if (a > 1 || a < -1) {  // the slope is bigger then 1 soo we swap roles between x and y in other function
		DrawLineOpp(firP, secP, color);
		return;
	}
	while (x <= secP[0]) // bresenham algo - follows x till p2 x
	{
		if (error > 0) //error > 0 means we need to go up one pixel
		{
			y += sign; error = error - 2 * deltaP;
		}
		PutPixel(x, y, color);
		x += 1;
		error = error + sign*2 * deltaQ;
	}
}

void Renderer::DrawLineOpp(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color)
{
	glm::ivec2 firP = p1, secP = p2; // without loss of generality y instead of x as the leading cordinate
	if (p1[1] > p2[1])  
		swap(firP, secP);
	int deltaP = (secP[0] - firP[0]), deltaQ = (secP[1] - firP[1]), x = firP[0], y = firP[1], error = -deltaQ,sign=1;
	float a = (float)deltaP / deltaQ;
	if (a < 0)
		sign = -1; // -1 if a < 0
	while (y <= secP[1]) // bresenham algo
	{
		if (error > 0)
		{
			x += sign; error = error - 2 * deltaQ;
		}
		PutPixel(x, y, color);
		y += 1;
		error = error + sign * 2 * deltaP;
	}
}
```
sanitycheck:
![sanity_check](<a href="https://imgbb.com/"><img src="https://i.ibb.co/5MX4gKF/sanity-check.png" alt="sanity-check" border="0"></a>)

Our Drawing:
![tasty_drawing](<a href="https://ibb.co/C55hSGN"><img src="https://i.ibb.co/3yyBPZG/tasty-Drawing.png" alt="tasty-Drawing" border="0"></a> "bright_day")