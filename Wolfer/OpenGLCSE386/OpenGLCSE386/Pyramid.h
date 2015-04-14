#ifndef ___PYRAMID_H___
#define ___PYRAMID_H___

#include "visualobject.h"
#include "LoadShaders.h"

class Pyramid : public VisualObject
{
	public:

		/**
		 * Constructor for the Pyramid class. Created a visual object in the shape of a pyramid. Vertex
		 * positions are expressed relative to an origin that is located in the center of the pyramid.
		 * @param OpenGLApp - reference to the OpenGLApplicationBase object that holds the projection
		 *        and viewing transformations
		 * @param w - width of the base of the pyramid. Default is 1 unit.
		 * @param h - height of the pyramid. Default is 1 unit.
		 */
		Pyramid(OpenGLApplicationBase * OpenGLApp, GLfloat w = 1.0f, GLfloat h = 1.0f);

		/**
		 * Builds shader program and initializes vertex array object that will be used to render the 
		 * the object.
		 */
		virtual void initialize();

		/*
		 * Renders the object using the visual object using the vertex array object that was created by
		 * the initialize method.
		 */
		virtual void draw();

		/**
		 * Updates modeling transformations as necessary based on time. Set the modeling, viewing, and
		 * projection transformations in the the shader program.
		 */
		virtual void update(float elapsedTimeSeconds);

	protected:

		/**
		 * Builds the shader program.
		 */
		void buildShaders();
		
		int numberOfIndices; // number of indices to be used during indexed rendering.
		
		GLuint ShaderProgram; // Identifier for the shader program

		GLfloat height; // height for the pyramid
		GLfloat width; // width for the pyramid
	
		// location in the shader of the projection matrix uniform variable
		GLuint projectionLocation; 

		// location in the shader of the model/view matrix uniform variable
		GLuint modelViewLocation;

};

#endif // ___PYRAMID_H___