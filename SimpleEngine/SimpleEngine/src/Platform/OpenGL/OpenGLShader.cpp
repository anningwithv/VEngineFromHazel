#include "OpenGLShader.h"
#include "glad/glad.h"
#include "VEngine/Log.h"
#include <glm/gtc/type_ptr.hpp>

namespace VEngine
{
	OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
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

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::UploadUniformInt(const std::string & name, const int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformFloat(const std::string & name, const float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}	
	
	void OpenGLShader::UploadUniformFloat2(const std::string & name, const glm::vec2 value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}	
	
	void OpenGLShader::UploadUniformFloat3(const std::string & name, const glm::vec3 value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}	
	
	void OpenGLShader::UploadUniformFloat4(const std::string & name, const glm::vec4 value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string & name, const glm::mat3 & matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string & name, const glm::mat4 & matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}
