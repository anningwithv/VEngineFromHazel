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

		Mesh(Ref<Material>& material);

		void SetupMesh(/*float vertices[]*/);
		void Draw(TimeStep ts);

	private:
		Ref<Material> m_Material;
		Ref<VertexArray> m_VertexArray;
		//Ref<ShaderLibrary> m_ShaderLibrary; 
		Ref<Texture2D> m_Texture;
		float m_Time;
	};
}
