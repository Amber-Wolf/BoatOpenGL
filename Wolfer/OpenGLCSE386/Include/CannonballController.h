#pragma once
#include "Controller.h"
class CannonballController :
	public Controller
{
public:
	CannonballController(vec3 pos, vec3 vel);
	~CannonballController(void);
	void update(float elapsedTimeSeconds);
	bool getLive();
	vec3 getPos(void);
	

protected:
	
	BOOLEAN live;

	// Position at which the object currently resides
	vec3 position; 
 
	// axis of rotation 
	vec3 velocity; 



};
