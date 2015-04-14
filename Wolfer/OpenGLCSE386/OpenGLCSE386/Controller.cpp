#include "Controller.h"


// Sample update method for demonstration purposes.
void Controller::update(float elapsedTimeSeconds)
{
	cout << "controller update " << elapsedTimeSeconds << endl;

} // end update

vec3 Controller::getPos(){
	return vec3(0.0f,0.0f,0.0f);
}