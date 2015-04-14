#include "SpinnerController.h"


SpinnerController::SpinnerController( vec3 pos, vec3 ax, GLfloat r)
	: position(pos), axis(ax), rate(r){
	rotation = 0.0f;
}

void SpinnerController::update(float elapsedTimeSeconds){
	rotation += elapsedTimeSeconds * rate;
	mat4 positionTransform = translate(mat4(1.0f), position);	
	target->modelMatrix = rotate(positionTransform , fmod(rotation, 360.0f), axis); 
}
