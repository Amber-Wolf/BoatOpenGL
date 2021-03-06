#ifndef ___VISUAL_OBJECT_H___
#define ___VISUAL_OBJECT_H___

#include <math.h>
#include <iostream>

using namespace std;

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>

#define GLM_SWIZZLE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/swizzle.hpp>
#include <glm/gtc/constants.hpp>

using namespace glm;

#include "LoadShaders.h"
#include "OpenGLApplicationBase.h"
#include "Controller.h"

// Base class for all visual objects that will appear in the scene.
class VisualObject 
{
public: 

	// Give method of the Controller class and subclasses
	// access to the private members of this object
	friend class Controller;

	// Default constructor for early labs
	VisualObject();

	virtual~VisualObject(); 


	// Constructor which sets the reference to the OpenGLApplicationBase
	// object that made the VisualObject and holds the 
	VisualObject(OpenGLApplicationBase * OpenGLApp);

	// Buffer all the data that is associated with the vertex 
	// processor for this object. Creates a Vertex Array Object that
	// holds references to the vertex buffers, the index buffer and 
	// the layout specification. 
	virtual void initialize();

	// Render the object and all of its children
	virtual void draw();

	// Update the object and all of its children
	virtual void update(float elapsedTimeSeconds);

	// Set the Controller object for this object
	virtual void addController(Controller * controller );

	//// Remove the Controller object for this object (untested)
	//virtual void removeController();

	// Add a VisualObject child to this object
	virtual void addChild( VisualObject * child );

	// Remove a specified child VisualObject. Returns a reference
	// to the removed object of NULL if the object is not found.
	virtual VisualObject* removeChild( int objectSerialNumber );

	// Detach this VisualObject from its parent
	virtual bool detachFromParent( );

	virtual VisualObject* getParent() { return parent; };

	void setUpVisualObject(OpenGLApplicationBase * OpenGLApp); 

	virtual mat4 getParentModelMatrix(); 

	// Modeling transform for this VisualObject
	// Should be protected. Temporarily left public for demonstration 
	// purposes
	mat4 modelMatrix;

	// Reference to a controller object which is responsible for manipulating
	// the the modeling transformation and other properties of this object 
	// over time.
	Controller* controller;


	virtual Controller* removeController(); 
 
	virtual bool removeAndDeleteController( ); 
 
	virtual bool removeAndDeleteChild( int childID ); 

	virtual Controller * getController(){ return controller;} 

	mat4 fixedTransformation; 
protected:

	// Encapsulates all the data that is associated with the vertex 
	// processor for this object. Instead of containing the actual data, 
	// it holds references to the vertex buffers, the index buffer and 
	// the layout specification of the vertex itself. 
	GLuint vertexArrayObject; 

	// Reference to the OpenGLApplication object that
	// created this object and holds information such as the
	// currect projection and view transformation matrices.
	OpenGLApplicationBase* OpenGLAPP;

	// Maintains a count of all VisualObject instantiated.
	// Shared by all VisualObjects.
	static int objectCounter;

	// Unique ID for this VisualObject instance
	const int objectSerialNumber;

	// Reference to the VisualObject parent of this VisualObject
	VisualObject* parent;

	// Vector containing references to all children of this node
	vector<VisualObject*> children;

}; // end VisualObject class


#endif /* ___VISUAL_OBJECT_H___ */


