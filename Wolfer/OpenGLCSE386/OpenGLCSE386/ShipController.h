#pragma once
#include "Controller.h"

const static float BUOYANCY_VELOCITY_VAL = 0.1f;
const static float BUOYANCY_HEIGHT_VAL = 0.5f;
const static float BUOYANCY_CUTTING_POINT = 0.08f;

class ShipController :
	public Controller
{
public:
	ShipController(vec3 pos, vec3 vel);
	~ShipController(void);
	void useBuoyancy(float elapsedTimeSeconds);
	void update(float elapsedTimeSeconds);
	virtual vec3 getPos(void);
	

protected:
	


	// Position at which the object currently resides
	vec3 position; 
 
	// axis of rotation 
	vec3 velocity; 



};

