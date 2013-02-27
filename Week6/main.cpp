#include <cstdlib>
#include <cstdio>
#include <ctime>

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

#include <cstdio>

#include "Font.hpp"

const float pi = acos(-1.0f);

#include "LoadShaders.h"

#define BUFFER_OFFSET(offset)  ((void *)(offset))

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum RenderBuffer_IDs { Color, Depth, NumRenderBuffers };
enum Tex_IDs { FontTex, RenderTex, NumTexs };
enum Digit_IDs { Digit0, Digit1, Digit2, Digit3, Digit4, Digit5, Digit6,
				 Digit7, NumDigits };

enum GlyphList { Glyph0, Glyph1, Glyph2, Glyph3, Glyph4, Glyph5, Glyph6,
				 Glyph7, Glyph8, Glyph9, GlyphColon, NumGlyphs };

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];
GLuint frameBuffer, renderBuffer[NumRenderBuffers];
GLuint DigitBuffers[NumDigits];
GLuint Texs[NumTexs];

GLuint transformUBB = 0;

const GLuint NumVertices = 4;

struct VertexData {
	vec4<GLfloat> position;
	vec4<GLubyte> color;
	vec2<GLfloat> tex;
};

/*

  Vertex Order:
  1___3
  |\  |
  | \ |
  |  \|
  0___2

  Vertex Coordinates:
  -1,1 |  1,1
  |
  _____|_____
  |
  |
  -1,-1| 1,-1

  Texture Coordinates:
  +T
  0,1
  |
  |
  0,0____1,0+S

*/
struct QuadData {
	VertexData vertices[4];
};

QuadData glyphQuads[NumGlyphs];

void checkForError()
{
	GLenum error = glGetError();
	if(error != GL_NO_ERROR) {
		std::cerr << gluErrorString(error) << std::endl;
	}
}

void initRenderBuffers()
{
	/* generate ids... */
	glGenRenderbuffers(NumRenderBuffers, renderBuffer);

	/* create color buffer */
	glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer[Color]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, 400, 300);

	/* create depth buffer */
	glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer[Depth]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 400, 300);

	/* generate frame buffer id */
	glGenFramebuffers(1, &frameBuffer);

	/* create offscreen frame buffer with previously created color and depth
	   buffers... */ 
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
	/*glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
	  GL_TEXTURE_2D, Texs[RenderTex], 0);*/
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
	  GL_RENDERBUFFER, renderBuffer[Color]);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
							  GL_RENDERBUFFER, renderBuffer[Depth]);

	/* Reset to window framebuffer... */
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void initTransforms()
{
	glGenBuffers(NumDigits, DigitBuffers);

	/* 0 */
	glBindBuffer(GL_UNIFORM_BUFFER, DigitBuffers[Digit0]);
	mat4<GLfloat> digit0(
		mat4<GLfloat>::Translate(vec4<GLfloat>(-0.8f, 0.1f, 0.0f, 0.0f)) *
		mat4<GLfloat>::UniformScale(0.1f));
	glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4<GLfloat>), &digit0,
		GL_STATIC_DRAW);

	/* 1 */
	glBindBuffer(GL_UNIFORM_BUFFER, DigitBuffers[Digit1]);
	mat4<GLfloat> digit1(
		mat4<GLfloat>::Translate(vec4<GLfloat>(-0.6f, 0.1f, 0.0f, 0.0f)) *
		mat4<GLfloat>::UniformScale(0.1f));
	glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4<GLfloat>), &digit1,
		GL_STATIC_DRAW);

	/* 2 */
	glBindBuffer(GL_UNIFORM_BUFFER, DigitBuffers[Digit2]);
	mat4<GLfloat> digit2(
		mat4<GLfloat>::Translate(vec4<GLfloat>(-0.375f, 0.1f, 0.0f, 0.0f)) *
		mat4<GLfloat>::UniformScale(0.05f));
	glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4<GLfloat>), &digit2,
		GL_STATIC_DRAW);

	/* 3 */
	glBindBuffer(GL_UNIFORM_BUFFER, DigitBuffers[Digit3]);
	mat4<GLfloat> digit3(
		mat4<GLfloat>::Translate(vec4<GLfloat>(-0.2f, 0.1f, 0.0f, 0.0f)) *
		mat4<GLfloat>::UniformScale(0.1f));
	glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4<GLfloat>), &digit3,
		GL_STATIC_DRAW);

	/* 4 */
	glBindBuffer(GL_UNIFORM_BUFFER, DigitBuffers[Digit4]);
	mat4<GLfloat> digit4(
		mat4<GLfloat>::Translate(vec4<GLfloat>(0.0f, 0.1f, 0.0f, 0.0f)) *
		mat4<GLfloat>::UniformScale(0.1f));
	glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4<GLfloat>), &digit4,
		GL_STATIC_DRAW);

	/* 5 */
	glBindBuffer(GL_UNIFORM_BUFFER, DigitBuffers[Digit5]);
	mat4<GLfloat> digit5(
		mat4<GLfloat>::Translate(vec4<GLfloat>(0.225f, 0.1f, 0.0f, 0.0f)) *
		mat4<GLfloat>::UniformScale(0.05f));
	glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4<GLfloat>), &digit5,
		GL_STATIC_DRAW);

	/* 6 */
	glBindBuffer(GL_UNIFORM_BUFFER, DigitBuffers[Digit6]);
	mat4<GLfloat> digit6(
		mat4<GLfloat>::Translate(vec4<GLfloat>(0.4f, 0.1f, 0.0f, 0.0f)) *
		mat4<GLfloat>::UniformScale(0.1f));
	glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4<GLfloat>), &digit6,
		GL_STATIC_DRAW);

	/* 7 */
	glBindBuffer(GL_UNIFORM_BUFFER, DigitBuffers[Digit7]);
	mat4<GLfloat> digit7(
		mat4<GLfloat>::Translate(vec4<GLfloat>(0.6f, 0.1f, 0.0f, 0.0f)) *
		mat4<GLfloat>::UniformScale(0.1f));
	glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4<GLfloat>), &digit7,
		GL_STATIC_DRAW);
}

