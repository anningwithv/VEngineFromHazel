#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>
#include <glad/glad.h>

#include "VEngine/Renderer/VertexArray.h"
#include "VEngine/Renderer/Shader/Shader.h"
#include "VEngine/Renderer/Texture/Texture.h"

namespace VEngine
{
	struct Vertex
	{
		// position
		glm::vec3 position;
		// normal
		glm::vec3 normal;
		// texCoords
		glm::vec2 uv;
		// tangent
		glm::vec3 tangent;
		// bitangent
		glm::vec3 bitangent;
	};

	class Mesh
	{
	public:
		//std::vector<Vertex> vertices;
		//std::vector<unsigned int> indices;
		//std::vector<Texture> textures;

		Mesh(/*float vertices[]*/);

		void SetupMesh(/*float vertices[]*/);
		void Draw();

	private:
		Ref<VertexArray> m_VertexArray;
		Ref<ShaderLibrary> m_ShaderLibrary; 
		Ref<Texture2D> m_Texture;
	};
}
