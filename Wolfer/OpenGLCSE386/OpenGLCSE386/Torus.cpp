#include "Torus.h"

#include <vector>
#include <math.h>
#define M_PI 3.1415926535897932384626433832795f
#include "vertexStructs.h"

Torus::Torus( OpenGLApplicationBase * OpenGLApp, float innerRadius, float outerRadius, vec4 color, int sides, int rings)
	:VisualObject(OpenGLApp), objectColor(color)
{
	dInnerRadius =  innerRadius;
	dOuterRadius = outerRadius;
	nSides = sides; 
	nRings = rings;
}

Torus::~Torus(void)
{
	glDeleteVertexArrays (1, &vertexArrayObject);
}

void Torus::CompileShaders()
{
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "pVSpSolidColor.vert" },
		{ GL_FRAGMENT_SHADER, "pFSc.frag"},
		{ GL_NONE, NULL } // signals that there are no more shaders
	};

	// Read the files and create the OpenGL shader program.
	ShaderProgram = LoadShaders(shaders);

	// This program will stay in effect for all draw calls until it is 
	// replaced with another or explicitly disabled (and the 
	// fixed function pipeline is enabld) by calling glUseProgram with NULL
    glUseProgram(ShaderProgram);

	projectionLocation = glGetUniformLocation(ShaderProgram, "ProjectionMatrix");
	assert(projectionLocation != 0xFFFFFFFF);

	modelViewLocation = glGetUniformLocation(ShaderProgram, "ModelViewMatrix");
	assert(modelViewLocation != 0xFFFFFFFF);

	solidColorLocation = glGetUniformLocation(ShaderProgram, "objectColor");
	assert(solidColorLocation != 0xFFFFFFFF);
}


void Torus::initialize()
{
	CompileShaders();

	VisualObject::initialize();

} // end initialize


// Preform drawing operations
void Torus::draw()
{
	glUseProgram(ShaderProgram);

	glutSolidTorus( dInnerRadius, dOuterRadius, nSides, nRings);

	// Draw all children
	VisualObject::draw();

} // end draw


void Torus::update(float elapsedTimeSeconds)
{
	mat4 Projection = OpenGLAPP->getProjectionMatrix();
	mat4 View = OpenGLAPP->getViewMatrix();

	if (controller != NULL ) {
		controller->update( elapsedTimeSeconds );
	}

	mat4 modelView = View * getParentModelMatrix() * modelMatrix * fixedTransformation;

	glUseProgram(ShaderProgram);

	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, value_ptr(Projection));
	glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, value_ptr(modelView));

	glUniform4fv(this->solidColorLocation, 1, value_ptr(objectColor));
	
	// Update all children
	VisualObject::update( elapsedTimeSeconds );
}