void initTextures()
{
	glGenTextures(NumTexs, Texs);

	/* Create Texture... */
	glBindTexture(GL_TEXTURE_2D, Texs[FontTex]);
	checkForError();

	FontFactory fontFactory;
	std::shared_ptr<IFont> font;
	fontFactory.loadFont("C:\\Windows/Fonts/cour.ttf", 128, font);

	std::string digits("0123456789:");
	font->preload(digits);

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_R8, font->pitch(), font->height());
	checkForError();

	for(size_t i = 0; i < font->height(); i++) {
		/* OpenGL origin is from bottom left (i.e. row 0 is the last).  This
		 requires us to flip the rows. */
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, font->height() - (i + 1),
			font->pitch(), 1, GL_RED, GL_UNSIGNED_BYTE,
			font->buffer() + i * font->pitch());
		checkForError();
	}

	for(size_t i = 0; i < digits.size(); i++) {
		Glyph glyph = font->getGlyph(digits[i]);

		float top = static_cast<float>(font->height() - glyph.topLeft().Y()) /
			static_cast<float>(font->height());
		float left = static_cast<float>(glyph.topLeft().X()) /
			static_cast<float>(font->pitch());
		float bottom = static_cast<float>(font->height() -
			(glyph.topLeft().Y() + glyph.size().Y())) /
			static_cast<float>(font->height());
		float right = static_cast<float>(glyph.topLeft().X() +
			glyph.size().X()) / static_cast<float>(font->pitch());

		QuadData glyphQuad = {
			{
				{
					/* Bottom left */
					vec4<GLfloat>(-1.0f,-1.0f,0.0f,1.0f),
					vec4<GLubyte>(255,255,255,255),
					vec2<GLfloat>(left, bottom)
				},
				{
					/* Top left */
					vec4<GLfloat>(-1.0f,1.0f,0.0f,1.0f),
					vec4<GLubyte>(255,255,255,255),
					vec2<GLfloat>(left, top)
				},
				{
					/* Bottom right */
					vec4<GLfloat>(1.0,-1.0f,0.0f,1.0f),
					vec4<GLubyte>(255,255,255,255),
					vec2<GLfloat>(right, bottom)
				},
				{
					/* Top right */
					vec4<GLfloat>(1.0f,1.0f,0.0f,1.0f),
					vec4<GLubyte>(255,255,255,255),
					vec2<GLfloat>(right, top)
				}
			}
		};

		glyphQuads[i] = glyphQuad;
	}

	/* Create render buffer texture */
	glBindTexture(GL_TEXTURE_2D, Texs[RenderTex]);
	checkForError();

	glTexImage2D(GL_TEXTURE_2D, 1, GL_RGBA8, 400, 300, 0, GL_RGBA,
		GL_UNSIGNED_INT_8_8_8_8, NULL);
	checkForError();
}

