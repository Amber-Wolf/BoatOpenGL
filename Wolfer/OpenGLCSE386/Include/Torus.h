#ifndef ___TORUS_H___
#define ___TORUS_H___

#include "visualobject.h"
#include "LoadShaders.h"


class Torus : public VisualObject
{
	public:
		Torus( OpenGLApplicationBase * OpenGLApp,  
			float innerRadius= 0.25f, float outerRadius = 1.0f, 
			vec4 color = vec4( 0.0f, 1.0f, 0.0f, 1.0f), 
			int sides = 8, int rings = 16);

		virtual ~Torus(void);
			
		virtual void initialize();

		virtual void draw();

		virtual void update(float elapsedTimeSeconds);

	protected:
	
		void CompileShaders();
		
		GLdouble dInnerRadius, dOuterRadius;
		GLint nSides, nRings;

		GLuint vertexArrayObject; 

		GLuint ShaderProgram;

		GLuint projectionLocation; 
		GLuint modelViewLocation;
		GLuint solidColorLocation;

		int indicesCount;

		vec4 objectColor;
};

#endif /* ___TORUS_H___ */

