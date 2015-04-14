#pragma once
#include "Controller.h"
class OrbitController : public Controller{
public:
	OrbitController( vec3 pos = vec3(0.0f, 0.0f, 0.0f), 
 vec3 ax = vec3(1.0, 0.0f, 0.0f), GLfloat r = 10.0f); 
	virtual void update(float elapsedTimeSeconds);

protected:

	float rotation1;
	float rotation2;
 
	// Position at which the object is spinning in place 
	 vec3 position; 
 
	// axis of rotation 
	vec3 axis; 
 
	// rate and direction of rotation 
	GLfloat rate;

	mat4 T, R1, R2;
};

