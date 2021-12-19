#pragma once

#include "Model.h"

namespace VEngine
{
	class CubeModel : public Model
	{
	public:
		CubeModel(glm::vec3 pos, glm::vec3 scale, glm::vec3 rotation, const std::string& shaderName, const std::string& diffuseTex, const std::string& specularTex);
		virtual void Draw(TimeStep ts) override;
	private:
		Mesh* m_Mesh;
	};
}
