#include "OrbitController.h"


OrbitController::OrbitController(vec3 pos, vec3 ax, GLfloat r)
	: position(pos), axis(ax), rate(r){
	rotation1 = 0.0f;
	rotation2 = 0.0f;
	T = translate(mat4(1.0f), position);
	R1 = rotate(mat4(1.0f), 65.0f, vec3(0.0f, 1.0f, 0.0f));
	//target->modelMatrix = R1 * T;
}

void OrbitController::update(float elapsedTimeSeconds){
	rotation1 += elapsedTimeSeconds * rate;
	mat4 positionTransform = translate(mat4(1.0f), position);	

	R1 = rotate(mat4(1.0f), fmod(-rotation1, 360.0f), vec3(0.0f, 1.0f, 0.0f));
	R2 = rotate(mat4(1.0f), fmod(rotation1 * 4.0f, 360.0f), vec3(1.0f, 0.0f, 0.0f));
	target->modelMatrix = R1 * T * R2;

	//target->modelMatrix = rotate(positionTransform , fmod(rotation1, 360.0f), axis); //mat4(1.0f);
}