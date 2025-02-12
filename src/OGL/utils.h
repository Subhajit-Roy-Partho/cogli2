/*
 * utils.h
 *
 *  Created on: Apr 13, 2020
 *      Author: lorenzo
 */

#ifndef OGL_UTILS_H_
#define OGL_UTILS_H_

#include <glad/gl.h>

#include <iostream>

inline GLenum glCheckError_(const char *file, int line) {
	GLenum errorCode;
	while((errorCode = glGetError()) != GL_NO_ERROR) {
		std::string error;
		switch(errorCode) {
		case GL_INVALID_ENUM:
			error = "INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			error = "INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			error = "INVALID_OPERATION";
			break;
		case GL_STACK_OVERFLOW:
			error = "STACK_OVERFLOW";
			break;
		case GL_STACK_UNDERFLOW:
			error = "STACK_UNDERFLOW";
			break;
		case GL_OUT_OF_MEMORY:
			error = "OUT_OF_MEMORY";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			error = "INVALID_FRAMEBUFFER_OPERATION";
			break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}

#define glCheckError() glCheckError_(__FILE__, __LINE__)

#endif /* OGL_UTILS_H_ */
