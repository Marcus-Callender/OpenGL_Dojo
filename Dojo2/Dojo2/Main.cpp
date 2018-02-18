#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n" // sets the position variable to 0
"layout (location = 1) in vec3 aColour;\n"
"out vec3 ourColour;\n" // output a color to the fragment shader
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos, 1.0);\n" // sets the first 3 elements of gl_Position to the 3 elements in aPos
"	 ourColour = aColour;\n" // set ourColour to the input color we got from the vertex data
"}\n\0";

const char* FragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColour;" // a globaly acsessable variabe
"void main()\n"
"{\n"
"    FragColor = vec4(ourColour, 1.0f);\n"
"}\n\0";

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// would need this line to run on a mac
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "My Window", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window.\n";
		glfwTerminate();

		return -1;
	}

	glfwMakeContextCurrent(window);

	// attemts to initialize GLAD as a pointer to open gl functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD.\n";

		return -2;
	}

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << ".\n";
	}

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &FragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << ".\n";
	}

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKER_ERROR\n" << infoLog << ".\n";
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glViewport(0, 0, 800, 600);

	// calls FramebufferSizeCallback when the window is resized
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

	float vertices[] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f,	0.0f, 0.0f, 1.0f,

		 /*0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f*/
	};

	unsigned int indices[] = {
		0, 1, 2,
		//1, 2, 3
	};

	unsigned int ElementBufferObject;
	glGenBuffers(1, &ElementBufferObject);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	unsigned int VertexBufferObject;
	glGenBuffers(1, &VertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int VertexArrayObject;
	glGenVertexArrays(1, &VertexArrayObject);

	glBindVertexArray(VertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// the index of the first vertex to configure, the size of the vertex attribute, the type of data
	// wheather to normalize the data, the size between the begining of each vertex, the data offset 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// keeps the window open until it should close
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// the background color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		// clears the previous buffer
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		float currentTime = (float)glfwGetTime(); // removes minor typecast warning
		float greenness = (sin(currentTime) / 2.0f) + 0.5f;
		int vertexLocation = glGetUniformLocation(shaderProgram, "uniformColor");
		glUniform4f(vertexLocation, 0.0f, greenness, 0.0f, 1.0f);

		// draw all vertexes in the specified array
		///glBindVertexArray(VertexArrayObject);
		///glDrawArrays(GL_TRIANGLES, 0, 3);

		// draw the specified buffers to draw multiple objects
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// draws the object as a wireframe
		///glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferObject);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// updates the windows visuals using a double buffer
		glfwSwapBuffers(window);

		// checks if any inputs have been givven to the window
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// called when the window is resized
void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	// resizes the viewport
	glViewport(0, 0, width, height);
}

// called whenever the window receves an input
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		// closes the window when it receves a escape key press
		glfwSetWindowShouldClose(window, true);
	}
}
