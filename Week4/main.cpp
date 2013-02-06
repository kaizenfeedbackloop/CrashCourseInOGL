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
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum RenderBuffer_IDs { Color, Depth, NumRenderBuffers };

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];
GLuint frameBuffer, renderBuffer[NumRenderBuffers];

const GLuint NumVertices = 5;

void initRenderBuffers()
{
	glGenRenderbuffers(NumRenderBuffers, renderBuffer);

	glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer[Color]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, 256, 256);

	glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer[Depth]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 256, 256);

	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);

	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
							  GL_RENDERBUFFER, renderBuffer[Color]);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
							  GL_RENDERBUFFER, renderBuffer[Depth]);

	glEnable(GL_DEPTH_TEST);
}

void init()
{
	initRenderBuffers();
	
	glGenVertexArrays(NumVAOs, VAOs);
	glBindVertexArray(VAOs[Triangles]);

	glGenBuffers(NumBuffers, Buffers);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);


	struct VertexData {
		vec4<GLfloat> position;
		vec4<GLubyte> color;
	};

	VertexData vertices[NumVertices] = {
		{ vec4<GLfloat>(0.0f,0.0f,0.0f,1.0f),vec4<GLubyte>(255,255,255,255) },
		{ vec4<GLfloat>(0.0f,0.9f,0.0f,1.0f),vec4<GLubyte>(255,0,0,255) },
		{ vec4<GLfloat>(0.7794f,-0.5f,0.0f,1.0f),vec4<GLubyte>(0,255,0,255) },
		{ vec4<GLfloat>(-0.7794f,-0.5f,0.0f,1.0f),vec4<GLubyte>(0,0,255,255) },
		{ vec4<GLfloat>(0.0f,0.9f,0.0f,1.0f),vec4<GLubyte>(255,0,0,255) }
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
		GL_STATIC_DRAW);

	GLchar const * vertexSource =
		"#version 410 core\r\n\
\r\n\
layout (location = 0) in vec4 position;\r\n\
layout (location = 1) in vec4 color;\r\n\
\r\n\
out vec4 diffuse;\r\n\
void main()\r\n\
{\r\n\
    gl_Position = position;\r\n\
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
	glVertexAttribPointer(
						  positionLocation,
						  4,
						  GL_FLOAT,
						  GL_FALSE,
						  sizeof(VertexData),
						  BUFFER_OFFSET(0)
						  );
	glEnableVertexAttribArray(positionLocation);

	GLint colorLocation = glGetAttribLocation(program, "color");
	glVertexAttribPointer(
						  colorLocation,
						  4,
						  GL_UNSIGNED_BYTE,
						  GL_TRUE /*normalize!!*/,
						  sizeof(VertexData),
						  BUFFER_OFFSET(sizeof(vec4<GLfloat>))
						  );
	glEnableVertexAttribArray(colorLocation);
}

void display()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
	glViewport(0, 0, 256, 256);

	/* Render... */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(VAOs[Triangles]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, NumVertices);

	glFlush();

	/* Present... */
	glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); /* Reset */

	glViewport(0,0,512,512);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBlitFramebuffer(0,0,255,255,0,256,255,511,
		GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glutSwapBuffers();
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
