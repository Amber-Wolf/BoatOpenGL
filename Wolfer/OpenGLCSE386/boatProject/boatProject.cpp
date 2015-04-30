//#include <math.h>
//#define GLM_MESSAGES
//#define GLM_FORCE_RADIANS

#define GLM_SWIZZLE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/swizzle.hpp>

using namespace glm;
 
#include "OpenGLApplicationBase.h"

#include "Cone.h"
#include "Cylinder.h"
#include "Sphere.h"
#include "Torus.h"
#include "ColoredPyramid.h"
#include "Floor.h"
#include "Controller.h"
#include "SpinnerController.h"
#include "OrbitController.h"
#include "WayPointController.h"
#include "Ship.h"
#include "ShipController.h"
#include "CannonballController.h"
#define PI 3.14159265

const float CANNON_SPEED = 30.0f;   //The speed of the cannonballs
const float SPAWN_SPEED = 4.0f;    //The rate at which enemy ships are spawned
const int INIT_SHIP = 3;   //The initial number of ships.

/*
*  Class is named after the DOOM you shall bring to your enemies!
*  Runs a program where you shoot enemy ships via the spacebar and arrow keys.
*/

class boatProject : public OpenGLApplicationBase
{
public:

	boatProject()
	{
		cannon = new Cylinder( this,1.0f,0.25f, vec4(0.7f,0.2f,0.2f,0.9f) );
		floor = new Floor(this, 100.0f);
		rotationX = 2.0f;
		rotationY = 0.0f;
		viewTranslate = -12.0f;
		cannonball = NULL;
		timeCount = 8.0f;
		sunkCount = 0;
		printToScreen = false;
	};

	~boatProject()
	{
		delete cannon;
		delete floor;
	};

	//Creates Ships at random coordinates.
	void createShip(){
		float startX = (rand() % 20) +35.0f;
		float startY = (rand() % 80) /100.0f;
		float startZ = (rand() %25) + 15.0f;
		float velX = ((rand() % 80) / 20.0f) + 0.5f;
		float velZ = ((rand() % 200) / 100.0f) - 1.0f; 
		Ship *ship = new Ship( this ); 
		ship->addController(new ShipController(vec3(-startX,startY,-startZ), vec3(velX,0.1f,velZ)));
		ship->initialize();
		RunningShips.push_back(ship);
	}

	//Updates all the ships
	void updateShips(float timeSec){
		unsigned int x = 0;
		Ship* ship = NULL;
		while(x < RunningShips.size()){
			ship = RunningShips.at(x);
			ship->update(timeSec);
			x++;
			if(cannonball != NULL){  //Checks for collisions, adds to the sunkCount if true.
				if(ship->getCollide(cannonball->getController()->getPos())){  //getCollide also sets ship to dead when there is a collision.
					sunkCount++;  
				}
			}
		}
	}

	//Draws all the ships
	void drawShips(){
		unsigned int x = 0;
		Ship* ship = NULL;
		while(x < RunningShips.size()){
			ship = RunningShips.at(x);
			if(ship->getLive()){  //Draw ship if it is alive, deletes it otherwise.
				ship->draw();
				x++;
			}else{
				RunningShips.erase(RunningShips.begin() + x,RunningShips.begin() + x + 1);  //remove ship.
				delete ship;
			}
		}
	}

	//Initializes starting variables.
    virtual bool initialize()
    {
		glClearColor(0.5f, 0.5f, 1.0f, 0.0f);
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);

		glEnable( GL_DEPTH_TEST );

		setUpMenus();

		cannon->initialize();

		this->ViewMatrix = translate(mat4(1.0f), vec3( 0.0f, 0.0f,-12.0f ));

		T1 = translate(mat4(1.0f), vec3(0.0f, -0.7, 10.0f));
		cannon->modelMatrix = T1;
	
		floor->initialize();
		floor->modelMatrix = translate(mat4(1.0f), vec3( 0.0f, -1.0f, 0.0f )); 
		//createShip();
		
