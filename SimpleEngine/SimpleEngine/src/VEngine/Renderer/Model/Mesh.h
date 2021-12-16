#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>
#include <glad/glad.h>

#include "VEngine/Renderer/VertexArray.h"
#include "VEngine/Renderer/Shader/Shader.h"
#include "VEngine/Renderer/Texture/Texture.h"
#include "VEngine/Core/TimeStep.h"
#include "VEngine/Renderer/Material/Material.h"

namespace VEngine
{
	class Model;

	class Vertex
	{
		//// position
		//glm::vec3 position;
		//// normal
		//glm::vec3 normal;
		//// texCoords
		//glm::vec2 uv;
		//// tangent
		////glm::vec3 tangent;
		//// bitangent
		////glm::vec3 bitangent;
		//Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv):position(position), normal(normal), uv(uv)
		//{
		//}
	public:

		GLfloat* vertices;
		int verticesSize;
		GLuint* indices;
		int indicesCount;

		Vertex(GLfloat* vertices, int verticesSize, GLuint* indices, int indicesSize) 
			:vertices(vertices), verticesSize(verticesSize), indices(indices), indicesCount(indicesSize)
		{
		}
	};

	struct TextureData
	{
		GLuint id;
		std::string type;
	};

	class Mesh
	{
	public:
		Vertex* verticeData;
		std::vector<TextureData> textures;
		Mesh(Model* model, Vertex* verticeData, std::vector<TextureData> textures);


		void SetupMesh(/*float vertices[]*/);
		void Draw(TimeStep ts);

	private:
		Model* m_Model;
		Ref<VertexArray> m_VertexArray;
		//Ref<ShaderLibrary> m_ShaderLibrary; 
		//Ref<Texture2D> m_Texture;
		float m_Time;
	};
}
