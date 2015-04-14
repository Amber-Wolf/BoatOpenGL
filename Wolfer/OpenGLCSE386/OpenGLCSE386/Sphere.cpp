#include "Sphere.h"

#include <vector>

#define PI 3.1415926535897932384626433832795f

Sphere::Sphere( OpenGLApplicationBase * OpenGLApp, float Radius, int Resolution, vec4 color)
	:VisualObject(OpenGLApp), Radius(Radius), Resolution(Resolution),numberOfIndices(0), objectColor(color)
{

}


void Sphere::CompileShaders()
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


// Construct visual object display list.
void Sphere::initialize()
{
	CompileShaders();

	// vector containers to hold  data
	// vertice positions
	vector<vec3> v;

	vector<vec3> n;

	GLuint VBO, NBO, IBO;

	// Vector contains indexes to the data
	vector<unsigned int> indices;

	// Declare the variables we are going to use
	float X1,Y1,X2,Y2,Z1,Z2;
	float inc1,inc2,inc3,inc4, Radius1, Radius2;

	for(int w = 0; w < Resolution; w++) {
		
		for(int h = (-Resolution/2); h < (Resolution/2); h++){

			inc1 = (w/(float)Resolution) * 2 * PI;
			inc2 = ((w+1)/(float)Resolution) * 2 * PI;
 
			inc3 = (h/(float)Resolution) * PI;
			inc4 = ((h+1)/(float)Resolution) * PI;

			X1 = sin(inc1);
			Y1 = cos(inc1);
			X2 = sin(inc2);
			Y2 = cos(inc2);

			// store the upper and lower radius, remember everything is going to be drawn as triangles
			Radius1 = Radius*cos(inc3);
			Radius2 = Radius*cos(inc4);

			Z1 = Radius*sin(inc3); 
			Z2 = Radius*sin(inc4);

			// insert the triangle coordinates
			v.push_back( vec3( Radius1*X1, Z1, Radius1*Y1));
			v.push_back( vec3( Radius1*X2, Z1, Radius1*Y2));
			v.push_back( vec3( Radius2*X2, Z2, Radius2*Y2));

			v.push_back( vec3( Radius1*X1, Z1, Radius1*Y1));
			v.push_back( vec3( Radius2*X2, Z2, Radius2*Y2));
			v.push_back( vec3( Radius2*X1, Z2, Radius2*Y1));
			
			n.push_back( vec3(X1,Z1,Y1) / length( vec3(X1,Z1,Y1) ));
			n.push_back( vec3(X2,Z1,Y2) / length( vec3(X2,Z1,Y2) ));
			n.push_back( vec3(X2,Z2,Y2) / length( vec3(X2,Z2,Y2) ));

			n.push_back( vec3(X1,Z1,Y1) / length( vec3(X1,Z1,Y1) ));
			n.push_back( vec3(X2,Z2,Y2) / length( vec3(X2,Z2,Y2) ));
			n.push_back( vec3(X1,Z2,Y1) / length( vec3(X1,Z2,Y1) ));
		}
	}

	for ( unsigned int i=0; i < v.size(); i++ ) {

		indices.push_back( i );
	}

	glGenVertexArrays (1, &vertexArrayObject);
	glBindVertexArray( vertexArrayObject );

	// finally, create the buffers and bind the data to them
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Buffer for vertex data
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(vec3), &v[0], GL_STATIC_DRAW); //Buffering vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	
	glGenBuffers(1, &NBO);
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, n.size() * sizeof(vec3), &n[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// Generate a buffer for the indices as well 
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0] , GL_STATIC_DRAW);

	// store the number of indices for later use
	numberOfIndices = indices.size();
 
	v.clear();
	n.clear();
	indices.clear();

	VisualObject::initialize();

} // end initialize


// Preform drawing operations
void Sphere::draw()
{
	glUseProgram(ShaderProgram);
	glBindVertexArray(vertexArrayObject);
    glDrawElements(GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, 0);

	// Draw all children
	VisualObject::draw();

} // end draw


void Sphere::update(float elapsedTimeSeconds)
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

} // end update
