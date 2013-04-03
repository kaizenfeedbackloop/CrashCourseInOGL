#ifndef LOAD_SHADERS
#define LOAD_SHADERS

typedef struct _ShaderInfo {
	GLenum Type;
	GLchar const * Source;
} ShaderInfo;

char * file2string(char const * fname)
{
	FILE *file;
	long flen, rlen;
	char *str;

	file = fopen(fname, "r");
	if (file == NULL) {
		fprintf(stderr, "Can't open file '%s' for reading\n", fname);
		goto done;
	}

	fseek(file, 0, SEEK_END);
	flen = ftell(file);
	rewind(file);

	str = (char *)malloc(sizeof(*str) * flen + 1);
	if (str == NULL) {
		fprintf(stderr, "Can't malloc space for '%s'\n", fname);
		goto done;
	}

	rlen = fread(str, sizeof(*str), flen, file);
	str[rlen] = '\0';

done:
	if(file != NULL)
		fclose(file);
	return str;
}

void PrintShaderErrorLog(GLuint shader)
{
	GLint maxLength = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

	GLchar* shaderErrorLog = (GLchar*)malloc(sizeof(GLchar) * (maxLength + 1));

	glGetShaderInfoLog(shader, maxLength, &maxLength, shaderErrorLog);
	shaderErrorLog[maxLength] = '\0';
	printf(shaderErrorLog);

	free(shaderErrorLog);
}

GLuint CreateShader(ShaderInfo const * shaderInfo)
{
	GLchar *shaderString = file2string(shaderInfo->Source);
	GLuint shader = glCreateShader(shaderInfo->Type);
	glShaderSource(shader, 1, (const GLchar**)(&shaderString), 0);
	glCompileShader(shader);

	GLint isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == 0) {
		PrintShaderErrorLog(shader);
		return 0;
	}

	return shader;
}

GLuint LoadShaders(ShaderInfo const * shaderInfo)
{
	ShaderInfo vertexShaderInfo = { GL_VERTEX_SHADER, "shader.vert" };
	GLuint vertexShader = CreateShader(&vertexShaderInfo);
	if(vertexShader == 0) {
		return 0;
	}

	ShaderInfo fragmentShaderInfo = { GL_FRAGMENT_SHADER, "shader.frag" };
	GLuint fragmentShader = CreateShader(&fragmentShaderInfo);
	if(fragmentShader == 0) {
		return 0;
	}

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);

	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);

	GLint isLinked = 0;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isLinked);
	if(isLinked == 0)
	{
		GLint maxLength = 0;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);
		GLchar * shaderProgramInfoLog = (GLchar*)malloc(sizeof(GLchar) * (maxLength+1));
		glGetProgramInfoLog(shaderProgram, maxLength, &maxLength,
			shaderProgramInfoLog);
		shaderProgramInfoLog[maxLength] = '\0';
		printf(shaderProgramInfoLog);
		free(shaderProgramInfoLog);
		return 0;
	}

	glUseProgram(shaderProgram);

	return shaderProgram;
}

#endif