		for(int r = 0; r < INIT_SHIP; r++){
			createShip();
		}
		return true;

    } // end initialize

	// Render scene objects
	virtual void drawScene()
	{ 
		floor->draw();
		cannon->draw();
		drawShips();

		if(cannonball != NULL){  //draws cannonball only if present.  (Attempting to draw NULL cannonball causes freezing!)
			cannonball->draw();
		}
		
	} // end drawScene

	void setViewPoint(  ) //Sets the viewpoint.
	{
		mat4 transView, rotateViewX, rotateViewY;

		transView = translate(mat4(1.0f), vec3( 0.0f, 0.0f, viewTranslate ));
		rotateViewX = rotate(mat4(1.0f), rotationX, vec3(1.0f, 0.0f, 0.0f)); 
		rotateViewY = rotate(mat4(1.0f), rotationY, vec3(0.0f, 1.0f, 0.0f)); 
				
		ViewMatrix = transView;// * rotateViewX * rotateViewY;

	} // end setViewPoint


	// Update scene objects inbetween frames
	virtual void updateScene( float elapsedTimeSec ) 
	{ 
		setViewPoint();
		static float rotation = 0.0f;

		floor->update(elapsedTimeSec);
		cannon->update(elapsedTimeSec);  //Update for visual object of cannon

		T1 = translate(mat4(1.0f), vec3(0.0f, -0.7, 10.0f));
		mat4 rotateX = rotate(mat4(1.0f), rotationX + 180.0f, vec3(1.0f, 0.0f, 0.0f)); 
		mat4 rotateY = rotate(mat4(1.0f), rotationY, vec3(0.0f, 1.0f, 0.0f)); 
		cannon->modelMatrix = T1 * rotateX * rotateY;  //Updates the cannon orientation

		updateShips(elapsedTimeSec);

		cannonball->update(elapsedTimeSec);

		timeCount += elapsedTimeSec;
		if(timeCount >= SPAWN_SPEED){  //The spawn counter- controls the creation of ships
			createShip();
			timeCount -= SPAWN_SPEED;
		}
		if((printToScreen)&&(cannonball->getController()->getPos().y < -5)){  //Controls the on console text.
			cout << "Enemies sunk: " << sunkCount << endl;
			printToScreen = false;
		}
	} 

	void SpecialKeyboardCB(int Key, int x, int y)
	{
		switch (Key) {
			case GLUT_KEY_RIGHT:
				rotationY++;
				break;
			case GLUT_KEY_LEFT:
				rotationY--;
				break;
			case GLUT_KEY_UP:
				rotationX++;
				break;
			case GLUT_KEY_DOWN:
				rotationX--;
				break;
			default:
				OpenGLApplicationBase::SpecialKeyboardCB(Key, x, y);
				break;
		}

	} // end SpecialKeyboardCB


	void KeyboardCB(unsigned char Key, int x, int y)
	{
		float xVal, yVal, zVal;
		switch (Key) {
			case '1': 
				break; 
			case '2':
				break; 
			case '3': 
			case '4': 
			case '5': 
			case 'w': case 'W':
				viewTranslate++;
				break;
			case 's': case 'S':
				viewTranslate--;
				break;
			case 'r': case 'R':  //Resets the score.  
				sunkCount = 0;
				cout << "Enemies sunk reset!" << endl;
				break;
			case ' ':   //Shoots a cannonball if there is no cannonball or the cannonball is sufficiently below sea-level.
				if((cannonball == NULL) || (cannonball->getController()->getPos().y < -5.0f)){
					delete cannonball;
					cannonball = new Sphere(this, 0.5f, 20, vec4((rand()%100/100.0f),(rand()%100/100.0f),(rand()%100/100.0f),1.0f));  //Makes cannonball
					xVal =  sin(rotationY * PI/180) * cos(rotationX * PI/180);
					yVal =  sin(rotationX * PI/180);
					zVal =  cos(rotationY * PI/180) * cos(rotationX * PI/180);
					cout << xVal << " " << yVal << " " << zVal << endl;
					cannonball->addController(new CannonballController(vec3(0.0f,-0.7f,10.0f), vec3(xVal,yVal,-zVal) * CANNON_SPEED));
					cannonball->initialize();
					printToScreen = true;
				}
				break;
			default:
				OpenGLApplicationBase::KeyboardCB(Key, x, y);
				break;
		}

	} // end KeyboardCB

	friend void mainMenu(int value);

	void setUpMenus()
	{	
		GLuint menu2id = createPolygonMenu();
		GLuint menu3id = createFrontFaceMenu();
		GLuint menu4id = createPointSizeMenu();
		GLuint menu5id = createAntialiasingMenu();

		// Create main menu
		topMenu = glutCreateMenu(mainMenu);
		glutAddSubMenu("Polygon Mode", menu2id); 
		glutAddSubMenu("Rendered Polygon Face ", menu3id);
		glutAddSubMenu("Point Size ", menu4id);
		glutAddSubMenu("Antialiasing ", menu5id);
		glutAddMenuEntry("Quit", 1); // Specify indentifier for "Quit"			  
		glutAttachMenu(GLUT_RIGHT_BUTTON); // Attach menu to right mouse button

	} //end setUpMenus

	VisualObject * pyramid0, * cannon, *pyramid2, *pyramid3, *pyramid4, *childSphere;
	mat4 T1, T2, T3, R30, R31, T4; 
	VisualObject* cannonball;
	vec3 P4;
	Floor* floor;
	VisualObject root;
	GLfloat rotationX, rotationY, viewTranslate;
	vector<Ship*> RunningShips;
	float timeCount;
	int sunkCount;
	bool printToScreen;

}; 


void mainMenu(int value)
{
	switch (value) {

		case(1):
			glutLeaveMainLoop(); // End the program
			break;
		default:
			cout << "Unknown Main Menu Selection!" << endl;
	}

} // end mainMenu



int main(int argc, char** argv)
{
    GLUTBaseInit(argc, argv);

	GLUTBaseCreateWindow( "PROJECT ONE" );

    projectOfDoom pApp;
	
	pApp.initialize();

	GLUTBaseRunApplication(&pApp);

    return 0;

} // end main
