#include "Model.h"
#include "Mesh.h"
#include <glad/glad.h>

namespace VEngine
{
	Model::Model()
	{
	}

	Model::Model(glm::vec3 pos, glm::vec3 scale, glm::vec3 rotation, Ref<Material> material)
	{
		//m_Material = std::make_shared<Material>(shaderName, diffuseTex, specularTex);
		m_Material = material;

		m_Position = pos;
		m_Rotation = rotation;
		m_Scale = scale;

		RefreshTranform();
	}

	Model::Model(glm::vec3 pos, glm::vec3 scale, glm::vec3 rotation, Ref<Material> mat, const std::string & modelPath)
		:Model(pos, scale, rotation, mat)
	{
		LoadModel(modelPath);
	}

	void Model::Draw(TimeStep ts)
	{
		m_Material->Draw(m_Transform);

		for (GLuint i = 0; i < this->meshes.size(); i++)
			this->meshes[i].Draw(ts);
	}

	void Model::SubmitMesh()
	{
		for (GLuint i = 0; i < this->meshes.size(); i++)
			this->meshes[i].Draw(0);
	}

	void Model::SetPosition(glm::vec3 position)
	{
		m_Position = position;
		RefreshTranform();
	}

	void Model::SetRotation(glm::vec3 rotation)
	{
		m_Rotation = rotation;
		RefreshTranform();
	}

	void Model::SetScale(glm::vec3 scale)
	{
		m_Scale = scale;
		RefreshTranform();
	}

	void Model::RefreshTranform()
	{
		m_Transform = glm::translate(glm::mat4(1.0f), m_Position)
			*glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.x), glm::vec3(1.0, 0.0, 0.0))
			*glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.y), glm::vec3(0.0, 1.0, 0.0))
			*glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.z), glm::vec3(0.0, 0.0, 1.0))
			* glm::scale(glm::mat4(1.0f), glm::vec3(m_Scale.x, m_Scale.y, m_Scale.z));
	}

	void Model::LoadModel(const std::string & path)
	{
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			VENGINE_CORE_ERROR("ERROR::ASSIMP::{0}", import.GetErrorString());
			return;
		}
		this->directory = path.substr(0, path.find_last_of('/'));

		this->ProcessNode(scene->mRootNode, scene);
	}

	void Model::ProcessNode(aiNode * node, const aiScene * scene)
	{
		// 添加当前节点中的所有Mesh
		for (GLuint i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			this->meshes.push_back(this->ProcessMesh(mesh, scene));
		}
		// 递归处理该节点的子孙节点
		for (GLuint i = 0; i < node->mNumChildren; i++)
		{
			this->ProcessNode(node->mChildren[i], scene);
		}
	}

	Mesh Model::ProcessMesh(aiMesh * mesh, const aiScene * scene)
	{
		// Data to fill
		//vector<GLuint> indices;
		//vector<Texture> textures;

		const int verticesSize = mesh->mNumVertices * 8;
		GLfloat* vertices = new GLfloat[verticesSize];
		// Walk through each of the mesh's vertices
		int index = 0;
		for (GLuint i = 0; i < mesh->mNumVertices; i++)
		{
			// Positions
			vertices[index + 0] = mesh->mVertices[i].x;
			vertices[index + 1] = mesh->mVertices[i].y;
			vertices[index + 2] = mesh->mVertices[i].z;

			// Texture Coordinates
			if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
			{
				// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vertices[index + 3] = mesh->mTextureCoords[0][i].x;
				vertices[index + 4] = mesh->mTextureCoords[0][i].y;

			}
			else
			{
				vertices[index + 3] = 0.0f;
				vertices[index + 4] = 0.0f;
			}

			// Normals
			vertices[index + 5] = mesh->mNormals[i].x;
			vertices[index + 6] = mesh->mNormals[i].y;
			vertices[index + 7] = mesh->mNormals[i].z;
			
			index += 8;
		}

		int indicesCount = 0;

		// Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (GLuint i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// Retrieve all indices of the face and store them in the indices vector
			for (GLuint j = 0; j < face.mNumIndices; j++)
				indicesCount++;
		}
		GLuint* indices = new GLuint[indicesCount];
		int indiceIndex = 0;
		for (GLuint i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// Retrieve all indices of the face and store them in the indices vector
			for (GLuint j = 0; j < face.mNumIndices; j++)
			{
				indices[indiceIndex] = face.mIndices[j];
				indiceIndex++;
			}
		}

		// Process materials
		//if (mesh->mMaterialIndex >= 0)
		//{
		//	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		//	// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
		//	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
		//	// Same applies to other texture as the following list summarizes:
		//	// Diffuse: texture_diffuseN
		//	// Specular: texture_specularN
		//	// Normal: texture_normalN

		//	// 1. Diffuse maps
		//	vector<TextureData> diffuseMaps = this->LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		//	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		//	// 2. Specular maps
		//	vector<TextureData> specularMaps = this->LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		//	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		//}

		Vertex* vertexData = new Vertex(vertices, verticesSize, indices, indicesCount);
		std::vector<TextureData> textures;
		Mesh result = Mesh(this, vertexData, textures);

		// Return a mesh object created from the extracted mesh data
		return result;
	}

	//std::vector<TextureData> Model::LoadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string & typeName)
	//{
	//	vector<TextureData> textures;
	//	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	//	{
	//		aiString str;
	//		mat->GetTexture(type, i, &str);
	//		// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
	//		TextureData texture;
	//		texture.id = TextureFromFile(str.C_Str(), this->directory);
	//		texture.type = typeName;
	//		texture.path = str;
	//		textures.push_back(texture);
	//	}
	//	return textures;
	//}

	glm::mat4 Model::GetTransform()
	{
		return m_Transform;
	}
}