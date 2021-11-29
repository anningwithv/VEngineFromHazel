#include "Shader.h"
#include "glad/glad.h"
#include "VEngine/Log.h"

namespace VEngine
{
	Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		//Create an empty vertex shader handler
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		//Send the vertex shader source code to GL
		const GLchar* source = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);
		//Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			//The max length include the null character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			//We don't need the shader anymore
			glDeleteShader(vertexShader);

			VENGINE_CORE_ERROR("{0}", infoLog.data());
			VENGINE_CORE_ASSERT(false, "Vertex shader compile failure!");

			return;
		}

		//Create en empty fragment shader handle
		GLuint fragementShader = glCreateShader(GL_FRAGMENT_SHADER);
		//Send the fragment shader source code to GL
		source = fragmentSrc.c_str();
		glShaderSource(fragementShader, 1, &source, 0);
		//Compile the fragment shader
		glCompileShader(fragementShader);

		glGetShaderiv(fragementShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragementShader, GL_INFO_LOG_LENGTH, &maxLength);

			//The max length include the null character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragementShader, maxLength, &maxLength, &infoLog[0]);

			//We don't need the shader anymore
			glDeleteShader(fragementShader);

			VENGINE_CORE_ERROR("{0}", infoLog.data());
			VENGINE_CORE_ASSERT(false, "Fragment shader compile failure!");

			return;
		}

		//Vertex and fragment shader are successfully compiled
		//Now time to link them together into a program
		//Get a program object
		m_RendererID = glCreateProgram();
		GLuint program = m_RendererID;

		//Attach shader to program
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragementShader);

		//Link program
		glLinkProgram(program);

		//Note the different functions here: glGetProgram* instead of GLGetShader*
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			//The max length include the null character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			//We don't need the program anymore
			glDeleteProgram(program);

			glDeleteShader(vertexShader);
			glDeleteShader(fragementShader);
		}

	}

	Shader::~Shader()
	{
		glDeleteProgram(m_RendererID);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}
}
