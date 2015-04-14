#include "ShipController.h"


ShipController::ShipController(vec3 pos, vec3 vel)
{
	this->position =  pos;
	this->velocity = vel;
}


ShipController::~ShipController(void)
{
}

void ShipController::update(float elapsedTimeSeconds){
	useBuoyancy( elapsedTimeSeconds);
	position = position + velocity * elapsedTimeSeconds;
	target->modelMatrix = translate(mat4(1.0f), position);	

}

void ShipController::useBuoyancy(float elapsedTimeSeconds){
	float height = position.y;
	if(abs(height) < BUOYANCY_HEIGHT_VAL){
		velocity = velocity + vec3(0.0f, -height/BUOYANCY_HEIGHT_VAL * BUOYANCY_VELOCITY_VAL * elapsedTimeSeconds * ((rand()%100)/100.0f),0.0f);
	} else {
		velocity = velocity + vec3(0.0f, -height * BUOYANCY_VELOCITY_VAL * elapsedTimeSeconds * ((rand()%100)/100.0f),0.0f);
	}
	if(velocity.y > BUOYANCY_CUTTING_POINT){
		velocity.y = velocity.y *.95;
	}
	//cout << velocity.y << endl;
}

vec3 ShipController::getPos(){
	return position; 
}
