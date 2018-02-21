#include "Shader.h"

Shader::Shader(const GLchar * vertexPath, const GLchar * fragmentPath)
{
	// 1. retreve the vertex and fragment source code from the specified file path
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vertexShaderFile;
	std::ifstream fragmentShaderFile;

	// ensure the files can throw exceptions if needed
	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// open the files
		vertexShaderFile.open(vertexPath);
		fragmentShaderFile.open(fragmentPath);
		
		std::stringstream vertexShaderStream;
		std::stringstream fragmentShaderStream;

		// read the file's contents into the stream variable
		vertexShaderStream << vertexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();

		// close the file handelers
		vertexShaderFile.close();
		fragmentShaderFile.close();

		// convert the stream into a string so it can be used
		vertexCode = vertexShaderStream.str();
		fragmentCode = fragmentShaderStream.str();
	}
	catch (std::ifstream::failure exception)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ.\n";
	}

	const char* vertexShaderCode = vertexCode.c_str();
	const char* fragmentShaderCode = fragmentCode.c_str();

	// 2. compile the shaders
	unsigned int vertex;
	unsigned int fragment;
	int sucsess;
	char infoLog[512];

	// compile the vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexShaderCode, NULL);
	glCompileShader(vertex);

	// check for any errors
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &sucsess);
	
	if (!sucsess)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "Error in vertex shader compilation : " << infoLog << ".\n";
	}

	// compile the fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragment);

	// check for any errors
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &sucsess);

	if (!sucsess)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "Error in fragment shader compilation : " << infoLog << ".\n";
	}

	// crate the shader program and link it to the vertex/fragment shaders
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	// print any errors when creating the shader program
	glGetProgramiv(ID, GL_LINK_STATUS, &sucsess);

	if (!sucsess)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED : " << infoLog << ".\n";
	}

	// delete the shader as they are now linked to this program and are no longer needed
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::Use()
{
	// sets this program to be used
	glUseProgram(ID);
}

void Shader::setBool(const std::string & name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string & name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string & name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
