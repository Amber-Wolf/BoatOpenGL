#include "Cone.h"

#include <vector>
#include <math.h>
#define M_PI 3.1415926535897932384626433832795f
#include "vertexStructs.h"



Cone::Cone( OpenGLApplicationBase * OpenGLApp, float height, float base, vec4 color, int numMajor, int numMinor)
	:VisualObject(OpenGLApp),stacks(numMajor), slices(numMinor), height(height), base(base) , objectColor(color)
{}

Cone::~Cone(void)
{
	glDeleteVertexArrays (1, &vertexArrayObjectForBottom);
}

void Cone::CompileShaders()
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


void Cone::initialize()
{
	CompileShaders();
	
	initializeConeBottom();
	initializeConeBody();

	VisualObject::initialize();

} // end initialize


void Cone::initializeConeBody()
{
    // Step in z and radius as stacks are drawn.
    double z0,z1;
	double r0, r1;

	const double angleStep = 2.0 * M_PI / slices;
	const double zStep = height / ( ( stacks > 0 ) ? stacks : 1 );
    const double rStep = base / ( ( stacks > 0 ) ? stacks : 1 );

	// Scaling factors for vertex normals 
    const double cosn = ( height / sqrt ( height * height + base * base ));
    const double sinn = ( base   / sqrt ( height * height + base * base ));

	// vector containers to hold  data
	vector<pnVertexData> v; // vertex positions
	vector<unsigned int> indices; // indices
	GLuint VBO, IBO; // Identifiers for buffer objects

	// Do the stacks
	z0 = 0.0;
    z1 = zStep;

    r0 = base;
    r1 = r0 - rStep;

	GLuint currentIndex = 0;

	for( int i = 0; i < stacks-1; i++ )
    {
        for( int j=0; j < slices; j++)
        {
            vec3 n01 = vec3(cos(j* angleStep) * cosn, sin(j* angleStep)*cosn, sinn);

            vec3 v0 = vec3(cos(j* angleStep) * r0,   sin(j* angleStep)*r0,   z0  );
			v.push_back( pnVertexData( v0, n01) ); // 0
            vec3 v1 = vec3(cos(j* angleStep) * r1,   sin(j* angleStep)*r1,   z1  );
			v.push_back( pnVertexData( v1, n01) ); // 1

			vec3 n23 = vec3(cos((j+1)* angleStep) * cosn, sin((j+1)* angleStep) * cosn, sinn);

            vec3 v2 = vec3(cos((j+1)* angleStep) * r0,   sin((j+1)* angleStep) * r0,   z0  );
			v.push_back( pnVertexData( v2, n23) ); // 2
            vec3 v3 = vec3(cos((j+1)* angleStep) * r1,   sin((j+1)* angleStep) * r1,   z1  );
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
        r0 = r1; 
		r1 -= rStep;
    }

	vec3 n = vec3(cos(0.0)*cosn, sin(0.0)*cosn, sinn);

	for (int j=0; j < slices; j++)
	{
		vec3 v0 = vec3( cos(j * angleStep) * r0, sin(j* angleStep) * r0, z0 );
		v.push_back( pnVertexData( v0, n) ); // 0
		vec3 v1 = vec3( 0, 0, height);
		v.push_back( pnVertexData( v1, n) ); // 1

		n = vec3 ( cos((j+1)* angleStep) * cosn, sin((j+1)* angleStep) * cosn, sinn );
		vec3 v2 = vec3( cos((j+1)* angleStep) * r0, sin((j+1)* angleStep)*r0, z0 );
		v.push_back( pnVertexData( v2, n) ); // 2

		indices.push_back(currentIndex);
		indices.push_back(currentIndex + 2);
		indices.push_back(currentIndex + 1);
		currentIndex += 3;
	}

	glGenVertexArrays (1, &vertexArrayObject);
	glBindVertexArray( vertexArrayObject );

	// Create the buffer to hold interleaved data and bind the data to them
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

} // end


void Cone::initializeConeBottom()
{
	double angleStep = 2.0 * M_PI / slices;

	// vector containers to hold  data
	vector<pnVertexData> v; // vertex positions
	vector<unsigned int> indices; // indices
	GLuint VBO, IBO; // Identifiers for buffer objects
	
	GLuint currentIndex = 0;

    vec3 n = vec3(0.0f,0.0f,-1.0f);
    v.push_back( pnVertexData( vec3( 0.0f, 0.0f, 0.0f ), n));
	indices.push_back(currentIndex++);

    for (int j = slices; j >= 0; j--) {			
		
		v.push_back( pnVertexData( vec3( cos( j* angleStep ) * base, sin( j* angleStep ) * base, 0.0f), n));
		indices.push_back(currentIndex++);
	}

	glGenVertexArrays (1, &vertexArrayObjectForBottom);
	glBindVertexArray( vertexArrayObjectForBottom );

	// Create the buffer to hold interleaved data and bind the data to them
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

} // end initializeConeBottom


// Preform drawing operations
void Cone::draw()
{
	glUseProgram(ShaderProgram);

	// Draw body
	glBindVertexArray(vertexArrayObject);
    glDrawElements(GL_TRIANGLES, bodyIndicesCount, GL_UNSIGNED_INT, 0);
	
	// Draw bottom
	glBindVertexArray(vertexArrayObjectForBottom);
	glDrawElements(GL_TRIANGLE_FAN, bottomIndicesCount, GL_UNSIGNED_INT, 0);

	// Draw all children
	VisualObject::draw();

} // end draw


void Cone::update(float elapsedTimeSeconds)
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

