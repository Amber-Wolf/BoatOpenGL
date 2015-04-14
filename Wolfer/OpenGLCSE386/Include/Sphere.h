#ifndef ___SPHERE_H___
#define ___SPHERE_H___

#include "visualobject.h"
#include "LoadShaders.h"


class Sphere : public VisualObject
{
	public:
		Sphere( OpenGLApplicationBase * OpenGLApp, float Radius = 1.0f, int Resolution = 20, vec4 color = vec4( 1.0f, 0.0f, 0.0f, 1.0f));
		
		virtual void initialize();

		virtual void draw();

		virtual void update(float elapsedTimeSeconds);

	protected:
	
		void CompileShaders();
		
		float Radius;
		int Resolution;

		GLuint vertexArrayObject; 

		// this variable will later be used to hold the number of indices
		int numberOfIndices;

		GLuint ShaderProgram;

		GLuint projectionLocation; 
		GLuint modelViewLocation;
		GLuint solidColorLocation;

		vec4 objectColor;
};

#endif /* ___SPHERE_H___ */

