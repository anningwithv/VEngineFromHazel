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
class Model;

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

	struct TextureData
	{
		GLuint id;
		std::string type;
	};

	class Mesh
	{
	public:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<TextureData> textures;
		Mesh(Model* model, Ref<Material>& material, std::vector<Vertex> vertices/*, std::vector<GLuint> indices, std::vector<TextureData> textures*/);

		void SetupMesh(/*float vertices[]*/);
		void Draw(TimeStep ts);

	private:
		Model* m_Model;
		Ref<Material> m_Material;
		Ref<VertexArray> m_VertexArray;
		//Ref<ShaderLibrary> m_ShaderLibrary; 
		//Ref<Texture2D> m_Texture;
		float m_Time;
	};
}
