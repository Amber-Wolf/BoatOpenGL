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

#include "ColoredPyramid.h"
#include "Floor.h"
#include "Controller.h"
#include "SpinnerController.h"
#include "OrbitController.h"
#include "WayPointController.h"

class bachmaerLab6 : public OpenGLApplicationBase
{
public:

	bachmaerLab6()
	{
		pyramid0 = new ColoredPyramid( this );
		pyramid0->addController(new SpinnerController(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), 25.0f));
		root.addChild(pyramid0);
		pyramid1 = new ColoredPyramid( this );
		pyramid1->addController(new SpinnerController(vec3(3.0f, 0.0, 0.0f), vec3(1.0f, 0.0f, 0.0f), 25.0f));
		root.addChild(pyramid1);
		pyramid2 = new ColoredPyramid( this, 2.0f, 2.0f );
		pyramid2->addController(new SpinnerController(vec3(-3.0f, 0.0, 0.0f), vec3(0.0f, 0.0f, 1.0f), 25.0f));
		root.addChild(pyramid2);
		pyramid3 = new ColoredPyramid( this );
		pyramid3->addController(new OrbitController(vec3(-10.0f, 0.0, 0.0f), vec3(0.0f, 0.0f, 1.0f), 25.0f));
		root.addChild(pyramid3);
		pyramid4 = new ColoredPyramid( this );
		pyramid4->addController(new WayPointController(1.0f));
		root.addChild(pyramid4);
		floor = new Floor(this);

