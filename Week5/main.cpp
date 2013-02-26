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

#include <cstdio>
#include <png.h>

const float pi = acos(-1.0f);

#include "LoadShaders.h"

#define BUFFER_OFFSET(offset)  ((void *)(offset))

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum RenderBuffer_IDs { Color, Depth, NumRenderBuffers };
enum YuvComponent_IDs { AllComponents, YComponent, UComponent,
						VComponent, NumYuvComponents };
enum Tex_IDs { BarnTex, RenderTex, NumTexs };

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];
GLuint frameBuffer, renderBuffer[NumRenderBuffers];
GLuint YuvBuffers[NumYuvComponents];
GLuint Texs[NumTexs];

const GLuint filterUBB = 0;

const GLuint NumVertices = 5;

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
	/* Just use identity matrix... */
	mat4<GLfloat> all(mat4<GLfloat>::Identity);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4<GLfloat>), &all,
		GL_STATIC_DRAW);

	/* Y... */
	glBindBuffer(GL_UNIFORM_BUFFER, YuvBuffers[YComponent]);
	mat4<GLfloat> y =
		yuvToRgb *
		/* isolates the Y... */
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
		/* isolates the U... */
		mat4<GLfloat>(0.0f, 0.0f, 0.0f, 0.0f,
					0.0f, -1.0f, 0.0f, 0.5f,
					0.0f, 1.0f, 0.0f, 0.5f,
					0.0f, 0.0f, 0.0f, 1.0f) *
		rgbToYuv;
	glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4<GLfloat>), &u,
		GL_STATIC_DRAW);

	/* V... */
	glBindBuffer(GL_UNIFORM_BUFFER, YuvBuffers[VComponent]);
	mat4<GLfloat> v =
		/* isolates the V... */
		mat4<GLfloat>(0.0f, 0.0f, 1.0f, 0.5f,
					0.0f, 0.0f, -1.0f, 0.5f,
					0.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f) *
		rgbToYuv;
	glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4<GLfloat>), &v,
		GL_STATIC_DRAW);
}

