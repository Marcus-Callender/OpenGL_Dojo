#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	// the shader program ID
	unsigned int ID;

	// shader to set up the class
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

	// function to activate the shader
	void Use();

	// functions to set variables within the shader
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
};

#endif // !SHADER_H
