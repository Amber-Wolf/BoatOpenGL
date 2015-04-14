#include "Ship.h"

#include <vector>

#include "vertexStructs.h"

Ship::Ship(OpenGLApplicationBase * OpenGLApp, GLfloat w, GLfloat h)
	:VisualObject( OpenGLApp ), height(h), width(w)
{ }

void Ship::buildShaders()
{
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "pVSpc.vert" },
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

	

	

} // end buildShaders


void Ship::initialize()
{
	buildShaders();

	live = true;

	GLuint VBO, IBO;

	glGenVertexArrays (1, &vertexArrayObject); 
	glBindVertexArray( vertexArrayObject );

	vector<VertexData> v;

    v.push_back( VertexData( vec3( -2.0f,  0.0f, 0.0f), vec4( 1.0f, 0.0f, 0.0f, 1.0f )));  //0
    v.push_back( VertexData( vec3(-1.5f, 0.0f, 0.5f), vec4( 0.0f, 1.0f, 0.0f, 1.0f )));
    v.push_back( VertexData( vec3(-1.5f, 0.0f, -0.5f), vec4( 0.0f, 0.0f, 1.0f, 1.0f )));  //2
    v.push_back( VertexData( vec3( -1.0f, -2.0f, 0.0f), vec4( 1.0f, 1.0f, 0.0f, 1.0f )));
    v.push_back( VertexData( vec3( 1.0f, -2.0f, 0.0f), vec4( 0.0f, 1.0f, 1.0f, 1.0f ))); //4
	v.push_back( VertexData( vec3( 1.5f, 0.0f, -1.0f), vec4( 0.0f, 0.0f, 1.0f, 1.0f )));  
    v.push_back( VertexData( vec3( 1.6f, 0.0f, -.5f), vec4( 1.0f, 1.0f, 0.0f, 1.0f ))); //6
    v.push_back( VertexData( vec3( 1.6f, 0.0f, .5f), vec4( 0.0f, 1.0f, 1.0f, 1.0f ))); //7
	v.push_back( VertexData( vec3( 1.5f, 0.0f, 1.0f), vec4( 0.0f, 1.0f, 1.0f, 1.0f ))); //8

 	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(VertexData), &v[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData),(const GLvoid*)sizeof(vec3));
	glEnableVertexAttribArray(2);

	vector<unsigned int> i;
	i.push_back(0);
	i.push_back(1);
	i.push_back(2);

	i.push_back(3);
	i.push_back(0);
	i.push_back(2);

	i.push_back(3);
	i.push_back(1);
	i.push_back(0);

	i.push_back(4);
	i.push_back(3);
	i.push_back(2);

	i.push_back(5);
	i.push_back(4);
	i.push_back(2);

	i.push_back(8);
	i.push_back(5);
	i.push_back(2);
	
	i.push_back(3);
	i.push_back(4);
	i.push_back(1);
	
	i.push_back(4);
	i.push_back(8);
	i.push_back(1);
	
	i.push_back(1);
	i.push_back(8);
	i.push_back(2);

	i.push_back(8);
	i.push_back(7);
	i.push_back(5);

	i.push_back(7);
	i.push_back(6);
	i.push_back(5);

	i.push_back(5);
	i.push_back(6);
	i.push_back(4);

	i.push_back(6);
	i.push_back(7);
	i.push_back(4);
	
	i.push_back(7);
	i.push_back(8);
	i.push_back(4);

	numberOfIndices = i.size() *sizeof( i );

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, i.size() * sizeof(unsigned int), &i[0], GL_STATIC_DRAW);

} // end initialize

// Preform drawing operations
void Ship::draw()
{
	glUseProgram(ShaderProgram);
	glBindVertexArray(vertexArrayObject);
    glDrawElements(GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, 0);

	// Draw all children
	VisualObject::draw();

} // end draw

void Ship::update(float elapsedTimeSeconds)
{
	mat4 Projection = OpenGLAPP->getProjectionMatrix();
	mat4 View = OpenGLAPP->getViewMatrix();

	if (controller != NULL ) {
		controller->update( elapsedTimeSeconds );
		if(controller->getPos().x > 50.0f){
			live = false;
		}
	}

	mat4 modelView = View * modelMatrix;

	glUseProgram(ShaderProgram);

	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, value_ptr(Projection));
	glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, value_ptr(modelView));

	// Update all children
	VisualObject::update( elapsedTimeSeconds );

} // end update

bool Ship::getLive(){
	return live;
}

bool Ship::getCollide(vec3 other){
	bool collide = false; 
	vec3 pos1 = getController()->getPos();
	vec3 pos2 = other;
	if(2 > sqrt(pow(pos1.x - pos2.x,2) + pow(pos1.y - pos2.y,2) + pow(pos1.z - pos2.z,2))){
		collide = true;
		live = false;
	}
	return collide;
}