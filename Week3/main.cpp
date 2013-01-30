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

#include "LoadShaders.h"

#define BUFFER_OFFSET(offset)  ((void *)(offset))

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

const GLuint NumVertices = 3;

void init()
{
	glGenVertexArrays(NumVAOs, VAOs);
	glBindVertexArray(VAOs[Triangles]);

	GLfloat vertices[NumVertices][2] = {
		{ 0.0f, 1.0f }, // Triangle 1
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f }
	};

	glGenBuffers(NumBuffers, Buffers);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
		GL_STATIC_DRAW);

	GLchar const * vertexSource =
		"#version 120\r\n\
\r\n\
void main()\r\n\
{\r\n\
    gl_Position = vPosition;\r\n\
}\r\n\
";

	GLchar const * fragmentSource =
		"#if __VERSION__ >= 140\r\n\
out vec4 fColor;\r\n\
#endif\r\n\
\r\n\
void main()\r\n\
{\r\n\
#if __VERSION__ >= 140\r\n\
    fColor = vec4(0.0, 0.0, 0.0, 1.0);\r\n\
#else\r\n\
    gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);\r\n\
#endif\r\n\
}\r\n\
";

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, vertexSource },
		{ GL_FRAGMENT_SHADER, fragmentSource },
		{ GL_NONE, NULL }
	};

	GLuint program = LoadShadersFromString(shaders);
	glUseProgram(program);

	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	glEnableVertexAttribArray(vPosition);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(VAOs[Triangles]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	glFlush();
}

void test()
{
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
