#include "Cylinder.h"

#include <vector>
#include <math.h>
#define M_PI 3.1415926535897932384626433832795f
#include "vertexStructs.h"

Cylinder::Cylinder( OpenGLApplicationBase * OpenGLApp, float height, float radius, vec4 color, int stacks, int slices)
	:VisualObject(OpenGLApp),stacks(stacks), slices(slices), height(height), radius(radius) , objectColor(color)
{}

Cylinder::~Cylinder(void)
{
	glDeleteVertexArrays (1, &vertexArrayObjectForBottom);
	glDeleteVertexArrays (1, &vertexArrayObjectForTop);
}

void Cylinder::CompileShaders()
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


void Cylinder::initialize()
{
	CompileShaders();
	
	initializeCylinderBottom();
	initializeCylinderBody();
	initializeCylinderTop();
	
	VisualObject::initialize();

} // end initialize

void Cylinder::initializeCylinderBody()
{
    // Step in z and radius as stacks are drawn.
    double z0,z1;
    const double zStep = height / ( ( stacks > 0 ) ? stacks : 1 );
	double angleStep = 2.0 * M_PI / slices;

	// vector containers to hold  data
	vector<pnVertexData> v; // vertex positions
	vector<unsigned int> indices; // indices
	GLuint VBO, IBO; // Identifiers for buffer objects

	// Do the stacks */
	z0 = 0.0;
    z1 = zStep;
	GLuint currentIndex = 0;

    for ( int i=1; i<=stacks; i++)
    {
        if (i==stacks) {
            z1 = height;
		}

        for (int j=0; j < slices; j++ )
        {
            vec3 n01 = vec3(cos(j* angleStep), sin(j* angleStep), 0.0 );
            
			vec3 v0 = vec3(cos(j * angleStep)*radius, sin(j* angleStep)*radius, z0  );
			v.push_back( pnVertexData( v0, n01) ); // 0
			
			vec3 v1 = vec3(cos(j * angleStep)*radius, sin(j* angleStep)*radius, z1  );
			v.push_back( pnVertexData( v1, n01) ); // 1

			vec3 n23 = vec3(cos((j+1)* angleStep), sin((j+1)* angleStep), 0.0 );

            vec3 v2 = vec3(cos((j+1) * angleStep)*radius, sin((j+1)* angleStep)*radius, z0  );
			v.push_back( pnVertexData( v2, n23) ); // 2

            vec3 v3 = vec3(cos((j+1) * angleStep)*radius, sin((j+1)* angleStep)*radius, z1  );
			v.push_back( pnVertexData( v3, n23) ); // 3

			indices.push_back(currentIndex);
			indices.push_back(currentIndex + 2);
			indices.push_back(currentIndex + 3);

			indices.push_back(currentIndex);
			indices.push_back(currentIndex + 3);
			indices.push_back(currentIndex + 1);

			currentIndex += 4;
        }

        z0 = z1; 
		z1 += zStep;
    }

	glGenVertexArrays (1, &vertexArrayObject);
	glBindVertexArray( vertexArrayObject );

	// finally, create the buffer to hold interleaved  and bind the data to them
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Buffer for vertex data
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(pnVertexData), &v[0], GL_STATIC_DRAW); //Buffering vertex data

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(pnVertexData), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(pnVertexData), (const GLvoid*)sizeof(vec3) );
	glEnableVertexAttribArray(1);

	// Generate a buffer for the indices
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0] , GL_STATIC_DRAW);

	// store the number of indices for later use
	bodyIndicesCount = indices.size();
 
	v.clear();
	indices.clear();

} // end initializeCylinderBody


