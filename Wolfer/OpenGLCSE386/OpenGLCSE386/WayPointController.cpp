#include "WayPointController.h"


WayPointController::WayPointController(GLfloat r, vec3 rot, vec3 pos)
	: rate(r), R(rot), P(pos){
	//target->modelMatrix = R1 * T;
	leg =0;
	i = vec3(1.0f, 0.0f, 0.0); 
	k = vec3(0.0f, 0.0f, 1.0);
	velocity = -k;
}

void WayPointController::update(float elapsedTimeSeconds){

		switch (leg) {

			case 0:

				if (P.z <= -3.5) {
					P.z = -3.5;
					velocity = -i;
					leg = 1;
				}
				break;
			case 1:

				if (P.x <= -3.5) {
					P.x = -3.5;
					velocity = k;
					leg = 2;
				}
				break;
			case 2:

				if (P.z >= 3.5) {
					P.z = 3.5;
					velocity = i;
					leg = 3;
				}
				break;
			case 3:

				if (P.x >= 3.5) {
					P.x = 3.5;
					velocity = -k;
					leg = 0;
				}
				break;
			default:
				cout << "invalid leg" << endl;
				break;
		}
		
		P = P + velocity * rate * elapsedTimeSeconds;
		target->modelMatrix = translate(mat4(1.0f), P) * rotate(mat4(1.0f), (-90.0f),R);

	//target->modelMatrix = rotate(positionTransform , fmod(rotation1, 360.0f), axis); //mat4(1.0f);
}