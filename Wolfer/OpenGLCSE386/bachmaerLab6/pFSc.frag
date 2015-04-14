// Targeting version 3.3 of GLSL. If the compiler does not support 
// 3.3 it will cause an error.
#version 130


in vec4 vertexColor0;

// The rasterizer runs over all the points that are inside
// the point, line, or triangle. For each point the fragment shader is executed. 

// Usually the job of the fragment shader is to determine the 
// color of the fragment (pixel). In addition, the fragment shader 
// can discard the pixel altogether or change its Z value (which will
// impact the result of subsequent Z test).
out vec4 FragColor;

void main()
{

    FragColor = vertexColor0; 
}