#include "Shader.h"
#include "CrashReporter.h"
#include <iostream>
Shader::Shader(char* vsFileName, char* psFileName)
{
	char * path1 = (char*)malloc(2048);
	strcpy(path1, SHADERS_SUBDIR);
	strcat(path1, vsFileName);
	FILE* file = fopen(path1, "r");
	char* str = strerror(errno);

	if (file == NULL)
		TriggerCrash("Could not found shader file");

	fseek(file, 0, SEEK_END);
	int flen = ftell(file);
	fseek(file, 0, SEEK_SET);
	char * vertexShaderSource = (char*)malloc(flen + 1);
	fread(vertexShaderSource, 1, flen, file);
	vertexShaderSource[flen] = 0;
	fclose(file);
	free(path1);

	char * path2 = (char*)malloc(2048);
	strcpy(path2, SHADERS_SUBDIR);
	strcat(path2, psFileName);
	file = fopen(path2, "r");
	if (file == NULL)
		TriggerCrash("Could not found shader file");
	fseek(file, 0, SEEK_END);
	flen = ftell(file);
	fseek(file, 0, SEEK_SET);
	char * pixelShaderSource = (char*)malloc(flen + 1);
	fread(pixelShaderSource, 1, flen, file);
	pixelShaderSource[flen] = 0;
	fclose(file);
	free(path2);

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	pixelShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Load Shader Sources
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexShaderSource, NULL);
	glShaderSource(pixelShader, 1, (const GLchar**)&pixelShaderSource, NULL);
	// Compile The Shaders
	glCompileShader(vertexShader);
	GLint compileStatus;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileStatus);

	if (compileStatus != GL_TRUE) {
		std::cout << "Shader failed to compile" << std::endl;
		GLint infoLoglength;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLoglength);
		GLchar* infoLog = new GLchar[infoLoglength + 1];
		glGetShaderInfoLog(vertexShader, infoLoglength, NULL, infoLog);
		std::cout << infoLog << std::endl;
		delete infoLog;
	}


	glCompileShader(pixelShader);
	compileStatus;
	glGetShaderiv(pixelShader, GL_COMPILE_STATUS, &compileStatus);

	if (compileStatus != GL_TRUE) {
		std::cout << "Shader failed to compile" << std::endl;
		GLint infoLoglength;
		glGetShaderiv(pixelShader, GL_INFO_LOG_LENGTH, &infoLoglength);
		GLchar* infoLog = new GLchar[infoLoglength + 1];
		glGetShaderInfoLog(pixelShader, infoLoglength, NULL, infoLog);
		std::cout << infoLog << std::endl;
		delete infoLog;
	}


	int len = 0;
	char* log = (char*)malloc(1024);
	glGetShaderInfoLog(vertexShader, 1024, &len, log);
	printf(log);
	glGetShaderInfoLog(pixelShader, 1024, &len, log);
	printf(log);
	free(log);
	// Create Shader And Program Objects
	program = glCreateProgram();

	// Attach The Shader Objects To The Program Object
	glAttachShader(program, vertexShader);
	glAttachShader(program, pixelShader);

	// Link The Program Object
	glLinkProgram(program);

	// Use The Program Object Instead Of Fixed Function OpenGL
	glUseProgram(program);

}

Shader::~Shader()
{
}


void Shader::onPrepare(void*)
{
	glUseProgram(program);
}
