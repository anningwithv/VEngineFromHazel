#include "OpenGLShader.h"
#include "glad/glad.h"
#include "VEngine/Log.h"
#include <glm/gtc/type_ptr.hpp>
#include <fstream>

namespace VEngine
{
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		VENGINE_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	//OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
	//{
	//	//Create an empty vertex shader handler
	//	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//	//Send the vertex shader source code to GL
	//	const GLchar* source = vertexSrc.c_str();
	//	glShaderSource(vertexShader, 1, &source, 0);
	//	//Compile the vertex shader
	//	glCompileShader(vertexShader);

	//	GLint isCompiled = 0;
	//	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	//	if (isCompiled == GL_FALSE)
	//	{
	//		GLint maxLength = 0;
	//		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

	//		//The max length include the null character
	//		std::vector<GLchar> infoLog(maxLength);
	//		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

	//		//We don't need the shader anymore
	//		glDeleteShader(vertexShader);

	//		VENGINE_CORE_ERROR("{0}", infoLog.data());
	//		VENGINE_CORE_ASSERT(false, "Vertex shader compile failure!");

	//		return;
	//	}

	//	//Create en empty fragment shader handle
	//	GLuint fragementShader = glCreateShader(GL_FRAGMENT_SHADER);
	//	//Send the fragment shader source code to GL
	//	source = fragmentSrc.c_str();
	//	glShaderSource(fragementShader, 1, &source, 0);
	//	//Compile the fragment shader
	//	glCompileShader(fragementShader);

	//	glGetShaderiv(fragementShader, GL_COMPILE_STATUS, &isCompiled);
	//	if (isCompiled == GL_FALSE)
	//	{
	//		GLint maxLength = 0;
	//		glGetShaderiv(fragementShader, GL_INFO_LOG_LENGTH, &maxLength);

	//		//The max length include the null character
	//		std::vector<GLchar> infoLog(maxLength);
	//		glGetShaderInfoLog(fragementShader, maxLength, &maxLength, &infoLog[0]);

	//		//We don't need the shader anymore
	//		glDeleteShader(fragementShader);

	//		VENGINE_CORE_ERROR("{0}", infoLog.data());
	//		VENGINE_CORE_ASSERT(false, "Fragment shader compile failure!");

	//		return;
	//	}

	//	//Vertex and fragment shader are successfully compiled
	//	//Now time to link them together into a program
	//	//Get a program object
	//	m_RendererID = glCreateProgram();
	//	GLuint program = m_RendererID;

	//	//Attach shader to program
	//	glAttachShader(program, vertexShader);
	//	glAttachShader(program, fragementShader);

	//	//Link program
	//	glLinkProgram(program);

	//	//Note the different functions here: glGetProgram* instead of GLGetShader*
	//	GLint isLinked = 0;
	//	glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
	//	if (isLinked == GL_FALSE)
	//	{
	//		GLint maxLength = 0;
	//		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

	//		//The max length include the null character
	//		std::vector<GLchar> infoLog(maxLength);
	//		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

	//		//We don't need the program anymore
	//		glDeleteProgram(program);

	//		glDeleteShader(vertexShader);
	//		glDeleteShader(fragementShader);
	//	}

	//}

	OpenGLShader::OpenGLShader(const std::string & filePath)
	{
		std::string source = ReadFile(filePath);
		auto shaderSource = PreProcess(source);
		Compile(shaderSource);
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

	std::string OpenGLShader::ReadFile(const std::string & filePath)
	{
		std::string result;
		std::ifstream in(filePath, std::ios::in, std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
			;
		}
		else
		{
			VENGINE_CORE_ERROR("Could not open file:" + filePath);
		}

		return result;
	}

	std::map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			VENGINE_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			//VENGINE_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");
			if (ShaderTypeFromString(type) == 0)
				break;

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(const std::map<GLenum, std::string>& shaderSources)
	{
		GLuint program = glCreateProgram();
		std::vector<GLenum> glShaderIDs(shaderSources.size());
		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			//Create an empty shader handler
			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = source.c_str();
			//Send the shader source code to GL
			glShaderSource(shader, 1, &sourceCStr, 0);

			//Compile the shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				VENGINE_CORE_ERROR("{0}", infoLog.data());
				VENGINE_CORE_ASSERT(false, "Shader compilation failure!");
				break;
			}

			glAttachShader(program, shader);
			glShaderIDs.push_back(shader);
		}

		m_RendererID = program;

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);

			for (auto id : glShaderIDs)
				glDeleteShader(id);

			VENGINE_CORE_ERROR("{0}", infoLog.data());
			VENGINE_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		for (auto id : glShaderIDs)
			glDetachShader(program, id);
	}
}