void initTextures()
{
	glGenTextures(NumTexs, Texs);

	/* Create 'Barn.png' Texture... */
	glBindTexture(GL_TEXTURE_2D, Texs[BarnTex]);
	checkForError();

	FILE *fp;
	if ((fp = fopen("Barn.png", "rb")) == NULL) {
		std::cerr << "Could not open 'Barn.png'.";
		return;
	}

	png_structp png_ptr = png_create_read_struct(
		PNG_LIBPNG_VER_STRING,
		NULL,
		NULL,
		NULL);
	if (png_ptr == NULL)
	{
		fclose(fp);
		std::cerr << "Could not create png read struct.";
		return;
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		fclose(fp);
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		std::cerr << "Could not create png create info struct.";
		return;
	}

	if (setjmp(png_jmpbuf(png_ptr)))
	{
		/* Free all of the memory associated with the png_ptr and info_ptr */
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(fp);
		/* If we get here, we had a problem reading the file */
		std::cerr << "Could not load png.";
		return;
	}

	png_init_io(png_ptr, fp);

	unsigned int sig_read = 0;
	png_set_sig_bytes(png_ptr, sig_read);

	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_BGR |
		PNG_TRANSFORM_SWAP_ALPHA, NULL);

	png_uint_32 width, height;
	int bit_depth, color_type, interlace_type;
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
		&interlace_type, NULL, NULL);

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
	checkForError();

	png_bytepp row_pointers;
	row_pointers = png_get_rows(png_ptr, info_ptr);

	for(png_uint_32 i = 0; i < height; i++) {
		/* OpenGL origin is from bottom left (i.e. row 0 is the last).  This
		 requires us to flip the rows. */
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, height - (i + 1), width, 1,
			GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, row_pointers[i]);
		checkForError();
	}
	
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

	fclose(fp);

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

	/* Create YUV color filter/transforms */
	initYuvTransforms();

	/* Create an id for the vertex array */
	glGenVertexArrays(NumVAOs, VAOs);
	glBindVertexArray(VAOs[Triangles]);

	/* Create a buffer to hold vertex information*/
	glGenBuffers(NumBuffers, Buffers);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);

	struct VertexData {
		vec4<GLfloat> position;
		vec4<GLubyte> color;
		vec2<GLfloat> tex;
	};

	VertexData vertices[NumVertices] = {
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
		{
			/* Bottom left */
			vec4<GLfloat>(-1.0f,-1.0f,0.0f,1.0f),
			vec4<GLubyte>(255,255,255,255),
			vec2<GLfloat>(0.0f, 0.0f)
		},
		{
			/* Top left */
			vec4<GLfloat>(-1.0f,1.0f,0.0f,1.0f),
			vec4<GLubyte>(255,255,255,255),
			vec2<GLfloat>(0.0f, 1.0f)
		},
		{
			/* Bottom right */
			vec4<GLfloat>(1.0,-1.0f,0.0f,1.0f),
			vec4<GLubyte>(255,255,255,255),
			vec2<GLfloat>(1.0f, 0.0f)
		},
		{
			/* Top right */
			vec4<GLfloat>(1.0f,1.0f,0.0f,1.0f),
			vec4<GLubyte>(255,255,255,255),
			vec2<GLfloat>(1.0f, 1.0f)
		}
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
		GL_STATIC_DRAW);

	GLchar const * vertexSource =
		"#version 410 core\r\n\
\r\n\
layout (location = 0) in vec4 position;\r\n\
layout (location = 1) in vec2 in_texCoord;\r\n\
\r\n\
out vec2 vs_texCoord;\r\n\
void main()\r\n\
{\r\n\
    gl_Position = position;\r\n\
    vs_texCoord = in_texCoord;\
}\r\n\
";

	GLchar const * fragmentSource =
		"#version 410 core\r\n\
uniform sampler2D tex;\r\n\
layout (std140) uniform ColorConversion {\r\n\
mat4 filter;\r\n\
};\r\n\
in vec2 vs_texCoord;\r\n\
out vec4 fColor;\r\n\
\r\n\
void main()\r\n\
{\r\n\
    fColor = filter * texture(tex, vs_texCoord);\r\n\
}\r\n\
";

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, vertexSource },
		{ GL_FRAGMENT_SHADER, fragmentSource },
		{ GL_NONE, NULL }
	};

	GLuint program = LoadShadersFromString(shaders);

	glUseProgram(program);

	glBindTexture(GL_TEXTURE_2D, Texs[BarnTex]);
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

	GLint filterUBI = glGetUniformBlockIndex(program, "ColorConversion");
	glUniformBlockBinding(program, filterUBI, filterUBB);
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

void display()
{
	/* Clear... */
	glViewport(0,0,800,600);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindBufferRange(GL_UNIFORM_BUFFER, filterUBB, YuvBuffers[AllComponents],
					  0, sizeof(mat4<GLfloat>));
	drawToOffScreenFrameBuffer();
	glBlitFramebuffer(0,0,399,299,0,300,399,599,
		GL_COLOR_BUFFER_BIT, GL_NEAREST);

	glBindBufferRange(GL_UNIFORM_BUFFER, filterUBB, YuvBuffers[YComponent],
					  0, sizeof(mat4<GLfloat>));
	drawToOffScreenFrameBuffer();
	glBlitFramebuffer(0,0,399,299,0,0,399,299,
		GL_COLOR_BUFFER_BIT, GL_NEAREST);

	glBindBufferRange(GL_UNIFORM_BUFFER, filterUBB, YuvBuffers[UComponent],
					  0, sizeof(mat4<GLfloat>));
	drawToOffScreenFrameBuffer();
	glBlitFramebuffer(0,0,399,299,400,300,799,599,
		GL_COLOR_BUFFER_BIT, GL_NEAREST);

	glBindBufferRange(GL_UNIFORM_BUFFER, filterUBB, YuvBuffers[VComponent],
					  0, sizeof(mat4<GLfloat>));
	drawToOffScreenFrameBuffer();
	glBlitFramebuffer(0,0,399,299,400,0,799,299,
		GL_COLOR_BUFFER_BIT, GL_NEAREST);

	glutSwapBuffers();
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

	glutDisplayFunc(display);

	glutMainLoop();
}