void Cylinder::initializeCylinderBottom()
{
	double angleStep = 2.0 * M_PI / slices;

	// vector containers to hold  data
	vector<pnVertexData> v; // vertex positions
	vector<unsigned int> indices; // indices
	GLuint VBO, IBO; // Identifiers for buffer objects

	GLuint currentIndex = 0;

    vec3 normal = vec3(0.0, 0.0, -1.0 );
	v.push_back( pnVertexData( vec3(0.0, 0.0,  0.0 ), normal) ); 
	indices.push_back(currentIndex++);

    for (int j = slices; j >=  0; j--) {

		v.push_back( pnVertexData( vec3( cos(j * angleStep) * radius, sin(j * angleStep)*radius, 0.0f ), normal) ); 
		indices.push_back(currentIndex++);
	}

	glGenVertexArrays (1, &vertexArrayObjectForBottom);
	glBindVertexArray( vertexArrayObjectForBottom );

	// finally, create the buffer to hold interleaved  and bind the data to them
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Buffer for vertex data
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(pnVertexData), &v[0], GL_STATIC_DRAW); //Buffering vertex data

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(pnVertexData), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(pnVertexData), (const GLvoid*)sizeof(vec3) );
	glEnableVertexAttribArray(1);

	// Generate a buffer for the indices
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0] , GL_STATIC_DRAW);

	// store the number of indices for later use
	bottomIndicesCount = indices.size();

	v.clear();
	indices.clear();

} // end initializeCylinderBottom


void Cylinder::initializeCylinderTop()
{
	double angleStep = 2.0 * M_PI / slices;

	// vector containers to hold  data
	vector<pnVertexData> v; // vertex positions
	vector<unsigned int> indices; // indices
	GLuint VBO, IBO; // Identifiers for buffer objects

	GLuint currentIndex = 0;

    vec3 normal = vec3(0.0, 0.0, 1.0 );
	v.push_back( pnVertexData( vec3(0.0, 0.0,  height ), normal) ); 
	indices.push_back(currentIndex++);

    for (int j = 0; j <= slices; j++) {

		v.push_back( pnVertexData( vec3( cos(j * angleStep) * radius, sin(j * angleStep)*radius, height ), normal) ); 
		indices.push_back(currentIndex++);
	}

	glGenVertexArrays (1, &vertexArrayObjectForTop);
	glBindVertexArray( vertexArrayObjectForTop);

	// finally, create the buffer to hold interleaved  and bind the data to them
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Buffer for vertex data
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(pnVertexData), &v[0], GL_STATIC_DRAW); //Buffering vertex data

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(pnVertexData), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(pnVertexData), (const GLvoid*)sizeof(vec3) );
	glEnableVertexAttribArray(1);

	// Generate a buffer for the indices
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0] , GL_STATIC_DRAW);

	// store the number of indices for later use
	topIndicesCount = indices.size();

	v.clear();
	indices.clear();

} // end initializeCylinderTop


// Preform drawing operations
void Cylinder::draw()
{
	glUseProgram(ShaderProgram);

	// Draw body
	glBindVertexArray(vertexArrayObject);
    glDrawElements(GL_TRIANGLES, bodyIndicesCount, GL_UNSIGNED_INT, 0);
	
	// Draw bottom
	glBindVertexArray(vertexArrayObjectForBottom);
    glDrawElements(GL_TRIANGLE_FAN, bottomIndicesCount, GL_UNSIGNED_INT, 0);

	// Draw top
	glBindVertexArray(vertexArrayObjectForTop);
    glDrawElements(GL_TRIANGLE_FAN, topIndicesCount, GL_UNSIGNED_INT, 0);

	// Draw all children
	VisualObject::draw();

} // end draw


void Cylinder::update(float elapsedTimeSeconds)
{
	mat4 Projection = OpenGLAPP->getProjectionMatrix();
	mat4 View = OpenGLAPP->getViewMatrix();

	if (controller != NULL ) {
		controller->update( elapsedTimeSeconds );
	}

	mat4 modelView = View * modelMatrix * fixedTransformation;

	glUseProgram(ShaderProgram);

	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, value_ptr(Projection));
	glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, value_ptr(modelView));

	glUniform4fv(this->solidColorLocation, 1, value_ptr(objectColor));
	
	// Update all children
	VisualObject::update( elapsedTimeSeconds );
}
