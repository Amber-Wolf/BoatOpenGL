#include "CannonballController.h"

const float grav = 9.8f;


CannonballController::CannonballController(vec3 pos, vec3 vel){
	this->position = pos;
	this->velocity = vel;
	live = true;
}


CannonballController::~CannonballController(void){

}

vec3 CannonballController::getPos(void){
	return position;
}

void CannonballController::update(float timeSec){
	velocity.y = velocity.y - grav * timeSec;
	position = position + velocity * timeSec;
	target->modelMatrix = translate(mat4(1.0f), position);	
	if(position.y < -10){
		live = false;
	}
}

bool CannonballController::getLive(){
	return live;
}
