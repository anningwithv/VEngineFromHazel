#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

#include "VEngine/Renderer/Shader/Shader.h"
#include "VEngine/Renderer/Camera/Camera.h"
#include "VEngine/Renderer/Texture/Texture.h"

namespace VEngine
{
	class Model
	{
	public:
		Model();

		Model(glm::vec3 pos, glm::vec3 scale, glm::vec3 rotation, Ref<Material> mat);
		
		Model(glm::vec3 pos, glm::vec3 scale, glm::vec3 rotation, Ref<Material> mat, const std::string& modelPath);

		 
		Model(std::string& path)
		{
			this->LoadModel(path);
		}

		//void Draw(Shader shader);
		virtual void Draw(TimeStep ts);
		virtual void SubmitMesh();
		void SetPosition(glm::vec3 position);
		void SetRotation(glm::vec3 rotation);
		void SetScale(glm::vec3 scale);
		glm::mat4 GetTransform();

	protected:
		void RefreshTranform();

	protected:
		Ref<Material> m_Material;

		Texture* m_DiffuseTexture;
		Texture* m_SpecularTexture;

		glm::mat4 m_Transform;

		//std::string m_ShaderName;

		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;

		/*  模型数据  */
		std::vector<Mesh> meshes;
		std::string directory;

		/*  私有成员函数   */
		void LoadModel(const std::string& path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string& typeName);
	};
}