		rotationX = 0.0f;
		rotationY = 0.0f;
		viewTranslate = -12.0f;
	};

	~bachmaerLab6()
	{
		delete pyramid0;
		delete pyramid1;
		delete pyramid2;
		delete pyramid3;
		delete pyramid4;
		delete floor;
	};


    virtual bool initialize()
    {
		glClearColor(0.5f, 0.5f, 1.0f, 0.0f);
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);

		glEnable( GL_DEPTH_TEST );

		setUpMenus();

		this->ViewMatrix = translate(mat4(1.0f), vec3( 0.0f, 0.0f,-12.0f ));

		T1 = translate(mat4(1.0f), vec3(3.0f, 0.0, 0.0f));

		T2 = translate(mat4(1.0f), vec3(-3.0f, 0.0, 0.0f));
		pyramid2->modelMatrix = rotate(T2, 45.0f, vec3(0.0f, 0.0f, 1.0f));

		T3 = translate(mat4(1.0f), vec3(10.0f, 0.0, 0.0f));
		R30 = rotate(mat4(1.0f), 65.0f, vec3(0.0f, 1.0f, 0.0f));
		pyramid3->modelMatrix = R30 * T3;

		P4 = vec3( 3.5f, -2.5f, 3.5f );
		pyramid4->modelMatrix = translate(mat4(1.0f), P4);
	
		floor->initialize();
		floor->modelMatrix = translate(mat4(1.0f), vec3( 0.0f, -3.0f, 0.0f ));

		root.initialize();

		return true;

    } // end initialize

	// Render scene objects
	virtual void drawScene()
	{ 
		root.draw();
		floor->draw();

	} // end drawScene

	void setViewPoint(  ) 
	{
		mat4 transView, rotateViewX, rotateViewY;

		transView = translate(mat4(1.0f), vec3( 0.0f, 0.0f, viewTranslate ));
		rotateViewX = rotate(mat4(1.0f), rotationX, vec3(1.0f, 0.0f, 0.0f)); 
		rotateViewY = rotate(mat4(1.0f), rotationY, vec3(0.0f, 1.0f, 0.0f)); 
				
		ViewMatrix = transView * rotateViewX * rotateViewY;

	} // end setViewPoint


	// Update scene objects inbetween frames
	virtual void updateScene( float elapsedTimeSec ) 
	{ 
		setViewPoint();
		static float rotation = 0.0f;

		//rotation += elapsedTimeSec * 25.0f;

		//pyramid0->modelMatrix = rotate(mat4(1.0f), fmod(rotation, 360.0f), vec3(0.0f, 1.0f, 0.0f)); //mat4(1.0f);
		//pyramid0->update( elapsedTimeSec);

		/*pyramid1->modelMatrix = rotate(T1, fmod(rotation, 360.0f), vec3(1.0f, 0.0f, 0.0f));
		pyramid1->update( elapsedTimeSec);

		pyramid2->modelMatrix = rotate(T2, fmod(rotation, 360.0f), vec3(0.0f, 0.0f, 1.0f));
		pyramid2->update( elapsedTimeSec);

		R30 = rotate(mat4(1.0f), fmod(-rotation, 360.0f), vec3(0.0f, 1.0f, 0.0f));
		R31 = rotate(mat4(1.0f), fmod(rotation * 4.0f, 360.0f), vec3(1.0f, 0.0f, 0.0f));
		pyramid3->modelMatrix = R30 * T3 * R31;
		pyramid3->update( elapsedTimeSec);

		static vec3 i = vec3(1.0f, 0.0f, 0.0); 
		static vec3 k = vec3(0.0f, 0.0f, 1.0); 

		static vec3 velocity = -k;
		static int leg = 0;

		switch (leg) {

			case 0:

				if (P4.z <= -3.5) {
					P4.z = -3.5;
					velocity = -i;
					leg = 1;
				}
				break;
			case 1:

				if (P4.x <= -3.5) {
					P4.x = -3.5;
					velocity = k;
					leg = 2;
				}
				break;
			case 2:

				if (P4.z >= 3.5) {
					P4.z = 3.5;
					velocity = i;
					leg = 3;
				}
				break;
			case 3:

				if (P4.x >= 3.5) {
					P4.x = 3.5;
					velocity = -k;
					leg = 0;
				}
				break;
			default:
				cout << "invalid leg" << endl;
				break;
		}
		
		P4 = P4 + velocity * elapsedTimeSec;
		pyramid4->modelMatrix = translate(mat4(1.0f), P4);
		pyramid4->update( elapsedTimeSec);*/
		
		floor->update(elapsedTimeSec);

		root.update(elapsedTimeSec);
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
		switch (Key) {
			case '1': 
				cout << "case 1 called" << endl;
				if (pyramid0->getParent() == NULL ) { 
					root.addChild(pyramid0); 
				}else { 
					pyramid0->detachFromParent(); 
				} 
				break; 			case '2':				if (pyramid1->getParent() == NULL ) { 
					root.addChild(pyramid1); 
				}else { 
					pyramid1->detachFromParent(); 
				} 
				break; 			case '3': 
				if (pyramid2->getParent() == NULL ) { 
					root.addChild(pyramid2); 
				}else { 
					pyramid2->detachFromParent(); 
				} 
				break; 			case '4': 
				if (pyramid3->getParent() == NULL ) { 
					root.addChild(pyramid3); 
				}else { 
					pyramid3->detachFromParent(); 
				} 
				break; 			case '5': 
				if (pyramid4->getParent() == NULL ) { 
					root.addChild(pyramid4); 
				}else { 
					pyramid4->detachFromParent(); 
				} 
				break; 
			case 'w': case 'W':
				viewTranslate++;
				break;
			case 's': case 'S':
				viewTranslate--;
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

	ColoredPyramid * pyramid0, * pyramid1, *pyramid2, *pyramid3, *pyramid4;
	mat4 T1, T2, T3, R30, R31, T4; 
	vec3 P4;
	Floor* floor;
	VisualObject root;
	GLfloat rotationX, rotationY, viewTranslate;

}; // end bachmaerLab6


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

	GLUTBaseCreateWindow( "CSE 386 Lab 6" );

    bachmaerLab6 pApp;
	
	pApp.initialize();

	GLUTBaseRunApplication(&pApp);

    return 0;

} // end main