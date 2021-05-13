#include <iostream>

#include "glerror.h"

using namespace std;

const char * explain_gl_error(const GLenum errcode)
{
	const char * s = NULL;

	switch (errcode)
	{
	case GL_INVALID_ENUM: s = "An unacceptable value is specified for an enumerated argument. The offending function is ignored, having no side effect other than to set the error flag."; break;
	case GL_INVALID_VALUE: s = "A numeric argument is out of range. The offending function is ignored, having no side effect other than to set the error flag."; break;
	case GL_INVALID_OPERATION: s = "The specified operation is not allowed in the current state. The offending function is ignored, having no side effect other than to set the error flag. (Or you have called glGetError() inside of glBegin()...glEnd().)"; break;
	case GL_NO_ERROR: s = "No error has been recorded. "; break;
	case GL_STACK_OVERFLOW: s = "This function would cause a stack overflow. The offending function is ignored, having no side effect other than to set the error flag."; break;
	case GL_STACK_UNDERFLOW: s = "This function would cause a stack underflow. The offending function is ignored, having no side effect other than to set the error flag."; break;
	case GL_OUT_OF_MEMORY: s = "There is not enough memory left to execute the function. The state of OpenGL is undefined, except for the state of the error flags, after this error is recorded."; break;
	default: s = "Unknown OpenGL error."; break;
	}

	return s;
}

void gl_check_error(void)
{
	GLenum err = glGetError();

	if ( err != GL_NO_ERROR)
	{
		cerr << "OpenGL error detected: ";
		cerr << explain_gl_error(err) << endl;
    cerr << "Press ENTER to continue..." << endl;
		getchar();
	}
}
