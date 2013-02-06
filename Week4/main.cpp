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

#include <iostream>

const float pi = acos(-1.0f);

#include "LoadShaders.h"

#define BUFFER_OFFSET(offset)  ((void *)(offset))

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum RenderBuffer_IDs { Color, Depth, NumRenderBuffers };
enum YuvComponent_IDs { AllComponents, YComponent, UComponent,
						VComponent, NumYuvComponents };

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];
GLuint frameBuffer, renderBuffer[NumRenderBuffers];
GLuint YuvBuffers[NumYuvComponents];

const GLuint filterUBB = 0;

const GLuint NumVertices = 5;

void checkForError()
{
	GLenum error = glGetError();
	if(error != GL_NO_ERROR) {
		std::cerr << gluErrorString(error);
	}
}

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

void initYuvTransforms()
{
	const mat4<GLfloat> rgbToYuv(0.299f, 0.587f, 0.114f, 0.0f,
								 -0.14713f, -0.28886f, 0.436f, 0.0f,
								 0.615f, -0.51499f, -0.10001f, 0.0f,
								 0.0f, 0.0f, 0.0f, 1.0f);
	
	const mat4<GLfloat> yuvToRgb(1.0f, 0.0f, 1.13983f, 0.0f,
								 1.0f, -0.39465f, -0.58060f, 0.0f,
								 1.0f, 2.03211f, 0.0f, 0.0f,
								 0.0f, 0.0f, 0.0f, 1.0f);

	glGenBuffers(NumYuvComponents, YuvBuffers);

	/* All... */
	glBindBuffer(GL_UNIFORM_BUFFER, YuvBuffers[AllComponents]);
	checkForError();
	mat4<GLfloat> all(mat4<GLfloat>::Identity);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4<GLfloat>), &all,
		GL_STATIC_DRAW);
	checkForError();

	/* Y... */
	glBindBuffer(GL_UNIFORM_BUFFER, YuvBuffers[YComponent]);
	mat4<GLfloat> y =
		yuvToRgb *
		mat4<GLfloat>(1.0f, 0.0f, 0.0f, 0.0f,
					 0.0f, 0.0f, 0.0f, 0.0f,
					 0.0f, 0.0f, 0.0f, 0.0f,
					 0.0f, 0.0f, 0.0f, 1.0f) *
		rgbToYuv;
	glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4<GLfloat>), &y,
		GL_STATIC_DRAW);

	/* U... */
	glBindBuffer(GL_UNIFORM_BUFFER, YuvBuffers[UComponent]);
	mat4<GLfloat> u =
		yuvToRgb *
		mat4<GLfloat>(0.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f) *
		rgbToYuv;
	glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4<GLfloat>), &u,
		GL_STATIC_DRAW);

	/* V... */
	glBindBuffer(GL_UNIFORM_BUFFER, YuvBuffers[VComponent]);
	mat4<GLfloat> v =
		yuvToRgb *
		mat4<GLfloat>(0.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f) *
		rgbToYuv;
	glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4<GLfloat>), &v,
		GL_STATIC_DRAW);
}

void init()
{
	initRenderBuffers();

	initYuvTransforms();
	
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
layout (std140) uniform ColorConversion {\r\n\
mat4 filter;\r\n\
};\r\n\
in vec4 diffuse;\r\n\
out vec4 fColor;\r\n\
\r\n\
void main()\r\n\
{\r\n\
    fColor = filter * diffuse;\r\n\
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

	GLint filterUBI = glGetUniformBlockIndex(program, "ColorConversion");
	checkForError();
	glUniformBlockBinding(program, filterUBI, filterUBB);
	checkForError();
}

void drawToOffScreenFrameBuffer()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
	glViewport(0, 0, 256, 256);

	/* Render... */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(VAOs[Triangles]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, NumVertices);

	glFlush();

	/* Rest to Window Frame Buffer... */
	glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glViewport(0,0,512,512);
}

void display()
{
	/* Clear... */
	glViewport(0,0,512,512);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindBufferRange(GL_UNIFORM_BUFFER, filterUBB, YuvBuffers[AllComponents],
					  0, sizeof(mat4<GLfloat>));
	checkForError();
	drawToOffScreenFrameBuffer();
	glBlitFramebuffer(0,0,255,255,0,256,255,511,
		GL_COLOR_BUFFER_BIT, GL_NEAREST);

	glBindBufferRange(GL_UNIFORM_BUFFER, filterUBB, YuvBuffers[YComponent],
					  0, sizeof(mat4<GLfloat>));
	drawToOffScreenFrameBuffer();
	glBlitFramebuffer(0,0,255,255,0,0,255,255,
		GL_COLOR_BUFFER_BIT, GL_NEAREST);

	glBindBufferRange(GL_UNIFORM_BUFFER, filterUBB, YuvBuffers[UComponent],
					  0, sizeof(mat4<GLfloat>));
	drawToOffScreenFrameBuffer();
	glBlitFramebuffer(0,0,255,255,256,256,511,511,
		GL_COLOR_BUFFER_BIT, GL_NEAREST);

	glBindBufferRange(GL_UNIFORM_BUFFER, filterUBB, YuvBuffers[VComponent],
					  0, sizeof(mat4<GLfloat>));
	drawToOffScreenFrameBuffer();
	glBlitFramebuffer(0,0,255,255,256,0,511,255,
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
