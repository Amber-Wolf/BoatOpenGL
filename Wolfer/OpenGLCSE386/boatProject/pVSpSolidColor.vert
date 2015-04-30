// Targeting version 1.30 of GLSL. If the compiler does not support 
// 1.30 it will cause an error.
#version 130

// Declares that a vertex specific attribute which is a vector of 3 floats
// will be known as 'Position' in the shader. 'Vertex specific' means that 
// for every invocation of the shader in the GPU the value of a new vertex 
// from the buffer will be supplied. The first section of the statement, 
// layout (location = 0), creates the binding between the attribute name 
// and attribute in the buffer. This is required for cases where the vertex 
// contains several attributes (position, normal, texture coordinates, etc).

in vec3 vertexPosition; 

out vec4 vertexColor0;

uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;

uniform vec4 objectColor;


// There can only be one main function for each shader stage
void main()
{
	// 'gl_Position' is a special built in variable that is supposed to 
	// contain the homogeneous (containing X, Y, Z and W components) of the vertex 
	// position. The rasterizer will look for that variable and will use it 
	// as the position in screen space (following a few more transformations).
    gl_Position = ProjectionMatrix * ModelViewMatrix * vec4(vertexPosition, 1.0f);

	vertexColor0 = objectColor;

}