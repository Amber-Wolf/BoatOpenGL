#ifndef ___CONE_H___
#define ___CONE_H___

#include "visualobject.h"
#include "LoadShaders.h"

class Cone : public VisualObject
{
	public:
		Cone( OpenGLApplicationBase * OpenGLApp, 
			float height = 2.0f, float base = 0.5f, 
			vec4 color = vec4( 1.0f, 1.0f, 0.0f, 1.0f), 
			int numMajor = 4, int numMinor = 16);

		virtual ~Cone(void);
			
		virtual void initialize();

		virtual void draw();

		virtual void update(float elapsedTimeSeconds);

	protected:
	
		void CompileShaders();

		void initializeConeBody();
		void initializeConeBottom();

		int stacks, slices;
		float height, base;

		GLuint vertexArrayObjectForBottom; 

		// this variable will later be used to hold the number of indices
		int bodyIndicesCount, bottomIndicesCount;

		GLuint ShaderProgram;

		GLuint projectionLocation; 
		GLuint modelViewLocation;
		GLuint solidColorLocation;

		vec4 objectColor;
};

#endif /* ___CONE_H___ */



