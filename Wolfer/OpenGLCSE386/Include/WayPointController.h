#pragma once
#include "Controller.h"
class WayPointController : public Controller{
public:
	WayPointController(GLfloat r = 1.0f, vec3 rot = vec3(1.0f, 0.0f, 0.0f), vec3 pos = vec3(0.0f, -3.0f, 0.0f)); 
	virtual void update(float elapsedTimeSeconds);

protected:

	int leg;
	vec3 i; 
	vec3 k;
	vec3 velocity;
 
	// rate and direction of rotation 
	GLfloat rate;

	mat4 T, R1, R2;

	vec3 P, R;
};