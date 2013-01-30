#include <stdlib.h>
#include <stdio.h>

#ifndef __APPLE__
#define GLEW
#endif

#ifdef GLEW
#include <GL/glew.h>
#else
#include <OpenGL/gl3.h>
#endif

#ifndef __APPLE__
#include <GL/glu.h>
#include <GL/glut.h>
#else
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#endif

#include <math/vector.hpp>
#include <math/matrix.hpp>
#include <cmath>

const float pi = acos(-1.0f);

#include "LoadShaders.h"

#define BUFFER_OFFSET(offset)  ((void *)(offset))

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, ColorBuffer, TransformBuffer, NumBuffers };

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

const GLuint NumVertices = 3;

void init()
{
	glGenVertexArrays(NumVAOs, VAOs);
	glBindVertexArray(VAOs[Triangles]);

	glGenBuffers(NumBuffers, Buffers);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
	GLfloat vertices[NumVertices][4] = {
		{ 0.0f, 1.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f, 1.0f }
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
		GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ColorBuffer]);
	GLfloat colors[2][4] = {
		{ 1.0f, 0.0f, 0.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f, 1.0f }
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors,
		GL_STATIC_DRAW);


	/* 	float theta = pi / 4.0f; 
		mat4<float> rotate(cos(theta), -sin(theta), 0.0f, 0.0f,
			sin(theta), cos(theta), 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
*/
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[TransformBuffer]);
	GLfloat translate[4*2][4] = {
 		{ 2.0f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 2.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 2.0f, 0.0f },
		{ -1.0f, -1.0f, 0.0f, 1.0f },
 		{ 2.0f * cos(pi), 0.0f, 0.0f, 0.0f },
		{ 0.0f, 2.0f * cos(pi), 0.0f, 0.0f },
		{ 0.0f, 0.0f, 2.0f, 0.0f },
		{ 1.0f, 1.0f, 0.0f, 1.0f }
 	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(translate), translate,
		GL_STATIC_DRAW);

	GLchar const * vertexSource =
		"#version 410 core\r\n\
\r\n\
layout (location = 0) in vec4 position;\r\n\
\r\n\
layout (location = 1) in vec4 color;\r\n\
layout (location = 2) in mat4 transform;\r\n\
\r\n\
out vec4 diffuse;\r\n\
void main()\r\n\
{\r\n\
    gl_Position = transform * position;\r\n\
    diffuse = color;\r\n\
}\r\n\
";

	GLchar const * fragmentSource =
		"#version 410 core\r\n\
in vec4 diffuse;\r\n\
out vec4 fColor;\r\n\
\r\n\
void main()\r\n\
{\r\n\
    fColor = diffuse;\r\n\
}\r\n\
";

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, vertexSource },
		{ GL_FRAGMENT_SHADER, fragmentSource },
		{ GL_NONE, NULL }
	};

	GLuint program = LoadShadersFromString(shaders);

	glUseProgram(program);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
	GLint positionLocation = glGetAttribLocation(program, "position");
	glVertexAttribPointer(positionLocation, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));
	glEnableVertexAttribArray(positionLocation);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ColorBuffer]);
	GLint colorLocation = glGetAttribLocation(program, "color");
	glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));
	glEnableVertexAttribArray(colorLocation);
	glVertexAttribDivisor(colorLocation, 1);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[TransformBuffer]);
	GLint transformLocation = glGetAttribLocation(program, "transform");
	for(size_t i = 0; i < 4; i++) {
		glVertexAttribPointer(transformLocation + i, 4, GL_FLOAT, GL_FALSE,
			sizeof(GLfloat[4][4]), BUFFER_OFFSET(sizeof(GLfloat[4]) * i));
		glEnableVertexAttribArray(transformLocation + i);
		glVertexAttribDivisor(transformLocation + i, 1);
	}

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(VAOs[Triangles]);
	glDrawArraysInstanced(GL_TRIANGLES, 0, NumVertices, 2);

	glFlush();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(512,512);

#ifndef __APPLE__
	//glutInitContextVersion(3, 2);
	//glutInitContextProfile(GLUT_CORE_PROFILE);
#endif
	glutCreateWindow("HelloOpenGL");

#ifdef GLEW
	if(glewInit()) {
		printf("Unable to initialize GLEW ... exiting\r\n");
		exit(EXIT_FAILURE);
	}
    
    if(!GL_VERSION_3_0) {
        printf("OpenGL 3.0 not supported.\r\n");
        exit(EXIT_FAILURE);
    }
#endif

	init();

	glutDisplayFunc(display);

	glutMainLoop();
}
