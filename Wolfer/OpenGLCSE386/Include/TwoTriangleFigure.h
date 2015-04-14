#pragma once
#include "visualobject.h"

class TwoTriangleFigure : public VisualObject
{
	public:
		TwoTriangleFigure( OpenGLApplicationBase * OpenGLApp );
		~TwoTriangleFigure(void);

		void CompileShaders();

		virtual void initialize();

		virtual void draw();

	protected:

		GLuint ShaderProgram;

		GLuint gWorldLocation;
		GLuint VBO;
		GLuint CBO;
}; 