void init()
{
	/* enable depth test for program */
	glEnable(GL_DEPTH_TEST);

	/* Create textures for loaded image and render buffer */
	initTextures();

	/* setup offscreen rendering */
	initRenderBuffers();

	/* Create transforms */
	initTransforms();

	/* Create an id for the vertex array */
	glGenVertexArrays(NumVAOs, VAOs);
	glBindVertexArray(VAOs[Triangles]);

	/* Create a buffer to hold vertex information*/
	glGenBuffers(NumBuffers, Buffers);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(glyphQuads), glyphQuads,
		GL_STATIC_DRAW);

	GLchar const * vertexSource =
		"#version 410 core\r\n\
\r\n\
layout (location = 0) in vec4 position;\r\n\
layout (location = 1) in vec2 in_texCoord;\r\n	\
layout (std140) uniform Transform {\r\n\
mat4 wvp;\r\n\
};\r\n\
\r\n\
out vec2 vs_texCoord;\r\n\
void main()\r\n\
{\r\n\
    gl_Position = wvp * position;\r\n\
    vs_texCoord = in_texCoord;\
}\r\n\
";

	GLchar const * fragmentSource =
		"#version 410 core\r\n\
uniform sampler2D tex;\r\n\
in vec2 vs_texCoord;\r\n\
out vec4 fColor;\r\n\
\r\n\
void main()\r\n\
{\r\n\
    fColor = texture(tex, vs_texCoord);\r\n\
}\r\n\
";

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, vertexSource },
		{ GL_FRAGMENT_SHADER, fragmentSource },
		{ GL_NONE, NULL }
	};

	GLuint program = LoadShadersFromString(shaders);

	glUseProgram(program);

	glBindTexture(GL_TEXTURE_2D, Texs[FontTex]);
	checkForError();

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
	checkForError();

	GLint positionLocation = glGetAttribLocation(program, "position");
	glVertexAttribPointer(
						  positionLocation,
						  4,
						  GL_FLOAT,
						  GL_FALSE,
						  sizeof(VertexData),
						  BUFFER_OFFSET(0)
						  );
	checkForError();
	glEnableVertexAttribArray(positionLocation);
	checkForError();

	GLint texCoordLocation = glGetAttribLocation(program, "in_texCoord");
	checkForError();
	glVertexAttribPointer(
						  texCoordLocation,
						  2,
						  GL_FLOAT,
						  GL_FALSE,
						  sizeof(VertexData),
						  BUFFER_OFFSET( sizeof(vec4<GLfloat>) +
						      sizeof(vec4<GLubyte>) )
						  );
	checkForError();
	glEnableVertexAttribArray(texCoordLocation);
	checkForError();

	GLint transformUBI = glGetUniformBlockIndex(program, "Transform");
	checkForError();
	glUniformBlockBinding(program, transformUBI, transformUBB);
	checkForError();
}

void drawToOffScreenFrameBuffer()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
	checkForError();

	glViewport(0, 0, 400, 300);
	checkForError();

	/* Render... */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	checkForError();

	glBindVertexArray(VAOs[Triangles]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, NumVertices);

	glFlush();

	/* Rest to Window Frame Buffer... */
	glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glViewport(0,0,800,600);
}

void displayTime()
{
	time_t rawTime = 0;
	time(&rawTime);

	struct tm * refinedTime = localtime(&rawTime);

	/* hours */
	glBindBufferRange(GL_UNIFORM_BUFFER, transformUBB, DigitBuffers[Digit0],
		0, sizeof(mat4<GLfloat>));
	checkForError();

	glDrawArrays(GL_TRIANGLE_STRIP, (refinedTime->tm_hour / 10) * NumVertices,
		NumVertices);
	checkForError();

	glBindBufferRange(GL_UNIFORM_BUFFER, transformUBB, DigitBuffers[Digit1],
		0, sizeof(mat4<GLfloat>));
	checkForError();

	glDrawArrays(GL_TRIANGLE_STRIP, (refinedTime->tm_hour % 10)  * NumVertices,
		NumVertices);
	checkForError();

	/* : */
	glBindBufferRange(GL_UNIFORM_BUFFER, transformUBB, DigitBuffers[Digit2],
		0, sizeof(mat4<GLfloat>));
	checkForError();

	glDrawArrays(GL_TRIANGLE_STRIP, GlyphColon * NumVertices, NumVertices);
	checkForError();

	/* minutes */
	glBindBufferRange(GL_UNIFORM_BUFFER, transformUBB, DigitBuffers[Digit3],
		0, sizeof(mat4<GLfloat>));
	checkForError();

	glDrawArrays(GL_TRIANGLE_STRIP, (refinedTime->tm_min / 10) * NumVertices,
		NumVertices);
	checkForError();

	glBindBufferRange(GL_UNIFORM_BUFFER, transformUBB, DigitBuffers[Digit4],
		0, sizeof(mat4<GLfloat>));
	checkForError();

	glDrawArrays(GL_TRIANGLE_STRIP, (refinedTime->tm_min % 10)  * NumVertices,
		NumVertices);
	checkForError();

	/* : */
	glBindBufferRange(GL_UNIFORM_BUFFER, transformUBB, DigitBuffers[Digit5],
		0, sizeof(mat4<GLfloat>));
	checkForError();

	glDrawArrays(GL_TRIANGLE_STRIP, GlyphColon * NumVertices, NumVertices);
	checkForError();

	/* seconds */
	glBindBufferRange(GL_UNIFORM_BUFFER, transformUBB, DigitBuffers[Digit6],
		0, sizeof(mat4<GLfloat>));
	checkForError();

	glDrawArrays(GL_TRIANGLE_STRIP, (refinedTime->tm_sec / 10) * NumVertices,
		NumVertices);
	checkForError();

	glBindBufferRange(GL_UNIFORM_BUFFER, transformUBB, DigitBuffers[Digit7],
		0, sizeof(mat4<GLfloat>));
	checkForError();

	glDrawArrays(GL_TRIANGLE_STRIP, (refinedTime->tm_sec % 10)  * NumVertices,
		NumVertices);
	checkForError();
}

void display()
{
	/* Clear... */
	glViewport(0,0,800,600);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(VAOs[Triangles]);
	displayTime();

	glFlush();

	glutSwapBuffers();
}

void idle()
{
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800,600);

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

	glutIdleFunc(idle);

	glutDisplayFunc(display);

	glutMainLoop();
}
