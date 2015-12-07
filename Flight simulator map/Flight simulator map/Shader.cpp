#include "Shader.h"
#include "CrashReporter.h"

std::list<Shader*> Shader::Shaders;

Shader::Shader(char* vsFileName, char* psFileName)
{
	char * path1 = (char*)malloc(2048);
	strcpy(path1, SHADERS_SUBDIR);
	strcat(path1, vsFileName);
	FILE* file = fopen(path1, "rb");
	char* str = strerror(errno);

	if (file == NULL)
		TriggerCrash("Could not found shader file");

	fseek(file, 0, SEEK_END);
	int flen = ftell(file);
	fseek(file, 0, SEEK_SET);
	char * vertexShaderSource = (char*)malloc(flen + 1);
	fread(vertexShaderSource, 1, flen, file);
	vertexShaderSource[flen] = '\0';
	fclose(file);
	free(path1);

	char * path2 = (char*)malloc(2048);
	strcpy(path2, SHADERS_SUBDIR);
	strcat(path2, psFileName);
	file = fopen(path2, "rb");
	if (file == NULL)
		TriggerCrash("Could not found shader file");
	fseek(file, 0, SEEK_END);
	flen = ftell(file);
	fseek(file, 0, SEEK_SET);
	char * pixelShaderSource = (char*)malloc(flen + 1);
	fread(pixelShaderSource, 1, flen, file);
	pixelShaderSource[flen] = '\0';
	fclose(file);
	free(path2);

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	pixelShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Load Shader Sources
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexShaderSource, NULL);
	glShaderSource(pixelShader, 1, (const GLchar**)&pixelShaderSource, NULL);

	// Compile The Shaders
	glCompileShader(vertexShader);
	glCompileShader(pixelShader);

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

	Shaders.push_back(this);
}

Shader::~Shader()
{
}


void Shader::onPrepare(void*)
{
	glUseProgram(program);
}

void Shader::onDraw(void*){

}